
/**
 * @file page_tankSize.h
 *
 */

#include "core/lv_obj_style.h"
#include "misc/lv_area.h"

#ifndef PAGE_TANKSIZE_H
#define PAGE_TANKSIZE_H

#ifdef __cplusplus
extern "C" {
#endif

//ESSENTIAL INCLUDES


//ACCESSORY INCLUDES


static void event_btn_clean_600(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);

  if(code == LV_EVENT_CLICKED) {
    previousState = currentState;
    currentState = Display_State_1_1_1_Cleaning_Tank;
    LV_LOG_USER("TANK SIZE Button PRESSED, new current state: %d",currentState);
    isEnteringInPage = 1;
  }  
}

static void event_btn_clean_1000(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);

  if(code == LV_EVENT_CLICKED) {
    previousState = currentState;
    currentState = Display_State_1_1_1_Cleaning_Tank;
    LV_LOG_USER("TANK SIZE Button PRESSED, new current state: %d",currentState);
    isEnteringInPage = 1;
  }  
}

static void event_btn_clean_custom(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);

  if(code == LV_EVENT_CLICKED) {
    previousState = currentState;
    currentState = Display_State_4_Tank;
    LV_LOG_USER("TANK SIZE Button PRESSED, new current state: %d",currentState);
    isEnteringInPage = 1;
  }  
}

static void event_btn_tankSize(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);

  if(code == LV_EVENT_CLICKED) {
    previousState = currentState;
    currentState = Display_State_11_Cleaning;
    LV_LOG_USER("TANK SIZE Button PRESSED, new current state: %d",currentState);
    isEnteringInPage = 1;
  }  
}       

static void event_tankSize_style_delete(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_DELETE) {
        //delete styles to prevent memoryleak
        lv_style_reset(&style_btn_back_tankSize);
        lv_style_reset(&style_btn_clean_600);
        lv_style_reset(&labelStyle_clean_600);
        lv_style_reset(&style_btn_clean_1000);
        lv_style_reset(&labelStyle_clean_1000);
        lv_style_reset(&style_btn_clean_custom);
        lv_style_reset(&labelStyle_clean_custom);
    }
}

//LV_SYMBOL_LEFT

