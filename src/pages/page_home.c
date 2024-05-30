/**
 * @file page_home.c
 *
 */

//ESSENTIAL INCLUDE
#include <lvgl.h>
#include "../../resources/splash.c"
#include "../../include/definitions.h"


extern struct gui_components gui;
//ACCESSORY INCLUDES


void event_btn_start(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);
  LV_LOG_USER("initErrors %d",initErrors);
  if(code == LV_EVENT_CLICKED) {
   if(initErrors == 0){
      menu();
      }
    else
      rebootBoard();
  }   
} 



void homePage(void)
{
    init_globals();

    lv_obj_del(lv_screen_active());

    gui.page.home.screen_home = lv_obj_create(NULL);

    lv_scr_load(gui.page.home.screen_home);
    
    if(initErrors == 0){
        gui.page.home.splashImage = lv_img_create(gui.page.home.screen_home);
        lv_img_set_src(gui.page.home.splashImage, &splash_img);
        lv_obj_align(gui.page.home.splashImage, LV_ALIGN_CENTER, 0 , 0);
        //lv_img_set_zoom(logo, 128);

        gui.page.home.startButton = lv_obj_create(gui.page.home.screen_home);
        lv_obj_align(gui.page.home.startButton, LV_ALIGN_BOTTOM_RIGHT, -10 , -7);                  
        lv_obj_set_size(gui.page.home.startButton, 60, 60);   
        lv_obj_add_event_cb(gui.page.home.startButton, event_btn_start, LV_EVENT_CLICKED, gui.page.home.startButton);
        lv_obj_set_style_bg_opa(gui.page.home.startButton, LV_OPA_TRANSP, 0);
        lv_obj_set_style_border_opa(gui.page.home.startButton, LV_OPA_TRANSP, 0);
        lv_obj_remove_flag(gui.page.home.startButton, LV_OBJ_FLAG_SCROLLABLE);   
      }
      else{
          
          lv_obj_set_style_bg_color(lv_screen_active(), lv_color_hex(RED), LV_PART_MAIN);

          gui.page.home.startButton = lv_obj_create(gui.page.home.screen_home);
          lv_obj_align(gui.page.home.startButton, LV_ALIGN_CENTER, 0 , -30);                  
          lv_obj_set_size(gui.page.home.startButton, 150, 150);   
          lv_obj_add_event_cb(gui.page.home.startButton, event_btn_start, LV_EVENT_CLICKED, gui.page.home.startButton);
          lv_obj_set_style_bg_color(gui.page.home.startButton, lv_color_hex(RED), LV_PART_MAIN);
          lv_obj_set_style_border_opa(gui.page.home.startButton, LV_OPA_TRANSP, 0);

          gui.page.home.errorButtonLabel = lv_label_create(gui.page.home.startButton);         
          lv_label_set_text(gui.page.home.errorButtonLabel, sdCard_icon);
          lv_obj_set_style_text_font(gui.page.home.errorButtonLabel, &FilMachineFontIcons_100, 0);              
          lv_obj_align(gui.page.home.errorButtonLabel, LV_ALIGN_CENTER, 0, 0);
          
          gui.page.home.errorLabel = lv_label_create(gui.page.home.screen_home);
          lv_obj_align(gui.page.home.errorLabel, LV_ALIGN_CENTER, 0 , 90);     
          lv_label_set_text(gui.page.home.errorLabel, initError_text); 
          lv_obj_set_style_text_font(gui.page.home.errorLabel, &lv_font_montserrat_20, 0);              
          lv_obj_set_style_text_align(gui.page.home.errorLabel , LV_TEXT_ALIGN_CENTER, 0);


      }
}
