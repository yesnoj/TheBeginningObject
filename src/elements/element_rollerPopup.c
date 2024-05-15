/**
 * @file element_rollerPopup.c
 *
 */


//ESSENTIAL INCLUDES
#include <lvgl.h>
#include "../../include/definitions.h"

extern struct gui_components gui;

//ACCESSORY INCLUDES

/*
    if(act_cb == gui.page.settings.tempSensorTuneButton){
      if(code == LV_EVENT_SHORT_CLICKED) {
          LV_LOG_USER("TUNE short click");
          rollerPopupCreate(gui.element.rollerPopup.tempCelsiusOptions,tuneTempPopupTitle_text,gui.page.settings.tempSensorTuneButton);
        }
      if(code == LV_EVENT_LONG_PRESSED_REPEAT) {
          LV_LOG_USER("TUNE Long click");
        }
    }
*/



void event_Roller(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t * obj = (lv_obj_t *)lv_event_get_target(e);
  lv_obj_t * objCont = (lv_obj_t *)lv_obj_get_parent(obj);
  lv_obj_t * mboxCont = (lv_obj_t *)lv_obj_get_parent(objCont);
  lv_obj_t * godFatherCont = (lv_obj_t *)lv_obj_get_parent(mboxCont);
  lv_obj_t * data = (lv_obj_t *)lv_event_get_user_data(e);

    if(code == LV_EVENT_CLICKED){
        if(obj == gui.element.rollerPopup.mBoxRollerButton)
        {
            if(data == gui.page.settings.tempSensorTuneButton){
              LV_LOG_USER("SET BUTTON from tempSensorTuneButton value %d:",rollerSelected);
              lv_style_reset(&gui.element.rollerPopup.style_mBoxRollerTitleLine);
              lv_style_reset(&gui.element.rollerPopup.style_roller);
              lv_msgbox_close(godFatherCont);
              lv_obj_delete(godFatherCont);
              gui.page.settings.calibratedTemp = rollerSelected;     
            }
            if(data == gui.page.processDetail.processTempTextArea){
              LV_LOG_USER("SET BUTTON from processTempTextArea value %d:",rollerSelected);
              itoa(rollerSelected, tempBuffer, 10);
              lv_style_reset(&gui.element.rollerPopup.style_mBoxRollerTitleLine);
              lv_textarea_set_text(gui.page.processDetail.processTempTextArea,tempBuffer);
              lv_style_reset(&gui.element.rollerPopup.style_roller);
              
              lv_msgbox_close(godFatherCont);
              lv_obj_delete(godFatherCont);
            }
            if(data == gui.page.processDetail.processToleranceTextArea){
              LV_LOG_USER("SET BUTTON from processToleranceTextArea value %d:",rollerSelected);
              
              lv_style_reset(&gui.element.rollerPopup.style_mBoxRollerTitleLine);
              lv_textarea_set_text(gui.page.processDetail.processToleranceTextArea, tempBuffer);
              lv_style_reset(&gui.element.rollerPopup.style_roller);
              lv_msgbox_close(godFatherCont);
              lv_obj_delete(godFatherCont);
            }
            if(data == gui.page.stepDetail.stepDetailMinTextArea){
              LV_LOG_USER("SET BUTTON from stepDetailMinTextArea value %d:",rollerSelected);
              
              lv_style_reset(&gui.element.rollerPopup.style_mBoxRollerTitleLine);
              lv_textarea_set_text(gui.page.stepDetail.stepDetailMinTextArea, tempBuffer);
              lv_style_reset(&gui.element.rollerPopup.style_roller);
              lv_msgbox_close(godFatherCont);
              lv_obj_delete(godFatherCont);
            }
            if(data == gui.page.stepDetail.stepDetailSecTextArea){
              LV_LOG_USER("SET BUTTON from stepDetailSecTextArea value %d:",rollerSelected);
              
              lv_style_reset(&gui.element.rollerPopup.style_mBoxRollerTitleLine);
              lv_textarea_set_text(gui.page.stepDetail.stepDetailSecTextArea, tempBuffer);
              lv_style_reset(&gui.element.rollerPopup.style_roller);
              lv_msgbox_close(godFatherCont);
              lv_obj_delete(godFatherCont);
            }
            if(data == gui.page.checkup.checkupTankSizeTextArea){
              LV_LOG_USER("SET BUTTON from checkupTankSizeTextArea value %d:",rollerSelected);
              
              lv_style_reset(&gui.element.rollerPopup.style_mBoxRollerTitleLine);
              lv_textarea_set_text(gui.page.checkup.checkupTankSizeTextArea, tempBuffer);
              lv_style_reset(&gui.element.rollerPopup.style_roller);
              lv_msgbox_close(godFatherCont);
              lv_obj_delete(godFatherCont);
            }
        }
    }

    if(code == LV_EVENT_VALUE_CHANGED){
      if(obj == gui.element.rollerPopup.roller){
          //if we want to want the index of the selected element
          rollerSelected = lv_roller_get_selected(obj) + 1;
          LV_LOG_USER("ROLLER value: %d", rollerSelected);

          
          lv_roller_get_selected_str(obj, tempBuffer, sizeof(tempBuffer));
          //if gui.element.rollerPopup.roller has strings to be read
          //lv_roller_get_selected_str(obj, rollerElementSelected, sizeof(rollerElementSelected));
          //LV_LOG_USER("ROLLER value: %s", rollerElementSelected);
      }
    }
}       


