/**
 * @file element_process.c
 *
 */


//ESSENTIAL INCLUDES
#include <lvgl.h>
#include "../../include/definitions.h"

extern struct gui_components gui;

//ACCESSORY INCLUDES


void event_processElement(lv_event_t * e){
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t * obj = (lv_obj_t *)lv_event_get_target(e);
  lv_obj_t * cont = (lv_obj_t *)lv_event_get_current_target(e);

  if(obj == cont && cont != gui.element.process.processElementSummary && cont != gui.element.process.preferredIcon)
    return;


  if(obj == gui.element.process.preferredIcon){
      if(code == LV_EVENT_CLICKED) {   
        LV_LOG_USER("Process is preferred : %d",gui.element.process.isPreferred);
        if(gui.element.process.isPreferred == 0){
            lv_obj_set_style_text_color(gui.element.process.preferredIcon, lv_color_hex(RED), LV_PART_MAIN);
            gui.element.process.isPreferred = 1;
          }
          else{
            lv_obj_set_style_text_color(gui.element.process.preferredIcon, lv_color_hex(WHITE), LV_PART_MAIN);
            gui.element.process.isPreferred = 0;
          }
      }
  }
  
  if(obj == gui.element.process.processElementSummary){
      if(code == LV_EVENT_SHORT_CLICKED) {    
        LV_LOG_USER("Process Element Details");
        //processDetail(gui.element.process.processElement); //won't work, cause definition in pages.h. Here we want to call processDetail, but this is not defined, because in page.h is defined after element_process.h
      }
      if(code == LV_EVENT_LONG_PRESSED_REPEAT){
        if(mBoxPopupParent == NULL){
          LV_LOG_USER("Process Element Long Press for popup delete");
          messagePopupCreate(deletePopupTitle_text,deletePopupBody_text, deleteButton_text, stepDetailCancel_text, gui.element.process.processElement);
        }
      }
  }
   if(code == LV_EVENT_DELETE) {
        lv_style_reset(&gui.element.process.style);
    }
}


void processElementCreate(lv_obj_t * processesReferenceList){
  /*********************
  *    PAGE HEADER
  *********************/
  LV_LOG_USER("Processes Creation");
  

  gui.element.process.processElement = lv_obj_create(processesReferenceList);
  lv_obj_set_pos(gui.element.process.processElement, -10, -10 + ((gui.element.process.processCounter * 70)));                           
  lv_obj_set_size(gui.element.process.processElement, 315, 70);
  lv_obj_remove_flag(gui.element.process.processElement, LV_OBJ_FLAG_SCROLLABLE); 
  lv_obj_set_style_border_opa(gui.element.process.processElement, LV_OPA_TRANSP, 0);

  /*********************
  *    PAGE ELEMENTS
  *********************/
        gui.element.process.processElementSummary = lv_obj_create(gui.element.process.processElement);
        //lv_obj_set_style_border_color(gui.element.process.processElementSummary, lv_color_hex(LV_PALETTE_ORANGE), 0);
        lv_obj_set_size(gui.element.process.processElementSummary, 270, 66);
        lv_obj_align(gui.element.process.processElementSummary, LV_ALIGN_TOP_LEFT, -16, -16);
        lv_obj_remove_flag(gui.element.process.processElementSummary, LV_OBJ_FLAG_SCROLLABLE);  
        lv_obj_add_event_cb(gui.element.process.processElementSummary, event_processElement, LV_EVENT_SHORT_CLICKED, gui.element.process.processElementSummary);
        lv_obj_add_event_cb(gui.element.process.processElementSummary, event_processElement, LV_EVENT_LONG_PRESSED_REPEAT, gui.element.process.processElementSummary);
  

        lv_style_init(&gui.element.process.style);

        lv_style_set_bg_opa(&gui.element.process.style, LV_OPA_60);
        lv_style_set_bg_color(&gui.element.process.style, lv_color_hex(WHITE));
        lv_style_set_border_color(&gui.element.process.style, lv_color_hex(GREEN_DARK));
        lv_style_set_border_width(&gui.element.process.style, 4);
        lv_style_set_border_opa(&gui.element.process.style, LV_OPA_50);
        lv_style_set_border_side(&gui.element.process.style, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT);

        lv_obj_add_style(gui.element.process.processElementSummary, &gui.element.process.style, 0);

        gui.element.process.processName = lv_label_create(gui.element.process.processElementSummary);         
        lv_label_set_text(gui.element.process.processName, "Precipitevolissimevolmente"); 
        lv_obj_set_style_text_font(gui.element.process.processName, &lv_font_montserrat_22, 0);      
        lv_label_set_long_mode(gui.element.process.processName, LV_LABEL_LONG_SCROLL_CIRCULAR);
        lv_obj_set_width(gui.element.process.processName, 220);        
        lv_obj_align(gui.element.process.processName, LV_ALIGN_LEFT_MID, -10, -10);
        lv_obj_remove_flag(gui.element.process.processName, LV_OBJ_FLAG_SCROLLABLE); 

        gui.element.process.processTempIcon = lv_label_create(gui.element.process.processElementSummary);          
        lv_label_set_text(gui.element.process.processTempIcon, temp_icon);                  
        lv_obj_set_style_text_font(gui.element.process.processTempIcon, &FilMachineFontIcons_20, 0);
        //lv_obj_set_style_text_color(gui.element.process.processTempIcon, lv_color_hex(GREY), LV_PART_MAIN);
        lv_obj_align(gui.element.process.processTempIcon, LV_ALIGN_LEFT_MID, -10, 17);
        
        gui.element.process.processTemp = lv_label_create(gui.element.process.processElementSummary);         
        lv_label_set_text(gui.element.process.processTemp, "38.0 °C"); 
        lv_obj_set_style_text_font(gui.element.process.processTemp, &lv_font_montserrat_18, 0);              
        lv_obj_align(gui.element.process.processTemp, LV_ALIGN_LEFT_MID, 7, 17);

        gui.element.process.processTypeIcon = lv_label_create(gui.element.process.processElementSummary);     
        lv_label_set_text(gui.element.process.processTypeIcon, colorpalette_icon);     
        lv_obj_set_style_text_font(gui.element.process.processTypeIcon, &FilMachineFontIcons_20, 0);              
        lv_obj_align(gui.element.process.processTypeIcon, LV_ALIGN_RIGHT_MID, 7, 0);

        gui.element.process.preferredIcon = lv_label_create(gui.element.process.processElement);          
        lv_label_set_text(gui.element.process.preferredIcon, preferred_icon);                  
        lv_obj_set_style_text_font(gui.element.process.preferredIcon, &FilMachineFontIcons_30, 0);
        lv_obj_set_style_text_color(gui.element.process.preferredIcon, lv_color_hex(WHITE), LV_PART_MAIN);
        lv_obj_align(gui.element.process.preferredIcon, LV_ALIGN_RIGHT_MID, 15, 0); 
        lv_obj_add_flag(gui.element.process.preferredIcon, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_event_cb(gui.element.process.preferredIcon, event_processElement, LV_EVENT_CLICKED, gui.element.process.preferredIcon);


  gui.element.process.processCounter += 1;
}

