#include "lv_api_map_v8.h"
#include "misc/lv_palette.h"
#include <sys/_stdint.h>

/**
 * @file page_checkup.h
 *
 */

#include "core/lv_obj_style.h"
#include "misc/lv_area.h"

#ifndef PAGE_CHECKUP_H
#define PAGE_CHECKUP_H

#ifdef __cplusplus
extern "C" {
#endif

//ESSENTIAL INCLUDES


//ACCESSORY INCLUDES


static lv_obj_t * checkupParent;

static lv_obj_t * checkupContainer;
static lv_obj_t * checkupNextStepsContainer;
static lv_obj_t * checkupProcessNameContainer;
static lv_obj_t * checkupStepContainer;
static lv_obj_t * checkupWaterFillContainer;
static lv_obj_t * checkupReachTempContainer;
static lv_obj_t * checkupTankAndFilmContainer;
static lv_obj_t * checkupStepSourceContainer;
static lv_obj_t * checkupTempControlContainer;
static lv_obj_t * checkupWaterTempContainer;
static lv_obj_t * checkupNextStepContainer;
static lv_obj_t * checkupSelectTankChemistryContainer;
static lv_obj_t * checkupFillWaterContainer;
static lv_obj_t * checkupTargetTempsContainer;
static lv_obj_t * checkupTargetTempContainer;
static lv_obj_t * checkupTargetWaterTempContainer;
static lv_obj_t * checkupTargetChemistryTempContainer;
static lv_obj_t * checkupTankIsPresentContainer;
static lv_obj_t * checkupFilmRotatingContainer;
static lv_obj_t * checkupFilmInPositionContainer;
static lv_obj_t * checkupProcessingContainer;


static lv_obj_t * checkupTankSizeLabel;
static lv_obj_t * checkupChemistryVolumeLabel;
static lv_obj_t * checkupNextStepsLabel;
static lv_obj_t * checkupWaterFillLabel;
static lv_obj_t * checkupReachTempLabel;
static lv_obj_t * checkupTankAndFilmLabel;
static lv_obj_t * checkupMachineWillDoLabel;
static lv_obj_t * checkupStopStepsButtonLabel;
static lv_obj_t * checkupCloseButtonLabel;
static lv_obj_t * checkupStepSourceLabel;
static lv_obj_t * checkupTempControlLabel;
static lv_obj_t * checkupWaterTempLabel;
static lv_obj_t * checkupNextStepLabel;
static lv_obj_t * checkupStopAfterButtonLabel;
static lv_obj_t * checkupStopNowButtonLabel;
static lv_obj_t * checkupStartButtonLabel;
static lv_obj_t * checkupProcessReadyLabel;
static lv_obj_t * checkupSelectBeforeStartLabel;
static lv_obj_t * checkupFillWaterLabel;
static lv_obj_t * checkupSkipButtonLabel;
static lv_obj_t * checkupTargetTempLabel;
static lv_obj_t * checkupTargetWaterTempLabel;
static lv_obj_t * checkupTargetChemistryTempLabel;
static lv_obj_t * checkupTankIsPresentLabel;
static lv_obj_t * checkupFilmRotatingLabel;

static lv_obj_t * checkupTargetTempValue;
static lv_obj_t * checkupTargetWaterTempValue;
static lv_obj_t * checkupTargetChemistryTempValue;
static lv_obj_t * checkupStepSourceValue;
static lv_obj_t * checkupTempControlValue;
static lv_obj_t * checkupWaterTempValue;
static lv_obj_t * checkupNextStepValue;
static lv_obj_t * checkupProcessNameValue;
static lv_obj_t * checkupTankIsPresentValue;
static lv_obj_t * checkupFilmRotatingValue;
static lv_obj_t * checkupStepTimeLeftValue;
static lv_obj_t * checkupProcessTimeLeftValue;
static lv_obj_t * checkupStepNameValue;
static lv_obj_t * checkupStepKindValue;

static lv_obj_t * checkupWaterFillStatusIcon;
static lv_obj_t * checkupReachTempStatusIcon;
static lv_obj_t * checkupTankAndFilmStatusIcon;

static lv_obj_t * lowVolumeChemRadioButton;
static lv_obj_t * highVolumeChemRadioButton;


static lv_obj_t * checkupSkipButton;
static lv_obj_t * checkupStartButton;
static lv_obj_t * checkupStopAfterButton;
static lv_obj_t * checkupStopNowButton;
static lv_obj_t * checkupStopStepsButton;
static lv_obj_t * checkupCloseButton;

static uint8_t  isProcessing = 0; // 0 or 1
static uint8_t  processStep = 0;//0 or 1 or 2 or 3 or 4
static uint32_t activeVolume_index = 0;

static uint8_t  stepFillWaterStatus = 0;
static uint8_t  stepReachTempStatus = 0;
static uint8_t  stepCheckFilmStatus = 0;

static lv_obj_t * stepArc;
static lv_obj_t * processArc;

void checkup();

void event_checkup(lv_event_t * e){
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t * obj = (lv_obj_t *)lv_event_get_target(e);
  lv_obj_t * objCont = (lv_obj_t *)lv_obj_get_parent(obj);
  lv_obj_t * mboxCont = (lv_obj_t *)lv_obj_get_parent(objCont);
  lv_obj_t * mboxParent = (lv_obj_t *)lv_obj_get_parent(mboxCont);
  lv_obj_t * data = (lv_obj_t *)lv_event_get_user_data(e);

  //to manage radio buttons
  lv_obj_t * cont = (lv_obj_t *)lv_event_get_current_target(e);
  uint32_t * active_id = (uint32_t *)lv_event_get_user_data(e);
  lv_obj_t * act_cb = (lv_obj_t *)lv_event_get_target(e);
  lv_obj_t * old_cb = (lv_obj_t *)lv_obj_get_child(cont, *active_id);

  if(code == LV_EVENT_FOCUSED) {
      if(data == checkupTankSizeTextArea){
          LV_LOG_USER("Set Tank Size");
          rollerPopupCreate(checkupTankSizesList,checkupTankSize_text,checkupTankSizeTextArea);
      }
  }

  if(act_cb == lowVolumeChemRadioButton || act_cb == highVolumeChemRadioButton){
   if(code == LV_EVENT_CLICKED) {
        lv_obj_remove_state(old_cb, LV_STATE_CHECKED);
        lv_obj_add_state(act_cb, LV_STATE_CHECKED); 
        *active_id = lv_obj_get_index(act_cb);
        LV_LOG_USER("Selected chemistry volume radio buttons: %d", (int)activeVolume_index);
   }
  }

  if(code == LV_EVENT_CLICKED){
    if(obj == checkupStartButton){
      if(data == checkupSelectTankChemistryContainer){
        LV_LOG_USER("User pressed checkupStartButton on Step 0");
        isProcessing = 0;
        processStep = 1;
        stepFillWaterStatus = 1;
        stepReachTempStatus = 0;
        stepCheckFilmStatus = 0;
        checkup();
      }

      if(data == checkupFilmRotatingContainer){
        LV_LOG_USER("User pressed checkupStartButton on Step 3");
        isProcessing = 1;
        processStep = 4;
        stepFillWaterStatus = 2;
        stepReachTempStatus = 2;
        stepCheckFilmStatus = 2;
        checkup();
      }
  }
    if(obj == checkupSkipButton){
      if(data == checkupFillWaterContainer){
        LV_LOG_USER("User pressed checkupSkipButton on Step 1");
        isProcessing = 0;
        processStep = 2;
        stepFillWaterStatus = 2;
        stepReachTempStatus = 1;
        stepCheckFilmStatus = 0;
        checkup();
      }
      if(data == checkupTargetWaterTempContainer){
        LV_LOG_USER("User pressed checkupSkipButton on Step 2");
        isProcessing = 0;
        processStep = 3;
        stepFillWaterStatus = 2;
        stepReachTempStatus = 2;
        stepCheckFilmStatus = 1;
        checkup();
      
      }
    }

    if(obj == checkupCloseButton){
        LV_LOG_USER("User pressed checkupCloseButtonLabel");
        lv_msgbox_close(mboxCont);
        lv_obj_delete(mboxCont);
    }
    if(obj == checkupStopAfterButton){
        LV_LOG_USER("User pressed checkupStopAfterButton");
        messagePopupCreate(warningPopupTitle_text,stopProcessPopupBody_text,checkupStop_text,stepDetailCancel_text, checkupParent);
    }
    if(obj == checkupStopNowButton){
        LV_LOG_USER("User pressed checkupStopNowButton");
        messagePopupCreate(warningPopupTitle_text,stopProcessPopupBody_text,checkupStop_text,stepDetailCancel_text, checkupParent);
    }
    if(obj == checkupStopStepsButton){
        LV_LOG_USER("User pressed checkupStopStepsButton");
        lv_msgbox_close(mboxParent);
        lv_obj_delete(mboxParent);
    }

  }
}


void checkup(void)
{  
      LV_LOG_USER("Final checks, current on processStep :%d",processStep);


      checkupParent = lv_obj_class_create_obj(&lv_msgbox_backdrop_class, lv_layer_top());
      lv_obj_class_init_obj(checkupParent);
      lv_obj_remove_flag(checkupParent, LV_OBJ_FLAG_IGNORE_LAYOUT);
      lv_obj_set_size(checkupParent, LV_PCT(100), LV_PCT(100));

      checkupContainer = lv_obj_create(checkupParent);
      lv_obj_align(checkupContainer, LV_ALIGN_CENTER, 0, 0);
      lv_obj_set_size(checkupContainer, LV_PCT(100), LV_PCT(100)); 
      lv_obj_remove_flag(checkupContainer, LV_OBJ_FLAG_SCROLLABLE); 

            checkupCloseButton = lv_button_create(checkupContainer);
            lv_obj_set_size(checkupCloseButton, BUTTON_POPUP_CLOSE_WIDTH * 1.2, BUTTON_POPUP_CLOSE_HEIGHT * 1.2);
            lv_obj_align(checkupCloseButton, LV_ALIGN_TOP_RIGHT, 7 , -10);
            lv_obj_add_event_cb(checkupCloseButton, event_checkup, LV_EVENT_CLICKED, checkupCloseButton);
            lv_obj_set_style_bg_color(checkupCloseButton, lv_color_hex(GREEN_DARK), LV_PART_MAIN);
            if(processStep > 0){
              lv_obj_add_state(checkupCloseButton, LV_STATE_DISABLED);            
            }

                  checkupCloseButtonLabel = lv_label_create(checkupCloseButton);         
                  lv_label_set_text(checkupCloseButtonLabel, closePopup_icon); 
                  lv_obj_set_style_text_font(checkupCloseButtonLabel, &FilMachineFontIcons_30, 0);              
                  lv_obj_align(checkupCloseButtonLabel, LV_ALIGN_CENTER, 0, 0);


            checkupProcessNameContainer = lv_obj_create(checkupContainer);
            lv_obj_remove_flag(checkupProcessNameContainer, LV_OBJ_FLAG_SCROLLABLE); 
            lv_obj_align(checkupProcessNameContainer, LV_ALIGN_TOP_LEFT, -10, -15);
            lv_obj_set_size(checkupProcessNameContainer, 350, 50); 
            //lv_obj_set_style_border_color(checkupProcessNameContainer, lv_color_hex(GREEN_DARK), 0);
            lv_obj_set_style_border_opa(checkupProcessNameContainer, LV_OPA_TRANSP, 0);

                  checkupProcessNameValue = lv_label_create(checkupProcessNameContainer);         
                  lv_label_set_text(checkupProcessNameValue, "E6 six baths"); 
                  lv_obj_set_width(checkupProcessNameValue, 300);
                  lv_obj_set_style_text_font(checkupProcessNameValue, &lv_font_montserrat_30, 0);              
                  lv_obj_align(checkupProcessNameValue, LV_ALIGN_TOP_LEFT, -10, -8);
                  lv_label_set_long_mode(checkupProcessNameValue, LV_LABEL_LONG_SCROLL_CIRCULAR);

            
        //LEFT SIDE OF SCREEN
        if(isProcessing == 0){
            checkupNextStepsContainer = lv_obj_create(checkupContainer);
            lv_obj_remove_flag(checkupNextStepsContainer, LV_OBJ_FLAG_SCROLLABLE); 
            lv_obj_align(checkupNextStepsContainer, LV_ALIGN_TOP_LEFT, -10, 35);
            lv_obj_set_size(checkupNextStepsContainer, 225, 265); 
            lv_obj_set_style_border_color(checkupNextStepsContainer, lv_color_hex(WHITE), 0);
            //lv_obj_set_style_border_opa(checkupNextStepsContainer, LV_OPA_TRANSP, 0);

                  checkupNextStepsLabel = lv_label_create(checkupNextStepsContainer);         
                  lv_label_set_text(checkupNextStepsLabel, checkupNexStepsTitle_text); 
                  lv_obj_set_width(checkupNextStepsLabel, LV_SIZE_CONTENT);
                  lv_obj_set_style_text_font(checkupNextStepsLabel, &lv_font_montserrat_22, 0);              
                  lv_obj_align(checkupNextStepsLabel, LV_ALIGN_TOP_LEFT, -10, -8);

                  checkupMachineWillDoLabel = lv_label_create(checkupNextStepsContainer);
                  lv_label_set_text(checkupMachineWillDoLabel, checkupTheMachineWillDo_text); 
                  lv_obj_set_width(checkupMachineWillDoLabel, LV_SIZE_CONTENT);
                  lv_obj_set_style_text_font(checkupMachineWillDoLabel, &lv_font_montserrat_18, 0);              
                  lv_obj_align(checkupMachineWillDoLabel, LV_ALIGN_TOP_LEFT, -10, 17);

                  checkupWaterFillContainer = lv_obj_create(checkupNextStepsContainer);
                  lv_obj_remove_flag(checkupWaterFillContainer, LV_OBJ_FLAG_SCROLLABLE); 
                  lv_obj_align(checkupWaterFillContainer, LV_ALIGN_TOP_LEFT, -10, 45);
                  lv_obj_set_size(checkupWaterFillContainer, 195, 45); 
                  //lv_obj_set_style_border_color(checkupWaterFillContainer, lv_color_hex(GREEN_DARK), 0);
                  lv_obj_set_style_border_opa(checkupWaterFillContainer, LV_OPA_TRANSP, 0);


                          checkupWaterFillStatusIcon = lv_label_create(checkupWaterFillContainer);         
                          lv_label_set_text(checkupWaterFillStatusIcon, checkupStepStatuses[0][stepFillWaterStatus]); 
                          lv_obj_set_style_text_font(checkupWaterFillStatusIcon, &FilMachineFontIcons_15, 0);              
                          lv_obj_align(checkupWaterFillStatusIcon, LV_ALIGN_LEFT_MID, -15, 0);

                          checkupWaterFillLabel = lv_label_create(checkupWaterFillContainer);         
                          lv_label_set_text(checkupWaterFillLabel, checkupFillWater_text); 
                          lv_obj_set_width(checkupWaterFillLabel, 168);
                          lv_obj_set_style_text_font(checkupWaterFillLabel, &lv_font_montserrat_18, 0);              
                          lv_obj_align(checkupWaterFillLabel, LV_ALIGN_LEFT_MID, 2, 0);
                          lv_label_set_long_mode(checkupWaterFillLabel, LV_LABEL_LONG_SCROLL_CIRCULAR);


                  checkupReachTempContainer = lv_obj_create(checkupNextStepsContainer);
                  lv_obj_remove_flag(checkupReachTempContainer, LV_OBJ_FLAG_SCROLLABLE); 
                  lv_obj_align(checkupReachTempContainer, LV_ALIGN_TOP_LEFT, -10, 90);
                  lv_obj_set_size(checkupReachTempContainer, 195, 45); 
                  //lv_obj_set_style_border_color(checkupReachTempContainer, lv_color_hex(GREEN_DARK), 0);
                  lv_obj_set_style_border_opa(checkupReachTempContainer, LV_OPA_TRANSP, 0);

                          checkupReachTempStatusIcon = lv_label_create(checkupReachTempContainer);         
                          lv_label_set_text(checkupReachTempStatusIcon, checkupStepStatuses[0][stepReachTempStatus]); 
                          lv_obj_set_style_text_font(checkupReachTempStatusIcon, &FilMachineFontIcons_15, 0);              
                          lv_obj_align(checkupReachTempStatusIcon, LV_ALIGN_LEFT_MID, -15, 0);


                          checkupReachTempLabel = lv_label_create(checkupReachTempContainer);         
                          lv_label_set_text(checkupReachTempLabel, checkupReachTemp_text); 
                          lv_obj_set_width(checkupReachTempLabel, 168);
                          lv_obj_set_style_text_font(checkupReachTempLabel, &lv_font_montserrat_18, 0);              
                          lv_obj_align(checkupReachTempLabel, LV_ALIGN_LEFT_MID, 2, 0);
                          lv_label_set_long_mode(checkupReachTempLabel, LV_LABEL_LONG_SCROLL_CIRCULAR);



                  checkupTankAndFilmContainer = lv_obj_create(checkupNextStepsContainer);
                  lv_obj_remove_flag(checkupTankAndFilmContainer, LV_OBJ_FLAG_SCROLLABLE); 
                  lv_obj_align(checkupTankAndFilmContainer, LV_ALIGN_TOP_LEFT, -10, 135);
                  lv_obj_set_size(checkupTankAndFilmContainer, 195, 45); 
                  //lv_obj_set_style_border_color(checkupTankAndFilmContainer, lv_color_hex(GREEN_DARK), 0);
                  lv_obj_set_style_border_opa(checkupTankAndFilmContainer, LV_OPA_TRANSP, 0);

                          checkupTankAndFilmStatusIcon = lv_label_create(checkupTankAndFilmContainer);         
                          lv_label_set_text(checkupTankAndFilmStatusIcon, checkupStepStatuses[0][stepCheckFilmStatus]); 
                          lv_obj_set_style_text_font(checkupTankAndFilmStatusIcon, &FilMachineFontIcons_15, 0);              
                          lv_obj_align(checkupTankAndFilmStatusIcon, LV_ALIGN_LEFT_MID, -15, 0);


                          checkupTankAndFilmLabel = lv_label_create(checkupTankAndFilmContainer);         
                          lv_label_set_text(checkupTankAndFilmLabel, checkupTankRotation_text); 
                          lv_obj_set_width(checkupTankAndFilmLabel, 168);
                          lv_obj_set_style_text_font(checkupTankAndFilmLabel, &lv_font_montserrat_18, 0);              
                          lv_obj_align(checkupTankAndFilmLabel, LV_ALIGN_LEFT_MID, 2, 0);
                          lv_label_set_long_mode(checkupTankAndFilmLabel, LV_LABEL_LONG_SCROLL_CIRCULAR);


                  checkupStopStepsButton = lv_button_create(checkupNextStepsContainer);
                  lv_obj_set_size(checkupStopStepsButton, BUTTON_PROCESS_WIDTH, BUTTON_PROCESS_HEIGHT);
                  lv_obj_align(checkupStopStepsButton, LV_ALIGN_BOTTOM_MID, 0, 10);
                  lv_obj_add_event_cb(checkupStopStepsButton, event_checkup, LV_EVENT_CLICKED, checkupStopStepsButton);
                  lv_obj_set_style_bg_color(checkupStopStepsButton, lv_color_hex(RED_DARK), LV_PART_MAIN);
                  lv_obj_move_foreground(checkupStopStepsButton);
                  if(processStep == 0){
                    lv_obj_add_state(checkupStopStepsButton, LV_STATE_DISABLED);            
                  }

                          checkupStopStepsButtonLabel = lv_label_create(checkupStopStepsButton);         
                          lv_label_set_text(checkupStopStepsButtonLabel, checkupStop_text); 
                          lv_obj_set_style_text_font(checkupStopStepsButtonLabel, &lv_font_montserrat_22, 0);              
                          lv_obj_align(checkupStopStepsButtonLabel, LV_ALIGN_CENTER, 0, 0);

        }
        else{

            checkupNextStepsContainer = lv_obj_create(checkupContainer);
            lv_obj_remove_flag(checkupNextStepsContainer, LV_OBJ_FLAG_SCROLLABLE); 
            lv_obj_align(checkupNextStepsContainer, LV_ALIGN_TOP_LEFT, -10, 35);
            lv_obj_set_size(checkupNextStepsContainer, 225, 265); 
            lv_obj_set_style_border_color(checkupNextStepsContainer, lv_color_hex(WHITE), 0);
            //lv_obj_set_style_border_opa(checkupNextStepsContainer, LV_OPA_TRANSP, 0);


                  checkupNextStepsLabel = lv_label_create(checkupNextStepsContainer);         
                  lv_label_set_text(checkupNextStepsLabel, checkupProcessingTitle_text); 
                  lv_obj_set_width(checkupNextStepsLabel, LV_SIZE_CONTENT);
                  lv_obj_set_style_text_font(checkupNextStepsLabel, &lv_font_montserrat_22, 0);              
                  lv_obj_align(checkupNextStepsLabel, LV_ALIGN_TOP_LEFT, -10, -8);


                  checkupStepSourceContainer = lv_obj_create(checkupNextStepsContainer);
                  lv_obj_remove_flag(checkupStepSourceContainer, LV_OBJ_FLAG_SCROLLABLE); 
                  lv_obj_align(checkupStepSourceContainer, LV_ALIGN_TOP_LEFT, -10, 17);
                  lv_obj_set_size(checkupStepSourceContainer, 215, 45); 
                  //lv_obj_set_style_border_color(checkupStepSourceContainer, lv_color_hex(GREEN_DARK), 0);
                  lv_obj_set_style_border_opa(checkupStepSourceContainer, LV_OPA_TRANSP, 0);


                          checkupStepSourceLabel = lv_label_create(checkupStepSourceContainer);         
                          lv_label_set_text(checkupStepSourceLabel, checkupStepSource_text); 
                          lv_obj_set_style_text_font(checkupStepSourceLabel, &lv_font_montserrat_18, 0);              
                          lv_obj_align(checkupStepSourceLabel, LV_ALIGN_LEFT_MID, -15, 0);

                          checkupStepSourceValue = lv_label_create(checkupStepSourceContainer);         
                          lv_label_set_text(checkupStepSourceValue, &processSourceList[1][0]); 
                          lv_obj_set_width(checkupStepSourceValue, LV_SIZE_CONTENT);
                          lv_obj_set_style_text_font(checkupStepSourceValue, &lv_font_montserrat_20, 0);              
                          lv_obj_align(checkupStepSourceValue, LV_ALIGN_RIGHT_MID, 10, 0);


                  checkupTempControlContainer = lv_obj_create(checkupNextStepsContainer);
                  lv_obj_remove_flag(checkupTempControlContainer, LV_OBJ_FLAG_SCROLLABLE); 
                  lv_obj_align(checkupTempControlContainer, LV_ALIGN_TOP_LEFT, -10, 62);
                  lv_obj_set_size(checkupTempControlContainer, 215, 45); 
                  //lv_obj_set_style_border_color(checkupTempControlContainer, lv_color_hex(GREEN_DARK), 0);
                  lv_obj_set_style_border_opa(checkupTempControlContainer, LV_OPA_TRANSP, 0);


                          checkupTempControlLabel = lv_label_create(checkupTempControlContainer);         
                          lv_label_set_text(checkupTempControlLabel, checkupTempControl_text); 
                          lv_obj_set_style_text_font(checkupTempControlLabel, &lv_font_montserrat_18, 0);              
                          lv_obj_align(checkupTempControlLabel, LV_ALIGN_LEFT_MID, -15, 0);

                          checkupTempControlValue = lv_label_create(checkupTempControlContainer);         
                          lv_label_set_text(checkupTempControlValue, &processTempControlList[1][0]); 
                          lv_obj_set_width(checkupTempControlValue, LV_SIZE_CONTENT);
                          lv_obj_set_style_text_font(checkupTempControlValue, &lv_font_montserrat_20, 0);              
                          lv_obj_align(checkupTempControlValue, LV_ALIGN_RIGHT_MID, 10, 0);



                  checkupWaterTempContainer = lv_obj_create(checkupNextStepsContainer);
                  lv_obj_remove_flag(checkupWaterTempContainer, LV_OBJ_FLAG_SCROLLABLE); 
                  lv_obj_align(checkupWaterTempContainer, LV_ALIGN_TOP_LEFT, -10, 107);
                  lv_obj_set_size(checkupWaterTempContainer, 215, 45); 
                  //lv_obj_set_style_border_color(checkupWaterTempContainer, lv_color_hex(GREEN_DARK), 0);
                  lv_obj_set_style_border_opa(checkupWaterTempContainer, LV_OPA_TRANSP, 0);


                          checkupWaterTempLabel = lv_label_create(checkupWaterTempContainer);         
                          lv_label_set_text(checkupWaterTempLabel, checkupWaterTemp_text); 
                          lv_obj_set_style_text_font(checkupWaterTempLabel, &lv_font_montserrat_18, 0);              
                          lv_obj_align(checkupWaterTempLabel, LV_ALIGN_LEFT_MID, -15, 0);

                          checkupWaterTempValue = lv_label_create(checkupWaterTempContainer);         
                          lv_label_set_text(checkupWaterTempValue, "36.4°C"); 
                          lv_obj_set_width(checkupWaterTempValue, LV_SIZE_CONTENT);
                          lv_obj_set_style_text_font(checkupWaterTempValue, &lv_font_montserrat_20, 0);              
                          lv_obj_align(checkupWaterTempValue, LV_ALIGN_RIGHT_MID, 10, 0);



                  checkupNextStepContainer = lv_obj_create(checkupNextStepsContainer);
                  lv_obj_remove_flag(checkupNextStepContainer, LV_OBJ_FLAG_SCROLLABLE); 
                  lv_obj_align(checkupNextStepContainer, LV_ALIGN_TOP_LEFT, -10, 152);
                  lv_obj_set_size(checkupNextStepContainer, 215, 45); 
                  //lv_obj_set_style_border_color(checkupNextStepContainer, lv_color_hex(GREEN_DARK), 0);
                  lv_obj_set_style_border_opa(checkupNextStepContainer, LV_OPA_TRANSP, 0);


                          checkupNextStepLabel = lv_label_create(checkupNextStepContainer);         
                          lv_label_set_text(checkupNextStepLabel, checkupNextStep_text); 
                          lv_obj_set_style_text_font(checkupNextStepLabel, &lv_font_montserrat_18, 0);              
                          lv_obj_align(checkupNextStepLabel, LV_ALIGN_LEFT_MID, -15, 0);

                          checkupNextStepValue = lv_label_create(checkupNextStepContainer);         
                          lv_label_set_text(checkupNextStepValue, "Pre-bleach"); 
                          lv_obj_set_width(checkupNextStepValue, 105);
                          lv_obj_set_style_text_font(checkupNextStepValue, &lv_font_montserrat_20, 0);              
                          lv_obj_align(checkupNextStepValue, LV_ALIGN_RIGHT_MID, 10, 0);
                          lv_label_set_long_mode(checkupNextStepValue, LV_LABEL_LONG_SCROLL_CIRCULAR);


                  checkupStopNowButton = lv_button_create(checkupNextStepsContainer);
                  lv_obj_set_size(checkupStopNowButton, BUTTON_PROCESS_WIDTH, BUTTON_PROCESS_HEIGHT);
                  lv_obj_align(checkupStopNowButton, LV_ALIGN_BOTTOM_LEFT, -10, 10);
                  lv_obj_add_event_cb(checkupStopNowButton, event_checkup, LV_EVENT_CLICKED, checkupStopStepsButton);
                  lv_obj_set_style_bg_color(checkupStopNowButton, lv_color_hex(RED_DARK), LV_PART_MAIN);
                  lv_obj_move_foreground(checkupStopNowButton);


                          checkupStopNowButtonLabel = lv_label_create(checkupStopNowButton);         
                          lv_label_set_text(checkupStopNowButtonLabel, checkupStopNow_text); 
                          lv_obj_set_style_text_font(checkupStopNowButtonLabel, &lv_font_montserrat_16, 0);              
                          lv_obj_align(checkupStopNowButtonLabel, LV_ALIGN_CENTER, 0, 0);

                  
                  checkupStopAfterButton = lv_button_create(checkupNextStepsContainer);
                  lv_obj_set_size(checkupStopAfterButton, BUTTON_PROCESS_WIDTH, BUTTON_PROCESS_HEIGHT);
                  lv_obj_align(checkupStopAfterButton, LV_ALIGN_BOTTOM_RIGHT, 10, 10);
                  lv_obj_add_event_cb(checkupStopAfterButton, event_checkup, LV_EVENT_CLICKED, checkupStopAfterButton);
                  lv_obj_set_style_bg_color(checkupStopAfterButton, lv_color_hex(RED_DARK), LV_PART_MAIN);
                  lv_obj_move_foreground(checkupStopAfterButton);


                          checkupStopAfterButtonLabel = lv_label_create(checkupStopAfterButton);         
                          lv_label_set_text(checkupStopAfterButtonLabel, checkupStopAfter_text); 
                          lv_obj_set_style_text_font(checkupStopAfterButtonLabel, &lv_font_montserrat_16, 0);              
                          lv_obj_align(checkupStopAfterButtonLabel, LV_ALIGN_CENTER, 0, 0);
        }

            //RIGHT SIDE OF SCREEN
            checkupStepContainer = lv_obj_create(checkupContainer);
            lv_obj_remove_flag(checkupStepContainer, LV_OBJ_FLAG_SCROLLABLE); 
            lv_obj_align(checkupStepContainer, LV_ALIGN_TOP_LEFT, 217, 35);
            lv_obj_set_size(checkupStepContainer, 240, 265); 
            lv_obj_set_style_border_color(checkupStepContainer, lv_palette_main(LV_PALETTE_GREEN), 0);
            //lv_obj_set_style_border_opa(checkupStepContainer, LV_OPA_TRANSP, 0);

                  if(processStep == 0){
                        checkupSelectTankChemistryContainer = lv_obj_create(checkupStepContainer);
                        lv_obj_remove_flag(checkupSelectTankChemistryContainer, LV_OBJ_FLAG_SCROLLABLE); 
                        lv_obj_align(checkupSelectTankChemistryContainer, LV_ALIGN_TOP_LEFT, -18, -18);
                        lv_obj_set_size(checkupSelectTankChemistryContainer, 240, 265); 
                        //lv_obj_set_style_border_color(checkupSelectTankChemistryContainer, lv_palette_main(LV_PALETTE_GREEN), 0);
                        lv_obj_set_style_border_opa(checkupSelectTankChemistryContainer, LV_OPA_TRANSP, 0);
                        lv_obj_add_event_cb(checkupSelectTankChemistryContainer, event_checkup, LV_EVENT_CLICKED, &activeVolume_index);


                              checkupProcessReadyLabel = lv_label_create(checkupSelectTankChemistryContainer);         
                              lv_label_set_text(checkupProcessReadyLabel, checkupProcessReady_text); 
                              lv_obj_set_width(checkupProcessReadyLabel, 230);
                              lv_obj_set_style_text_font(checkupProcessReadyLabel, &lv_font_montserrat_22, 0);              
                              lv_obj_align(checkupProcessReadyLabel, LV_ALIGN_TOP_LEFT, -10, -8);

                              lowVolumeChemRadioButton = create_radiobutton(checkupSelectTankChemistryContainer, checkupChemistryLowVol_text, -105, 45, 27, &lv_font_montserrat_18, lv_color_hex(GREEN_DARK), lv_palette_main(LV_PALETTE_GREEN));
                              highVolumeChemRadioButton = create_radiobutton(checkupSelectTankChemistryContainer, checkupChemistryHighVol_text, -10, 45, 27, &lv_font_montserrat_18, lv_color_hex(GREEN_DARK), lv_palette_main(LV_PALETTE_GREEN));

                              checkupTankSizeLabel = lv_label_create(checkupSelectTankChemistryContainer);         
                              lv_label_set_text(checkupTankSizeLabel, checkupTankSize_text); 
                              lv_obj_set_width(checkupTankSizeLabel, LV_SIZE_CONTENT);
                              lv_obj_set_style_text_font(checkupTankSizeLabel, &lv_font_montserrat_18, 0);              
                              lv_obj_align(checkupTankSizeLabel, LV_ALIGN_TOP_MID, 0, 20);

                              checkupTankSizeTextArea = lv_textarea_create(checkupSelectTankChemistryContainer);
                              lv_textarea_set_one_line(checkupTankSizeTextArea, true);
                              lv_textarea_set_placeholder_text(checkupTankSizeTextArea, checkupTankSizePlaceHolder_text);
                              lv_obj_align(checkupTankSizeTextArea, LV_ALIGN_TOP_MID, 0, 45);
                              lv_obj_set_width(checkupTankSizeTextArea, 100);

                              lv_obj_add_event_cb(checkupTankSizeTextArea, event_checkup, LV_EVENT_ALL, checkupTankSizeTextArea);
                              lv_obj_add_state(checkupTankSizeTextArea, LV_STATE_FOCUSED); /*To be sure the cursor is visible*/
                              lv_obj_set_style_bg_color(checkupTankSizeTextArea, lv_palette_darken(LV_PALETTE_GREY, 3), 0);
                              lv_obj_set_style_text_align(checkupTankSizeTextArea , LV_TEXT_ALIGN_CENTER, 0);
                              //lv_style_set_text_font(&textAreaStyle, &lv_font_montserrat_18);
                              lv_obj_add_style(checkupTankSizeTextArea, &textAreaStyle, LV_PART_MAIN);
                              lv_obj_set_style_border_color(checkupTankSizeTextArea, lv_color_hex(WHITE), 0);

                              checkupChemistryVolumeLabel = lv_label_create(checkupSelectTankChemistryContainer);         
                              lv_label_set_text(checkupChemistryVolumeLabel, checkupChemistryVolume_text); 
                              lv_obj_set_width(checkupChemistryVolumeLabel, LV_SIZE_CONTENT);
                              lv_obj_set_style_text_font(checkupChemistryVolumeLabel, &lv_font_montserrat_18, 0);              
                              lv_obj_align(checkupChemistryVolumeLabel, LV_ALIGN_TOP_MID, 0, 110);



                        checkupStartButton = lv_button_create(checkupSelectTankChemistryContainer);
                        lv_obj_set_size(checkupStartButton, BUTTON_PROCESS_WIDTH, BUTTON_PROCESS_HEIGHT);
                        lv_obj_align(checkupStartButton, LV_ALIGN_BOTTOM_MID, 0, 10);
                        lv_obj_add_event_cb(checkupStartButton, event_checkup, LV_EVENT_CLICKED, checkupSelectTankChemistryContainer);
                        lv_obj_set_style_bg_color(checkupStartButton, lv_color_hex(GREEN_DARK), LV_PART_MAIN);
                        lv_obj_move_foreground(checkupStartButton);

                                checkupStartButtonLabel = lv_label_create(checkupStartButton);         
                                lv_label_set_text(checkupStartButtonLabel, checkupStart_text); 
                                lv_obj_set_style_text_font(checkupStartButtonLabel, &lv_font_montserrat_22, 0);              
                                lv_obj_align(checkupStartButtonLabel, LV_ALIGN_CENTER, 0, 0);
                  }

                  if(processStep == 1){
                        checkupFillWaterContainer = lv_obj_create(checkupStepContainer);
                        lv_obj_remove_flag(checkupFillWaterContainer, LV_OBJ_FLAG_SCROLLABLE); 
                        lv_obj_align(checkupFillWaterContainer, LV_ALIGN_TOP_LEFT, -18, -18);
                        lv_obj_set_size(checkupFillWaterContainer, 240, 265); 
                        //lv_obj_set_style_border_color(checkupFillWaterContainer, lv_palette_main(LV_PALETTE_GREEN), 0);
                        lv_obj_set_style_border_opa(checkupFillWaterContainer, LV_OPA_TRANSP, 0);


                                checkupFillWaterLabel = lv_label_create(checkupFillWaterContainer);         
                                lv_label_set_text(checkupFillWaterLabel, checkupFillWaterMachine_text); 
                                lv_obj_set_style_text_font(checkupFillWaterLabel, &lv_font_montserrat_18, 0);              
                                lv_obj_align(checkupFillWaterLabel, LV_ALIGN_CENTER, 0, -20);
                                lv_obj_set_style_text_align(checkupFillWaterLabel , LV_TEXT_ALIGN_CENTER, 0);
                                lv_label_set_long_mode(checkupFillWaterLabel, LV_LABEL_LONG_WRAP);
                                lv_obj_set_size(checkupFillWaterLabel, 235, LV_SIZE_CONTENT);



                                checkupSkipButton = lv_button_create(checkupFillWaterContainer);
                                lv_obj_set_size(checkupSkipButton, BUTTON_PROCESS_WIDTH, BUTTON_PROCESS_HEIGHT);
                                lv_obj_align(checkupSkipButton, LV_ALIGN_BOTTOM_MID, 0, 10);
                                lv_obj_add_event_cb(checkupSkipButton, event_checkup, LV_EVENT_CLICKED, checkupFillWaterContainer);
                                lv_obj_set_style_bg_color(checkupSkipButton, lv_color_hex(GREEN_DARK), LV_PART_MAIN);
                                lv_obj_move_foreground(checkupSkipButton);

                                        checkupSkipButtonLabel = lv_label_create(checkupSkipButton);         
                                        lv_label_set_text(checkupSkipButtonLabel, checkupSkip_text); 
                                        lv_obj_set_style_text_font(checkupSkipButtonLabel, &lv_font_montserrat_22, 0);              
                                        lv_obj_align(checkupSkipButtonLabel, LV_ALIGN_CENTER, 0, 0);
                  }


                  if(processStep == 2){
                        checkupTargetTempsContainer = lv_obj_create(checkupStepContainer);
                        lv_obj_remove_flag(checkupTargetTempsContainer, LV_OBJ_FLAG_SCROLLABLE); 
                        lv_obj_align(checkupTargetTempsContainer, LV_ALIGN_TOP_LEFT, -18, -18);
                        lv_obj_set_size(checkupTargetTempsContainer, 240, 265); 
                        //lv_obj_set_style_border_color(checkupTargetTempsContainer, lv_palette_main(LV_PALETTE_GREEN), 0);
                        lv_obj_set_style_border_opa(checkupTargetTempsContainer, LV_OPA_TRANSP, 0);


                                checkupTargetTempContainer = lv_obj_create(checkupStepContainer);
                                lv_obj_remove_flag(checkupTargetTempContainer, LV_OBJ_FLAG_SCROLLABLE); 
                                lv_obj_align(checkupTargetTempContainer, LV_ALIGN_TOP_MID, 0, 0);
                                lv_obj_set_size(checkupTargetTempContainer, 200, 80); 
                                //lv_obj_set_style_border_color(checkupTargetTempContainer, lv_palette_main(LV_PALETTE_GREEN), 0);
                                lv_obj_set_style_border_opa(checkupTargetTempContainer, LV_OPA_TRANSP, 0);

                                        checkupTargetTempLabel = lv_label_create(checkupTargetTempContainer);         
                                        lv_label_set_text(checkupTargetTempLabel, checkupTargetTemp_text); 
                                        lv_obj_set_style_text_font(checkupTargetTempLabel, &lv_font_montserrat_18, 0);              
                                        lv_obj_align(checkupTargetTempLabel, LV_ALIGN_CENTER, 0, -15);

                                        checkupTargetTempValue = lv_label_create(checkupTargetTempContainer);         
                                        lv_label_set_text(checkupTargetTempValue, "20.4°C"); 
                                        lv_obj_set_style_text_font(checkupTargetTempValue, &lv_font_montserrat_20, 0);              
                                        lv_obj_align(checkupTargetTempValue, LV_ALIGN_CENTER, 0, 20);


                                checkupTargetWaterTempContainer = lv_obj_create(checkupStepContainer);
                                lv_obj_remove_flag(checkupTargetWaterTempContainer, LV_OBJ_FLAG_SCROLLABLE); 
                                lv_obj_align(checkupTargetWaterTempContainer, LV_ALIGN_BOTTOM_LEFT, 0, -50);
                                lv_obj_set_size(checkupTargetWaterTempContainer, 100, 80); 
                                //lv_obj_set_style_border_color(checkupTargetWaterTempContainer, lv_palette_main(LV_PALETTE_GREEN), 0);
                                lv_obj_set_style_border_opa(checkupTargetWaterTempContainer, LV_OPA_TRANSP, 0);

                                        checkupTargetWaterTempLabel = lv_label_create(checkupTargetWaterTempContainer);         
                                        lv_label_set_text(checkupTargetWaterTempLabel, checkupWater_text); 
                                        lv_obj_set_style_text_font(checkupTargetWaterTempLabel, &lv_font_montserrat_18, 0);              
                                        lv_obj_align(checkupTargetWaterTempLabel, LV_ALIGN_CENTER, 0, -15);

                                        checkupTargetWaterTempValue = lv_label_create(checkupTargetWaterTempContainer);         
                                        lv_label_set_text(checkupTargetWaterTempValue, "15.4°C"); 
                                        lv_obj_set_style_text_font(checkupTargetWaterTempValue, &lv_font_montserrat_20, 0);              
                                        lv_obj_align(checkupTargetWaterTempValue, LV_ALIGN_CENTER, 0, 20);



                                checkupTargetChemistryTempContainer = lv_obj_create(checkupStepContainer);
                                lv_obj_remove_flag(checkupTargetChemistryTempContainer, LV_OBJ_FLAG_SCROLLABLE); 
                                lv_obj_align(checkupTargetChemistryTempContainer, LV_ALIGN_BOTTOM_RIGHT, 0, -50);
                                lv_obj_set_size(checkupTargetChemistryTempContainer, 100, 80); 
                                //lv_obj_set_style_border_color(checkupTargetChemistryTempContainer, lv_palette_main(LV_PALETTE_GREEN), 0);
                                lv_obj_set_style_border_opa(checkupTargetChemistryTempContainer, LV_OPA_TRANSP, 0);

                                        checkupTargetChemistryTempLabel = lv_label_create(checkupTargetChemistryTempContainer);         
                                        lv_label_set_text(checkupTargetChemistryTempLabel, checkupChemistry_text); 
                                        lv_obj_set_style_text_font(checkupTargetChemistryTempLabel, &lv_font_montserrat_18, 0);              
                                        lv_obj_align(checkupTargetChemistryTempLabel, LV_ALIGN_CENTER, 0, -15);

                                        checkupTargetChemistryTempValue = lv_label_create(checkupTargetChemistryTempContainer);         
                                        lv_label_set_text(checkupTargetChemistryTempValue, "49.8°C"); 
                                        lv_obj_set_style_text_font(checkupTargetChemistryTempValue, &lv_font_montserrat_20, 0);              
                                        lv_obj_align(checkupTargetChemistryTempValue, LV_ALIGN_CENTER, 0, 20);



                                checkupSkipButton = lv_button_create(checkupTargetTempsContainer);
                                lv_obj_set_size(checkupSkipButton, BUTTON_PROCESS_WIDTH, BUTTON_PROCESS_HEIGHT);
                                lv_obj_align(checkupSkipButton, LV_ALIGN_BOTTOM_MID, 0, 10);
                                lv_obj_add_event_cb(checkupSkipButton, event_checkup, LV_EVENT_CLICKED, checkupTargetWaterTempContainer);
                                lv_obj_set_style_bg_color(checkupSkipButton, lv_color_hex(GREEN_DARK), LV_PART_MAIN);
                                lv_obj_move_foreground(checkupSkipButton);

                                        checkupSkipButtonLabel = lv_label_create(checkupSkipButton);         
                                        lv_label_set_text(checkupSkipButtonLabel, checkupSkip_text); 
                                        lv_obj_set_style_text_font(checkupSkipButtonLabel, &lv_font_montserrat_22, 0);              
                                        lv_obj_align(checkupSkipButtonLabel, LV_ALIGN_CENTER, 0, 0);
                  }

                  if(processStep == 3){
                        checkupFilmRotatingContainer = lv_obj_create(checkupStepContainer);
                        lv_obj_remove_flag(checkupFilmRotatingContainer, LV_OBJ_FLAG_SCROLLABLE); 
                        lv_obj_align(checkupFilmRotatingContainer, LV_ALIGN_TOP_LEFT, -18, -18);
                        lv_obj_set_size(checkupFilmRotatingContainer, 240, 265); 
                        //lv_obj_set_style_border_color(checkupFilmRotatingContainer, lv_palette_main(LV_PALETTE_GREEN), 0);
                        lv_obj_set_style_border_opa(checkupFilmRotatingContainer, LV_OPA_TRANSP, 0);
                        
                  
                                checkupTankIsPresentContainer = lv_obj_create(checkupFilmRotatingContainer);
                                lv_obj_remove_flag(checkupTankIsPresentContainer, LV_OBJ_FLAG_SCROLLABLE); 
                                lv_obj_align(checkupTankIsPresentContainer, LV_ALIGN_CENTER, 0, -55);
                                lv_obj_set_size(checkupTankIsPresentContainer, 200, 80); 
                                //lv_obj_set_style_border_color(checkupTankIsPresentContainer, lv_palette_main(LV_PALETTE_GREEN), 0);
                                lv_obj_set_style_border_opa(checkupTankIsPresentContainer, LV_OPA_TRANSP, 0);

                                        checkupTankIsPresentLabel = lv_label_create(checkupTankIsPresentContainer);         
                                        lv_label_set_text(checkupTankIsPresentLabel, checkupTankPosition_text); 
                                        lv_obj_set_style_text_font(checkupTankIsPresentLabel, &lv_font_montserrat_18, 0);              
                                        lv_obj_align(checkupTankIsPresentLabel, LV_ALIGN_CENTER, 0, -15);

                                        checkupTankIsPresentValue = lv_label_create(checkupTankIsPresentContainer);         
                                        lv_label_set_text(checkupTankIsPresentValue, checkupYes_text); 
                                        lv_obj_set_style_text_font(checkupTankIsPresentValue, &lv_font_montserrat_24, 0);              
                                        lv_obj_align(checkupTankIsPresentValue, LV_ALIGN_CENTER, 0, 20);
                  

                                checkupFilmInPositionContainer = lv_obj_create(checkupFilmRotatingContainer);
                                lv_obj_remove_flag(checkupFilmInPositionContainer, LV_OBJ_FLAG_SCROLLABLE); 
                                lv_obj_align(checkupFilmInPositionContainer, LV_ALIGN_CENTER, 0, 40);
                                lv_obj_set_size(checkupFilmInPositionContainer, 200, 80); 
                                //lv_obj_set_style_border_color(checkupFilmInPositionContainer, lv_palette_main(LV_PALETTE_GREEN), 0);
                                lv_obj_set_style_border_opa(checkupFilmInPositionContainer, LV_OPA_TRANSP, 0);

                                        checkupFilmRotatingLabel = lv_label_create(checkupFilmInPositionContainer);         
                                        lv_label_set_text(checkupFilmRotatingLabel, checkupFilmRotation_text); 
                                        lv_obj_set_style_text_font(checkupFilmRotatingLabel, &lv_font_montserrat_18, 0);              
                                        lv_obj_align(checkupFilmRotatingLabel, LV_ALIGN_CENTER, 0, -15);

                                        checkupFilmRotatingValue = lv_label_create(checkupFilmInPositionContainer);         
                                        lv_label_set_text(checkupFilmRotatingValue, checkupChecking_text); 
                                        lv_obj_set_style_text_font(checkupFilmRotatingValue, &lv_font_montserrat_24, 0);              
                                        lv_obj_align(checkupFilmRotatingValue, LV_ALIGN_CENTER, 0, 20);


                        checkupStartButton = lv_button_create(checkupFilmRotatingContainer);
                        lv_obj_set_size(checkupStartButton, BUTTON_PROCESS_WIDTH, BUTTON_PROCESS_HEIGHT);
                        lv_obj_align(checkupStartButton, LV_ALIGN_BOTTOM_MID, 0, 10);
                        lv_obj_add_event_cb(checkupStartButton, event_checkup, LV_EVENT_CLICKED, checkupFilmRotatingContainer);
                        lv_obj_set_style_bg_color(checkupStartButton, lv_color_hex(GREEN_DARK), LV_PART_MAIN);
                        lv_obj_move_foreground(checkupStartButton);

                                checkupStartButtonLabel = lv_label_create(checkupStartButton);         
                                lv_label_set_text(checkupStartButtonLabel, checkupStart_text); 
                                lv_obj_set_style_text_font(checkupStartButtonLabel, &lv_font_montserrat_22, 0);              
                                lv_obj_align(checkupStartButtonLabel, LV_ALIGN_CENTER, 0, 0);
                  }

                  if(processStep == 4){
                        checkupProcessingContainer = lv_obj_create(checkupStepContainer);
                        lv_obj_remove_flag(checkupProcessingContainer, LV_OBJ_FLAG_SCROLLABLE); 
                        lv_obj_align(checkupProcessingContainer, LV_ALIGN_TOP_LEFT, -18, -18);
                        lv_obj_set_size(checkupProcessingContainer, 240, 265); 
                        //lv_obj_set_style_border_color(checkupProcessingContainer, lv_palette_main(LV_PALETTE_GREEN), 0);
                        lv_obj_set_style_border_opa(checkupProcessingContainer, LV_OPA_TRANSP, 0);
                        

                                checkupStepTimeLeftValue = lv_label_create(checkupProcessingContainer);         
                                lv_label_set_text(checkupStepTimeLeftValue, "20sec"); 
                                lv_obj_set_style_text_font(checkupStepTimeLeftValue, &lv_font_montserrat_20, 0);              
                                lv_obj_align(checkupStepTimeLeftValue, LV_ALIGN_CENTER, 0, -60);

                                checkupStepNameValue = lv_label_create(checkupProcessingContainer);         
                                lv_label_set_text(checkupStepNameValue, testLongString); 
                                lv_obj_set_style_text_font(checkupStepNameValue, &lv_font_montserrat_22, 0);              
                                lv_obj_align(checkupStepNameValue, LV_ALIGN_CENTER, 0, -40);
                                lv_obj_set_width(checkupStepNameValue, 130);
                                lv_label_set_long_mode(checkupStepNameValue, LV_LABEL_LONG_SCROLL_CIRCULAR);

                                checkupStepKindValue = lv_label_create(checkupProcessingContainer);         
                                lv_label_set_text(checkupStepKindValue, &currentStep[1][0]); 
                                lv_obj_set_style_text_font(checkupStepKindValue, &lv_font_montserrat_20, 0);              
                                lv_obj_align(checkupStepKindValue, LV_ALIGN_CENTER, 0, 10);

                                checkupProcessTimeLeftValue = lv_label_create(checkupProcessingContainer);         
                                lv_label_set_text(checkupProcessTimeLeftValue, "20min 45sec"); 
                                lv_obj_set_style_text_font(checkupProcessTimeLeftValue, &lv_font_montserrat_24, 0);              
                                lv_obj_align(checkupProcessTimeLeftValue, LV_ALIGN_CENTER, 0, 30);
                              
                                processArc = lv_arc_create(checkupProcessingContainer);
                                lv_obj_set_size(processArc, 220, 220);
                                lv_arc_set_rotation(processArc, 140);
                                lv_arc_set_bg_angles(processArc, 0, 260);
                                lv_arc_set_value(processArc, 10);
                                lv_obj_align(processArc, LV_ALIGN_CENTER, 0, 10);
                                lv_obj_remove_style(processArc, NULL, LV_PART_KNOB);
                                lv_obj_remove_flag(processArc, LV_OBJ_FLAG_CLICKABLE);
                                lv_obj_set_style_arc_color(processArc,lv_color_hex(GREEN) , LV_PART_INDICATOR);
                                lv_obj_set_style_arc_color(processArc, lv_color_hex(GREEN_DARK), LV_PART_MAIN);

                                stepArc = lv_arc_create(checkupProcessingContainer);
                                lv_obj_set_size(stepArc, 220, 220);
                                lv_arc_set_rotation(stepArc, 230);
                                lv_arc_set_bg_angles(stepArc, 0, 80);
                                lv_arc_set_value(stepArc, 20);
                                lv_obj_align(stepArc, LV_ALIGN_CENTER, 0, 160);
                                lv_obj_remove_style(stepArc, NULL, LV_PART_KNOB);
                                lv_obj_remove_flag(stepArc, LV_OBJ_FLAG_CLICKABLE);
                                lv_obj_set_style_arc_color(stepArc,lv_color_hex(ORANGE_LIGHT) , LV_PART_INDICATOR);
                                lv_obj_set_style_arc_color(stepArc, lv_color_hex(ORANGE_DARK), LV_PART_MAIN);
                                lv_obj_move_foreground(stepArc);

                  }
}






#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*PAGE_CHECKUP_H*/