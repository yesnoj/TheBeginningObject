/*Using LVGL with Arduino requires some extra steps:
 *Be sure to read the docs here: https://docs.lvgl.io/master/get-started/platforms/arduino.html  */
#define LGFX_USE_V1
#include <lvgl.h>
#include <LovyanGFX.hpp>
#include <driver/i2c.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SPI.h>
#include <SD.h>

//USEFUL TO RUN LVGL EXAMPLES IF NEEDED
#include <demos/lv_demos.h>
#include <examples/lv_examples.h>

#include "definitions.h"
#include "pages.h"

//OLD GUI APPROACH
//#include "display_manager.h"
//#include "pages/page_home.h"
//#include "pages/page_program.h"



/*To use the built-in examples and demos of LVGL uncomment the includes below respectively.
 *You also need to copy `lvgl/examples` to `lvgl/src/examples`. Similarly for the demos `lvgl/demos` to `lvgl/src/demos`.
 *Note that the `lv_examples` library is for LVGL v7 and you shouldn't install it for this version (since LVGL v8)
 *as the examples and demos are now part of the main LVGL library. */


lv_display_t *lvDisplay;
lv_indev_t *lvInput;


// ESP32でLovyanGFXを独自設定で利用する場合の設定例

/// 独自の設定を行うクラスを、LGFX_Deviceから派生して作成します。
class LGFX : public lgfx::LGFX_Device
{
    static constexpr int I2C_PORT_NUM = I2C_NUM_0;
    static constexpr int I2C_PIN_SDA = I2C_SDA;
    static constexpr int I2C_PIN_SCL = I2C_SCL;
    static constexpr int I2C_PIN_INT = I2C_INT;

    lgfx::Panel_ILI9488     _panel_instance;
    lgfx::Bus_Parallel16    _bus_instance; 
    lgfx::Light_PWM         _light_instance;
    lgfx::Touch_FT5x06      _touch_instance;
public:

  // コンストラクタを作成し、ここで各種設定を行います。
  // クラス名を変更した場合はコンストラクタも同じ名前を指定してください。
  LGFX(void)
    {
        {                                      
            auto cfg = _bus_instance.config(); 
            
            cfg.port = 0;              
            cfg.freq_write = 20000000; 
            cfg.pin_wr = LCD_WR;           
            cfg.pin_rd = LCD_RD;           
            cfg.pin_rs = LCD_RS;           

            cfg.pin_d0 = LCD_D0;
            cfg.pin_d1 = LCD_D1;
            cfg.pin_d2 = LCD_D2;
            cfg.pin_d3 = LCD_D3;
            cfg.pin_d4 = LCD_D4;
            cfg.pin_d5 = LCD_D5;
            cfg.pin_d6 = LCD_D6;
            cfg.pin_d7 = LCD_D7;
            cfg.pin_d8 = LCD_D8;
            cfg.pin_d9 = LCD_D9;
            cfg.pin_d10 = LCD_D10;
            cfg.pin_d11 = LCD_D11;
            cfg.pin_d12 = LCD_D12;
            cfg.pin_d13 = LCD_D13;
            cfg.pin_d14 = LCD_D14;
            cfg.pin_d15 = LCD_D15;

            _bus_instance.config(cfg);              
            _panel_instance.setBus(&_bus_instance); 
        }

        {                                        
            auto cfg = _panel_instance.config(); 

            cfg.pin_cs = LCD_CS;   
            cfg.pin_rst = LCD_RST;  
            cfg.pin_busy = LCD_BSY; 

            cfg.memory_width = TFT_WIDTH;   
            cfg.memory_height = TFT_HEIGHT;  
            cfg.panel_width = TFT_WIDTH;    
            cfg.panel_height = TFT_HEIGHT;   
            cfg.offset_x = 0;         
            cfg.offset_y = 0;         
            cfg.offset_rotation = 0;  
            cfg.dummy_read_pixel = 8; 
            cfg.dummy_read_bits = 1;  
            cfg.readable = true;      
            cfg.invert = false;       
            cfg.rgb_order = false;    
            cfg.dlen_16bit = true;    
            cfg.bus_shared = false;//true;    

            _panel_instance.config(cfg);
}

    {
      auto cfg = _light_instance.config();    

      cfg.pin_bl = LCD_BLK;              
      cfg.invert = false;           
      cfg.freq   = 44100;           
      cfg.pwm_channel = 7;          

      _light_instance.config(cfg);
      _panel_instance.setLight(&_light_instance);  
    }

    { 
      auto cfg = _touch_instance.config();

      cfg.x_min      = 0;
      cfg.x_max      = TFT_WIDTH - 1;
      cfg.y_min      = 0;  
      cfg.y_max      = TFT_HEIGHT - 1;
      cfg.pin_int    = I2C_PIN_INT;  
      cfg.bus_shared = true; 
      cfg.offset_rotation = 0;

      cfg.i2c_port = I2C_PORT_NUM;     
      cfg.i2c_addr = I2C_ADR;  
      cfg.pin_sda  = I2C_PIN_SDA;   
      cfg.pin_scl  = I2C_PIN_SCL;   
      cfg.freq = 400000;  

      _touch_instance.config(cfg);
      _panel_instance.setTouch(&_touch_instance);  
    }
        setPanel(&_panel_instance); 
    }
};

