/*Using LVGL with Arduino requires some extra steps:
 *Be sure to read the docs here: https://docs.lvgl.io/master/get-started/platforms/arduino.html  */


#include <lvgl.h>
#include <driver/i2c.h>

#include "SD.h"
#include "SPI.h"

#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ElegantOTA.h>

#include "include/definitions.h"
#include "include/accessory.c"


void lv_example_obj_2(void);
static void drag_event_handler(lv_event_t * e);


AsyncWebServer otaServer(80);
unsigned long ota_progress_millis = 0;


lv_display_t *lvDisplay;
lv_indev_t *lvInput;

LGFX lcd;

bool stopMotorManTask = false;
uint8_t initErrors = 0;

/*LVGL draw into this buffer, 1/10 screen size usually works well. The size is in bytes*/
const unsigned int lvBufferSize = (TFT_HOR_RES * TFT_VER_RES * 2) / 10;
void *lvBuffer1 = heap_caps_malloc(lvBufferSize, MALLOC_CAP_INTERNAL);
void *lvBuffer2 = heap_caps_malloc(lvBufferSize, MALLOC_CAP_INTERNAL);

#define LVGL_TICK_PERIOD_MS    1
static void increase_lvgl_tick( void *arg ) {
  lv_tick_inc(LVGL_TICK_PERIOD_MS);
}

#define MEM_MSG_DISPLAY_TIME  20000
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
    } 
    #if FILM_USE_LOG != 0
    else {  
      LV_LOG_USER("\nFree Heap: %u bytes\n"
        "  MALLOC_CAP_SPIRAM    %7zu bytes\n"
        "  MALLOC_CAP_INTERNAL  %7zu bytes\n",
        xPortGetFreeHeapSize(),
        heap_caps_get_free_size(MALLOC_CAP_SPIRAM),
        heap_caps_get_free_size(MALLOC_CAP_INTERNAL)
      );
    }
    #endif
	}
}


void motorMan(void *arg) {
    int8_t rotation = 1;
    uint8_t prevRotation = rotation;
    uint16_t msg;


    while(!stopMotorManTask) {  // Questo è un task che gira per sempre
        TickType_t interval = pdMS_TO_TICKS(1000); // Intervallo di default

        if (xQueueReceive(gui.sysActionQ, &msg, interval)) {
            // Aggiungi qui le cose da fare ogni intervallo
        } else {
            switch(rotation) {
                case 1:
                    runMotorFW(MOTOR_IN1_PIN, MOTOR_IN2_PIN);
                    prevRotation = 1;
                    rotation = 0;
                    interval = pdMS_TO_TICKS(getRandomRotationInterval() * 1000); // Imposta l'intervallo per runMotorFW
                    break;
                case -1:
                    runMotorRV(MOTOR_IN1_PIN, MOTOR_IN2_PIN);
                    prevRotation = -1;
                    rotation = 0;
                    interval = pdMS_TO_TICKS(getRandomRotationInterval() * 1000); // Imposta l'intervallo per runMotorRV
                    break;
                case 0:
                default:
                    stopMotor(MOTOR_IN1_PIN, MOTOR_IN2_PIN);
                    if (prevRotation == 1) {
                        rotation = -1;
                    } else if (prevRotation == -1) {
                        rotation = 1;
                    } else {
                        rotation = 1; // Valore iniziale se prevRotation non è 1 o -1
                    }
                    prevRotation = 0;
                    interval = pdMS_TO_TICKS(1000); // Intervallo per stopMotor
                    break;
            }
        }

        // Aggiungi un ritardo per rispettare l'intervallo calcolato
        vTaskDelay(interval);
    }

    // Codice per pulire e fermare il task
    stopMotor(MOTOR_IN1_PIN, MOTOR_IN2_PIN);
    vTaskDelete(NULL); // Termina il task
}

void stopMotorTask() {
    stopMotorManTask = true;
}

void runMotorTask() {
    xTaskCreatePinnedToCore( motorMan, "motorMan", 4096, NULL, 8,  NULL, 0 ); 
    stopMotorManTask = false;
}


void onOTAStart() {
  LV_LOG_USER("OTA update started!");
}

void onOTAProgress(size_t current, size_t final) {
  // Log every 1 second
  uint8_t percentage;
  float_t cur,fin;
/*
  cur = (float_t)(current);
  fin = (float_t)(final);
  if (millis() - ota_progress_millis > 1000) {
    ota_progress_millis = millis();
    percentage = (uint8_t)((cur / fin) * 100);
    LV_LOG_USER("OTA Progress Current: %u %%, %u bytes, Final: %u bytes\n", percentage, current, final);
  }
  */
}

void onOTAEnd(bool success) {
  if (success) {
    LV_LOG_USER("OTA update finished successfully!");
  } else {
    LV_LOG_USER("There was an error during OTA update!");
  }
}

void connectOtaAP(){
  if (!WiFi.softAP(WIFI_SSID, WIFI_PASS)) {
    LV_LOG_USER("Soft AP creation failed.");
    while(1);
  }
  IPAddress myIP = WiFi.softAPIP();
  LV_LOG_USER("AP IP address: %s",myIP.toString());
 

  ElegantOTA.setAutoReboot(false);
  ElegantOTA.setAuth(USERNAME, PASSWORD);
  ElegantOTA.begin(&otaServer);
  otaServer.begin();
  LV_LOG_USER("Server started");

  // ElegantOTA callbacks
  ElegantOTA.onStart(onOTAStart);
  ElegantOTA.onProgress(onOTAProgress);
  ElegantOTA.onEnd(onOTAEnd);
}
  

void connectOtaSTA(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID_LOCAL, WIFI_PASS_LOCAL);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    LV_LOG_USER(".");
  }
  Serial.println("");
  LV_LOG_USER("Connected to %s, IP address: %s",WIFI_SSID_LOCAL,WiFi.localIP().toString());

  otaServer.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Hi! This is ElegantOTA AsyncDemo.");
  });

  ElegantOTA.begin(&otaServer);    // Start ElegantOTA
  // ElegantOTA callbacks
  ElegantOTA.onStart(onOTAStart);
  ElegantOTA.onProgress(onOTAProgress);
  ElegantOTA.onEnd(onOTAEnd);

  otaServer.begin();
  LV_LOG_USER("HTTP server started");
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
    
    initGlobals();

    initPinouts();
    homePage();
            
    /* Create System message queue */
    gui.sysActionQ = xQueueCreate( 16, sizeof( uint16_t ) );
    /* Create task to process external functions which will slow the GUI response */							
    xTaskCreatePinnedToCore( sysMan, "sysMan", 4096, NULL, 8,  NULL, 0 ); 

    readConfigFile(SD, FILENAME_SAVE, false);
    readMachineStats(&gui.page.tools.machineStats);

    //this create a bootloop...
    //connectOtaAP(); 
}

void loop()
{
    ElegantOTA.loop();
    lv_task_handler(); /* let the GUI do its work */
    delay(5);
}


