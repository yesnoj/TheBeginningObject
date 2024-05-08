/**
 * @file page_program.h
 *
 */

#include "core/lv_obj_style.h"
#include "misc/lv_area.h"

#ifndef PAGE_PROGRAM_H
#define PAGE_PROGRAM_H

#ifdef __cplusplus
extern "C" {
#endif

//ESSENTIAL INCLUDES


//ACCESSORY INCLUDES


static void event_btn_clean(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);

  if(code == LV_EVENT_CLICKED) {
    previousState = currentState;
    currentState = Display_State_1_1_Cleaning;
    LV_LOG_USER("CLEAN Button PRESSED, new current state: %d",currentState);
    isEnteringInPage = 1;
  }   
}       

static void event_btn_develop(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);

  if(code == LV_EVENT_CLICKED) {
    previousState = currentState;
    currentState = Display_State_1_2_Develop;
    LV_LOG_USER("DEVELOP Button PRESSED, new current state: %d",currentState);
    isEnteringInPage = 1;
  }   
}    

static void event_btn_mdc(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);

  if(code == LV_EVENT_CLICKED) {
    previousState = currentState;
    currentState = Display_State_1_3_MDC;
    LV_LOG_USER("MDC Button PRESSED, new current state: %d",currentState);
    isEnteringInPage = 1;

  }  
}    


static void event_program_style_delete(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_DELETE) {
        //delete styles to prevent memoryleak
        lv_style_reset(&style_btn_back_program);
        lv_style_reset(&style_btn_clean);
        
        lv_style_reset(&labelStyle_clean);
        lv_style_reset(&style_btn_develop);
        lv_style_reset(&labelStyle_develop);
        lv_style_reset(&style_btn_mdc);
        lv_style_reset(&labelStyle_mdc);
    }
}


//LV_SYMBOL_LEFT