// 準備したクラスのインスタンスを作成します。
LGFX lcd;

void my_touchpad_read(lv_indev_t* dev, lv_indev_data_t* data) {
      uint16_t touchX, touchY;

      data->state = LV_INDEV_STATE_REL;

      if (lcd.getTouch(&touchX, &touchY))
      {
          data->state = LV_INDEV_STATE_PR;
          data->point.x = touchX;
          data->point.y = touchY;
      }
}



/*
void my_touchpad_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data) {
  uint16_t touchX, touchY;

  bool touched = lcd.getTouch(&touchX, &touchY, 600);

  if (!touched) {
    data->state = LV_INDEV_STATE_REL;
  } else {
    data->state = LV_INDEV_STATE_PR;


    data->point.x = touchX;
    data->point.y = touchY;

    Serial.print("Data x ");
    Serial.println(touchX);

    Serial.print("Data y ");
    Serial.println(touchY);
  }
}
*/

/*LVGL draw into this buffer, 1/10 screen size usually works well. The size is in bytes*/
const unsigned int lvBufferSize = TFT_HOR_RES * TFT_VER_RES * 2 / 10;
void *lvBuffer1 = malloc(lvBufferSize);
void *lvBuffer2 = malloc(lvBufferSize);


#if LV_USE_LOG != 0
void my_print( lv_log_level_t level, const char * buf )
{
    LV_UNUSED(level);
    Serial.println(buf);
    Serial.flush();
}
#endif

 //Prefer the DMA because is faster
 
void my_disp_flush(lv_display_t* display, const lv_area_t* area, unsigned char* data) {

  uint16_t w = (area->x2 - area->x1 + 1);
  uint16_t h = (area->y2 - area->y1 + 1);
  uint32_t size =  w * h * 2;

  lv_draw_sw_rgb565_swap(data, size);
  lcd.pushImageDMA(area->x1, area->y1, w, h, (uint16_t*)data);
  lv_display_flush_ready(display);
}


/*
void my_disp_flush(lv_display_t* display, const lv_area_t* area, unsigned char* data) {
 
  uint32_t w = lv_area_get_width(area);
  uint32_t h = lv_area_get_height(area);

  lv_draw_sw_rgb565_swap(data, w*h);
       
        if (lcd.getStartCount() == 0)
        {   // Processing if not yet started
             lcd.startWrite();
        }
        lcd.pushImageDMA( area->x1
                , area->y1
                , area->x2 - area->x1 + 1
                , area->y2 - area->y1 + 1
                ,(uint16_t*) data); 

        lv_display_flush_ready(display);
    }
*/



