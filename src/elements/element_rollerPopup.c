/**
 * @file element_rollerPopup.c
 *
 */


//ESSENTIAL INCLUDES
#include <lvgl.h>
#include "../../include/definitions.h"

extern struct gui_components gui;

//ACCESSORY INCLUDES

static uint32_t rollerSelected;
static uint8_t isScrolled = 0;

void event_Roller(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t * obj = (lv_obj_t *)lv_event_get_target(e);
  lv_obj_t * objCont = (lv_obj_t *)lv_obj_get_parent(obj);
  lv_obj_t * mboxCont = (lv_obj_t *)lv_obj_get_parent(objCont);
  lv_obj_t * godFatherCont = (lv_obj_t *)lv_obj_get_parent(mboxCont);
  void * data = lv_event_get_user_data(e);

    if(code == LV_EVENT_CLICKED){
        LV_LOG_USER("LV_EVENT_CLICKED");
        lv_obj_clear_state(gui.element.rollerPopup.mBoxRollerButton, LV_STATE_DISABLED);
 
        if(obj == gui.element.rollerPopup.mBoxRollerButton)
        {   
            if((lv_obj_t *)data == gui.page.settings.tempSensorTuneButton){
              LV_LOG_USER("SET BUTTON from tempSensorTuneButton value %d:",rollerSelected);
              lv_style_reset(&gui.element.rollerPopup.style_mBoxRollerTitleLine);
              lv_style_reset(&gui.element.rollerPopup.style_roller);
              lv_msgbox_close(godFatherCont);
              //lv_obj_delete(godFatherCont);
              gui.page.settings.settingsParams.calibratedTemp = rollerSelected;  
              qSysAction( SAVE_PROCESS_CONFIG );
              return;   
            }
            if((lv_obj_t *)data == gui.tempProcessNode->process.processDetails->processTempTextArea){
              
              lv_style_reset(&gui.element.rollerPopup.style_mBoxRollerTitleLine);
              if(isScrolled == 0){
                  itoa(lv_roller_get_selected(gui.element.rollerPopup.roller) + 1, tempBuffer, 10);
                  LV_LOG_USER("SET BUTTON from processTempTextArea value %d:",lv_roller_get_selected(gui.element.rollerPopup.roller) + 1);
                  gui.tempProcessNode->process.processDetails->temp = lv_roller_get_selected(gui.element.rollerPopup.roller) + 1;
                  lv_textarea_set_text(gui.tempProcessNode->process.processDetails->processTempTextArea,tempBuffer);
                  }
                else{
                    itoa(rollerSelected, tempBuffer, 10);
                    LV_LOG_USER("SET BUTTON from processTempTextArea value %d:",rollerSelected);
                    gui.tempProcessNode->process.processDetails->temp = rollerSelected;
                    lv_textarea_set_text(gui.tempProcessNode->process.processDetails->processTempTextArea,tempBuffer);
                    isScrolled = 0;
                    }

              gui.tempProcessNode->process.processDetails->somethingChanged = 1;
              lv_obj_send_event(gui.tempProcessNode->process.processDetails->processSaveButton, LV_EVENT_REFRESH, NULL);

              lv_style_reset(&gui.element.rollerPopup.style_roller);
              lv_msgbox_close(godFatherCont);
              //lv_obj_delete(godFatherCont);
              return; 
            }
            if((lv_obj_t *)data == gui.tempProcessNode->process.processDetails->processToleranceTextArea){
             lv_style_reset(&gui.element.rollerPopup.style_mBoxRollerTitleLine);


              if(isScrolled == 0){
                  itoa(lv_roller_get_selected(gui.element.rollerPopup.roller) + 1, tempBuffer, 10);
                  LV_LOG_USER("SET BUTTON from processToleranceTextArea value %d:",lv_roller_get_selected(gui.element.rollerPopup.roller) + 1);
                  gui.tempProcessNode->process.processDetails->tempTolerance = lv_roller_get_selected(gui.element.rollerPopup.roller) + 1;
                  lv_textarea_set_text(gui.tempProcessNode->process.processDetails->processToleranceTextArea,tempBuffer);
                  }
                else{
                    itoa(rollerSelected, tempBuffer, 10);
                    LV_LOG_USER("SET BUTTON from processToleranceTextArea value %d:",rollerSelected - 1);
                    gui.tempProcessNode->process.processDetails->tempTolerance = rollerSelected - 1;
                    lv_textarea_set_text(gui.tempProcessNode->process.processDetails->processToleranceTextArea, getRollerStringIndex(rollerSelected - 1,gui.element.rollerPopup.tempCelsiusToleranceOptions));
                    isScrolled = 0;
                    }

              gui.tempProcessNode->process.processDetails->somethingChanged = 1;
              lv_obj_send_event(gui.tempProcessNode->process.processDetails->processSaveButton, LV_EVENT_REFRESH, NULL);

              lv_style_reset(&gui.element.rollerPopup.style_roller);
              lv_msgbox_close(godFatherCont);
              //lv_obj_delete(godFatherCont);
              return; 
            }
            if((lv_obj_t *)data == gui.tempStepNode->step.stepDetails->stepDetailMinTextArea){
              if(isScrolled == 0){
                  itoa(lv_roller_get_selected(gui.element.rollerPopup.roller), tempBuffer, 10);
                  LV_LOG_USER("SET BUTTON from stepDetailMinTextArea value %d:",lv_roller_get_selected(gui.element.rollerPopup.roller));
                  gui.tempStepNode->step.stepDetails->timeMins = lv_roller_get_selected(gui.element.rollerPopup.roller);
                  lv_textarea_set_text(gui.tempStepNode->step.stepDetails->stepDetailMinTextArea, tempBuffer);
                  }
                else{
                    itoa(rollerSelected, tempBuffer, 10);
                    LV_LOG_USER("SET BUTTON from stepDetailMinTextArea value %d:",rollerSelected - 1);
                    gui.tempStepNode->step.stepDetails->timeMins = rollerSelected - 1;
                    lv_textarea_set_text(gui.tempStepNode->step.stepDetails->stepDetailMinTextArea, getRollerStringIndex(rollerSelected - 1,gui.element.rollerPopup.minutesOptions));
                    isScrolled = 0;
                    }

              lv_style_reset(&gui.element.rollerPopup.style_mBoxRollerTitleLine);
              lv_style_reset(&gui.element.rollerPopup.style_roller);
              lv_msgbox_close(gui.element.rollerPopup.mBoxRollerParent);
              lv_obj_send_event(gui.tempStepNode->step.stepDetails->stepSaveButton, LV_EVENT_REFRESH, NULL);
              //lv_obj_delete(gui.element.rollerPopup.mBoxRollerParent);
              return; 
            }
            if((lv_obj_t *)data == gui.tempStepNode->step.stepDetails->stepDetailSecTextArea){
              if(isScrolled == 0){
                  itoa(lv_roller_get_selected(gui.element.rollerPopup.roller), tempBuffer, 10);
                  LV_LOG_USER("SET BUTTON from stepDetailMinTextArea value %d:",lv_roller_get_selected(gui.element.rollerPopup.roller));
                  gui.tempStepNode->step.stepDetails->timeSecs = lv_roller_get_selected(gui.element.rollerPopup.roller);
                  lv_textarea_set_text(gui.tempStepNode->step.stepDetails->stepDetailSecTextArea, tempBuffer);
                  }
                else{
                    itoa(rollerSelected, tempBuffer, 10);
                    LV_LOG_USER("SET BUTTON from stepDetailMinTextArea value %d:",rollerSelected - 1);
                    gui.tempStepNode->step.stepDetails->timeSecs = rollerSelected - 1;
                    lv_textarea_set_text(gui.tempStepNode->step.stepDetails->stepDetailSecTextArea, getRollerStringIndex(rollerSelected - 1,gui.element.rollerPopup.secondsOptions));
                    isScrolled = 0;
                    }

              lv_style_reset(&gui.element.rollerPopup.style_mBoxRollerTitleLine);
              lv_style_reset(&gui.element.rollerPopup.style_roller);
              lv_msgbox_close(gui.element.rollerPopup.mBoxRollerParent);
              //lv_obj_delete(gui.element.rollerPopup.mBoxRollerParent);
              lv_obj_send_event(gui.tempStepNode->step.stepDetails->stepSaveButton, LV_EVENT_REFRESH, NULL);
              return; 
            }
            if((lv_obj_t*)data == gui.tempProcessNode->process.processDetails->checkup->checkupTankSizeTextArea){
              LV_LOG_USER("SET BUTTON from checkupTankSizeTextArea value %d:",rollerSelected);
              
              gui.tempProcessNode->process.processDetails->checkup->tankSize = rollerSelected;
              lv_style_reset(&gui.element.rollerPopup.style_mBoxRollerTitleLine);
              lv_textarea_set_text(gui.tempProcessNode->process.processDetails->checkup->checkupTankSizeTextArea, tempBuffer);
              lv_style_reset(&gui.element.rollerPopup.style_roller);
              lv_msgbox_close(godFatherCont);
              //lv_obj_delete(godFatherCont);
              return; 
            }
          rollerSelected = 0;
        }
        
    }

    if(code == LV_EVENT_VALUE_CHANGED){
      if(obj == gui.element.rollerPopup.roller){
          isScrolled = 1;
          lv_obj_clear_state(gui.element.rollerPopup.mBoxRollerButton, LV_STATE_DISABLED);
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


void rollerPopupCreate(const char * tempOptions,const char * popupTitle, void *whoCallMe, uint32_t currentVal){
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
  lv_obj_add_event_cb(gui.element.rollerPopup.roller, event_Roller, LV_EVENT_CLICKED, NULL);
  lv_obj_add_event_cb(gui.element.rollerPopup.roller, event_Roller, LV_EVENT_VALUE_CHANGED, NULL);
  lv_obj_add_style(gui.element.rollerPopup.roller, &gui.element.rollerPopup.style_roller, LV_PART_SELECTED);  
  lv_obj_set_style_border_color(gui.element.rollerPopup.roller, lv_color_hex(WHITE), LV_PART_MAIN);


   gui.element.rollerPopup.mBoxRollerButton = lv_button_create(gui.element.rollerPopup.mBoxRollerRollerContainer);
   lv_obj_set_size(gui.element.rollerPopup.mBoxRollerButton, BUTTON_TUNE_WIDTH, BUTTON_TUNE_HEIGHT);
   lv_obj_align(gui.element.rollerPopup.mBoxRollerButton, LV_ALIGN_BOTTOM_MID, 0 , 0);
   lv_obj_add_event_cb(gui.element.rollerPopup.mBoxRollerButton, event_Roller, LV_EVENT_CLICKED, gui.element.rollerPopup.whoCallMe);
   lv_obj_add_event_cb(gui.element.rollerPopup.mBoxRollerButton, event_Roller, LV_EVENT_VALUE_CHANGED, gui.element.rollerPopup.whoCallMe);
  if(currentVal == -1){
      lv_roller_set_selected(gui.element.rollerPopup.roller, 0, LV_ANIM_OFF);
      lv_obj_add_state(gui.element.rollerPopup.mBoxRollerButton, LV_STATE_DISABLED);
      }
  else{
      lv_roller_set_selected(gui.element.rollerPopup.roller, currentVal, LV_ANIM_OFF);
      lv_obj_clear_state(gui.element.rollerPopup.mBoxRollerButton, LV_STATE_DISABLED);
      }

         gui.element.rollerPopup.mBoxRollerButtonLabel = lv_label_create(gui.element.rollerPopup.mBoxRollerButton);         
         lv_label_set_text(gui.element.rollerPopup.mBoxRollerButtonLabel, tuneRollerButton_text); 
         lv_obj_set_style_text_font(gui.element.rollerPopup.mBoxRollerButtonLabel, &lv_font_montserrat_18, 0);              
         lv_obj_align(gui.element.rollerPopup.mBoxRollerButtonLabel, LV_ALIGN_CENTER, 0, 0);
}