void programPage()
{  
    lv_obj_del(lv_screen_active());
    screen_program = lv_obj_create(NULL);
    lv_scr_load(screen_program);

    lv_obj_set_style_bg_color(screen_program, LV_COLOR_MAKE(255, 255, 255), LV_PART_MAIN);

/*********************
 *    PAGE HEADER
 *********************/

    lv_obj_t * title = lv_label_create(screen_program);
    lv_label_set_text(title, "What do you want to do?!");
    lv_obj_set_style_text_color(screen_program, lv_color_hex(BLACK), LV_PART_MAIN);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_22, 0);
    lv_obj_align(title, LV_ALIGN_TOP_LEFT, 10 , 10);  

    lv_obj_t * btn_back = lv_button_create(screen_program);
    lv_obj_set_size(btn_back, 46, 46);
    //lv_obj_set_pos(btn_back, 320, 210); 
    lv_obj_align(btn_back, LV_ALIGN_TOP_RIGHT, -10 , 10);    
    lv_obj_add_event_cb(btn_back, event_btn_back, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(btn_back, event_program_style_delete, LV_EVENT_DELETE, NULL);

    lv_obj_t * label_btn_back = lv_label_create(btn_back);
    lv_label_set_text(label_btn_back, LV_SYMBOL_LEFT);
    lv_obj_align(label_btn_back, LV_ALIGN_CENTER, 0, 0);

    lv_style_init(&style_btn_back_program);
    lv_style_set_bg_color(&style_btn_back_program, lv_color_hex(GREEN));
    lv_style_set_border_color(&style_btn_back_program, lv_color_hex(GREEN_DARK));
    lv_style_set_border_width(&style_btn_back_program, 3);
    lv_style_set_shadow_width(&style_btn_back_program, 10);
    lv_style_set_shadow_offset_y(&style_btn_back_program, 5);
    lv_style_set_shadow_opa(&style_btn_back_program, LV_OPA_50);
    lv_obj_add_style(btn_back, &style_btn_back_program, 0);


/*********************
 *    PAGE ELEMENTS
 *********************/
    
    lv_obj_t * btn_clean = lv_button_create(screen_program);
    lv_obj_set_size(btn_clean, BUTTON_START_WIDTH, BUTTON_START_HEIGHT);
    //lv_obj_set_pos(btn_back, 320, 210); 
    lv_obj_align(btn_clean, LV_ALIGN_LEFT_MID, 10 , 0);    
    lv_obj_add_event_cb(btn_clean, event_btn_clean, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(btn_clean, event_program_style_delete, LV_EVENT_DELETE, NULL);


    lv_style_init(&style_btn_clean);
    lv_style_set_bg_color(&style_btn_clean, lv_color_hex(BLUE));
    lv_style_set_border_color(&style_btn_clean, lv_color_hex(BLUE_DARK));
    lv_style_set_border_width(&style_btn_clean, 3);
    lv_style_set_shadow_width(&style_btn_clean, 10);
    lv_style_set_shadow_offset_y(&style_btn_clean, 5);
    lv_style_set_shadow_opa(&style_btn_clean, LV_OPA_50);
    lv_obj_add_style(btn_clean, &style_btn_clean, 0);

    lv_style_init(&labelStyle_clean);
    lv_obj_t * label_btn_clean = lv_label_create(btn_clean);
    lv_label_set_text(label_btn_clean, "CLEAN");
    lv_obj_set_style_text_font(label_btn_clean, &lv_font_montserrat_22, 0);
    lv_style_set_text_color(&labelStyle_clean, lv_color_hex(WHITE));
    lv_obj_add_style(label_btn_clean, &labelStyle_clean, 0);
    lv_obj_align(label_btn_clean, LV_ALIGN_CENTER, 0, 0);

    

    lv_obj_t * btn_develop = lv_button_create(screen_program);
    lv_obj_set_size(btn_develop, BUTTON_START_WIDTH, BUTTON_START_HEIGHT);
    //lv_obj_set_pos(btn_back, 320, 210); 
    lv_obj_align(btn_develop, LV_ALIGN_CENTER, 0 , 0);
    lv_obj_add_event_cb(btn_develop, event_btn_develop, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(btn_clean, event_program_style_delete, LV_EVENT_DELETE, NULL);

    
    lv_style_init(&style_btn_develop);
    lv_style_set_bg_color(&style_btn_develop, lv_color_hex(GREEN));
    lv_style_set_border_color(&style_btn_develop, lv_color_hex(GREEN_DARK));
    lv_style_set_border_width(&style_btn_develop, 3);
    lv_style_set_shadow_width(&style_btn_develop, 10);
    lv_style_set_shadow_offset_y(&style_btn_develop, 5);
    lv_style_set_shadow_opa(&style_btn_develop, LV_OPA_50);
    lv_obj_add_style(btn_develop, &style_btn_develop, 0);


    lv_style_init(&labelStyle_develop);
    lv_obj_t * label_btn_develop = lv_label_create(btn_develop);
    lv_label_set_text(label_btn_develop, "DEVELOP");
    lv_obj_set_style_text_font(label_btn_develop, &lv_font_montserrat_22, 0);
    lv_style_set_text_color(&labelStyle_develop, lv_color_hex(WHITE));
    lv_obj_add_style(label_btn_develop, &labelStyle_develop, 0);
    lv_obj_align(label_btn_develop, LV_ALIGN_CENTER, 0, 0);

    
    lv_obj_t * btn_mdc = lv_button_create(screen_program);
    lv_obj_set_size(btn_mdc, BUTTON_START_WIDTH, BUTTON_START_HEIGHT);
    //lv_obj_set_pos(btn_back, 320, 210); 
    lv_obj_align(btn_mdc, LV_ALIGN_RIGHT_MID, -10 , 0);
    lv_obj_add_event_cb(btn_mdc, event_btn_mdc, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(btn_mdc, event_program_style_delete, LV_EVENT_DELETE, NULL);

    
    lv_style_init(&style_btn_mdc);
    lv_style_set_bg_color(&style_btn_mdc, lv_color_hex(ORANGE));
    lv_style_set_border_color(&style_btn_mdc, lv_color_hex(ORANGE_DARK));
    lv_style_set_border_width(&style_btn_mdc, 3);
    lv_style_set_shadow_width(&style_btn_mdc, 10);
    lv_style_set_shadow_offset_y(&style_btn_mdc, 5);
    lv_style_set_shadow_opa(&style_btn_mdc, LV_OPA_50);
    lv_obj_add_style(btn_mdc, &style_btn_mdc, 0);

    lv_style_init(&labelStyle_mdc);
    lv_obj_t * label_btn_mdc= lv_label_create(btn_mdc);
    lv_label_set_text(label_btn_mdc, "MDC DB");
    lv_obj_set_style_text_font(label_btn_mdc, &lv_font_montserrat_22, 0);
    lv_style_set_text_color(&labelStyle_mdc, lv_color_hex(WHITE));
    lv_obj_add_style(label_btn_mdc, &labelStyle_develop, 0);
    lv_obj_align(label_btn_mdc, LV_ALIGN_CENTER, 0, 0);
}






#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*PAGE_PROGRAM_H*/