void setup()
{
    Serial.begin(115200);

    String LVGL_Arduino = "Hello Arduino! ";
    LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();
    Serial.println(LVGL_Arduino);

    lcd.init();
    lcd.setRotation(1);

    lv_init();

    lvDisplay = lv_display_create(TFT_HOR_RES, TFT_VER_RES);
    lv_display_set_color_format(lvDisplay, LV_COLOR_FORMAT_RGB565);
    lv_display_set_flush_cb(lvDisplay, my_disp_flush);    
    lv_display_set_buffers(lvDisplay, lvBuffer1, lvBuffer2, lvBufferSize, LV_DISPLAY_RENDER_MODE_PARTIAL);

    lvInput = lv_indev_create();
    lv_indev_set_type(lvInput, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(lvInput, my_touchpad_read);

    //testVariEdEventuali();
    actualMillis = millis();

    sd_init();
    homePage();
    //lv_example_dropdown_1();
}

void loop()
{
/*
  if (isEnteringInPage)
     displayManager();
*/
  LVGL_TaskManager();
  //updateValues(); //Causa problemi quando si torna indietro nella pagina precedente
}



void testVariEdEventuali(){
/*
    lv_obj_t *label = lv_label_create( lv_scr_act() );
    lv_label_set_text( label, "Hello Arduino, I'm LVGL!" );
    lv_obj_align( label, LV_ALIGN_CENTER, 0, 0 );
    */
    //lv_demo_benchmark(); //OK
    //lv_example_get_started_1(); //Hello world OK
    //lv_example_get_started_2(); //bottone con testo che cabia a pressione OK
    //lv_example_get_started_3(); //bottoni con shape diverse e colori diversi OK
    //lv_example_get_started_4(); //slider OK
   //lv_example_tabview_2();  //tab verticali a sx, funziona...OK
   //lv_example_obj_1(); //due quadrati di cui uno con luminescenza, carina...OK
   
  
    //lv_example_event_1(); //bottone con testo che cambia a pressione OK
    //lv_example_event_2(); //bottone OK
    //lv_example_event_3(); //griglia con bottoni scrollabili e premibili OK
    //lv_example_event_4(); //pallina centrale che ingrandisce....OK

    //lv_example_msgbox_1(); //popup con testo, tasto Apply Cancel e X per chiudere, OK
    
    //lv_example_table_1(); //lista OK

    //lv_example_list_1(); //lista icone, con categorie ed elementi innestati OK
    //lv_example_list_2(); //lista con bottoni OK

    //lv_example_anim_timeline_1(); //timeline con start e pause button OK

    //lv_example_button_2(); //bottone con ombra, non si capisce se funziona OK???
    //lv_example_button_3(); //bottone con animazione gommosa OK
    //lv_example_button_1(); //bottone e bottone toggle OK
    //lv_example_imagebutton_1(); //bottone con ombra e colorato, non si capisce se funziona OK???

    //lv_example_led_1(); //Led di diversi colori OK
    
    //lv_example_anim_2(); //pallina OK
    //lv_example_anim_3(); //graphs e sliders OK
    
    //lv_example_checkbox_1(); //Switch list, anche read only OK
    //lv_example_checkbox_2(); // radiobutton OK

    //lv_example_switch_1(); //switch funziona, anche se nella versione lv_example_anim_1 freezava. OK
    //lv_example_spinbox_1(); //Text con tasti incrementali/decrementali con valore massimo OK
    //lv_example_spinner_1(); //Spinner OK
    //lv_example_span_1(); //Text area con testi con diversi caratteri e formattazioni OK
    
    //lv_example_flex_1(); //bottoni in fila verticale e orizzontale OK
    //lv_example_flex_5(); //tastiera con bottoni animata gommosa OK

    //lv_example_scroll_1(); //scroll innestati, casini vari ma funziona come deve OK
    //lv_example_scroll_2(); //scroll orizzontsle di bottoni e uno switck OK
    
    //lv_example_style_1(); //text view con HELLO OK
    //lv_example_animimg_1(); //animazione bicchiere caffè OK
    
    //lv_example_arc_2(); //spinner rotondo OK

    //lv_example_bar_1();  //bargraph tondina OK
    //lv_example_bar_2(); //bargraph molto quadrata OK
    //lv_example_bar_3(); //bargraph tondina verticale con gradiente colorato OK
    //lv_example_bar_5(); //bargraph con riempimenti da sx o dx OK
    //lv_example_bar_6(); //bargraph con percentuale di riempimento dentro la barra...OK
    //lv_example_bar_7(); //bargraph from top to bottom OK

    //lv_example_roller_1(); //scroll list con i mesi OK
    //lv_example_roller_2(); //scroll list con numeri e sfondo personalizzabili, anche il font cambia OK
    
    //lv_example_slider_1(); //slider basico con cursore tondo OK (a volte crasha)
    //lv_example_slider_3(); //slider con riempimento dal centro OK
    //lv_example_slider_4(); //slider basico con cursore tondo riempimento da destra OK (a volte crasha)
    
    //lv_example_win_1(); //OK

    //lv_example_observer_3(); //set orario OK
    //lv_example_observer_1(); //slider con valore temperatura OK

    //lv_example_image_1(); //immagine basica OK
    //lv_example_image_2(); //immagine basica con slider per cambiare RGB OK
    //lv_example_image_3(); //immagine animata che si muove nello spazio OK
    //lv_example_image_4(); //una roba strana tipo gif OK
    //lv_example_keyboard_1(); //due text area con una con placeholder
    

    //ESEMPI PROBLEMATICI
    //lv_example_anim_1(); //switch button works just at first pressure, then not working...display freeze?!
    //lv_example_arc_1(); //slider rotondo, ma si FREEZA CAZZO!
    //lv_example_observer_4(); //Observer Design Pattern https://youtu.be/NlU4DTx7_1k?si=08U7FqNFycIbeYaU&t=262 link per vederlo, NON FUNZIONA!
    //lv_example_bar_4(); //bargraph tondina ma NON FUNZIONA
    //lv_example_obj_2(); //Drag&drop bottone...NON FUNZIONA!
    //lv_example_roller_3(); //non lo carica, c'è scritto TODO nel codice
    //lv_example_slider_2(); //slider basico personalizzato, ma non si capisce se crasha...probabilmente si
    //lv_demo_widgets(); //Sul grafico circolare, Monthly Target, freeza
}