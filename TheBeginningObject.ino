/*Using LVGL with Arduino requires some extra steps:
 *Be sure to read the docs here: https://docs.lvgl.io/master/get-started/platforms/arduino.html  */


#include <lvgl.h>
#include <driver/i2c.h>

#include "SD.h"
#include "SPI.h"


#include "include/definitions.h"
#include "include/accessory.c"


void lv_example_obj_2(void);
static void drag_event_handler(lv_event_t * e);


lv_display_t *lvDisplay;
lv_indev_t *lvInput;

LGFX lcd;

uint8_t initErrors = 0;

/*LVGL draw into this buffer, 1/10 screen size usually works well. The size is in bytes*/
const unsigned int lvBufferSize = (TFT_HOR_RES * TFT_VER_RES * 2) / 10;
void *lvBuffer1 = malloc(lvBufferSize);
void *lvBuffer2 = malloc(lvBufferSize);



#define LVGL_TICK_PERIOD_MS    1
static void increase_lvgl_tick( void *arg ) {
  lv_tick_inc(LVGL_TICK_PERIOD_MS);
}

#define MEM_MSG_DISPLAY_TIME  60000
void sysMan( void *arg ) {

  uint16_t  msg;
		
	while(1) {  // This is a task which runs for ever
		
    /* This will time out after MEM_MSG_DISPLAY_TIME and print memory then wait again */
		if( xQueueReceive( gui.sysActionQ, &msg, pdMS_TO_TICKS(MEM_MSG_DISPLAY_TIME) ) ) { 
			switch(msg) {

			case SAVE_PROCESS_CONFIG:
          writeConfigFile(SD, FILENAME_SAVE, false);
          break;
      // Add Further processor intensive tasks here to keep them out of the GUI execution path

			case SAVE_MACHINE_STATS:
	        LV_LOG_USER("Save On EEPROM!");
          writeMachineStats(&gui.page.tools.machineStats);
          break;
      case RELOAD_CFG:
          LV_LOG_USER("Reload FilMachine.cfg from backup");
          if(copyAndRenameFile(SD, FILENAME_BACKUP, FILENAME_SAVE))
              rebootBoard();
      default:
          LV_LOG_USER( "Unknown System Manager Request!");
          break;
      }
    } else {
      LV_LOG_USER("\nFree Heap: %u bytes\n"
        "  MALLOC_CAP_SPIRAM    %7zu bytes\n"
        "  MALLOC_CAP_INTERNAL  %7zu bytes\n",
        xPortGetFreeHeapSize(),
        heap_caps_get_free_size(MALLOC_CAP_SPIRAM),
        heap_caps_get_free_size(MALLOC_CAP_INTERNAL)
      );
    }
	}
}

