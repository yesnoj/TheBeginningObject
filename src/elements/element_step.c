/**
 * @file element_step.c
 *
 */


//ESSENTIAL INCLUDES
#include <lvgl.h>
#include "../../include/definitions.h"

extern struct gui_components gui;

//ACCESSORY INCLUDES

static lv_style_t stepStyle;
static lv_obj_t * stepElementSummary;

static lv_obj_t * stepName;
static lv_obj_t * stepTime;
static lv_obj_t * stepTimeIcon;
static lv_obj_t * stepTypeIcon;


void event_stepElement(lv_event_t * e){
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t * obj = (lv_obj_t *)lv_event_get_target(e);
 
  

  if(obj == stepElementSummary){
      if(code == LV_EVENT_CLICKED) {    
        LV_LOG_USER("Process Element Details");
        //stepDetail(stepElement); //won't work, cause definition in pages.h. Here we want to call stepDetail, but this is not defined, because in page.h is defined after element_step.h
      }
      if(code == LV_EVENT_LONG_PRESSED_REPEAT) {    
        if(mBoxPopupParent == NULL){
        LV_LOG_USER("Long press element");
        messagePopupCreate(deletePopupTitle_text,deletePopupBody_text, deleteButton_text, stepDetailCancel_text, stepElement);
      }
    }
  }
  
  if(code == LV_EVENT_DELETE) {
        lv_style_reset(&stepStyle);
  }
}


void stepElementCreate(lv_obj_t * stepContainer){
  /*********************
  *    PAGE HEADER
  *********************/
  LV_LOG_USER("Processes Creation");
  

  stepElement = lv_obj_create(stepContainer);
  lv_obj_set_pos(stepElement, -13, -13 + ((stepCounter * 70)));                           
  lv_obj_set_size(stepElement, 240, 70);
  lv_obj_remove_flag(stepElement, LV_OBJ_FLAG_SCROLLABLE); 
  lv_obj_set_style_border_opa(stepElement, LV_OPA_TRANSP, 0);

  /*********************
  *    PAGE ELEMENTS
  *********************/
        stepElementSummary = lv_obj_create(stepElement);
        //lv_obj_set_style_border_color(stepElementSummary, lv_color_hex(LV_PALETTE_ORANGE), 0);
        lv_obj_set_size(stepElementSummary, 235, 66);
        lv_obj_align(stepElementSummary, LV_ALIGN_TOP_LEFT, -16, -16);
        lv_obj_remove_flag(stepElementSummary, LV_OBJ_FLAG_SCROLLABLE);  
        lv_obj_add_event_cb(stepElementSummary, event_stepElement, LV_EVENT_CLICKED, stepElementSummary);  
        lv_obj_add_event_cb(stepElementSummary, event_stepElement, LV_EVENT_LONG_PRESSED_REPEAT, stepElementSummary);
        lv_style_init(&stepStyle);

        lv_style_set_bg_opa(&stepStyle, LV_OPA_60);
        lv_style_set_bg_color(&stepStyle, lv_color_hex(WHITE));
        lv_style_set_border_color(&stepStyle, lv_color_hex(GREEN_DARK));
        lv_style_set_border_width(&stepStyle, 4);
        lv_style_set_border_opa(&stepStyle, LV_OPA_50);
        lv_style_set_border_side(&stepStyle, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT);

        lv_obj_add_style(stepElementSummary, &stepStyle, 0);

        stepName = lv_label_create(stepElementSummary);         
        lv_label_set_text(stepName, "Precipitevolissimevolmente"); 
        lv_obj_set_style_text_font(stepName, &lv_font_montserrat_22, 0);      
        lv_label_set_long_mode(stepName, LV_LABEL_LONG_SCROLL_CIRCULAR);
        lv_obj_set_width(stepName, 190);        
        lv_obj_align(stepName, LV_ALIGN_LEFT_MID, -10, -10);
        lv_obj_remove_flag(stepName, LV_OBJ_FLAG_SCROLLABLE); 

        stepTimeIcon = lv_label_create(stepElementSummary);          
        lv_label_set_text(stepTimeIcon, clock_Icon);                  
        lv_obj_set_style_text_font(stepTimeIcon, &FilMachineFontIcons_20, 0);
        //lv_obj_set_style_text_color(stepTimeIcon, lv_color_hex(GREY), LV_PART_MAIN);
        lv_obj_align(stepTimeIcon, LV_ALIGN_LEFT_MID, -10, 17);
        
        stepTime = lv_label_create(stepElementSummary);         
        lv_label_set_text(stepTime, "6m30s"); 
        lv_obj_set_style_text_font(stepTime, &lv_font_montserrat_18, 0);              
        lv_obj_align(stepTime, LV_ALIGN_LEFT_MID, 10, 17);

        stepTypeIcon = lv_label_create(stepElementSummary);     
        lv_label_set_text(stepTypeIcon, chemical_Icon);     
        lv_obj_set_style_text_font(stepTypeIcon, &FilMachineFontIcons_20, 0);              
        lv_obj_align(stepTypeIcon, LV_ALIGN_RIGHT_MID, 9, 0);


  stepCounter += 1;
}

