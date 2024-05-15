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

  if(code == LV_EVENT_CLICKED) {
   menu();
  }   
} 


void homePage(void)
{
    lv_obj_del(lv_screen_active());
    screen_home = lv_obj_create(NULL);
    lv_scr_load(screen_home);

    lv_obj_t * logo = lv_img_create(lv_scr_act());
    lv_img_set_src(logo, &splash_img);
    lv_obj_align(logo, LV_ALIGN_CENTER, 0 , 0);
    //lv_img_set_zoom(logo, 128);

    
    //lv_obj_t * img_obj = lv_img_create(lv_scr_act(), NULL); 
    //lv_img_set_src(img_obj, "/sd/images/04.png");

    lv_obj_t * btn_start = lv_obj_create(screen_home);
    lv_obj_align(btn_start, LV_ALIGN_BOTTOM_RIGHT, -10 , -7);                  
    lv_obj_set_size(btn_start, 60, 60);   
    lv_obj_add_event_cb(btn_start, event_btn_start, LV_EVENT_CLICKED, btn_start);
    lv_obj_set_style_bg_opa(btn_start, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_opa(btn_start, LV_OPA_TRANSP, 0);
    lv_obj_remove_flag(btn_start, LV_OBJ_FLAG_SCROLLABLE);     
}