void tankSizePage()
{  
    lv_obj_del(lv_screen_active());
    screen_tankSize = lv_obj_create(NULL);
    lv_scr_load(screen_tankSize);

    lv_obj_set_style_bg_color(screen_tankSize, LV_COLOR_MAKE(255, 255, 255), LV_PART_MAIN);

/*********************
 *    PAGE HEADER
 *********************/


    lv_obj_t * title = lv_label_create(screen_tankSize);
    lv_label_set_text(title, dev_clean_tankSize_title_text);
    lv_obj_set_style_text_color(screen_tankSize, lv_color_hex(BLACK), LV_PART_MAIN);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_22, 0);
    lv_obj_align(title, LV_ALIGN_TOP_LEFT, 10 , 10);  

    lv_obj_t * btn_back_tankSize = lv_button_create(screen_tankSize);
    lv_obj_set_size(btn_back_tankSize, 46, 46);
    //lv_obj_set_pos(btn_back, 320, 210); 
    lv_obj_align(btn_back_tankSize, LV_ALIGN_TOP_RIGHT, -10 , 10);    
    lv_obj_add_event_cb(btn_back_tankSize, event_btn_back, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(btn_back_tankSize, event_tankSize_style_delete, LV_EVENT_DELETE, NULL);

    lv_obj_t * label_btn_back_tankSize = lv_label_create(btn_back_tankSize);
    lv_label_set_text(label_btn_back_tankSize, LV_SYMBOL_LEFT);
    lv_obj_align(label_btn_back_tankSize, LV_ALIGN_CENTER, 0, 0);

    lv_style_init(&style_btn_back_tankSize);
    lv_style_set_bg_color(&style_btn_back_tankSize, lv_color_hex(GREEN));
    lv_style_set_border_color(&style_btn_back_tankSize, lv_color_hex(GREEN_DARK));
    lv_style_set_border_width(&style_btn_back_tankSize, 3);
    lv_style_set_shadow_width(&style_btn_back_tankSize, 10);
    lv_style_set_shadow_offset_y(&style_btn_back_tankSize, 5);
    lv_style_set_shadow_opa(&style_btn_back_tankSize, LV_OPA_50);
    lv_obj_add_style(btn_back_tankSize, &style_btn_back_tankSize, 0);



/*********************
 *    PAGE ELEMENTS
 *********************/
    


    lv_obj_t * btn_tankSize_600 = lv_button_create(screen_program);
    lv_obj_set_size(btn_tankSize_600, BUTTON_START_WIDTH, BUTTON_START_HEIGHT);
    //lv_obj_set_pos(btn_back, 320, 210); 
    lv_obj_align(btn_tankSize_600, LV_ALIGN_LEFT_MID, 10 , 0);    
    lv_obj_add_event_cb(btn_tankSize_600, event_btn_clean_600, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(btn_tankSize_600, event_tankSize_style_delete, LV_EVENT_DELETE, NULL);

    lv_style_init(&style_btn_clean_600);
    lv_style_set_bg_color(&style_btn_clean_600, lv_color_hex(LIGHT_BLUE));
    lv_style_set_border_color(&style_btn_clean_600, lv_color_hex(LIGHT_BLUE_DARK));
    lv_style_set_border_width(&style_btn_clean_600, 3);
    lv_style_set_shadow_width(&style_btn_clean_600, 10);
    lv_style_set_shadow_offset_y(&style_btn_clean_600, 5);
    lv_style_set_shadow_opa(&style_btn_clean_600, LV_OPA_50);
    lv_obj_add_style(btn_tankSize_600, &style_btn_clean_600, 0);

    lv_style_init(&labelStyle_clean_600);
    lv_obj_t * label_btn_clean_600 = lv_label_create(btn_tankSize_600);
    lv_label_set_text(label_btn_clean_600, button600ml_label);
    lv_obj_set_style_text_font(label_btn_clean_600, &lv_font_montserrat_22, 0);
    lv_style_set_text_color(&labelStyle_clean_600, lv_color_hex(WHITE));
    lv_obj_add_style(label_btn_clean_600, &labelStyle_clean_600, 0);
    lv_obj_align(label_btn_clean_600, LV_ALIGN_CENTER, 0, 0);

    

    lv_obj_t * btn_tankSize_1000 = lv_button_create(screen_program);
    lv_obj_set_size(btn_tankSize_1000, BUTTON_START_WIDTH, BUTTON_START_HEIGHT);
    //lv_obj_set_pos(btn_back, 320, 210); 
    lv_obj_align(btn_tankSize_1000, LV_ALIGN_CENTER, 0 , 0);
    lv_obj_add_event_cb(btn_tankSize_1000, event_btn_clean_1000, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(btn_tankSize_1000, event_tankSize_style_delete, LV_EVENT_DELETE, NULL);

    lv_style_init(&style_btn_clean_1000);
    lv_style_set_bg_color(&style_btn_clean_1000, lv_color_hex(LIGHT_BLUE));
    lv_style_set_border_color(&style_btn_clean_1000, lv_color_hex(LIGHT_BLUE_DARK));
    lv_style_set_border_width(&style_btn_clean_1000, 3);
    lv_style_set_shadow_width(&style_btn_clean_1000, 10);
    lv_style_set_shadow_offset_y(&style_btn_clean_1000, 5);
    lv_style_set_shadow_opa(&style_btn_clean_1000, LV_OPA_50);
    lv_obj_add_style(btn_tankSize_1000, &style_btn_clean_1000, 0);


    lv_style_init(&labelStyle_clean_1000);
    lv_obj_t * label_btn_clean_1000 = lv_label_create(btn_tankSize_1000);
    lv_label_set_text(label_btn_clean_1000, button1000ml_label);
    lv_obj_set_style_text_font(label_btn_clean_1000, &lv_font_montserrat_22, 0);
    lv_style_set_text_color(&labelStyle_clean_1000, lv_color_hex(WHITE));
    lv_obj_add_style(label_btn_clean_1000, &labelStyle_clean_1000, 0);
    lv_obj_align(label_btn_clean_1000, LV_ALIGN_CENTER, 0, 0);

    
    lv_obj_t * btn_clean_custom = lv_button_create(screen_program);
    lv_obj_set_size(btn_clean_custom, BUTTON_START_WIDTH, BUTTON_START_HEIGHT);
    //lv_obj_set_pos(btn_back, 320, 210); 
    lv_obj_align(btn_clean_custom, LV_ALIGN_RIGHT_MID, -10 , 0);
    lv_obj_add_event_cb(btn_clean_custom, event_btn_clean_custom, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(btn_clean_custom, event_tankSize_style_delete, LV_EVENT_DELETE, NULL);

    lv_style_init(&style_btn_clean_custom);
    lv_style_set_bg_color(&style_btn_clean_custom, lv_color_hex(GREEN));
    lv_style_set_border_color(&style_btn_clean_custom, lv_color_hex(GREEN_DARK));
    lv_style_set_border_width(&style_btn_clean_custom, 3);
    lv_style_set_shadow_width(&style_btn_clean_custom, 10);
    lv_style_set_shadow_offset_y(&style_btn_clean_custom, 5);
    lv_style_set_shadow_opa(&style_btn_clean_custom, LV_OPA_50);
    lv_obj_add_style(btn_clean_custom, &style_btn_clean_custom, 0);

    lv_style_init(&labelStyle_clean_custom);
    lv_obj_t * label_btn_clean_custom= lv_label_create(btn_clean_custom);
    lv_label_set_text(label_btn_clean_custom, buttonCustom_label);
    lv_obj_set_style_text_font(label_btn_clean_custom, &lv_font_montserrat_22, 0);
    lv_style_set_text_color(&labelStyle_clean_custom, lv_color_hex(WHITE));
    lv_obj_add_style(label_btn_clean_custom, &labelStyle_clean_custom, 0);
    lv_obj_align(label_btn_clean_custom, LV_ALIGN_CENTER, 0, 0);

    
}






#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*PAGE_TANKSIZE_H*/