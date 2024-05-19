/**
 * @file page_checkup.c
 *
 */


//ESSENTIAL INCLUDES
#include <lvgl.h>
#include "../../include/definitions.h"

extern struct gui_components gui;

//ACCESSORY INCLUDES

processNode *referenceProcess;

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
      if(data == referenceProcess->process.processDetails->checkup->checkupTankSizeTextArea){
          LV_LOG_USER("Set Tank Size");
          rollerPopupCreate(checkupTankSizesList,checkupTankSize_text,referenceProcess);
      }
  }

  if(act_cb == referenceProcess->process.processDetails->checkup->lowVolumeChemRadioButton || act_cb == referenceProcess->process.processDetails->checkup->highVolumeChemRadioButton){
   if(code == LV_EVENT_CLICKED) {
        lv_obj_remove_state(old_cb, LV_STATE_CHECKED);
        lv_obj_add_state(act_cb, LV_STATE_CHECKED); 
        *active_id = lv_obj_get_index(act_cb);
        LV_LOG_USER("Selected chemistry volume radio buttons: %d", (int)referenceProcess->process.processDetails->checkup->activeVolume_index);
   }
  }

  if(code == LV_EVENT_CLICKED){
    if(obj == referenceProcess->process.processDetails->checkup->checkupStartButton){
      if(data == referenceProcess->process.processDetails->checkup->checkupSelectTankChemistryContainer){
        LV_LOG_USER("User pressed referenceProcess->process.processDetails->checkup->checkupStartButton on Step 0");
        referenceProcess->process.processDetails->checkup->isProcessing = 0;
        referenceProcess->process.processDetails->checkup->processStep = 1;
        referenceProcess->process.processDetails->checkup->stepFillWaterStatus = 1;
        referenceProcess->process.processDetails->checkup->stepReachTempStatus = 0;
        referenceProcess->process.processDetails->checkup->stepCheckFilmStatus = 0;
        checkup(referenceProcess);
      }

      if(data == referenceProcess->process.processDetails->checkup->checkupFilmRotatingContainer){
        LV_LOG_USER("User pressed referenceProcess->process.processDetails->checkup->checkupStartButton on Step 3");
        referenceProcess->process.processDetails->checkup->isProcessing = 1;
        referenceProcess->process.processDetails->checkup->processStep = 4;
        referenceProcess->process.processDetails->checkup->stepFillWaterStatus = 2;
        referenceProcess->process.processDetails->checkup->stepReachTempStatus = 2;
        referenceProcess->process.processDetails->checkup->stepCheckFilmStatus = 2;
        checkup(referenceProcess);
      }
  }
    if(obj == referenceProcess->process.processDetails->checkup->checkupSkipButton){
      if(data == referenceProcess->process.processDetails->checkup->checkupFillWaterContainer){
        LV_LOG_USER("User pressed referenceProcess->process.processDetails->checkup->checkupSkipButton on Step 1");
        referenceProcess->process.processDetails->checkup->isProcessing = 0;
        referenceProcess->process.processDetails->checkup->processStep = 2;
        referenceProcess->process.processDetails->checkup->stepFillWaterStatus = 2;
        referenceProcess->process.processDetails->checkup->stepReachTempStatus = 1;
        referenceProcess->process.processDetails->checkup->stepCheckFilmStatus = 0;
        checkup(referenceProcess);
      }
      if(data == referenceProcess->process.processDetails->checkup->checkupTargetWaterTempContainer){
        LV_LOG_USER("User pressed referenceProcess->process.processDetails->checkup->checkupSkipButton on Step 2");
        referenceProcess->process.processDetails->checkup->isProcessing = 0;
        referenceProcess->process.processDetails->checkup->processStep = 3;
        referenceProcess->process.processDetails->checkup->stepFillWaterStatus = 2;
        referenceProcess->process.processDetails->checkup->stepReachTempStatus = 2;
        referenceProcess->process.processDetails->checkup->stepCheckFilmStatus = 1;
        checkup(referenceProcess);
      
      }
    }

    if(obj == referenceProcess->process.processDetails->checkup->checkupCloseButton){
        LV_LOG_USER("User pressed referenceProcess->process.processDetails->checkup->checkupCloseButtonLabel");
        lv_msgbox_close(mboxCont);
        lv_obj_delete(mboxCont);
    }
    if(obj == referenceProcess->process.processDetails->checkup->checkupStopAfterButton){
        LV_LOG_USER("User pressed referenceProcess->process.processDetails->checkup->checkupStopAfterButton");
        messagePopupCreate(warningPopupTitle_text,stopProcessPopupBody_text,checkupStop_text,stepDetailCancel_text, referenceProcess->process.processDetails->checkup->checkupParent);
    }
    if(obj == referenceProcess->process.processDetails->checkup->checkupStopNowButton){
        LV_LOG_USER("User pressed referenceProcess->process.processDetails->checkup->checkupStopNowButton");
        messagePopupCreate(warningPopupTitle_text,stopProcessPopupBody_text,checkupStop_text,stepDetailCancel_text, referenceProcess->process.processDetails->checkup->checkupParent);
    }
    if(obj == referenceProcess->process.processDetails->checkup->checkupStopStepsButton){
        LV_LOG_USER("User pressed referenceProcess->process.processDetails->checkup->checkupStopStepsButton");
        lv_msgbox_close(mboxParent);
        lv_obj_delete(mboxParent);
    }

  }
}