void setup()
{
    Serial.begin(115200);

    LV_LOG_USER("Hello FilMachine! - This software uses LVGL V%d.%d.%d",lv_version_major(),lv_version_minor(),lv_version_patch());

    pinMode(LCD_CS, OUTPUT);
    pinMode(LCD_BLK, OUTPUT);

    digitalWrite(LCD_CS, LOW);
    digitalWrite(LCD_BLK, HIGH);

    lcd.init();
    lcd.setRotation(1);

    SPI.begin(SD_SCLK, SD_MISO, SD_MOSI);

    LV_LOG_USER("Install LVGL tick timer");
    // Tick interface for LVGL (using esp_timer to generate LVGL_TICK_PERIOD_MS periodic event)
    const esp_timer_create_args_t lvgl_tick_timer_args = {
        .callback = increase_lvgl_tick,
        .name = "lvgl_tick"
    };
    esp_timer_handle_t lvgl_tick_timer = NULL;
    esp_timer_create(&lvgl_tick_timer_args, &lvgl_tick_timer);
    esp_timer_start_periodic(lvgl_tick_timer, LVGL_TICK_PERIOD_MS * 1000);

    lv_init();

    lvDisplay = lv_display_create(TFT_HOR_RES, TFT_VER_RES);
    lv_display_set_color_format(lvDisplay, LV_COLOR_FORMAT_RGB565);
    lv_display_set_flush_cb(lvDisplay, my_disp_flush);

    lv_display_set_buffers(lvDisplay, lvBuffer1, lvBuffer2, lvBufferSize, LV_DISPLAY_RENDER_MODE_PARTIAL);

    lvInput = lv_indev_create();
    lv_indev_set_type(lvInput, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(lvInput, my_touchpad_read);
    
    init_globals();

    initSD_I2C_MCP23017();
    homePage();
    //lv_example_obj_2();
    /* Create System message queue */
    gui.sysActionQ = xQueueCreate( 16, sizeof( uint16_t ) );
    /* Create task to process external functions which will slow the GUI response */							
    xTaskCreatePinnedToCore( sysMan, "sysMan", 4096, NULL, 8,  NULL, 0 ); 
 
    //writeFile(SD, FILENAME_SAVE, "Hello, this is a test file\n");
    //writeJSONFile(SD, FILENAME_SAVE, gui.page.settings.settingsParams);
    //readFile(SD, FILENAME_SAVE);
   
    //readJSONFile(SD, FILENAME_SAVE, gui.page.settings.settingsParams);
    
    //writeFullJSONFile(SD, FILENAME_SAVE,gui);
    //readFULLJSONFile(SD, FILENAME_SAVE, gui, true);
    readConfigFile(SD, FILENAME_SAVE, false);
    readMachineStats(&gui.page.tools.machineStats);
}

void loop()
{
    lv_task_handler(); /* let the GUI do its work */
    delay(5);
    //testPin(WATER_RLY);
}




void event_handler_2(lv_event_t *e) {
    lv_obj_t *obj = (lv_obj_t *)lv_event_get_target(e);
    lv_event_code_t code = lv_event_get_code(e);

    static lv_point_t last_point;
    static bool dragging = false;

    if (code == LV_EVENT_PRESSED) {
        // Porta l'oggetto in primo piano
        lv_obj_move_foreground(obj);

        // Ottieni il punto corrente
        lv_indev_get_point(lv_indev_get_act(), &last_point);
        dragging = true;
    } 
    else if (code == LV_EVENT_RELEASED) {
        dragging = false;
    } 
    else if (code == LV_EVENT_PRESSING) {
        if (dragging) {
            lv_point_t current_point;
            lv_indev_get_point(lv_indev_get_act(), &current_point);

            lv_coord_t dx = current_point.x - last_point.x;
            lv_coord_t dy = current_point.y - last_point.y;

            // Sposta l'oggetto
            lv_obj_set_pos(obj, lv_obj_get_x(obj) + dx, lv_obj_get_y(obj) + dy);

            // Aggiorna il punto precedente
            last_point = current_point;

            // Richiedi il ridisegno dell'area
            lv_obj_invalidate(obj);
        }
    }
}



/**
 * Make an object draggable.
 */
void lv_example_obj_2(void)
{
    lv_obj_t * obj;
    lv_obj_t * obj2;
    obj = lv_obj_create(lv_screen_active());
    lv_obj_set_size(obj, 150, 100);
    lv_obj_add_event_cb(obj, event_handler_2, LV_EVENT_ALL, NULL);

    obj2 = lv_obj_create(lv_screen_active());
    lv_obj_set_size(obj2, 150, 100);
    lv_obj_add_event_cb(obj2, event_handler_2, LV_EVENT_ALL, NULL);


    lv_obj_t * label = lv_label_create(obj);
    lv_obj_t * label2 = lv_label_create(obj2);
    lv_label_set_text(label, "Drag me1");
    lv_label_set_text(label2, "Drag me2");

    lv_obj_center(label);
    lv_obj_center(label2);

}