void rollerPopupCreate(const char * tempOptions,const char * popupTitle, lv_obj_t *whoCallMe){
  /*********************
  *    PAGE HEADER
  *********************/
   LV_LOG_USER("Roller popup create");  
   gui.element.rollerPopup.whoCallMe = whoCallMe;

   gui.element.rollerPopup.mBoxRollerParent = lv_obj_class_create_obj(&lv_msgbox_backdrop_class, lv_layer_top());
   lv_obj_class_init_obj(gui.element.rollerPopup.mBoxRollerParent);
   lv_obj_remove_flag(gui.element.rollerPopup.mBoxRollerParent, LV_OBJ_FLAG_IGNORE_LAYOUT);
   lv_obj_set_size(gui.element.rollerPopup.mBoxRollerParent, LV_PCT(100), LV_PCT(100));

         gui.element.rollerPopup.mBoxRollerContainer = lv_obj_create(gui.element.rollerPopup.mBoxRollerParent);
         lv_obj_align(gui.element.rollerPopup.mBoxRollerContainer, LV_ALIGN_CENTER, 0, 0);
         lv_obj_set_size(gui.element.rollerPopup.mBoxRollerContainer, 250, 220); 
         lv_obj_remove_flag(gui.element.rollerPopup.mBoxRollerContainer, LV_OBJ_FLAG_SCROLLABLE); 

         gui.element.rollerPopup.mBoxRollerTitle = lv_label_create(gui.element.rollerPopup.mBoxRollerContainer);         
         lv_label_set_text(gui.element.rollerPopup.mBoxRollerTitle, popupTitle); 
         lv_obj_set_style_text_font(gui.element.rollerPopup.mBoxRollerTitle, &lv_font_montserrat_22, 0);              
         lv_obj_align(gui.element.rollerPopup.mBoxRollerTitle, LV_ALIGN_TOP_MID, 0, - 10);


   /*Create style*/
   lv_style_init(&gui.element.rollerPopup.style_mBoxRollerTitleLine);
   lv_style_set_line_width(&gui.element.rollerPopup.style_mBoxRollerTitleLine, 2);
   lv_style_set_line_rounded(&gui.element.rollerPopup.style_mBoxRollerTitleLine, true);

   /*Create a line and apply the new style*/
   gui.element.rollerPopup.mBoxRollerTitleLine = lv_line_create(gui.element.rollerPopup.mBoxRollerContainer);
   lv_line_set_points(gui.element.rollerPopup.mBoxRollerTitleLine, gui.element.rollerPopup.titleLinePoints, 2);
   lv_obj_add_style(gui.element.rollerPopup.mBoxRollerTitleLine, &gui.element.rollerPopup.style_mBoxRollerTitleLine, 0);
   lv_obj_align(gui.element.rollerPopup.mBoxRollerTitleLine, LV_ALIGN_TOP_MID, 0, 23);


  /*********************
  *    PAGE ELEMENTS
  *********************/

  gui.element.rollerPopup.mBoxRollerRollerContainer = lv_obj_create(gui.element.rollerPopup.mBoxRollerContainer);
  lv_obj_align(gui.element.rollerPopup.mBoxRollerRollerContainer, LV_ALIGN_TOP_MID, 0, 30);
  lv_obj_set_size(gui.element.rollerPopup.mBoxRollerRollerContainer, 235, 170); 
  lv_obj_set_style_border_opa(gui.element.rollerPopup.mBoxRollerRollerContainer, LV_OPA_TRANSP, 0);
  //lv_obj_set_style_border_color(gui.element.rollerPopup.mBoxRollerRollerContainer, lv_color_hex(GREEN_DARK), 0);
  lv_obj_remove_flag(gui.element.rollerPopup.mBoxRollerContainer, LV_OBJ_FLAG_SCROLLABLE);

  
  lv_style_init(&gui.element.rollerPopup.style_roller);
  lv_style_set_text_font(&gui.element.rollerPopup.style_roller, &lv_font_montserrat_30);
  lv_style_set_bg_color(&gui.element.rollerPopup.style_roller, lv_color_hex(LIGHT_BLUE));
  lv_style_set_border_width(&gui.element.rollerPopup.style_roller, 2);
  lv_style_set_border_color(&gui.element.rollerPopup.style_roller, lv_color_hex(LIGHT_BLUE_DARK));
  
  gui.element.rollerPopup.roller = lv_roller_create(gui.element.rollerPopup.mBoxRollerRollerContainer);
  lv_roller_set_options(gui.element.rollerPopup.roller, tempOptions, LV_ROLLER_MODE_NORMAL);
  lv_roller_set_visible_row_count(gui.element.rollerPopup.roller, 4);
  lv_obj_set_width(gui.element.rollerPopup.roller, 140);
  lv_obj_set_height(gui.element.rollerPopup.roller, 100);
  lv_obj_align(gui.element.rollerPopup.roller, LV_ALIGN_CENTER, 0, -30);
  lv_obj_add_event_cb(gui.element.rollerPopup.roller, event_Roller, LV_EVENT_ALL, NULL);
  lv_obj_add_style(gui.element.rollerPopup.roller, &gui.element.rollerPopup.style_roller, LV_PART_SELECTED);  
  lv_roller_set_selected(gui.element.rollerPopup.roller, 0, LV_ANIM_OFF);   
  lv_obj_set_style_border_color(gui.element.rollerPopup.roller, lv_color_hex(WHITE), LV_PART_MAIN);

   gui.element.rollerPopup.mBoxRollerButton = lv_button_create(gui.element.rollerPopup.mBoxRollerRollerContainer);
   lv_obj_set_size(gui.element.rollerPopup.mBoxRollerButton, BUTTON_TUNE_WIDTH, BUTTON_TUNE_HEIGHT);
   lv_obj_align(gui.element.rollerPopup.mBoxRollerButton, LV_ALIGN_BOTTOM_MID, 0 , 0);
   lv_obj_add_event_cb(gui.element.rollerPopup.mBoxRollerButton, event_Roller, LV_EVENT_CLICKED, gui.element.rollerPopup.whoCallMe);

         gui.element.rollerPopup.mBoxRollerButtonLabel = lv_label_create(gui.element.rollerPopup.mBoxRollerButton);         
         lv_label_set_text(gui.element.rollerPopup.mBoxRollerButtonLabel, tuneRollerButton_text); 
         lv_obj_set_style_text_font(gui.element.rollerPopup.mBoxRollerButtonLabel, &lv_font_montserrat_18, 0);              
         lv_obj_align(gui.element.rollerPopup.mBoxRollerButtonLabel, LV_ALIGN_CENTER, 0, 0);
}

