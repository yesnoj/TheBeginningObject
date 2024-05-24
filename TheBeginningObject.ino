/*Using LVGL with Arduino requires some extra steps:
 *Be sure to read the docs here: https://docs.lvgl.io/master/get-started/platforms/arduino.html  */

#include <lvgl.h>
#include <driver/i2c.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include "SD.h"
#include "SPI.h"

#include "include/definitions.h"
#include "include/accessory.c"

lv_display_t *lvDisplay;
lv_indev_t *lvInput;

LGFX lcd;

uint8_t initErrors = 0;

/*LVGL draw into this buffer, 1/10 screen size usually works well. The size is in bytes*/
const unsigned int lvBufferSize = TFT_HOR_RES * TFT_VER_RES * 2 / 10;
void *lvBuffer1 = malloc(lvBufferSize);
void *lvBuffer2 = malloc(lvBufferSize);


void eventSave(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);
  if(code == LV_EVENT_REFRESH){
    LV_LOG_USER("Save JSON!");
  }
    writeJSONFile(SD, FILENAME_SAVE, gui.page.settings.settingsParams);
}


void setup()
{
    Serial.begin(115200);

    LV_LOG_USER("Hello Arduino! V%d.%d.%d",lv_version_major(),lv_version_minor(),lv_version_patch());

    pinMode(LCD_CS, OUTPUT);
    pinMode(LCD_BLK, OUTPUT);

    digitalWrite(LCD_CS, LOW);
    digitalWrite(LCD_BLK, HIGH);

    lcd.init();
    lcd.setRotation(1);

    SPI.begin(SD_SCLK, SD_MISO, SD_MOSI);

    lv_init();

    lvDisplay = lv_display_create(TFT_HOR_RES, TFT_VER_RES);
    lv_display_set_color_format(lvDisplay, LV_COLOR_FORMAT_RGB565);
    lv_display_set_flush_cb(lvDisplay, my_disp_flush);    
    lv_display_set_buffers(lvDisplay, lvBuffer1, lvBuffer2, lvBufferSize, LV_DISPLAY_RENDER_MODE_PARTIAL);

    lvInput = lv_indev_create();
    lv_indev_set_type(lvInput, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(lvInput, my_touchpad_read);


    actualMillis = millis();
    
 
    initSD_I2C();
    homePage();
    
    fakeObject = lv_obj_create(NULL);

    lv_obj_add_event_cb(fakeObject, eventSave, LV_EVENT_REFRESH, NULL);  

    //writeFile(SD, FILENAME_SAVE, "Hello, this is a test file\n");
    //writeJSONFile(SD, FILENAME_SAVE, gui.page.settings.settingsParams);
    //readFile(SD, FILENAME_SAVE);
   
    readJSONFile(SD, FILENAME_SAVE, gui.page.settings.settingsParams);
}

void loop()
{
    delay(1);
    lv_tick_inc(1);
    lv_task_handler(); /* let the GUI do its work */
}