void checkup(processNode *processToCheckup)
{  

      referenceProcess = (processNode*)malloc(sizeof(processNode));

      referenceProcess = processToCheckup;

      LV_LOG_USER("Final checks, current on processToCheckup->process.processDetails->checkup->processStep :%d",processToCheckup->process.processDetails->checkup->processStep);


      processToCheckup->process.processDetails->checkup->checkupParent = lv_obj_class_create_obj(&lv_msgbox_backdrop_class, lv_layer_top());
      lv_obj_class_init_obj(processToCheckup->process.processDetails->checkup->checkupParent);
      lv_obj_remove_flag(processToCheckup->process.processDetails->checkup->checkupParent, LV_OBJ_FLAG_IGNORE_LAYOUT);
      lv_obj_set_size(processToCheckup->process.processDetails->checkup->checkupParent, LV_PCT(100), LV_PCT(100));

      processToCheckup->process.processDetails->checkup->checkupContainer = lv_obj_create(processToCheckup->process.processDetails->checkup->checkupParent);
      lv_obj_align(processToCheckup->process.processDetails->checkup->checkupContainer, LV_ALIGN_CENTER, 0, 0);
      lv_obj_set_size(processToCheckup->process.processDetails->checkup->checkupContainer, LV_PCT(100), LV_PCT(100)); 
      lv_obj_remove_flag(processToCheckup->process.processDetails->checkup->checkupContainer, LV_OBJ_FLAG_SCROLLABLE); 

            processToCheckup->process.processDetails->checkup->checkupCloseButton = lv_button_create(processToCheckup->process.processDetails->checkup->checkupContainer);
            lv_obj_set_size(processToCheckup->process.processDetails->checkup->checkupCloseButton, BUTTON_POPUP_CLOSE_WIDTH * 1.2, BUTTON_POPUP_CLOSE_HEIGHT * 1.2);
            lv_obj_align(processToCheckup->process.processDetails->checkup->checkupCloseButton, LV_ALIGN_TOP_RIGHT, 7 , -10);
            lv_obj_add_event_cb(processToCheckup->process.processDetails->checkup->checkupCloseButton, event_checkup, LV_EVENT_CLICKED, processToCheckup->process.processDetails->checkup->checkupCloseButton);
            lv_obj_set_style_bg_color(processToCheckup->process.processDetails->checkup->checkupCloseButton, lv_color_hex(GREEN_DARK), LV_PART_MAIN);
            if(processToCheckup->process.processDetails->checkup->processStep > 0){
              lv_obj_add_state(processToCheckup->process.processDetails->checkup->checkupCloseButton, LV_STATE_DISABLED);            
            }

                  processToCheckup->process.processDetails->checkup->checkupCloseButtonLabel = lv_label_create(processToCheckup->process.processDetails->checkup->checkupCloseButton);         
                  lv_label_set_text(processToCheckup->process.processDetails->checkup->checkupCloseButtonLabel, closePopup_icon); 
                  lv_obj_set_style_text_font(processToCheckup->process.processDetails->checkup->checkupCloseButtonLabel, &FilMachineFontIcons_30, 0);              
                  lv_obj_align(processToCheckup->process.processDetails->checkup->checkupCloseButtonLabel, LV_ALIGN_CENTER, 0, 0);


            processToCheckup->process.processDetails->checkup->checkupProcessNameContainer = lv_obj_create(processToCheckup->process.processDetails->checkup->checkupContainer);
            lv_obj_remove_flag(processToCheckup->process.processDetails->checkup->checkupProcessNameContainer, LV_OBJ_FLAG_SCROLLABLE); 
            lv_obj_align(processToCheckup->process.processDetails->checkup->checkupProcessNameContainer, LV_ALIGN_TOP_LEFT, -10, -15);
            lv_obj_set_size(processToCheckup->process.processDetails->checkup->checkupProcessNameContainer, 350, 50); 
            //lv_obj_set_style_border_color(processToCheckup->process.processDetails->checkup->checkupProcessNameContainer, lv_color_hex(GREEN_DARK), 0);
            lv_obj_set_style_border_opa(processToCheckup->process.processDetails->checkup->checkupProcessNameContainer, LV_OPA_TRANSP, 0);

                  processToCheckup->process.processDetails->checkup->checkupProcessNameValue = lv_label_create(processToCheckup->process.processDetails->checkup->checkupProcessNameContainer);         
                  lv_label_set_text(processToCheckup->process.processDetails->checkup->checkupProcessNameValue, "E6 six baths"); 
                  lv_obj_set_width(processToCheckup->process.processDetails->checkup->checkupProcessNameValue, 300);
                  lv_obj_set_style_text_font(processToCheckup->process.processDetails->checkup->checkupProcessNameValue, &lv_font_montserrat_30, 0);              
                  lv_obj_align(processToCheckup->process.processDetails->checkup->checkupProcessNameValue, LV_ALIGN_TOP_LEFT, -10, -8);
                  lv_label_set_long_mode(processToCheckup->process.processDetails->checkup->checkupProcessNameValue, LV_LABEL_LONG_SCROLL_CIRCULAR);

            
        //LEFT SIDE OF SCREEN
        if(processToCheckup->process.processDetails->checkup->isProcessing == 0){
            processToCheckup->process.processDetails->checkup->checkupNextStepsContainer = lv_obj_create(processToCheckup->process.processDetails->checkup->checkupContainer);
            lv_obj_remove_flag(processToCheckup->process.processDetails->checkup->checkupNextStepsContainer, LV_OBJ_FLAG_SCROLLABLE); 
            lv_obj_align(processToCheckup->process.processDetails->checkup->checkupNextStepsContainer, LV_ALIGN_TOP_LEFT, -10, 35);
            lv_obj_set_size(processToCheckup->process.processDetails->checkup->checkupNextStepsContainer, 225, 265); 
            lv_obj_set_style_border_color(processToCheckup->process.processDetails->checkup->checkupNextStepsContainer, lv_color_hex(WHITE), 0);
            //lv_obj_set_style_border_opa(processToCheckup->process.processDetails->checkup->checkupNextStepsContainer, LV_OPA_TRANSP, 0);

                  processToCheckup->process.processDetails->checkup->checkupNextStepsLabel = lv_label_create(processToCheckup->process.processDetails->checkup->checkupNextStepsContainer);         
                  lv_label_set_text(processToCheckup->process.processDetails->checkup->checkupNextStepsLabel, checkupNexStepsTitle_text); 
                  lv_obj_set_width(processToCheckup->process.processDetails->checkup->checkupNextStepsLabel, LV_SIZE_CONTENT);
                  lv_obj_set_style_text_font(processToCheckup->process.processDetails->checkup->checkupNextStepsLabel, &lv_font_montserrat_22, 0);              
                  lv_obj_align(processToCheckup->process.processDetails->checkup->checkupNextStepsLabel, LV_ALIGN_TOP_LEFT, -10, -8);

                  processToCheckup->process.processDetails->checkup->checkupMachineWillDoLabel = lv_label_create(processToCheckup->process.processDetails->checkup->checkupNextStepsContainer);
                  lv_label_set_text(processToCheckup->process.processDetails->checkup->checkupMachineWillDoLabel, checkupTheMachineWillDo_text); 
                  lv_obj_set_width(processToCheckup->process.processDetails->checkup->checkupMachineWillDoLabel, LV_SIZE_CONTENT);
                  lv_obj_set_style_text_font(processToCheckup->process.processDetails->checkup->checkupMachineWillDoLabel, &lv_font_montserrat_18, 0);              
                  lv_obj_align(processToCheckup->process.processDetails->checkup->checkupMachineWillDoLabel, LV_ALIGN_TOP_LEFT, -10, 17);

                  processToCheckup->process.processDetails->checkup->checkupWaterFillContainer = lv_obj_create(processToCheckup->process.processDetails->checkup->checkupNextStepsContainer);
                  lv_obj_remove_flag(processToCheckup->process.processDetails->checkup->checkupWaterFillContainer, LV_OBJ_FLAG_SCROLLABLE); 
                  lv_obj_align(processToCheckup->process.processDetails->checkup->checkupWaterFillContainer, LV_ALIGN_TOP_LEFT, -10, 45);
                  lv_obj_set_size(processToCheckup->process.processDetails->checkup->checkupWaterFillContainer, 195, 45); 
                  //lv_obj_set_style_border_color(processToCheckup->process.processDetails->checkup->checkupWaterFillContainer, lv_color_hex(GREEN_DARK), 0);
                  lv_obj_set_style_border_opa(processToCheckup->process.processDetails->checkup->checkupWaterFillContainer, LV_OPA_TRANSP, 0);


                          processToCheckup->process.processDetails->checkup->checkupWaterFillStatusIcon = lv_label_create(processToCheckup->process.processDetails->checkup->checkupWaterFillContainer);         
                          lv_label_set_text(processToCheckup->process.processDetails->checkup->checkupWaterFillStatusIcon, checkupStepStatuses[0][processToCheckup->process.processDetails->checkup->stepFillWaterStatus]); 
                          lv_obj_set_style_text_font(processToCheckup->process.processDetails->checkup->checkupWaterFillStatusIcon, &FilMachineFontIcons_15, 0);              
                          lv_obj_align(processToCheckup->process.processDetails->checkup->checkupWaterFillStatusIcon, LV_ALIGN_LEFT_MID, -15, 0);

                          processToCheckup->process.processDetails->checkup->checkupWaterFillLabel = lv_label_create(processToCheckup->process.processDetails->checkup->checkupWaterFillContainer);         
                          lv_label_set_text(processToCheckup->process.processDetails->checkup->checkupWaterFillLabel, checkupFillWater_text); 
                          lv_obj_set_width(processToCheckup->process.processDetails->checkup->checkupWaterFillLabel, 168);
                          lv_obj_set_style_text_font(processToCheckup->process.processDetails->checkup->checkupWaterFillLabel, &lv_font_montserrat_18, 0);              
                          lv_obj_align(processToCheckup->process.processDetails->checkup->checkupWaterFillLabel, LV_ALIGN_LEFT_MID, 2, 0);
                          lv_label_set_long_mode(processToCheckup->process.processDetails->checkup->checkupWaterFillLabel, LV_LABEL_LONG_SCROLL_CIRCULAR);


                  processToCheckup->process.processDetails->checkup->checkupReachTempContainer = lv_obj_create(processToCheckup->process.processDetails->checkup->checkupNextStepsContainer);
                  lv_obj_remove_flag(processToCheckup->process.processDetails->checkup->checkupReachTempContainer, LV_OBJ_FLAG_SCROLLABLE); 
                  lv_obj_align(processToCheckup->process.processDetails->checkup->checkupReachTempContainer, LV_ALIGN_TOP_LEFT, -10, 90);
                  lv_obj_set_size(processToCheckup->process.processDetails->checkup->checkupReachTempContainer, 195, 45); 
                  //lv_obj_set_style_border_color(processToCheckup->process.processDetails->checkup->checkupReachTempContainer, lv_color_hex(GREEN_DARK), 0);
                  lv_obj_set_style_border_opa(processToCheckup->process.processDetails->checkup->checkupReachTempContainer, LV_OPA_TRANSP, 0);

                          processToCheckup->process.processDetails->checkup->checkupReachTempStatusIcon = lv_label_create(processToCheckup->process.processDetails->checkup->checkupReachTempContainer);         
                          lv_label_set_text(processToCheckup->process.processDetails->checkup->checkupReachTempStatusIcon, checkupStepStatuses[0][processToCheckup->process.processDetails->checkup->stepReachTempStatus]); 
                          lv_obj_set_style_text_font(processToCheckup->process.processDetails->checkup->checkupReachTempStatusIcon, &FilMachineFontIcons_15, 0);              
                          lv_obj_align(processToCheckup->process.processDetails->checkup->checkupReachTempStatusIcon, LV_ALIGN_LEFT_MID, -15, 0);


                          processToCheckup->process.processDetails->checkup->checkupReachTempLabel = lv_label_create(processToCheckup->process.processDetails->checkup->checkupReachTempContainer);         
                          lv_label_set_text(processToCheckup->process.processDetails->checkup->checkupReachTempLabel, checkupReachTemp_text); 
                          lv_obj_set_width(processToCheckup->process.processDetails->checkup->checkupReachTempLabel, 168);
                          lv_obj_set_style_text_font(processToCheckup->process.processDetails->checkup->checkupReachTempLabel, &lv_font_montserrat_18, 0);              
                          lv_obj_align(processToCheckup->process.processDetails->checkup->checkupReachTempLabel, LV_ALIGN_LEFT_MID, 2, 0);
                          lv_label_set_long_mode(processToCheckup->process.processDetails->checkup->checkupReachTempLabel, LV_LABEL_LONG_SCROLL_CIRCULAR);



                  processToCheckup->process.processDetails->checkup->checkupTankAndFilmContainer = lv_obj_create(processToCheckup->process.processDetails->checkup->checkupNextStepsContainer);
                  lv_obj_remove_flag(processToCheckup->process.processDetails->checkup->checkupTankAndFilmContainer, LV_OBJ_FLAG_SCROLLABLE); 
                  lv_obj_align(processToCheckup->process.processDetails->checkup->checkupTankAndFilmContainer, LV_ALIGN_TOP_LEFT, -10, 135);
                  lv_obj_set_size(processToCheckup->process.processDetails->checkup->checkupTankAndFilmContainer, 195, 45); 
                  //lv_obj_set_style_border_color(processToCheckup->process.processDetails->checkup->checkupTankAndFilmContainer, lv_color_hex(GREEN_DARK), 0);
                  lv_obj_set_style_border_opa(processToCheckup->process.processDetails->checkup->checkupTankAndFilmContainer, LV_OPA_TRANSP, 0);

                          processToCheckup->process.processDetails->checkup->checkupTankAndFilmStatusIcon = lv_label_create(processToCheckup->process.processDetails->checkup->checkupTankAndFilmContainer);         
                          lv_label_set_text(processToCheckup->process.processDetails->checkup->checkupTankAndFilmStatusIcon, checkupStepStatuses[0][processToCheckup->process.processDetails->checkup->stepCheckFilmStatus]); 
                          lv_obj_set_style_text_font(processToCheckup->process.processDetails->checkup->checkupTankAndFilmStatusIcon, &FilMachineFontIcons_15, 0);              
                          lv_obj_align(processToCheckup->process.processDetails->checkup->checkupTankAndFilmStatusIcon, LV_ALIGN_LEFT_MID, -15, 0);


                          processToCheckup->process.processDetails->checkup->checkupTankAndFilmLabel = lv_label_create(processToCheckup->process.processDetails->checkup->checkupTankAndFilmContainer);         
                          lv_label_set_text(processToCheckup->process.processDetails->checkup->checkupTankAndFilmLabel, checkupTankRotation_text); 
                          lv_obj_set_width(processToCheckup->process.processDetails->checkup->checkupTankAndFilmLabel, 168);
                          lv_obj_set_style_text_font(processToCheckup->process.processDetails->checkup->checkupTankAndFilmLabel, &lv_font_montserrat_18, 0);              
                          lv_obj_align(processToCheckup->process.processDetails->checkup->checkupTankAndFilmLabel, LV_ALIGN_LEFT_MID, 2, 0);
                          lv_label_set_long_mode(processToCheckup->process.processDetails->checkup->checkupTankAndFilmLabel, LV_LABEL_LONG_SCROLL_CIRCULAR);


                  processToCheckup->process.processDetails->checkup->checkupStopStepsButton = lv_button_create(processToCheckup->process.processDetails->checkup->checkupNextStepsContainer);
                  lv_obj_set_size(processToCheckup->process.processDetails->checkup->checkupStopStepsButton, BUTTON_PROCESS_WIDTH, BUTTON_PROCESS_HEIGHT);
                  lv_obj_align(processToCheckup->process.processDetails->checkup->checkupStopStepsButton, LV_ALIGN_BOTTOM_MID, 0, 10);
                  lv_obj_add_event_cb(processToCheckup->process.processDetails->checkup->checkupStopStepsButton, event_checkup, LV_EVENT_CLICKED, processToCheckup->process.processDetails->checkup->checkupStopStepsButton);
                  lv_obj_set_style_bg_color(processToCheckup->process.processDetails->checkup->checkupStopStepsButton, lv_color_hex(RED_DARK), LV_PART_MAIN);
                  lv_obj_move_foreground(processToCheckup->process.processDetails->checkup->checkupStopStepsButton);
                  if(processToCheckup->process.processDetails->checkup->processStep == 0){
                    lv_obj_add_state(processToCheckup->process.processDetails->checkup->checkupStopStepsButton, LV_STATE_DISABLED);            
                  }

                          processToCheckup->process.processDetails->checkup->checkupStopStepsButtonLabel = lv_label_create(processToCheckup->process.processDetails->checkup->checkupStopStepsButton);         
                          lv_label_set_text(processToCheckup->process.processDetails->checkup->checkupStopStepsButtonLabel, checkupStop_text); 
                          lv_obj_set_style_text_font(processToCheckup->process.processDetails->checkup->checkupStopStepsButtonLabel, &lv_font_montserrat_22, 0);              
                          lv_obj_align(processToCheckup->process.processDetails->checkup->checkupStopStepsButtonLabel, LV_ALIGN_CENTER, 0, 0);

        }
        else{

            processToCheckup->process.processDetails->checkup->checkupNextStepsContainer = lv_obj_create(processToCheckup->process.processDetails->checkup->checkupContainer);
            lv_obj_remove_flag(processToCheckup->process.processDetails->checkup->checkupNextStepsContainer, LV_OBJ_FLAG_SCROLLABLE); 
            lv_obj_align(processToCheckup->process.processDetails->checkup->checkupNextStepsContainer, LV_ALIGN_TOP_LEFT, -10, 35);
            lv_obj_set_size(processToCheckup->process.processDetails->checkup->checkupNextStepsContainer, 225, 265); 
            lv_obj_set_style_border_color(processToCheckup->process.processDetails->checkup->checkupNextStepsContainer, lv_color_hex(WHITE), 0);
            //lv_obj_set_style_border_opa(processToCheckup->process.processDetails->checkup->checkupNextStepsContainer, LV_OPA_TRANSP, 0);


                  processToCheckup->process.processDetails->checkup->checkupNextStepsLabel = lv_label_create(processToCheckup->process.processDetails->checkup->checkupNextStepsContainer);         
                  lv_label_set_text(processToCheckup->process.processDetails->checkup->checkupNextStepsLabel, checkupProcessingTitle_text); 
                  lv_obj_set_width(processToCheckup->process.processDetails->checkup->checkupNextStepsLabel, LV_SIZE_CONTENT);
                  lv_obj_set_style_text_font(processToCheckup->process.processDetails->checkup->checkupNextStepsLabel, &lv_font_montserrat_22, 0);              
                  lv_obj_align(processToCheckup->process.processDetails->checkup->checkupNextStepsLabel, LV_ALIGN_TOP_LEFT, -10, -8);


                  processToCheckup->process.processDetails->checkup->checkupStepSourceContainer = lv_obj_create(processToCheckup->process.processDetails->checkup->checkupNextStepsContainer);
                  lv_obj_remove_flag(processToCheckup->process.processDetails->checkup->checkupStepSourceContainer, LV_OBJ_FLAG_SCROLLABLE); 
                  lv_obj_align(processToCheckup->process.processDetails->checkup->checkupStepSourceContainer, LV_ALIGN_TOP_LEFT, -10, 17);
                  lv_obj_set_size(processToCheckup->process.processDetails->checkup->checkupStepSourceContainer, 215, 45); 
                  //lv_obj_set_style_border_color(processToCheckup->process.processDetails->checkup->checkupStepSourceContainer, lv_color_hex(GREEN_DARK), 0);
                  lv_obj_set_style_border_opa(processToCheckup->process.processDetails->checkup->checkupStepSourceContainer, LV_OPA_TRANSP, 0);


                          processToCheckup->process.processDetails->checkup->checkupStepSourceLabel = lv_label_create(processToCheckup->process.processDetails->checkup->checkupStepSourceContainer);         
                          lv_label_set_text(processToCheckup->process.processDetails->checkup->checkupStepSourceLabel, checkupStepSource_text); 
                          lv_obj_set_style_text_font(processToCheckup->process.processDetails->checkup->checkupStepSourceLabel, &lv_font_montserrat_18, 0);              
                          lv_obj_align(processToCheckup->process.processDetails->checkup->checkupStepSourceLabel, LV_ALIGN_LEFT_MID, -15, 0);

                          processToCheckup->process.processDetails->checkup->checkupStepSourceValue = lv_label_create(processToCheckup->process.processDetails->checkup->checkupStepSourceContainer);         
                          lv_label_set_text(processToCheckup->process.processDetails->checkup->checkupStepSourceValue, &processSourceList[1][0]); 
                          lv_obj_set_width(processToCheckup->process.processDetails->checkup->checkupStepSourceValue, LV_SIZE_CONTENT);
                          lv_obj_set_style_text_font(processToCheckup->process.processDetails->checkup->checkupStepSourceValue, &lv_font_montserrat_20, 0);              
                          lv_obj_align(processToCheckup->process.processDetails->checkup->checkupStepSourceValue, LV_ALIGN_RIGHT_MID, 10, 0);


                  processToCheckup->process.processDetails->checkup->checkupTempControlContainer = lv_obj_create(processToCheckup->process.processDetails->checkup->checkupNextStepsContainer);
                  lv_obj_remove_flag(processToCheckup->process.processDetails->checkup->checkupTempControlContainer, LV_OBJ_FLAG_SCROLLABLE); 
                  lv_obj_align(processToCheckup->process.processDetails->checkup->checkupTempControlContainer, LV_ALIGN_TOP_LEFT, -10, 62);
                  lv_obj_set_size(processToCheckup->process.processDetails->checkup->checkupTempControlContainer, 215, 45); 
                  //lv_obj_set_style_border_color(processToCheckup->process.processDetails->checkup->checkupTempControlContainer, lv_color_hex(GREEN_DARK), 0);
                  lv_obj_set_style_border_opa(processToCheckup->process.processDetails->checkup->checkupTempControlContainer, LV_OPA_TRANSP, 0);


                          processToCheckup->process.processDetails->checkup->checkupTempControlLabel = lv_label_create(processToCheckup->process.processDetails->checkup->checkupTempControlContainer);         
                          lv_label_set_text(processToCheckup->process.processDetails->checkup->checkupTempControlLabel, checkupTempControl_text); 
                          lv_obj_set_style_text_font(processToCheckup->process.processDetails->checkup->checkupTempControlLabel, &lv_font_montserrat_18, 0);              
                          lv_obj_align(processToCheckup->process.processDetails->checkup->checkupTempControlLabel, LV_ALIGN_LEFT_MID, -15, 0);

                          processToCheckup->process.processDetails->checkup->checkupTempControlValue = lv_label_create(processToCheckup->process.processDetails->checkup->checkupTempControlContainer);         
                          lv_label_set_text(processToCheckup->process.processDetails->checkup->checkupTempControlValue, &processTempControlList[1][0]); 
                          lv_obj_set_width(processToCheckup->process.processDetails->checkup->checkupTempControlValue, LV_SIZE_CONTENT);
                          lv_obj_set_style_text_font(processToCheckup->process.processDetails->checkup->checkupTempControlValue, &lv_font_montserrat_20, 0);              
                          lv_obj_align(processToCheckup->process.processDetails->checkup->checkupTempControlValue, LV_ALIGN_RIGHT_MID, 10, 0);



                  processToCheckup->process.processDetails->checkup->checkupWaterTempContainer = lv_obj_create(processToCheckup->process.processDetails->checkup->checkupNextStepsContainer);
                  lv_obj_remove_flag(processToCheckup->process.processDetails->checkup->checkupWaterTempContainer, LV_OBJ_FLAG_SCROLLABLE); 
                  lv_obj_align(processToCheckup->process.processDetails->checkup->checkupWaterTempContainer, LV_ALIGN_TOP_LEFT, -10, 107);
                  lv_obj_set_size(processToCheckup->process.processDetails->checkup->checkupWaterTempContainer, 215, 45); 
                  //lv_obj_set_style_border_color(processToCheckup->process.processDetails->checkup->checkupWaterTempContainer, lv_color_hex(GREEN_DARK), 0);
                  lv_obj_set_style_border_opa(processToCheckup->process.processDetails->checkup->checkupWaterTempContainer, LV_OPA_TRANSP, 0);


                          processToCheckup->process.processDetails->checkup->checkupWaterTempLabel = lv_label_create(processToCheckup->process.processDetails->checkup->checkupWaterTempContainer);         
                          lv_label_set_text(processToCheckup->process.processDetails->checkup->checkupWaterTempLabel, checkupWaterTemp_text); 
                          lv_obj_set_style_text_font(processToCheckup->process.processDetails->checkup->checkupWaterTempLabel, &lv_font_montserrat_18, 0);              
                          lv_obj_align(processToCheckup->process.processDetails->checkup->checkupWaterTempLabel, LV_ALIGN_LEFT_MID, -15, 0);

                          processToCheckup->process.processDetails->checkup->checkupWaterTempValue = lv_label_create(processToCheckup->process.processDetails->checkup->checkupWaterTempContainer);         
                          lv_label_set_text(processToCheckup->process.processDetails->checkup->checkupWaterTempValue, "36.4°C"); 
                          lv_obj_set_width(processToCheckup->process.processDetails->checkup->checkupWaterTempValue, LV_SIZE_CONTENT);
                          lv_obj_set_style_text_font(processToCheckup->process.processDetails->checkup->checkupWaterTempValue, &lv_font_montserrat_20, 0);              
                          lv_obj_align(processToCheckup->process.processDetails->checkup->checkupWaterTempValue, LV_ALIGN_RIGHT_MID, 10, 0);



                  processToCheckup->process.processDetails->checkup->checkupNextStepContainer = lv_obj_create(processToCheckup->process.processDetails->checkup->checkupNextStepsContainer);
                  lv_obj_remove_flag(processToCheckup->process.processDetails->checkup->checkupNextStepContainer, LV_OBJ_FLAG_SCROLLABLE); 
                  lv_obj_align(processToCheckup->process.processDetails->checkup->checkupNextStepContainer, LV_ALIGN_TOP_LEFT, -10, 152);
                  lv_obj_set_size(processToCheckup->process.processDetails->checkup->checkupNextStepContainer, 215, 45); 
                  //lv_obj_set_style_border_color(processToCheckup->process.processDetails->checkup->checkupNextStepContainer, lv_color_hex(GREEN_DARK), 0);
                  lv_obj_set_style_border_opa(processToCheckup->process.processDetails->checkup->checkupNextStepContainer, LV_OPA_TRANSP, 0);


                          processToCheckup->process.processDetails->checkup->checkupNextStepLabel = lv_label_create(processToCheckup->process.processDetails->checkup->checkupNextStepContainer);         
                          lv_label_set_text(processToCheckup->process.processDetails->checkup->checkupNextStepLabel, checkupNextStep_text); 
                          lv_obj_set_style_text_font(processToCheckup->process.processDetails->checkup->checkupNextStepLabel, &lv_font_montserrat_18, 0);              
                          lv_obj_align(processToCheckup->process.processDetails->checkup->checkupNextStepLabel, LV_ALIGN_LEFT_MID, -15, 0);

                          processToCheckup->process.processDetails->checkup->checkupNextStepValue = lv_label_create(processToCheckup->process.processDetails->checkup->checkupNextStepContainer);         
                          lv_label_set_text(processToCheckup->process.processDetails->checkup->checkupNextStepValue, "Pre-bleach"); 
                          lv_obj_set_width(processToCheckup->process.processDetails->checkup->checkupNextStepValue, 105);
                          lv_obj_set_style_text_font(processToCheckup->process.processDetails->checkup->checkupNextStepValue, &lv_font_montserrat_20, 0);              
                          lv_obj_align(processToCheckup->process.processDetails->checkup->checkupNextStepValue, LV_ALIGN_RIGHT_MID, 10, 0);
                          lv_label_set_long_mode(processToCheckup->process.processDetails->checkup->checkupNextStepValue, LV_LABEL_LONG_SCROLL_CIRCULAR);


                  processToCheckup->process.processDetails->checkup->checkupStopNowButton = lv_button_create(processToCheckup->process.processDetails->checkup->checkupNextStepsContainer);
                  lv_obj_set_size(processToCheckup->process.processDetails->checkup->checkupStopNowButton, BUTTON_PROCESS_WIDTH, BUTTON_PROCESS_HEIGHT);
                  lv_obj_align(processToCheckup->process.processDetails->checkup->checkupStopNowButton, LV_ALIGN_BOTTOM_LEFT, -10, 10);
                  lv_obj_add_event_cb(processToCheckup->process.processDetails->checkup->checkupStopNowButton, event_checkup, LV_EVENT_CLICKED, processToCheckup->process.processDetails->checkup->checkupStopStepsButton);
                  lv_obj_set_style_bg_color(processToCheckup->process.processDetails->checkup->checkupStopNowButton, lv_color_hex(RED_DARK), LV_PART_MAIN);
                  lv_obj_move_foreground(processToCheckup->process.processDetails->checkup->checkupStopNowButton);


                          processToCheckup->process.processDetails->checkup->checkupStopNowButtonLabel = lv_label_create(processToCheckup->process.processDetails->checkup->checkupStopNowButton);         
                          lv_label_set_text(processToCheckup->process.processDetails->checkup->checkupStopNowButtonLabel, checkupStopNow_text); 
                          lv_obj_set_style_text_font(processToCheckup->process.processDetails->checkup->checkupStopNowButtonLabel, &lv_font_montserrat_16, 0);              
                          lv_obj_align(processToCheckup->process.processDetails->checkup->checkupStopNowButtonLabel, LV_ALIGN_CENTER, 0, 0);

                  
                  processToCheckup->process.processDetails->checkup->checkupStopAfterButton = lv_button_create(processToCheckup->process.processDetails->checkup->checkupNextStepsContainer);
                  lv_obj_set_size(processToCheckup->process.processDetails->checkup->checkupStopAfterButton, BUTTON_PROCESS_WIDTH, BUTTON_PROCESS_HEIGHT);
                  lv_obj_align(processToCheckup->process.processDetails->checkup->checkupStopAfterButton, LV_ALIGN_BOTTOM_RIGHT, 10, 10);
                  lv_obj_add_event_cb(processToCheckup->process.processDetails->checkup->checkupStopAfterButton, event_checkup, LV_EVENT_CLICKED, processToCheckup->process.processDetails->checkup->checkupStopAfterButton);
                  lv_obj_set_style_bg_color(processToCheckup->process.processDetails->checkup->checkupStopAfterButton, lv_color_hex(RED_DARK), LV_PART_MAIN);
                  lv_obj_move_foreground(processToCheckup->process.processDetails->checkup->checkupStopAfterButton);


                          processToCheckup->process.processDetails->checkup->checkupStopAfterButtonLabel = lv_label_create(processToCheckup->process.processDetails->checkup->checkupStopAfterButton);         
                          lv_label_set_text(processToCheckup->process.processDetails->checkup->checkupStopAfterButtonLabel, checkupStopAfter_text); 
                          lv_obj_set_style_text_font(processToCheckup->process.processDetails->checkup->checkupStopAfterButtonLabel, &lv_font_montserrat_16, 0);              
                          lv_obj_align(processToCheckup->process.processDetails->checkup->checkupStopAfterButtonLabel, LV_ALIGN_CENTER, 0, 0);
        }

            //RIGHT SIDE OF SCREEN
            processToCheckup->process.processDetails->checkup->checkupStepContainer = lv_obj_create(processToCheckup->process.processDetails->checkup->checkupContainer);
            lv_obj_remove_flag(processToCheckup->process.processDetails->checkup->checkupStepContainer, LV_OBJ_FLAG_SCROLLABLE); 
            lv_obj_align(processToCheckup->process.processDetails->checkup->checkupStepContainer, LV_ALIGN_TOP_LEFT, 217, 35);
            lv_obj_set_size(processToCheckup->process.processDetails->checkup->checkupStepContainer, 240, 265); 
            lv_obj_set_style_border_color(processToCheckup->process.processDetails->checkup->checkupStepContainer, lv_palette_main(LV_PALETTE_GREEN), 0);
            //lv_obj_set_style_border_opa(processToCheckup->process.processDetails->checkup->checkupStepContainer, LV_OPA_TRANSP, 0);

                  if(processToCheckup->process.processDetails->checkup->processStep == 0){
                        processToCheckup->process.processDetails->checkup->checkupSelectTankChemistryContainer = lv_obj_create(processToCheckup->process.processDetails->checkup->checkupStepContainer);
                        lv_obj_remove_flag(processToCheckup->process.processDetails->checkup->checkupSelectTankChemistryContainer, LV_OBJ_FLAG_SCROLLABLE); 
                        lv_obj_align(processToCheckup->process.processDetails->checkup->checkupSelectTankChemistryContainer, LV_ALIGN_TOP_LEFT, -18, -18);
                        lv_obj_set_size(processToCheckup->process.processDetails->checkup->checkupSelectTankChemistryContainer, 240, 265); 
                        //lv_obj_set_style_border_color(processToCheckup->process.processDetails->checkup->checkupSelectTankChemistryContainer, lv_palette_main(LV_PALETTE_GREEN), 0);
                        lv_obj_set_style_border_opa(processToCheckup->process.processDetails->checkup->checkupSelectTankChemistryContainer, LV_OPA_TRANSP, 0);
                        lv_obj_add_event_cb(processToCheckup->process.processDetails->checkup->checkupSelectTankChemistryContainer, event_checkup, LV_EVENT_CLICKED, &processToCheckup->process.processDetails->checkup->activeVolume_index);


                              processToCheckup->process.processDetails->checkup->checkupProcessReadyLabel = lv_label_create(processToCheckup->process.processDetails->checkup->checkupSelectTankChemistryContainer);         
                              lv_label_set_text(processToCheckup->process.processDetails->checkup->checkupProcessReadyLabel, checkupProcessReady_text); 
                              lv_obj_set_width(processToCheckup->process.processDetails->checkup->checkupProcessReadyLabel, 230);
                              lv_obj_set_style_text_font(processToCheckup->process.processDetails->checkup->checkupProcessReadyLabel, &lv_font_montserrat_22, 0);              
                              lv_obj_align(processToCheckup->process.processDetails->checkup->checkupProcessReadyLabel, LV_ALIGN_TOP_LEFT, -10, -8);

                              processToCheckup->process.processDetails->checkup->lowVolumeChemRadioButton = create_radiobutton(processToCheckup->process.processDetails->checkup->checkupSelectTankChemistryContainer, checkupChemistryLowVol_text, -105, 45, 27, &lv_font_montserrat_18, lv_color_hex(GREEN_DARK), lv_palette_main(LV_PALETTE_GREEN));
                              processToCheckup->process.processDetails->checkup->highVolumeChemRadioButton = create_radiobutton(processToCheckup->process.processDetails->checkup->checkupSelectTankChemistryContainer, checkupChemistryHighVol_text, -10, 45, 27, &lv_font_montserrat_18, lv_color_hex(GREEN_DARK), lv_palette_main(LV_PALETTE_GREEN));

                              processToCheckup->process.processDetails->checkup->checkupTankSizeLabel = lv_label_create(processToCheckup->process.processDetails->checkup->checkupSelectTankChemistryContainer);         
                              lv_label_set_text(processToCheckup->process.processDetails->checkup->checkupTankSizeLabel, checkupTankSize_text); 
                              lv_obj_set_width(processToCheckup->process.processDetails->checkup->checkupTankSizeLabel, LV_SIZE_CONTENT);
                              lv_obj_set_style_text_font(processToCheckup->process.processDetails->checkup->checkupTankSizeLabel, &lv_font_montserrat_18, 0);              
                              lv_obj_align(processToCheckup->process.processDetails->checkup->checkupTankSizeLabel, LV_ALIGN_TOP_MID, 0, 20);

                              processToCheckup->process.processDetails->checkup->checkupTankSizeTextArea = lv_textarea_create(processToCheckup->process.processDetails->checkup->checkupSelectTankChemistryContainer);
                              lv_textarea_set_one_line(processToCheckup->process.processDetails->checkup->checkupTankSizeTextArea, true);
                              lv_textarea_set_placeholder_text(processToCheckup->process.processDetails->checkup->checkupTankSizeTextArea, checkupTankSizePlaceHolder_text);
                              lv_obj_align(processToCheckup->process.processDetails->checkup->checkupTankSizeTextArea, LV_ALIGN_TOP_MID, 0, 45);
                              lv_obj_set_width(processToCheckup->process.processDetails->checkup->checkupTankSizeTextArea, 100);

                              lv_obj_add_event_cb(processToCheckup->process.processDetails->checkup->checkupTankSizeTextArea, event_checkup, LV_EVENT_ALL, processToCheckup->process.processDetails->checkup->checkupTankSizeTextArea);
                              lv_obj_add_state(processToCheckup->process.processDetails->checkup->checkupTankSizeTextArea, LV_STATE_FOCUSED); /*To be sure the cursor is visible*/
                              lv_obj_set_style_bg_color(processToCheckup->process.processDetails->checkup->checkupTankSizeTextArea, lv_palette_darken(LV_PALETTE_GREY, 3), 0);
                              lv_obj_set_style_text_align(processToCheckup->process.processDetails->checkup->checkupTankSizeTextArea , LV_TEXT_ALIGN_CENTER, 0);
                              //lv_style_set_text_font(&textAreaStyle, &lv_font_montserrat_18);
                              lv_obj_add_style(processToCheckup->process.processDetails->checkup->checkupTankSizeTextArea, &textAreaStyle, LV_PART_MAIN);
                              lv_obj_set_style_border_color(processToCheckup->process.processDetails->checkup->checkupTankSizeTextArea, lv_color_hex(WHITE), 0);

                              processToCheckup->process.processDetails->checkup->checkupChemistryVolumeLabel = lv_label_create(processToCheckup->process.processDetails->checkup->checkupSelectTankChemistryContainer);         
                              lv_label_set_text(processToCheckup->process.processDetails->checkup->checkupChemistryVolumeLabel, checkupChemistryVolume_text); 
                              lv_obj_set_width(processToCheckup->process.processDetails->checkup->checkupChemistryVolumeLabel, LV_SIZE_CONTENT);
                              lv_obj_set_style_text_font(processToCheckup->process.processDetails->checkup->checkupChemistryVolumeLabel, &lv_font_montserrat_18, 0);              
                              lv_obj_align(processToCheckup->process.processDetails->checkup->checkupChemistryVolumeLabel, LV_ALIGN_TOP_MID, 0, 110);



                        processToCheckup->process.processDetails->checkup->checkupStartButton = lv_button_create(processToCheckup->process.processDetails->checkup->checkupSelectTankChemistryContainer);
                        lv_obj_set_size(processToCheckup->process.processDetails->checkup->checkupStartButton, BUTTON_PROCESS_WIDTH, BUTTON_PROCESS_HEIGHT);
                        lv_obj_align(processToCheckup->process.processDetails->checkup->checkupStartButton, LV_ALIGN_BOTTOM_MID, 0, 10);
                        lv_obj_add_event_cb(processToCheckup->process.processDetails->checkup->checkupStartButton, event_checkup, LV_EVENT_CLICKED, processToCheckup->process.processDetails->checkup->checkupSelectTankChemistryContainer);
                        lv_obj_set_style_bg_color(processToCheckup->process.processDetails->checkup->checkupStartButton, lv_color_hex(GREEN_DARK), LV_PART_MAIN);
                        lv_obj_move_foreground(processToCheckup->process.processDetails->checkup->checkupStartButton);

                                processToCheckup->process.processDetails->checkup->checkupStartButtonLabel = lv_label_create(processToCheckup->process.processDetails->checkup->checkupStartButton);         
                                lv_label_set_text(processToCheckup->process.processDetails->checkup->checkupStartButtonLabel, checkupStart_text); 
                                lv_obj_set_style_text_font(processToCheckup->process.processDetails->checkup->checkupStartButtonLabel, &lv_font_montserrat_22, 0);              
                                lv_obj_align(processToCheckup->process.processDetails->checkup->checkupStartButtonLabel, LV_ALIGN_CENTER, 0, 0);
                  }

                  if(processToCheckup->process.processDetails->checkup->processStep == 1){
                        processToCheckup->process.processDetails->checkup->checkupFillWaterContainer = lv_obj_create(processToCheckup->process.processDetails->checkup->checkupStepContainer);
                        lv_obj_remove_flag(processToCheckup->process.processDetails->checkup->checkupFillWaterContainer, LV_OBJ_FLAG_SCROLLABLE); 
                        lv_obj_align(processToCheckup->process.processDetails->checkup->checkupFillWaterContainer, LV_ALIGN_TOP_LEFT, -18, -18);
                        lv_obj_set_size(processToCheckup->process.processDetails->checkup->checkupFillWaterContainer, 240, 265); 
                        //lv_obj_set_style_border_color(processToCheckup->process.processDetails->checkup->checkupFillWaterContainer, lv_palette_main(LV_PALETTE_GREEN), 0);
                        lv_obj_set_style_border_opa(processToCheckup->process.processDetails->checkup->checkupFillWaterContainer, LV_OPA_TRANSP, 0);


                                processToCheckup->process.processDetails->checkup->checkupFillWaterLabel = lv_label_create(processToCheckup->process.processDetails->checkup->checkupFillWaterContainer);         
                                lv_label_set_text(processToCheckup->process.processDetails->checkup->checkupFillWaterLabel, checkupFillWaterMachine_text); 
                                lv_obj_set_style_text_font(processToCheckup->process.processDetails->checkup->checkupFillWaterLabel, &lv_font_montserrat_18, 0);              
                                lv_obj_align(processToCheckup->process.processDetails->checkup->checkupFillWaterLabel, LV_ALIGN_CENTER, 0, -20);
                                lv_obj_set_style_text_align(processToCheckup->process.processDetails->checkup->checkupFillWaterLabel , LV_TEXT_ALIGN_CENTER, 0);
                                lv_label_set_long_mode(processToCheckup->process.processDetails->checkup->checkupFillWaterLabel, LV_LABEL_LONG_WRAP);
                                lv_obj_set_size(processToCheckup->process.processDetails->checkup->checkupFillWaterLabel, 235, LV_SIZE_CONTENT);



                                processToCheckup->process.processDetails->checkup->checkupSkipButton = lv_button_create(processToCheckup->process.processDetails->checkup->checkupFillWaterContainer);
                                lv_obj_set_size(processToCheckup->process.processDetails->checkup->checkupSkipButton, BUTTON_PROCESS_WIDTH, BUTTON_PROCESS_HEIGHT);
                                lv_obj_align(processToCheckup->process.processDetails->checkup->checkupSkipButton, LV_ALIGN_BOTTOM_MID, 0, 10);
                                lv_obj_add_event_cb(processToCheckup->process.processDetails->checkup->checkupSkipButton, event_checkup, LV_EVENT_CLICKED, processToCheckup->process.processDetails->checkup->checkupFillWaterContainer);
                                lv_obj_set_style_bg_color(processToCheckup->process.processDetails->checkup->checkupSkipButton, lv_color_hex(GREEN_DARK), LV_PART_MAIN);
                                lv_obj_move_foreground(processToCheckup->process.processDetails->checkup->checkupSkipButton);

                                        processToCheckup->process.processDetails->checkup->checkupSkipButtonLabel = lv_label_create(processToCheckup->process.processDetails->checkup->checkupSkipButton);         
                                        lv_label_set_text(processToCheckup->process.processDetails->checkup->checkupSkipButtonLabel, checkupSkip_text); 
                                        lv_obj_set_style_text_font(processToCheckup->process.processDetails->checkup->checkupSkipButtonLabel, &lv_font_montserrat_22, 0);              
                                        lv_obj_align(processToCheckup->process.processDetails->checkup->checkupSkipButtonLabel, LV_ALIGN_CENTER, 0, 0);
                  }


                  if(processToCheckup->process.processDetails->checkup->processStep == 2){
                        processToCheckup->process.processDetails->checkup->checkupTargetTempsContainer = lv_obj_create(processToCheckup->process.processDetails->checkup->checkupStepContainer);
                        lv_obj_remove_flag(processToCheckup->process.processDetails->checkup->checkupTargetTempsContainer, LV_OBJ_FLAG_SCROLLABLE); 
                        lv_obj_align(processToCheckup->process.processDetails->checkup->checkupTargetTempsContainer, LV_ALIGN_TOP_LEFT, -18, -18);
                        lv_obj_set_size(processToCheckup->process.processDetails->checkup->checkupTargetTempsContainer, 240, 265); 
                        //lv_obj_set_style_border_color(processToCheckup->process.processDetails->checkup->checkupTargetTempsContainer, lv_palette_main(LV_PALETTE_GREEN), 0);
                        lv_obj_set_style_border_opa(processToCheckup->process.processDetails->checkup->checkupTargetTempsContainer, LV_OPA_TRANSP, 0);


                                processToCheckup->process.processDetails->checkup->checkupTargetTempContainer = lv_obj_create(processToCheckup->process.processDetails->checkup->checkupStepContainer);
                                lv_obj_remove_flag(processToCheckup->process.processDetails->checkup->checkupTargetTempContainer, LV_OBJ_FLAG_SCROLLABLE); 
                                lv_obj_align(processToCheckup->process.processDetails->checkup->checkupTargetTempContainer, LV_ALIGN_TOP_MID, 0, 0);
                                lv_obj_set_size(processToCheckup->process.processDetails->checkup->checkupTargetTempContainer, 200, 80); 
                                //lv_obj_set_style_border_color(processToCheckup->process.processDetails->checkup->checkupTargetTempContainer, lv_palette_main(LV_PALETTE_GREEN), 0);
                                lv_obj_set_style_border_opa(processToCheckup->process.processDetails->checkup->checkupTargetTempContainer, LV_OPA_TRANSP, 0);

                                        processToCheckup->process.processDetails->checkup->checkupTargetTempLabel = lv_label_create(processToCheckup->process.processDetails->checkup->checkupTargetTempContainer);         
                                        lv_label_set_text(processToCheckup->process.processDetails->checkup->checkupTargetTempLabel, checkupTargetTemp_text); 
                                        lv_obj_set_style_text_font(processToCheckup->process.processDetails->checkup->checkupTargetTempLabel, &lv_font_montserrat_18, 0);              
                                        lv_obj_align(processToCheckup->process.processDetails->checkup->checkupTargetTempLabel, LV_ALIGN_CENTER, 0, -15);

                                        processToCheckup->process.processDetails->checkup->checkupTargetTempValue = lv_label_create(processToCheckup->process.processDetails->checkup->checkupTargetTempContainer);         
                                        lv_label_set_text(processToCheckup->process.processDetails->checkup->checkupTargetTempValue, "20.4°C"); 
                                        lv_obj_set_style_text_font(processToCheckup->process.processDetails->checkup->checkupTargetTempValue, &lv_font_montserrat_20, 0);              
                                        lv_obj_align(processToCheckup->process.processDetails->checkup->checkupTargetTempValue, LV_ALIGN_CENTER, 0, 20);


                                processToCheckup->process.processDetails->checkup->checkupTargetWaterTempContainer = lv_obj_create(processToCheckup->process.processDetails->checkup->checkupStepContainer);
                                lv_obj_remove_flag(processToCheckup->process.processDetails->checkup->checkupTargetWaterTempContainer, LV_OBJ_FLAG_SCROLLABLE); 
                                lv_obj_align(processToCheckup->process.processDetails->checkup->checkupTargetWaterTempContainer, LV_ALIGN_BOTTOM_LEFT, 0, -50);
                                lv_obj_set_size(processToCheckup->process.processDetails->checkup->checkupTargetWaterTempContainer, 100, 80); 
                                //lv_obj_set_style_border_color(processToCheckup->process.processDetails->checkup->checkupTargetWaterTempContainer, lv_palette_main(LV_PALETTE_GREEN), 0);
                                lv_obj_set_style_border_opa(processToCheckup->process.processDetails->checkup->checkupTargetWaterTempContainer, LV_OPA_TRANSP, 0);

                                        processToCheckup->process.processDetails->checkup->checkupTargetWaterTempLabel = lv_label_create(processToCheckup->process.processDetails->checkup->checkupTargetWaterTempContainer);         
                                        lv_label_set_text(processToCheckup->process.processDetails->checkup->checkupTargetWaterTempLabel, checkupWater_text); 
                                        lv_obj_set_style_text_font(processToCheckup->process.processDetails->checkup->checkupTargetWaterTempLabel, &lv_font_montserrat_18, 0);              
                                        lv_obj_align(processToCheckup->process.processDetails->checkup->checkupTargetWaterTempLabel, LV_ALIGN_CENTER, 0, -15);

                                        processToCheckup->process.processDetails->checkup->checkupTargetWaterTempValue = lv_label_create(processToCheckup->process.processDetails->checkup->checkupTargetWaterTempContainer);         
                                        lv_label_set_text(processToCheckup->process.processDetails->checkup->checkupTargetWaterTempValue, "15.4°C"); 
                                        lv_obj_set_style_text_font(processToCheckup->process.processDetails->checkup->checkupTargetWaterTempValue, &lv_font_montserrat_20, 0);              
                                        lv_obj_align(processToCheckup->process.processDetails->checkup->checkupTargetWaterTempValue, LV_ALIGN_CENTER, 0, 20);



                                processToCheckup->process.processDetails->checkup->checkupTargetChemistryTempContainer = lv_obj_create(processToCheckup->process.processDetails->checkup->checkupStepContainer);
                                lv_obj_remove_flag(processToCheckup->process.processDetails->checkup->checkupTargetChemistryTempContainer, LV_OBJ_FLAG_SCROLLABLE); 
                                lv_obj_align(processToCheckup->process.processDetails->checkup->checkupTargetChemistryTempContainer, LV_ALIGN_BOTTOM_RIGHT, 0, -50);
                                lv_obj_set_size(processToCheckup->process.processDetails->checkup->checkupTargetChemistryTempContainer, 100, 80); 
                                //lv_obj_set_style_border_color(processToCheckup->process.processDetails->checkup->checkupTargetChemistryTempContainer, lv_palette_main(LV_PALETTE_GREEN), 0);
                                lv_obj_set_style_border_opa(processToCheckup->process.processDetails->checkup->checkupTargetChemistryTempContainer, LV_OPA_TRANSP, 0);

                                        processToCheckup->process.processDetails->checkup->checkupTargetChemistryTempLabel = lv_label_create(processToCheckup->process.processDetails->checkup->checkupTargetChemistryTempContainer);         
                                        lv_label_set_text(processToCheckup->process.processDetails->checkup->checkupTargetChemistryTempLabel, checkupChemistry_text); 
                                        lv_obj_set_style_text_font(processToCheckup->process.processDetails->checkup->checkupTargetChemistryTempLabel, &lv_font_montserrat_18, 0);              
                                        lv_obj_align(processToCheckup->process.processDetails->checkup->checkupTargetChemistryTempLabel, LV_ALIGN_CENTER, 0, -15);

                                        processToCheckup->process.processDetails->checkup->checkupTargetChemistryTempValue = lv_label_create(processToCheckup->process.processDetails->checkup->checkupTargetChemistryTempContainer);         
                                        lv_label_set_text(processToCheckup->process.processDetails->checkup->checkupTargetChemistryTempValue, "49.8°C"); 
                                        lv_obj_set_style_text_font(processToCheckup->process.processDetails->checkup->checkupTargetChemistryTempValue, &lv_font_montserrat_20, 0);              
                                        lv_obj_align(processToCheckup->process.processDetails->checkup->checkupTargetChemistryTempValue, LV_ALIGN_CENTER, 0, 20);



                                processToCheckup->process.processDetails->checkup->checkupSkipButton = lv_button_create(processToCheckup->process.processDetails->checkup->checkupTargetTempsContainer);
                                lv_obj_set_size(processToCheckup->process.processDetails->checkup->checkupSkipButton, BUTTON_PROCESS_WIDTH, BUTTON_PROCESS_HEIGHT);
                                lv_obj_align(processToCheckup->process.processDetails->checkup->checkupSkipButton, LV_ALIGN_BOTTOM_MID, 0, 10);
                                lv_obj_add_event_cb(processToCheckup->process.processDetails->checkup->checkupSkipButton, event_checkup, LV_EVENT_CLICKED, processToCheckup->process.processDetails->checkup->checkupTargetWaterTempContainer);
                                lv_obj_set_style_bg_color(processToCheckup->process.processDetails->checkup->checkupSkipButton, lv_color_hex(GREEN_DARK), LV_PART_MAIN);
                                lv_obj_move_foreground(processToCheckup->process.processDetails->checkup->checkupSkipButton);

                                        processToCheckup->process.processDetails->checkup->checkupSkipButtonLabel = lv_label_create(processToCheckup->process.processDetails->checkup->checkupSkipButton);         
                                        lv_label_set_text(processToCheckup->process.processDetails->checkup->checkupSkipButtonLabel, checkupSkip_text); 
                                        lv_obj_set_style_text_font(processToCheckup->process.processDetails->checkup->checkupSkipButtonLabel, &lv_font_montserrat_22, 0);              
                                        lv_obj_align(processToCheckup->process.processDetails->checkup->checkupSkipButtonLabel, LV_ALIGN_CENTER, 0, 0);
                  }

                  if(processToCheckup->process.processDetails->checkup->processStep == 3){
                        processToCheckup->process.processDetails->checkup->checkupFilmRotatingContainer = lv_obj_create(processToCheckup->process.processDetails->checkup->checkupStepContainer);
                        lv_obj_remove_flag(processToCheckup->process.processDetails->checkup->checkupFilmRotatingContainer, LV_OBJ_FLAG_SCROLLABLE); 
                        lv_obj_align(processToCheckup->process.processDetails->checkup->checkupFilmRotatingContainer, LV_ALIGN_TOP_LEFT, -18, -18);
                        lv_obj_set_size(processToCheckup->process.processDetails->checkup->checkupFilmRotatingContainer, 240, 265); 
                        //lv_obj_set_style_border_color(processToCheckup->process.processDetails->checkup->checkupFilmRotatingContainer, lv_palette_main(LV_PALETTE_GREEN), 0);
                        lv_obj_set_style_border_opa(processToCheckup->process.processDetails->checkup->checkupFilmRotatingContainer, LV_OPA_TRANSP, 0);
                        
                  
                                processToCheckup->process.processDetails->checkup->checkupTankIsPresentContainer = lv_obj_create(processToCheckup->process.processDetails->checkup->checkupFilmRotatingContainer);
                                lv_obj_remove_flag(processToCheckup->process.processDetails->checkup->checkupTankIsPresentContainer, LV_OBJ_FLAG_SCROLLABLE); 
                                lv_obj_align(processToCheckup->process.processDetails->checkup->checkupTankIsPresentContainer, LV_ALIGN_CENTER, 0, -55);
                                lv_obj_set_size(processToCheckup->process.processDetails->checkup->checkupTankIsPresentContainer, 200, 80); 
                                //lv_obj_set_style_border_color(processToCheckup->process.processDetails->checkup->checkupTankIsPresentContainer, lv_palette_main(LV_PALETTE_GREEN), 0);
                                lv_obj_set_style_border_opa(processToCheckup->process.processDetails->checkup->checkupTankIsPresentContainer, LV_OPA_TRANSP, 0);

                                        processToCheckup->process.processDetails->checkup->checkupTankIsPresentLabel = lv_label_create(processToCheckup->process.processDetails->checkup->checkupTankIsPresentContainer);         
                                        lv_label_set_text(processToCheckup->process.processDetails->checkup->checkupTankIsPresentLabel, checkupTankPosition_text); 
                                        lv_obj_set_style_text_font(processToCheckup->process.processDetails->checkup->checkupTankIsPresentLabel, &lv_font_montserrat_18, 0);              
                                        lv_obj_align(processToCheckup->process.processDetails->checkup->checkupTankIsPresentLabel, LV_ALIGN_CENTER, 0, -15);

                                        processToCheckup->process.processDetails->checkup->checkupTankIsPresentValue = lv_label_create(processToCheckup->process.processDetails->checkup->checkupTankIsPresentContainer);         
                                        lv_label_set_text(processToCheckup->process.processDetails->checkup->checkupTankIsPresentValue, checkupYes_text); 
                                        lv_obj_set_style_text_font(processToCheckup->process.processDetails->checkup->checkupTankIsPresentValue, &lv_font_montserrat_24, 0);              
                                        lv_obj_align(processToCheckup->process.processDetails->checkup->checkupTankIsPresentValue, LV_ALIGN_CENTER, 0, 20);
                  

                                processToCheckup->process.processDetails->checkup->checkupFilmInPositionContainer = lv_obj_create(processToCheckup->process.processDetails->checkup->checkupFilmRotatingContainer);
                                lv_obj_remove_flag(processToCheckup->process.processDetails->checkup->checkupFilmInPositionContainer, LV_OBJ_FLAG_SCROLLABLE); 
                                lv_obj_align(processToCheckup->process.processDetails->checkup->checkupFilmInPositionContainer, LV_ALIGN_CENTER, 0, 40);
                                lv_obj_set_size(processToCheckup->process.processDetails->checkup->checkupFilmInPositionContainer, 200, 80); 
                                //lv_obj_set_style_border_color(processToCheckup->process.processDetails->checkup->checkupFilmInPositionContainer, lv_palette_main(LV_PALETTE_GREEN), 0);
                                lv_obj_set_style_border_opa(processToCheckup->process.processDetails->checkup->checkupFilmInPositionContainer, LV_OPA_TRANSP, 0);

                                        processToCheckup->process.processDetails->checkup->checkupFilmRotatingLabel = lv_label_create(processToCheckup->process.processDetails->checkup->checkupFilmInPositionContainer);         
                                        lv_label_set_text(processToCheckup->process.processDetails->checkup->checkupFilmRotatingLabel, checkupFilmRotation_text); 
                                        lv_obj_set_style_text_font(processToCheckup->process.processDetails->checkup->checkupFilmRotatingLabel, &lv_font_montserrat_18, 0);              
                                        lv_obj_align(processToCheckup->process.processDetails->checkup->checkupFilmRotatingLabel, LV_ALIGN_CENTER, 0, -15);

                                        processToCheckup->process.processDetails->checkup->checkupFilmRotatingValue = lv_label_create(processToCheckup->process.processDetails->checkup->checkupFilmInPositionContainer);         
                                        lv_label_set_text(processToCheckup->process.processDetails->checkup->checkupFilmRotatingValue, checkupChecking_text); 
                                        lv_obj_set_style_text_font(processToCheckup->process.processDetails->checkup->checkupFilmRotatingValue, &lv_font_montserrat_24, 0);              
                                        lv_obj_align(processToCheckup->process.processDetails->checkup->checkupFilmRotatingValue, LV_ALIGN_CENTER, 0, 20);


                        processToCheckup->process.processDetails->checkup->checkupStartButton = lv_button_create(processToCheckup->process.processDetails->checkup->checkupFilmRotatingContainer);
                        lv_obj_set_size(processToCheckup->process.processDetails->checkup->checkupStartButton, BUTTON_PROCESS_WIDTH, BUTTON_PROCESS_HEIGHT);
                        lv_obj_align(processToCheckup->process.processDetails->checkup->checkupStartButton, LV_ALIGN_BOTTOM_MID, 0, 10);
                        lv_obj_add_event_cb(processToCheckup->process.processDetails->checkup->checkupStartButton, event_checkup, LV_EVENT_CLICKED, processToCheckup->process.processDetails->checkup->checkupFilmRotatingContainer);
                        lv_obj_set_style_bg_color(processToCheckup->process.processDetails->checkup->checkupStartButton, lv_color_hex(GREEN_DARK), LV_PART_MAIN);
                        lv_obj_move_foreground(processToCheckup->process.processDetails->checkup->checkupStartButton);

                                processToCheckup->process.processDetails->checkup->checkupStartButtonLabel = lv_label_create(processToCheckup->process.processDetails->checkup->checkupStartButton);         
                                lv_label_set_text(processToCheckup->process.processDetails->checkup->checkupStartButtonLabel, checkupStart_text); 
                                lv_obj_set_style_text_font(processToCheckup->process.processDetails->checkup->checkupStartButtonLabel, &lv_font_montserrat_22, 0);              
                                lv_obj_align(processToCheckup->process.processDetails->checkup->checkupStartButtonLabel, LV_ALIGN_CENTER, 0, 0);
                  }

                  if(processToCheckup->process.processDetails->checkup->processStep == 4){
                        processToCheckup->process.processDetails->checkup->checkupProcessingContainer = lv_obj_create(processToCheckup->process.processDetails->checkup->checkupStepContainer);
                        lv_obj_remove_flag(processToCheckup->process.processDetails->checkup->checkupProcessingContainer, LV_OBJ_FLAG_SCROLLABLE); 
                        lv_obj_align(processToCheckup->process.processDetails->checkup->checkupProcessingContainer, LV_ALIGN_TOP_LEFT, -18, -18);
                        lv_obj_set_size(processToCheckup->process.processDetails->checkup->checkupProcessingContainer, 240, 265); 
                        //lv_obj_set_style_border_color(processToCheckup->process.processDetails->checkup->checkupProcessingContainer, lv_palette_main(LV_PALETTE_GREEN), 0);
                        lv_obj_set_style_border_opa(processToCheckup->process.processDetails->checkup->checkupProcessingContainer, LV_OPA_TRANSP, 0);
                        

                                processToCheckup->process.processDetails->checkup->checkupStepTimeLeftValue = lv_label_create(processToCheckup->process.processDetails->checkup->checkupProcessingContainer);         
                                lv_label_set_text(processToCheckup->process.processDetails->checkup->checkupStepTimeLeftValue, "20sec"); 
                                lv_obj_set_style_text_font(processToCheckup->process.processDetails->checkup->checkupStepTimeLeftValue, &lv_font_montserrat_20, 0);              
                                lv_obj_align(processToCheckup->process.processDetails->checkup->checkupStepTimeLeftValue, LV_ALIGN_CENTER, 0, -60);

                                processToCheckup->process.processDetails->checkup->checkupStepNameValue = lv_label_create(processToCheckup->process.processDetails->checkup->checkupProcessingContainer);         
                                lv_label_set_text(processToCheckup->process.processDetails->checkup->checkupStepNameValue, testLongString); 
                                lv_obj_set_style_text_font(processToCheckup->process.processDetails->checkup->checkupStepNameValue, &lv_font_montserrat_22, 0);              
                                lv_obj_align(processToCheckup->process.processDetails->checkup->checkupStepNameValue, LV_ALIGN_CENTER, 0, -40);
                                lv_obj_set_width(processToCheckup->process.processDetails->checkup->checkupStepNameValue, 130);
                                lv_label_set_long_mode(processToCheckup->process.processDetails->checkup->checkupStepNameValue, LV_LABEL_LONG_SCROLL_CIRCULAR);

                                processToCheckup->process.processDetails->checkup->checkupStepKindValue = lv_label_create(processToCheckup->process.processDetails->checkup->checkupProcessingContainer);         
                                lv_label_set_text(processToCheckup->process.processDetails->checkup->checkupStepKindValue, &currentStep[1][0]); 
                                lv_obj_set_style_text_font(processToCheckup->process.processDetails->checkup->checkupStepKindValue, &lv_font_montserrat_20, 0);              
                                lv_obj_align(processToCheckup->process.processDetails->checkup->checkupStepKindValue, LV_ALIGN_CENTER, 0, 10);

                                processToCheckup->process.processDetails->checkup->checkupProcessTimeLeftValue = lv_label_create(processToCheckup->process.processDetails->checkup->checkupProcessingContainer);         
                                lv_label_set_text(processToCheckup->process.processDetails->checkup->checkupProcessTimeLeftValue, "20min 45sec"); 
                                lv_obj_set_style_text_font(processToCheckup->process.processDetails->checkup->checkupProcessTimeLeftValue, &lv_font_montserrat_24, 0);              
                                lv_obj_align(processToCheckup->process.processDetails->checkup->checkupProcessTimeLeftValue, LV_ALIGN_CENTER, 0, 30);
                              
                                processToCheckup->process.processDetails->checkup->processArc = lv_arc_create(processToCheckup->process.processDetails->checkup->checkupProcessingContainer);
                                lv_obj_set_size(processToCheckup->process.processDetails->checkup->processArc, 220, 220);
                                lv_arc_set_rotation(processToCheckup->process.processDetails->checkup->processArc, 140);
                                lv_arc_set_bg_angles(processToCheckup->process.processDetails->checkup->processArc, 0, 260);
                                lv_arc_set_value(processToCheckup->process.processDetails->checkup->processArc, 10);
                                lv_obj_align(processToCheckup->process.processDetails->checkup->processArc, LV_ALIGN_CENTER, 0, 10);
                                lv_obj_remove_style(processToCheckup->process.processDetails->checkup->processArc, NULL, LV_PART_KNOB);
                                lv_obj_remove_flag(processToCheckup->process.processDetails->checkup->processArc, LV_OBJ_FLAG_CLICKABLE);
                                lv_obj_set_style_arc_color(processToCheckup->process.processDetails->checkup->processArc,lv_color_hex(GREEN) , LV_PART_INDICATOR);
                                lv_obj_set_style_arc_color(processToCheckup->process.processDetails->checkup->processArc, lv_color_hex(GREEN_DARK), LV_PART_MAIN);

                                processToCheckup->process.processDetails->checkup->stepArc = lv_arc_create(processToCheckup->process.processDetails->checkup->checkupProcessingContainer);
                                lv_obj_set_size(processToCheckup->process.processDetails->checkup->stepArc, 220, 220);
                                lv_arc_set_rotation(processToCheckup->process.processDetails->checkup->stepArc, 230);
                                lv_arc_set_bg_angles(processToCheckup->process.processDetails->checkup->stepArc, 0, 80);
                                lv_arc_set_value(processToCheckup->process.processDetails->checkup->stepArc, 20);
                                lv_obj_align(processToCheckup->process.processDetails->checkup->stepArc, LV_ALIGN_CENTER, 0, 160);
                                lv_obj_remove_style(processToCheckup->process.processDetails->checkup->stepArc, NULL, LV_PART_KNOB);
                                lv_obj_remove_flag(processToCheckup->process.processDetails->checkup->stepArc, LV_OBJ_FLAG_CLICKABLE);
                                lv_obj_set_style_arc_color(processToCheckup->process.processDetails->checkup->stepArc,lv_color_hex(ORANGE_LIGHT) , LV_PART_INDICATOR);
                                lv_obj_set_style_arc_color(processToCheckup->process.processDetails->checkup->stepArc, lv_color_hex(ORANGE_DARK), LV_PART_MAIN);
                                lv_obj_move_foreground(referenceProcess->process.processDetails->checkup->stepArc);

                  }
}


