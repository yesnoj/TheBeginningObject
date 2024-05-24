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

LV_IMG_DECLARE(splash_img);

void event_btn_start(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);

  if(code == LV_EVENT_CLICKED) {
   menu();
  }   
} 


void homePage(void)
{
    init_globals();
  
    lv_obj_del(lv_screen_active());

    gui.page.home.screen_home = lv_obj_create(NULL);

    lv_scr_load(gui.page.home.screen_home);

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
