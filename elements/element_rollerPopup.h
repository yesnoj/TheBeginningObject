#include "font/lv_font.h"
#include <sys/_stdint.h>
#include "misc/lv_palette.h"
#include "core/lv_obj_pos.h"
#include "misc/lv_color.h"
#include "misc/lv_types.h"
/**
 * @file element_rollerPopup.h
 *
 */
#include "widgets/menu/lv_menu.h"
#include "core/lv_obj_style.h"
#include "misc/lv_area.h"


#ifndef ELEMENT_ROLLERPOPUP_H
#define ELEMENT_ROLLERPOPUP_H

#ifdef __cplusplus
extern "C" {
#endif

//ESSENTIAL INCLUDES

//ACCESSORY INCLUDES

lv_obj_t * mBoxRollerContainer;
lv_obj_t * mBoxRollerTitle;
lv_obj_t * mBoxRollerButton;
lv_obj_t * mBoxRollerButtonLabel;
lv_obj_t * mBoxRollerRollerContainer;


static void event_Roller(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t * obj = (lv_obj_t *)lv_event_get_target(e);
  lv_obj_t * objCont = (lv_obj_t *)lv_obj_get_parent(obj);
  lv_obj_t * mboxCont = (lv_obj_t *)lv_obj_get_parent(objCont);
  lv_obj_t * godFatherCont = (lv_obj_t *)lv_obj_get_parent(mboxCont);
  lv_obj_t * data = (lv_obj_t *)lv_event_get_user_data(e);

    if(code == LV_EVENT_CLICKED){
        if(obj == mBoxRollerButton)
        {
            if(data == tempSensorTuneButton){
              LV_LOG_USER("SET BUTTON from tempSensorTuneButton value %d:",rolleTempSelected);
              lv_style_reset(&style_mBoxRollerTitleLine);
              lv_style_reset(&style_roller);
              lv_msgbox_close(godFatherCont);
              lv_obj_delete(godFatherCont);
            }
            if(data == processTempTextArea){
              LV_LOG_USER("SET BUTTON from processTempTextArea value %d:",rolleTempSelected);
              itoa(rolleTempSelected, tempBuffer, 10);
              lv_style_reset(&style_mBoxRollerTitleLine);
              lv_textarea_set_text(processTempTextArea,tempBuffer);
              lv_style_reset(&style_roller);
              
              lv_msgbox_close(godFatherCont);
              lv_obj_delete(godFatherCont);
            }
            if(data == processToleranceTextArea){
              LV_LOG_USER("SET BUTTON from processToleranceTextArea value %d:",rolleTempSelected);
              
              lv_style_reset(&style_mBoxRollerTitleLine);
              lv_textarea_set_text(processToleranceTextArea, tempBuffer);
              lv_style_reset(&style_roller);
              lv_msgbox_close(godFatherCont);
              lv_obj_delete(godFatherCont);
            }
            if(data == stepDetailMinTextArea){
              LV_LOG_USER("SET BUTTON from stepDetailMinTextArea value %d:",rolleTempSelected);
              
              lv_style_reset(&style_mBoxRollerTitleLine);
              lv_textarea_set_text(stepDetailMinTextArea, tempBuffer);
              lv_style_reset(&style_roller);
              lv_msgbox_close(godFatherCont);
              lv_obj_delete(godFatherCont);
            }
            if(data == stepDetailSecTextArea){
              LV_LOG_USER("SET BUTTON from stepDetailSecTextArea value %d:",rolleTempSelected);
              
              lv_style_reset(&style_mBoxRollerTitleLine);
              lv_textarea_set_text(stepDetailSecTextArea, tempBuffer);
              lv_style_reset(&style_roller);
              lv_msgbox_close(godFatherCont);
              lv_obj_delete(godFatherCont);
            }
            if(data == checkupTankSizeTextArea){
              LV_LOG_USER("SET BUTTON from checkupTankSizeTextArea value %d:",rolleTempSelected);
              
              lv_style_reset(&style_mBoxRollerTitleLine);
              lv_textarea_set_text(checkupTankSizeTextArea, tempBuffer);
              lv_style_reset(&style_roller);
              lv_msgbox_close(godFatherCont);
              lv_obj_delete(godFatherCont);
            }
        }
    }

    if(code == LV_EVENT_VALUE_CHANGED){
      if(obj == roller){
          //if we want to want the index of the selected element
          rolleTempSelected = lv_roller_get_selected(obj) + 1;
          LV_LOG_USER("ROLLER value: %d", rolleTempSelected);

          
          lv_roller_get_selected_str(obj, tempBuffer, sizeof(tempBuffer));
          //if roller has strings to be read
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
   mBoxRollerParent = lv_obj_class_create_obj(&lv_msgbox_backdrop_class, lv_layer_top());
   lv_obj_class_init_obj(mBoxRollerParent);
   lv_obj_remove_flag(mBoxRollerParent, LV_OBJ_FLAG_IGNORE_LAYOUT);
   lv_obj_set_size(mBoxRollerParent, LV_PCT(100), LV_PCT(100));

         mBoxRollerContainer = lv_obj_create(mBoxRollerParent);
         lv_obj_align(mBoxRollerContainer, LV_ALIGN_CENTER, 0, 0);
         lv_obj_set_size(mBoxRollerContainer, 250, 220); 
         lv_obj_remove_flag(mBoxRollerContainer, LV_OBJ_FLAG_SCROLLABLE); 

         mBoxRollerTitle = lv_label_create(mBoxRollerContainer);         
         lv_label_set_text(mBoxRollerTitle, popupTitle); 
         lv_obj_set_style_text_font(mBoxRollerTitle, &lv_font_montserrat_22, 0);              
         lv_obj_align(mBoxRollerTitle, LV_ALIGN_TOP_MID, 0, - 10);


   /*Create style*/
   lv_style_init(&style_mBoxRollerTitleLine);
   lv_style_set_line_width(&style_mBoxRollerTitleLine, 2);
   lv_style_set_line_rounded(&style_mBoxRollerTitleLine, true);

   /*Create a line and apply the new style*/
   mBoxRollerTitleLine = lv_line_create(mBoxRollerContainer);
   lv_line_set_points(mBoxRollerTitleLine, mBoxTitleLine_points, 2);
   lv_obj_add_style(mBoxRollerTitleLine, &style_mBoxRollerTitleLine, 0);
   lv_obj_align(mBoxRollerTitleLine, LV_ALIGN_TOP_MID, 0, 23);


  /*********************
  *    PAGE ELEMENTS
  *********************/

  mBoxRollerRollerContainer = lv_obj_create(mBoxRollerContainer);
  lv_obj_align(mBoxRollerRollerContainer, LV_ALIGN_TOP_MID, 0, 30);
  lv_obj_set_size(mBoxRollerRollerContainer, 235, 170); 
  lv_obj_set_style_border_opa(mBoxRollerRollerContainer, LV_OPA_TRANSP, 0);
  //lv_obj_set_style_border_color(mBoxRollerRollerContainer, lv_color_hex(GREEN_DARK), 0);
  lv_obj_remove_flag(mBoxRollerContainer, LV_OBJ_FLAG_SCROLLABLE);

  
  lv_style_init(&style_roller);
  lv_style_set_text_font(&style_roller, &lv_font_montserrat_30);
  lv_style_set_bg_color(&style_roller, lv_color_hex(LIGHT_BLUE));
  lv_style_set_border_width(&style_roller, 2);
  lv_style_set_border_color(&style_roller, lv_color_hex(LIGHT_BLUE_DARK));
  
  roller = lv_roller_create(mBoxRollerRollerContainer);
  lv_roller_set_options(roller, tempOptions, LV_ROLLER_MODE_NORMAL);
  lv_roller_set_visible_row_count(roller, 4);
  lv_obj_set_width(roller, 140);
  lv_obj_set_height(roller, 100);
  lv_obj_align(roller, LV_ALIGN_CENTER, 0, -30);
  lv_obj_add_event_cb(roller, event_Roller, LV_EVENT_ALL, NULL);
  lv_obj_add_style(roller, &style_roller, LV_PART_SELECTED);  
  lv_roller_set_selected(roller, 0, LV_ANIM_OFF);   
  lv_obj_set_style_border_color(roller, lv_color_hex(WHITE), LV_PART_MAIN);

   mBoxRollerButton = lv_button_create(mBoxRollerRollerContainer);
   lv_obj_set_size(mBoxRollerButton, BUTTON_TUNE_WIDTH, BUTTON_TUNE_HEIGHT);
   lv_obj_align(mBoxRollerButton, LV_ALIGN_BOTTOM_MID, 0 , 0);
   lv_obj_add_event_cb(mBoxRollerButton, event_Roller, LV_EVENT_CLICKED, whoCallMe);

         mBoxRollerButtonLabel = lv_label_create(mBoxRollerButton);         
         lv_label_set_text(mBoxRollerButtonLabel, tuneRollerButton_text); 
         lv_obj_set_style_text_font(mBoxRollerButtonLabel, &lv_font_montserrat_18, 0);              
         lv_obj_align(mBoxRollerButtonLabel, LV_ALIGN_CENTER, 0, 0);
}

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*ELEMENT_ROLLERPOPUP_H*/