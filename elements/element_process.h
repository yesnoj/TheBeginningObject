#include <sys/_stdint.h>
#include "misc/lv_palette.h"
#include "core/lv_obj_pos.h"
#include "misc/lv_color.h"
#include "misc/lv_types.h"
/**
 * @file element_process.h
 *
 */
#include "widgets/menu/lv_menu.h"
#include "core/lv_obj_style.h"
#include "misc/lv_area.h"

#ifndef ELEMENT_PROCESS_H
#define ELEMENT_PROCESS_H

#ifdef __cplusplus
extern "C" {
#endif

//ESSENTIAL INCLUDES

//ACCESSORY INCLUDES

static lv_style_t style;
static lv_obj_t * preferredIcon;
static lv_obj_t * processElementSummary;

static lv_obj_t * processName;
static lv_obj_t * processTemp;
static lv_obj_t * processTempIcon;
static lv_obj_t * processTypeIcon;



static void event_processElement(lv_event_t * e){
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t * obj = (lv_obj_t *)lv_event_get_target(e);
  lv_obj_t * cont = (lv_obj_t *)lv_event_get_current_target(e);

  if(obj == cont && cont != processElementSummary && cont != preferredIcon)
    return;


  if(obj == preferredIcon){
      if(code == LV_EVENT_CLICKED) {   
        LV_LOG_USER("Process is preferred : %d",isPreferred);
        if(isPreferred == 0){
            lv_obj_set_style_text_color(preferredIcon, lv_color_hex(RED), LV_PART_MAIN);
            isPreferred = 1;
          }
          else{
            lv_obj_set_style_text_color(preferredIcon, lv_color_hex(WHITE), LV_PART_MAIN);
            isPreferred = 0;
          }
      }
  }
  
  if(obj == processElementSummary){
      if(code == LV_EVENT_SHORT_CLICKED) {    
        LV_LOG_USER("Process Element Details");
        //processDetail(processElement); //won't work, cause definition in pages.h. Here we want to call processDetail, but this is not defined, because in page.h is defined after element_process.h
      }
      if(code == LV_EVENT_LONG_PRESSED_REPEAT){
        if(mBoxPopupParent == NULL){
          LV_LOG_USER("Process Element Long Press for popup delete");
          messagePopupCreate(deletePopupTitle_text,deletePopupBody_text, deleteButton_text, stepDetailCancel_text, processElement);
        }
      }
  }
   if(code == LV_EVENT_DELETE) {
        lv_style_reset(&style);
    }
}


void processElementCreate(lv_obj_t * processesReferenceList){
  /*********************
  *    PAGE HEADER
  *********************/
  LV_LOG_USER("Processes Creation");
  

  processElement = lv_obj_create(processesReferenceList);
  lv_obj_set_pos(processElement, -10, -10 + ((processCounter * 70)));                           
  lv_obj_set_size(processElement, 315, 70);
  lv_obj_remove_flag(processElement, LV_OBJ_FLAG_SCROLLABLE); 
  lv_obj_set_style_border_opa(processElement, LV_OPA_TRANSP, 0);

  /*********************
  *    PAGE ELEMENTS
  *********************/
        processElementSummary = lv_obj_create(processElement);
        //lv_obj_set_style_border_color(processElementSummary, lv_color_hex(LV_PALETTE_ORANGE), 0);
        lv_obj_set_size(processElementSummary, 270, 66);
        lv_obj_align(processElementSummary, LV_ALIGN_TOP_LEFT, -16, -16);
        lv_obj_remove_flag(processElementSummary, LV_OBJ_FLAG_SCROLLABLE);  
        lv_obj_add_event_cb(processElementSummary, event_processElement, LV_EVENT_SHORT_CLICKED, processElementSummary);
        lv_obj_add_event_cb(processElementSummary, event_processElement, LV_EVENT_LONG_PRESSED_REPEAT, processElementSummary);
  

        lv_style_init(&style);

        lv_style_set_bg_opa(&style, LV_OPA_60);
        lv_style_set_bg_color(&style, lv_color_hex(WHITE));
        lv_style_set_border_color(&style, lv_color_hex(GREEN_DARK));
        lv_style_set_border_width(&style, 4);
        lv_style_set_border_opa(&style, LV_OPA_50);
        lv_style_set_border_side(&style, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT);

        lv_obj_add_style(processElementSummary, &style, 0);

        processName = lv_label_create(processElementSummary);         
        lv_label_set_text(processName, "Precipitevolissimevolmente"); 
        lv_obj_set_style_text_font(processName, &lv_font_montserrat_22, 0);      
        lv_label_set_long_mode(processName, LV_LABEL_LONG_SCROLL_CIRCULAR);
        lv_obj_set_width(processName, 220);        
        lv_obj_align(processName, LV_ALIGN_LEFT_MID, -10, -10);
        lv_obj_remove_flag(processName, LV_OBJ_FLAG_SCROLLABLE); 

        processTempIcon = lv_label_create(processElementSummary);          
        lv_label_set_text(processTempIcon, temp_icon);                  
        lv_obj_set_style_text_font(processTempIcon, &FilMachineFontIcons_20, 0);
        //lv_obj_set_style_text_color(processTempIcon, lv_color_hex(GREY), LV_PART_MAIN);
        lv_obj_align(processTempIcon, LV_ALIGN_LEFT_MID, -10, 17);
        
        processTemp = lv_label_create(processElementSummary);         
        lv_label_set_text(processTemp, "38.0 °C"); 
        lv_obj_set_style_text_font(processTemp, &lv_font_montserrat_18, 0);              
        lv_obj_align(processTemp, LV_ALIGN_LEFT_MID, 7, 17);

        processTypeIcon = lv_label_create(processElementSummary);     
        lv_label_set_text(processTypeIcon, colorpalette_icon);     
        lv_obj_set_style_text_font(processTypeIcon, &FilMachineFontIcons_20, 0);              
        lv_obj_align(processTypeIcon, LV_ALIGN_RIGHT_MID, 7, 0);

        preferredIcon = lv_label_create(processElement);          
        lv_label_set_text(preferredIcon, preferred_icon);                  
        lv_obj_set_style_text_font(preferredIcon, &FilMachineFontIcons_30, 0);
        lv_obj_set_style_text_color(preferredIcon, lv_color_hex(WHITE), LV_PART_MAIN);
        lv_obj_align(preferredIcon, LV_ALIGN_RIGHT_MID, 15, 0); 
        lv_obj_add_flag(preferredIcon, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_event_cb(preferredIcon, event_processElement, LV_EVENT_CLICKED, preferredIcon);


  processCounter += 1;
}




#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*ELEMENT_PROCESS_H*/