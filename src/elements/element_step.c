/**
 * @file element_step.c
 *
 */


//ESSENTIAL INCLUDES
#include <lvgl.h>
#include "../../include/definitions.h"

extern struct gui_components gui;

//ACCESSORY INCLUDES


void event_stepElement(lv_event_t * e){
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t * obj = (lv_obj_t *)lv_event_get_target(e);
 
  

  if(obj == gui.element.step.stepElementSummary){
      if(code == LV_EVENT_CLICKED) {    
        LV_LOG_USER("Process Element Details");
        //stepDetail(gui.element.step.stepElement); //won't work, cause definition in pages.h. Here we want to call stepDetail, but this is not defined, because in page.h is defined after element_step.h
      }
      if(code == LV_EVENT_LONG_PRESSED_REPEAT) {    
        if(gui.element.messagePopup.mBoxPopupParent == NULL){
        LV_LOG_USER("Long press element");
        messagePopupCreate(deletePopupTitle_text,deletePopupBody_text, deleteButton_text, stepDetailCancel_text, gui.element.step.stepElement);
      }
    }
  }
  
  if(code == LV_EVENT_DELETE) {
        lv_style_reset(&gui.element.step.stepStyle);
  }
}


void stepElementCreate(lv_obj_t * stepContainer){
  /*********************
  *    PAGE HEADER
  *********************/
  LV_LOG_USER("Processes Creation");
  

  gui.element.step.stepElement = lv_obj_create(stepContainer);
  //lv_obj_set_pos(gui.element.step.stepElement, -13, -13 + ((gui.element.step.stepCounter * 70)));                           
  lv_obj_set_size(gui.element.step.stepElement, 240, 70);
  lv_obj_remove_flag(gui.element.step.stepElement, LV_OBJ_FLAG_SCROLLABLE); 
  lv_obj_set_style_border_opa(gui.element.step.stepElement, LV_OPA_TRANSP, 0);

  /*********************
  *    PAGE ELEMENTS
  *********************/
        gui.element.step.stepElementSummary = lv_obj_create(gui.element.step.stepElement);
        //lv_obj_set_style_border_color(gui.element.step.stepElementSummary, lv_color_hex(LV_PALETTE_ORANGE), 0);
        lv_obj_set_size(gui.element.step.stepElementSummary, 235, 66);
        lv_obj_align(gui.element.step.stepElementSummary, LV_ALIGN_TOP_LEFT, -16, -16);
        lv_obj_remove_flag(gui.element.step.stepElementSummary, LV_OBJ_FLAG_SCROLLABLE);  
        lv_obj_add_event_cb(gui.element.step.stepElementSummary, event_stepElement, LV_EVENT_CLICKED, gui.element.step.stepElementSummary);  
        lv_obj_add_event_cb(gui.element.step.stepElementSummary, event_stepElement, LV_EVENT_LONG_PRESSED_REPEAT, gui.element.step.stepElementSummary);
        lv_style_init(&gui.element.step.stepStyle);

        lv_style_set_bg_opa(&gui.element.step.stepStyle, LV_OPA_60);
        lv_style_set_bg_color(&gui.element.step.stepStyle, lv_color_hex(WHITE));
        lv_style_set_border_color(&gui.element.step.stepStyle, lv_color_hex(GREEN_DARK));
        lv_style_set_border_width(&gui.element.step.stepStyle, 4);
        lv_style_set_border_opa(&gui.element.step.stepStyle, LV_OPA_50);
        lv_style_set_border_side(&gui.element.step.stepStyle, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT);

        lv_obj_add_style(gui.element.step.stepElementSummary, &gui.element.step.stepStyle, 0);

        gui.element.step.stepName = lv_label_create(gui.element.step.stepElementSummary);         
        lv_label_set_text(gui.element.step.stepName, "Precipitevolissimevolmente"); 
        lv_obj_set_style_text_font(gui.element.step.stepName, &lv_font_montserrat_22, 0);      
        lv_label_set_long_mode(gui.element.step.stepName, LV_LABEL_LONG_SCROLL_CIRCULAR);
        lv_obj_set_width(gui.element.step.stepName, 190);        
        lv_obj_align(gui.element.step.stepName, LV_ALIGN_LEFT_MID, -10, -10);
        lv_obj_remove_flag(gui.element.step.stepName, LV_OBJ_FLAG_SCROLLABLE); 

        gui.element.step.stepTimeIcon = lv_label_create(gui.element.step.stepElementSummary);          
        lv_label_set_text(gui.element.step.stepTimeIcon, clock_Icon);                  
        lv_obj_set_style_text_font(gui.element.step.stepTimeIcon, &FilMachineFontIcons_20, 0);
        //lv_obj_set_style_text_color(gui.element.step.stepTimeIcon, lv_color_hex(GREY), LV_PART_MAIN);
        lv_obj_align(gui.element.step.stepTimeIcon, LV_ALIGN_LEFT_MID, -10, 17);
        
        gui.element.step.stepTime = lv_label_create(gui.element.step.stepElementSummary);         
        lv_label_set_text(gui.element.step.stepTime, "6m30s"); 
        lv_obj_set_style_text_font(gui.element.step.stepTime, &lv_font_montserrat_18, 0);              
        lv_obj_align(gui.element.step.stepTime, LV_ALIGN_LEFT_MID, 10, 17);

        gui.element.step.stepTypeIcon = lv_label_create(gui.element.step.stepElementSummary);     
        lv_label_set_text(gui.element.step.stepTypeIcon, chemical_Icon);     
        lv_obj_set_style_text_font(gui.element.step.stepTypeIcon, &FilMachineFontIcons_20, 0);              
        lv_obj_align(gui.element.step.stepTypeIcon, LV_ALIGN_RIGHT_MID, 9, 0);


  //gui.element.step.stepCounter += 1;
}

