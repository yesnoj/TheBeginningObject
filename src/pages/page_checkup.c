/**
 * @file page_checkup.c
 *
 */


//ESSENTIAL INCLUDES
#include <lvgl.h>
#include "../../include/definitions.h"

extern struct gui_components gui;

//ACCESSORY INCLUDES



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
      if(data == gui.page.checkup.checkupTankSizeTextArea){
          LV_LOG_USER("Set Tank Size");
          rollerPopupCreate(checkupTankSizesList,checkupTankSize_text,gui.page.checkup.checkupTankSizeTextArea);
      }
  }

  if(act_cb == gui.page.checkup.lowVolumeChemRadioButton || act_cb == gui.page.checkup.highVolumeChemRadioButton){
   if(code == LV_EVENT_CLICKED) {
        lv_obj_remove_state(old_cb, LV_STATE_CHECKED);
        lv_obj_add_state(act_cb, LV_STATE_CHECKED); 
        *active_id = lv_obj_get_index(act_cb);
        LV_LOG_USER("Selected chemistry volume radio buttons: %d", (int)gui.page.checkup.activeVolume_index);
   }
  }

  if(code == LV_EVENT_CLICKED){
    if(obj == gui.page.checkup.checkupStartButton){
      if(data == gui.page.checkup.checkupSelectTankChemistryContainer){
        LV_LOG_USER("User pressed gui.page.checkup.checkupStartButton on Step 0");
        gui.page.checkup.isProcessing = 0;
        gui.page.checkup.processStep = 1;
        gui.page.checkup.stepFillWaterStatus = 1;
        gui.page.checkup.stepReachTempStatus = 0;
        gui.page.checkup.stepCheckFilmStatus = 0;
        checkup();
      }

      if(data == gui.page.checkup.checkupFilmRotatingContainer){
        LV_LOG_USER("User pressed gui.page.checkup.checkupStartButton on Step 3");
        gui.page.checkup.isProcessing = 1;
        gui.page.checkup.processStep = 4;
        gui.page.checkup.stepFillWaterStatus = 2;
        gui.page.checkup.stepReachTempStatus = 2;
        gui.page.checkup.stepCheckFilmStatus = 2;
        checkup();
      }
  }
    if(obj == gui.page.checkup.checkupSkipButton){
      if(data == gui.page.checkup.checkupFillWaterContainer){
        LV_LOG_USER("User pressed gui.page.checkup.checkupSkipButton on Step 1");
        gui.page.checkup.isProcessing = 0;
        gui.page.checkup.processStep = 2;
        gui.page.checkup.stepFillWaterStatus = 2;
        gui.page.checkup.stepReachTempStatus = 1;
        gui.page.checkup.stepCheckFilmStatus = 0;
        checkup();
      }
      if(data == gui.page.checkup.checkupTargetWaterTempContainer){
        LV_LOG_USER("User pressed gui.page.checkup.checkupSkipButton on Step 2");
        gui.page.checkup.isProcessing = 0;
        gui.page.checkup.processStep = 3;
        gui.page.checkup.stepFillWaterStatus = 2;
        gui.page.checkup.stepReachTempStatus = 2;
        gui.page.checkup.stepCheckFilmStatus = 1;
        checkup();
      
      }
    }

    if(obj == gui.page.checkup.checkupCloseButton){
        LV_LOG_USER("User pressed gui.page.checkup.checkupCloseButtonLabel");
        lv_msgbox_close(mboxCont);
        lv_obj_delete(mboxCont);
    }
    if(obj == gui.page.checkup.checkupStopAfterButton){
        LV_LOG_USER("User pressed gui.page.checkup.checkupStopAfterButton");
        messagePopupCreate(warningPopupTitle_text,stopProcessPopupBody_text,checkupStop_text,stepDetailCancel_text, gui.page.checkup.checkupParent);
    }
    if(obj == gui.page.checkup.checkupStopNowButton){
        LV_LOG_USER("User pressed gui.page.checkup.checkupStopNowButton");
        messagePopupCreate(warningPopupTitle_text,stopProcessPopupBody_text,checkupStop_text,stepDetailCancel_text, gui.page.checkup.checkupParent);
    }
    if(obj == gui.page.checkup.checkupStopStepsButton){
        LV_LOG_USER("User pressed gui.page.checkup.checkupStopStepsButton");
        lv_msgbox_close(mboxParent);
        lv_obj_delete(mboxParent);
    }

  }
}


void checkup(void)
{  
      LV_LOG_USER("Final checks, current on gui.page.checkup.processStep :%d",gui.page.checkup.processStep);


      gui.page.checkup.checkupParent = lv_obj_class_create_obj(&lv_msgbox_backdrop_class, lv_layer_top());
      lv_obj_class_init_obj(gui.page.checkup.checkupParent);
      lv_obj_remove_flag(gui.page.checkup.checkupParent, LV_OBJ_FLAG_IGNORE_LAYOUT);
      lv_obj_set_size(gui.page.checkup.checkupParent, LV_PCT(100), LV_PCT(100));

      gui.page.checkup.checkupContainer = lv_obj_create(gui.page.checkup.checkupParent);
      lv_obj_align(gui.page.checkup.checkupContainer, LV_ALIGN_CENTER, 0, 0);
      lv_obj_set_size(gui.page.checkup.checkupContainer, LV_PCT(100), LV_PCT(100)); 
      lv_obj_remove_flag(gui.page.checkup.checkupContainer, LV_OBJ_FLAG_SCROLLABLE); 

            gui.page.checkup.checkupCloseButton = lv_button_create(gui.page.checkup.checkupContainer);
            lv_obj_set_size(gui.page.checkup.checkupCloseButton, BUTTON_POPUP_CLOSE_WIDTH * 1.2, BUTTON_POPUP_CLOSE_HEIGHT * 1.2);
            lv_obj_align(gui.page.checkup.checkupCloseButton, LV_ALIGN_TOP_RIGHT, 7 , -10);
            lv_obj_add_event_cb(gui.page.checkup.checkupCloseButton, event_checkup, LV_EVENT_CLICKED, gui.page.checkup.checkupCloseButton);
            lv_obj_set_style_bg_color(gui.page.checkup.checkupCloseButton, lv_color_hex(GREEN_DARK), LV_PART_MAIN);
            if(gui.page.checkup.processStep > 0){
              lv_obj_add_state(gui.page.checkup.checkupCloseButton, LV_STATE_DISABLED);            
            }

                  gui.page.checkup.checkupCloseButtonLabel = lv_label_create(gui.page.checkup.checkupCloseButton);         
                  lv_label_set_text(gui.page.checkup.checkupCloseButtonLabel, closePopup_icon); 
                  lv_obj_set_style_text_font(gui.page.checkup.checkupCloseButtonLabel, &FilMachineFontIcons_30, 0);              
                  lv_obj_align(gui.page.checkup.checkupCloseButtonLabel, LV_ALIGN_CENTER, 0, 0);


            gui.page.checkup.checkupProcessNameContainer = lv_obj_create(gui.page.checkup.checkupContainer);
            lv_obj_remove_flag(gui.page.checkup.checkupProcessNameContainer, LV_OBJ_FLAG_SCROLLABLE); 
            lv_obj_align(gui.page.checkup.checkupProcessNameContainer, LV_ALIGN_TOP_LEFT, -10, -15);
            lv_obj_set_size(gui.page.checkup.checkupProcessNameContainer, 350, 50); 
            //lv_obj_set_style_border_color(gui.page.checkup.checkupProcessNameContainer, lv_color_hex(GREEN_DARK), 0);
            lv_obj_set_style_border_opa(gui.page.checkup.checkupProcessNameContainer, LV_OPA_TRANSP, 0);

                  gui.page.checkup.checkupProcessNameValue = lv_label_create(gui.page.checkup.checkupProcessNameContainer);         
                  lv_label_set_text(gui.page.checkup.checkupProcessNameValue, "E6 six baths"); 
                  lv_obj_set_width(gui.page.checkup.checkupProcessNameValue, 300);
                  lv_obj_set_style_text_font(gui.page.checkup.checkupProcessNameValue, &lv_font_montserrat_30, 0);              
                  lv_obj_align(gui.page.checkup.checkupProcessNameValue, LV_ALIGN_TOP_LEFT, -10, -8);
                  lv_label_set_long_mode(gui.page.checkup.checkupProcessNameValue, LV_LABEL_LONG_SCROLL_CIRCULAR);

            
        //LEFT SIDE OF SCREEN
        if(gui.page.checkup.isProcessing == 0){
            gui.page.checkup.checkupNextStepsContainer = lv_obj_create(gui.page.checkup.checkupContainer);
            lv_obj_remove_flag(gui.page.checkup.checkupNextStepsContainer, LV_OBJ_FLAG_SCROLLABLE); 
            lv_obj_align(gui.page.checkup.checkupNextStepsContainer, LV_ALIGN_TOP_LEFT, -10, 35);
            lv_obj_set_size(gui.page.checkup.checkupNextStepsContainer, 225, 265); 
            lv_obj_set_style_border_color(gui.page.checkup.checkupNextStepsContainer, lv_color_hex(WHITE), 0);
            //lv_obj_set_style_border_opa(gui.page.checkup.checkupNextStepsContainer, LV_OPA_TRANSP, 0);

                  gui.page.checkup.checkupNextStepsLabel = lv_label_create(gui.page.checkup.checkupNextStepsContainer);         
                  lv_label_set_text(gui.page.checkup.checkupNextStepsLabel, checkupNexStepsTitle_text); 
                  lv_obj_set_width(gui.page.checkup.checkupNextStepsLabel, LV_SIZE_CONTENT);
                  lv_obj_set_style_text_font(gui.page.checkup.checkupNextStepsLabel, &lv_font_montserrat_22, 0);              
                  lv_obj_align(gui.page.checkup.checkupNextStepsLabel, LV_ALIGN_TOP_LEFT, -10, -8);

                  gui.page.checkup.checkupMachineWillDoLabel = lv_label_create(gui.page.checkup.checkupNextStepsContainer);
                  lv_label_set_text(gui.page.checkup.checkupMachineWillDoLabel, checkupTheMachineWillDo_text); 
                  lv_obj_set_width(gui.page.checkup.checkupMachineWillDoLabel, LV_SIZE_CONTENT);
                  lv_obj_set_style_text_font(gui.page.checkup.checkupMachineWillDoLabel, &lv_font_montserrat_18, 0);              
                  lv_obj_align(gui.page.checkup.checkupMachineWillDoLabel, LV_ALIGN_TOP_LEFT, -10, 17);

                  gui.page.checkup.checkupWaterFillContainer = lv_obj_create(gui.page.checkup.checkupNextStepsContainer);
                  lv_obj_remove_flag(gui.page.checkup.checkupWaterFillContainer, LV_OBJ_FLAG_SCROLLABLE); 
                  lv_obj_align(gui.page.checkup.checkupWaterFillContainer, LV_ALIGN_TOP_LEFT, -10, 45);
                  lv_obj_set_size(gui.page.checkup.checkupWaterFillContainer, 195, 45); 
                  //lv_obj_set_style_border_color(gui.page.checkup.checkupWaterFillContainer, lv_color_hex(GREEN_DARK), 0);
                  lv_obj_set_style_border_opa(gui.page.checkup.checkupWaterFillContainer, LV_OPA_TRANSP, 0);


                          gui.page.checkup.checkupWaterFillStatusIcon = lv_label_create(gui.page.checkup.checkupWaterFillContainer);         
                          lv_label_set_text(gui.page.checkup.checkupWaterFillStatusIcon, checkupStepStatuses[0][gui.page.checkup.stepFillWaterStatus]); 
                          lv_obj_set_style_text_font(gui.page.checkup.checkupWaterFillStatusIcon, &FilMachineFontIcons_15, 0);              
                          lv_obj_align(gui.page.checkup.checkupWaterFillStatusIcon, LV_ALIGN_LEFT_MID, -15, 0);

                          gui.page.checkup.checkupWaterFillLabel = lv_label_create(gui.page.checkup.checkupWaterFillContainer);         
                          lv_label_set_text(gui.page.checkup.checkupWaterFillLabel, checkupFillWater_text); 
                          lv_obj_set_width(gui.page.checkup.checkupWaterFillLabel, 168);
                          lv_obj_set_style_text_font(gui.page.checkup.checkupWaterFillLabel, &lv_font_montserrat_18, 0);              
                          lv_obj_align(gui.page.checkup.checkupWaterFillLabel, LV_ALIGN_LEFT_MID, 2, 0);
                          lv_label_set_long_mode(gui.page.checkup.checkupWaterFillLabel, LV_LABEL_LONG_SCROLL_CIRCULAR);


                  gui.page.checkup.checkupReachTempContainer = lv_obj_create(gui.page.checkup.checkupNextStepsContainer);
                  lv_obj_remove_flag(gui.page.checkup.checkupReachTempContainer, LV_OBJ_FLAG_SCROLLABLE); 
                  lv_obj_align(gui.page.checkup.checkupReachTempContainer, LV_ALIGN_TOP_LEFT, -10, 90);
                  lv_obj_set_size(gui.page.checkup.checkupReachTempContainer, 195, 45); 
                  //lv_obj_set_style_border_color(gui.page.checkup.checkupReachTempContainer, lv_color_hex(GREEN_DARK), 0);
                  lv_obj_set_style_border_opa(gui.page.checkup.checkupReachTempContainer, LV_OPA_TRANSP, 0);

                          gui.page.checkup.checkupReachTempStatusIcon = lv_label_create(gui.page.checkup.checkupReachTempContainer);         
                          lv_label_set_text(gui.page.checkup.checkupReachTempStatusIcon, checkupStepStatuses[0][gui.page.checkup.stepReachTempStatus]); 
                          lv_obj_set_style_text_font(gui.page.checkup.checkupReachTempStatusIcon, &FilMachineFontIcons_15, 0);              
                          lv_obj_align(gui.page.checkup.checkupReachTempStatusIcon, LV_ALIGN_LEFT_MID, -15, 0);


                          gui.page.checkup.checkupReachTempLabel = lv_label_create(gui.page.checkup.checkupReachTempContainer);         
                          lv_label_set_text(gui.page.checkup.checkupReachTempLabel, checkupReachTemp_text); 
                          lv_obj_set_width(gui.page.checkup.checkupReachTempLabel, 168);
                          lv_obj_set_style_text_font(gui.page.checkup.checkupReachTempLabel, &lv_font_montserrat_18, 0);              
                          lv_obj_align(gui.page.checkup.checkupReachTempLabel, LV_ALIGN_LEFT_MID, 2, 0);
                          lv_label_set_long_mode(gui.page.checkup.checkupReachTempLabel, LV_LABEL_LONG_SCROLL_CIRCULAR);



                  gui.page.checkup.checkupTankAndFilmContainer = lv_obj_create(gui.page.checkup.checkupNextStepsContainer);
                  lv_obj_remove_flag(gui.page.checkup.checkupTankAndFilmContainer, LV_OBJ_FLAG_SCROLLABLE); 
                  lv_obj_align(gui.page.checkup.checkupTankAndFilmContainer, LV_ALIGN_TOP_LEFT, -10, 135);
                  lv_obj_set_size(gui.page.checkup.checkupTankAndFilmContainer, 195, 45); 
                  //lv_obj_set_style_border_color(gui.page.checkup.checkupTankAndFilmContainer, lv_color_hex(GREEN_DARK), 0);
                  lv_obj_set_style_border_opa(gui.page.checkup.checkupTankAndFilmContainer, LV_OPA_TRANSP, 0);

                          gui.page.checkup.checkupTankAndFilmStatusIcon = lv_label_create(gui.page.checkup.checkupTankAndFilmContainer);         
                          lv_label_set_text(gui.page.checkup.checkupTankAndFilmStatusIcon, checkupStepStatuses[0][gui.page.checkup.stepCheckFilmStatus]); 
                          lv_obj_set_style_text_font(gui.page.checkup.checkupTankAndFilmStatusIcon, &FilMachineFontIcons_15, 0);              
                          lv_obj_align(gui.page.checkup.checkupTankAndFilmStatusIcon, LV_ALIGN_LEFT_MID, -15, 0);


                          gui.page.checkup.checkupTankAndFilmLabel = lv_label_create(gui.page.checkup.checkupTankAndFilmContainer);         
                          lv_label_set_text(gui.page.checkup.checkupTankAndFilmLabel, checkupTankRotation_text); 
                          lv_obj_set_width(gui.page.checkup.checkupTankAndFilmLabel, 168);
                          lv_obj_set_style_text_font(gui.page.checkup.checkupTankAndFilmLabel, &lv_font_montserrat_18, 0);              
                          lv_obj_align(gui.page.checkup.checkupTankAndFilmLabel, LV_ALIGN_LEFT_MID, 2, 0);
                          lv_label_set_long_mode(gui.page.checkup.checkupTankAndFilmLabel, LV_LABEL_LONG_SCROLL_CIRCULAR);


                  gui.page.checkup.checkupStopStepsButton = lv_button_create(gui.page.checkup.checkupNextStepsContainer);
                  lv_obj_set_size(gui.page.checkup.checkupStopStepsButton, BUTTON_PROCESS_WIDTH, BUTTON_PROCESS_HEIGHT);
                  lv_obj_align(gui.page.checkup.checkupStopStepsButton, LV_ALIGN_BOTTOM_MID, 0, 10);
                  lv_obj_add_event_cb(gui.page.checkup.checkupStopStepsButton, event_checkup, LV_EVENT_CLICKED, gui.page.checkup.checkupStopStepsButton);
                  lv_obj_set_style_bg_color(gui.page.checkup.checkupStopStepsButton, lv_color_hex(RED_DARK), LV_PART_MAIN);
                  lv_obj_move_foreground(gui.page.checkup.checkupStopStepsButton);
                  if(gui.page.checkup.processStep == 0){
                    lv_obj_add_state(gui.page.checkup.checkupStopStepsButton, LV_STATE_DISABLED);            
                  }

                          gui.page.checkup.checkupStopStepsButtonLabel = lv_label_create(gui.page.checkup.checkupStopStepsButton);         
                          lv_label_set_text(gui.page.checkup.checkupStopStepsButtonLabel, checkupStop_text); 
                          lv_obj_set_style_text_font(gui.page.checkup.checkupStopStepsButtonLabel, &lv_font_montserrat_22, 0);              
                          lv_obj_align(gui.page.checkup.checkupStopStepsButtonLabel, LV_ALIGN_CENTER, 0, 0);

        }
        else{

            gui.page.checkup.checkupNextStepsContainer = lv_obj_create(gui.page.checkup.checkupContainer);
            lv_obj_remove_flag(gui.page.checkup.checkupNextStepsContainer, LV_OBJ_FLAG_SCROLLABLE); 
            lv_obj_align(gui.page.checkup.checkupNextStepsContainer, LV_ALIGN_TOP_LEFT, -10, 35);
            lv_obj_set_size(gui.page.checkup.checkupNextStepsContainer, 225, 265); 
            lv_obj_set_style_border_color(gui.page.checkup.checkupNextStepsContainer, lv_color_hex(WHITE), 0);
            //lv_obj_set_style_border_opa(gui.page.checkup.checkupNextStepsContainer, LV_OPA_TRANSP, 0);


                  gui.page.checkup.checkupNextStepsLabel = lv_label_create(gui.page.checkup.checkupNextStepsContainer);         
                  lv_label_set_text(gui.page.checkup.checkupNextStepsLabel, checkupProcessingTitle_text); 
                  lv_obj_set_width(gui.page.checkup.checkupNextStepsLabel, LV_SIZE_CONTENT);
                  lv_obj_set_style_text_font(gui.page.checkup.checkupNextStepsLabel, &lv_font_montserrat_22, 0);              
                  lv_obj_align(gui.page.checkup.checkupNextStepsLabel, LV_ALIGN_TOP_LEFT, -10, -8);


                  gui.page.checkup.checkupStepSourceContainer = lv_obj_create(gui.page.checkup.checkupNextStepsContainer);
                  lv_obj_remove_flag(gui.page.checkup.checkupStepSourceContainer, LV_OBJ_FLAG_SCROLLABLE); 
                  lv_obj_align(gui.page.checkup.checkupStepSourceContainer, LV_ALIGN_TOP_LEFT, -10, 17);
                  lv_obj_set_size(gui.page.checkup.checkupStepSourceContainer, 215, 45); 
                  //lv_obj_set_style_border_color(gui.page.checkup.checkupStepSourceContainer, lv_color_hex(GREEN_DARK), 0);
                  lv_obj_set_style_border_opa(gui.page.checkup.checkupStepSourceContainer, LV_OPA_TRANSP, 0);


                          gui.page.checkup.checkupStepSourceLabel = lv_label_create(gui.page.checkup.checkupStepSourceContainer);         
                          lv_label_set_text(gui.page.checkup.checkupStepSourceLabel, checkupStepSource_text); 
                          lv_obj_set_style_text_font(gui.page.checkup.checkupStepSourceLabel, &lv_font_montserrat_18, 0);              
                          lv_obj_align(gui.page.checkup.checkupStepSourceLabel, LV_ALIGN_LEFT_MID, -15, 0);

                          gui.page.checkup.checkupStepSourceValue = lv_label_create(gui.page.checkup.checkupStepSourceContainer);         
                          lv_label_set_text(gui.page.checkup.checkupStepSourceValue, &processSourceList[1][0]); 
                          lv_obj_set_width(gui.page.checkup.checkupStepSourceValue, LV_SIZE_CONTENT);
                          lv_obj_set_style_text_font(gui.page.checkup.checkupStepSourceValue, &lv_font_montserrat_20, 0);              
                          lv_obj_align(gui.page.checkup.checkupStepSourceValue, LV_ALIGN_RIGHT_MID, 10, 0);


                  gui.page.checkup.checkupTempControlContainer = lv_obj_create(gui.page.checkup.checkupNextStepsContainer);
                  lv_obj_remove_flag(gui.page.checkup.checkupTempControlContainer, LV_OBJ_FLAG_SCROLLABLE); 
                  lv_obj_align(gui.page.checkup.checkupTempControlContainer, LV_ALIGN_TOP_LEFT, -10, 62);
                  lv_obj_set_size(gui.page.checkup.checkupTempControlContainer, 215, 45); 
                  //lv_obj_set_style_border_color(gui.page.checkup.checkupTempControlContainer, lv_color_hex(GREEN_DARK), 0);
                  lv_obj_set_style_border_opa(gui.page.checkup.checkupTempControlContainer, LV_OPA_TRANSP, 0);


                          gui.page.checkup.checkupTempControlLabel = lv_label_create(gui.page.checkup.checkupTempControlContainer);         
                          lv_label_set_text(gui.page.checkup.checkupTempControlLabel, checkupTempControl_text); 
                          lv_obj_set_style_text_font(gui.page.checkup.checkupTempControlLabel, &lv_font_montserrat_18, 0);              
                          lv_obj_align(gui.page.checkup.checkupTempControlLabel, LV_ALIGN_LEFT_MID, -15, 0);

                          gui.page.checkup.checkupTempControlValue = lv_label_create(gui.page.checkup.checkupTempControlContainer);         
                          lv_label_set_text(gui.page.checkup.checkupTempControlValue, &processTempControlList[1][0]); 
                          lv_obj_set_width(gui.page.checkup.checkupTempControlValue, LV_SIZE_CONTENT);
                          lv_obj_set_style_text_font(gui.page.checkup.checkupTempControlValue, &lv_font_montserrat_20, 0);              
                          lv_obj_align(gui.page.checkup.checkupTempControlValue, LV_ALIGN_RIGHT_MID, 10, 0);



                  gui.page.checkup.checkupWaterTempContainer = lv_obj_create(gui.page.checkup.checkupNextStepsContainer);
                  lv_obj_remove_flag(gui.page.checkup.checkupWaterTempContainer, LV_OBJ_FLAG_SCROLLABLE); 
                  lv_obj_align(gui.page.checkup.checkupWaterTempContainer, LV_ALIGN_TOP_LEFT, -10, 107);
                  lv_obj_set_size(gui.page.checkup.checkupWaterTempContainer, 215, 45); 
                  //lv_obj_set_style_border_color(gui.page.checkup.checkupWaterTempContainer, lv_color_hex(GREEN_DARK), 0);
                  lv_obj_set_style_border_opa(gui.page.checkup.checkupWaterTempContainer, LV_OPA_TRANSP, 0);


                          gui.page.checkup.checkupWaterTempLabel = lv_label_create(gui.page.checkup.checkupWaterTempContainer);         
                          lv_label_set_text(gui.page.checkup.checkupWaterTempLabel, checkupWaterTemp_text); 
                          lv_obj_set_style_text_font(gui.page.checkup.checkupWaterTempLabel, &lv_font_montserrat_18, 0);              
                          lv_obj_align(gui.page.checkup.checkupWaterTempLabel, LV_ALIGN_LEFT_MID, -15, 0);

                          gui.page.checkup.checkupWaterTempValue = lv_label_create(gui.page.checkup.checkupWaterTempContainer);         
                          lv_label_set_text(gui.page.checkup.checkupWaterTempValue, "36.4°C"); 
                          lv_obj_set_width(gui.page.checkup.checkupWaterTempValue, LV_SIZE_CONTENT);
                          lv_obj_set_style_text_font(gui.page.checkup.checkupWaterTempValue, &lv_font_montserrat_20, 0);              
                          lv_obj_align(gui.page.checkup.checkupWaterTempValue, LV_ALIGN_RIGHT_MID, 10, 0);



                  gui.page.checkup.checkupNextStepContainer = lv_obj_create(gui.page.checkup.checkupNextStepsContainer);
                  lv_obj_remove_flag(gui.page.checkup.checkupNextStepContainer, LV_OBJ_FLAG_SCROLLABLE); 
                  lv_obj_align(gui.page.checkup.checkupNextStepContainer, LV_ALIGN_TOP_LEFT, -10, 152);
                  lv_obj_set_size(gui.page.checkup.checkupNextStepContainer, 215, 45); 
                  //lv_obj_set_style_border_color(gui.page.checkup.checkupNextStepContainer, lv_color_hex(GREEN_DARK), 0);
                  lv_obj_set_style_border_opa(gui.page.checkup.checkupNextStepContainer, LV_OPA_TRANSP, 0);


                          gui.page.checkup.checkupNextStepLabel = lv_label_create(gui.page.checkup.checkupNextStepContainer);         
                          lv_label_set_text(gui.page.checkup.checkupNextStepLabel, checkupNextStep_text); 
                          lv_obj_set_style_text_font(gui.page.checkup.checkupNextStepLabel, &lv_font_montserrat_18, 0);              
                          lv_obj_align(gui.page.checkup.checkupNextStepLabel, LV_ALIGN_LEFT_MID, -15, 0);

                          gui.page.checkup.checkupNextStepValue = lv_label_create(gui.page.checkup.checkupNextStepContainer);         
                          lv_label_set_text(gui.page.checkup.checkupNextStepValue, "Pre-bleach"); 
                          lv_obj_set_width(gui.page.checkup.checkupNextStepValue, 105);
                          lv_obj_set_style_text_font(gui.page.checkup.checkupNextStepValue, &lv_font_montserrat_20, 0);              
                          lv_obj_align(gui.page.checkup.checkupNextStepValue, LV_ALIGN_RIGHT_MID, 10, 0);
                          lv_label_set_long_mode(gui.page.checkup.checkupNextStepValue, LV_LABEL_LONG_SCROLL_CIRCULAR);


                  gui.page.checkup.checkupStopNowButton = lv_button_create(gui.page.checkup.checkupNextStepsContainer);
                  lv_obj_set_size(gui.page.checkup.checkupStopNowButton, BUTTON_PROCESS_WIDTH, BUTTON_PROCESS_HEIGHT);
                  lv_obj_align(gui.page.checkup.checkupStopNowButton, LV_ALIGN_BOTTOM_LEFT, -10, 10);
                  lv_obj_add_event_cb(gui.page.checkup.checkupStopNowButton, event_checkup, LV_EVENT_CLICKED, gui.page.checkup.checkupStopStepsButton);
                  lv_obj_set_style_bg_color(gui.page.checkup.checkupStopNowButton, lv_color_hex(RED_DARK), LV_PART_MAIN);
                  lv_obj_move_foreground(gui.page.checkup.checkupStopNowButton);


                          gui.page.checkup.checkupStopNowButtonLabel = lv_label_create(gui.page.checkup.checkupStopNowButton);         
                          lv_label_set_text(gui.page.checkup.checkupStopNowButtonLabel, checkupStopNow_text); 
                          lv_obj_set_style_text_font(gui.page.checkup.checkupStopNowButtonLabel, &lv_font_montserrat_16, 0);              
                          lv_obj_align(gui.page.checkup.checkupStopNowButtonLabel, LV_ALIGN_CENTER, 0, 0);

                  
                  gui.page.checkup.checkupStopAfterButton = lv_button_create(gui.page.checkup.checkupNextStepsContainer);
                  lv_obj_set_size(gui.page.checkup.checkupStopAfterButton, BUTTON_PROCESS_WIDTH, BUTTON_PROCESS_HEIGHT);
                  lv_obj_align(gui.page.checkup.checkupStopAfterButton, LV_ALIGN_BOTTOM_RIGHT, 10, 10);
                  lv_obj_add_event_cb(gui.page.checkup.checkupStopAfterButton, event_checkup, LV_EVENT_CLICKED, gui.page.checkup.checkupStopAfterButton);
                  lv_obj_set_style_bg_color(gui.page.checkup.checkupStopAfterButton, lv_color_hex(RED_DARK), LV_PART_MAIN);
                  lv_obj_move_foreground(gui.page.checkup.checkupStopAfterButton);


                          gui.page.checkup.checkupStopAfterButtonLabel = lv_label_create(gui.page.checkup.checkupStopAfterButton);         
                          lv_label_set_text(gui.page.checkup.checkupStopAfterButtonLabel, checkupStopAfter_text); 
                          lv_obj_set_style_text_font(gui.page.checkup.checkupStopAfterButtonLabel, &lv_font_montserrat_16, 0);              
                          lv_obj_align(gui.page.checkup.checkupStopAfterButtonLabel, LV_ALIGN_CENTER, 0, 0);
        }

            //RIGHT SIDE OF SCREEN
            gui.page.checkup.checkupStepContainer = lv_obj_create(gui.page.checkup.checkupContainer);
            lv_obj_remove_flag(gui.page.checkup.checkupStepContainer, LV_OBJ_FLAG_SCROLLABLE); 
            lv_obj_align(gui.page.checkup.checkupStepContainer, LV_ALIGN_TOP_LEFT, 217, 35);
            lv_obj_set_size(gui.page.checkup.checkupStepContainer, 240, 265); 
            lv_obj_set_style_border_color(gui.page.checkup.checkupStepContainer, lv_palette_main(LV_PALETTE_GREEN), 0);
            //lv_obj_set_style_border_opa(gui.page.checkup.checkupStepContainer, LV_OPA_TRANSP, 0);

                  if(gui.page.checkup.processStep == 0){
                        gui.page.checkup.checkupSelectTankChemistryContainer = lv_obj_create(gui.page.checkup.checkupStepContainer);
                        lv_obj_remove_flag(gui.page.checkup.checkupSelectTankChemistryContainer, LV_OBJ_FLAG_SCROLLABLE); 
                        lv_obj_align(gui.page.checkup.checkupSelectTankChemistryContainer, LV_ALIGN_TOP_LEFT, -18, -18);
                        lv_obj_set_size(gui.page.checkup.checkupSelectTankChemistryContainer, 240, 265); 
                        //lv_obj_set_style_border_color(gui.page.checkup.checkupSelectTankChemistryContainer, lv_palette_main(LV_PALETTE_GREEN), 0);
                        lv_obj_set_style_border_opa(gui.page.checkup.checkupSelectTankChemistryContainer, LV_OPA_TRANSP, 0);
                        lv_obj_add_event_cb(gui.page.checkup.checkupSelectTankChemistryContainer, event_checkup, LV_EVENT_CLICKED, &gui.page.checkup.activeVolume_index);


                              gui.page.checkup.checkupProcessReadyLabel = lv_label_create(gui.page.checkup.checkupSelectTankChemistryContainer);         
                              lv_label_set_text(gui.page.checkup.checkupProcessReadyLabel, checkupProcessReady_text); 
                              lv_obj_set_width(gui.page.checkup.checkupProcessReadyLabel, 230);
                              lv_obj_set_style_text_font(gui.page.checkup.checkupProcessReadyLabel, &lv_font_montserrat_22, 0);              
                              lv_obj_align(gui.page.checkup.checkupProcessReadyLabel, LV_ALIGN_TOP_LEFT, -10, -8);

                              gui.page.checkup.lowVolumeChemRadioButton = create_radiobutton(gui.page.checkup.checkupSelectTankChemistryContainer, checkupChemistryLowVol_text, -105, 45, 27, &lv_font_montserrat_18, lv_color_hex(GREEN_DARK), lv_palette_main(LV_PALETTE_GREEN));
                              gui.page.checkup.highVolumeChemRadioButton = create_radiobutton(gui.page.checkup.checkupSelectTankChemistryContainer, checkupChemistryHighVol_text, -10, 45, 27, &lv_font_montserrat_18, lv_color_hex(GREEN_DARK), lv_palette_main(LV_PALETTE_GREEN));

                              gui.page.checkup.checkupTankSizeLabel = lv_label_create(gui.page.checkup.checkupSelectTankChemistryContainer);         
                              lv_label_set_text(gui.page.checkup.checkupTankSizeLabel, checkupTankSize_text); 
                              lv_obj_set_width(gui.page.checkup.checkupTankSizeLabel, LV_SIZE_CONTENT);
                              lv_obj_set_style_text_font(gui.page.checkup.checkupTankSizeLabel, &lv_font_montserrat_18, 0);              
                              lv_obj_align(gui.page.checkup.checkupTankSizeLabel, LV_ALIGN_TOP_MID, 0, 20);

                              gui.page.checkup.checkupTankSizeTextArea = lv_textarea_create(gui.page.checkup.checkupSelectTankChemistryContainer);
                              lv_textarea_set_one_line(gui.page.checkup.checkupTankSizeTextArea, true);
                              lv_textarea_set_placeholder_text(gui.page.checkup.checkupTankSizeTextArea, checkupTankSizePlaceHolder_text);
                              lv_obj_align(gui.page.checkup.checkupTankSizeTextArea, LV_ALIGN_TOP_MID, 0, 45);
                              lv_obj_set_width(gui.page.checkup.checkupTankSizeTextArea, 100);

                              lv_obj_add_event_cb(gui.page.checkup.checkupTankSizeTextArea, event_checkup, LV_EVENT_ALL, gui.page.checkup.checkupTankSizeTextArea);
                              lv_obj_add_state(gui.page.checkup.checkupTankSizeTextArea, LV_STATE_FOCUSED); /*To be sure the cursor is visible*/
                              lv_obj_set_style_bg_color(gui.page.checkup.checkupTankSizeTextArea, lv_palette_darken(LV_PALETTE_GREY, 3), 0);
                              lv_obj_set_style_text_align(gui.page.checkup.checkupTankSizeTextArea , LV_TEXT_ALIGN_CENTER, 0);
                              //lv_style_set_text_font(&textAreaStyle, &lv_font_montserrat_18);
                              lv_obj_add_style(gui.page.checkup.checkupTankSizeTextArea, &textAreaStyle, LV_PART_MAIN);
                              lv_obj_set_style_border_color(gui.page.checkup.checkupTankSizeTextArea, lv_color_hex(WHITE), 0);

                              gui.page.checkup.checkupChemistryVolumeLabel = lv_label_create(gui.page.checkup.checkupSelectTankChemistryContainer);         
                              lv_label_set_text(gui.page.checkup.checkupChemistryVolumeLabel, checkupChemistryVolume_text); 
                              lv_obj_set_width(gui.page.checkup.checkupChemistryVolumeLabel, LV_SIZE_CONTENT);
                              lv_obj_set_style_text_font(gui.page.checkup.checkupChemistryVolumeLabel, &lv_font_montserrat_18, 0);              
                              lv_obj_align(gui.page.checkup.checkupChemistryVolumeLabel, LV_ALIGN_TOP_MID, 0, 110);



                        gui.page.checkup.checkupStartButton = lv_button_create(gui.page.checkup.checkupSelectTankChemistryContainer);
                        lv_obj_set_size(gui.page.checkup.checkupStartButton, BUTTON_PROCESS_WIDTH, BUTTON_PROCESS_HEIGHT);
                        lv_obj_align(gui.page.checkup.checkupStartButton, LV_ALIGN_BOTTOM_MID, 0, 10);
                        lv_obj_add_event_cb(gui.page.checkup.checkupStartButton, event_checkup, LV_EVENT_CLICKED, gui.page.checkup.checkupSelectTankChemistryContainer);
                        lv_obj_set_style_bg_color(gui.page.checkup.checkupStartButton, lv_color_hex(GREEN_DARK), LV_PART_MAIN);
                        lv_obj_move_foreground(gui.page.checkup.checkupStartButton);

                                gui.page.checkup.checkupStartButtonLabel = lv_label_create(gui.page.checkup.checkupStartButton);         
                                lv_label_set_text(gui.page.checkup.checkupStartButtonLabel, checkupStart_text); 
                                lv_obj_set_style_text_font(gui.page.checkup.checkupStartButtonLabel, &lv_font_montserrat_22, 0);              
                                lv_obj_align(gui.page.checkup.checkupStartButtonLabel, LV_ALIGN_CENTER, 0, 0);
                  }

                  if(gui.page.checkup.processStep == 1){
                        gui.page.checkup.checkupFillWaterContainer = lv_obj_create(gui.page.checkup.checkupStepContainer);
                        lv_obj_remove_flag(gui.page.checkup.checkupFillWaterContainer, LV_OBJ_FLAG_SCROLLABLE); 
                        lv_obj_align(gui.page.checkup.checkupFillWaterContainer, LV_ALIGN_TOP_LEFT, -18, -18);
                        lv_obj_set_size(gui.page.checkup.checkupFillWaterContainer, 240, 265); 
                        //lv_obj_set_style_border_color(gui.page.checkup.checkupFillWaterContainer, lv_palette_main(LV_PALETTE_GREEN), 0);
                        lv_obj_set_style_border_opa(gui.page.checkup.checkupFillWaterContainer, LV_OPA_TRANSP, 0);


                                gui.page.checkup.checkupFillWaterLabel = lv_label_create(gui.page.checkup.checkupFillWaterContainer);         
                                lv_label_set_text(gui.page.checkup.checkupFillWaterLabel, checkupFillWaterMachine_text); 
                                lv_obj_set_style_text_font(gui.page.checkup.checkupFillWaterLabel, &lv_font_montserrat_18, 0);              
                                lv_obj_align(gui.page.checkup.checkupFillWaterLabel, LV_ALIGN_CENTER, 0, -20);
                                lv_obj_set_style_text_align(gui.page.checkup.checkupFillWaterLabel , LV_TEXT_ALIGN_CENTER, 0);
                                lv_label_set_long_mode(gui.page.checkup.checkupFillWaterLabel, LV_LABEL_LONG_WRAP);
                                lv_obj_set_size(gui.page.checkup.checkupFillWaterLabel, 235, LV_SIZE_CONTENT);



                                gui.page.checkup.checkupSkipButton = lv_button_create(gui.page.checkup.checkupFillWaterContainer);
                                lv_obj_set_size(gui.page.checkup.checkupSkipButton, BUTTON_PROCESS_WIDTH, BUTTON_PROCESS_HEIGHT);
                                lv_obj_align(gui.page.checkup.checkupSkipButton, LV_ALIGN_BOTTOM_MID, 0, 10);
                                lv_obj_add_event_cb(gui.page.checkup.checkupSkipButton, event_checkup, LV_EVENT_CLICKED, gui.page.checkup.checkupFillWaterContainer);
                                lv_obj_set_style_bg_color(gui.page.checkup.checkupSkipButton, lv_color_hex(GREEN_DARK), LV_PART_MAIN);
                                lv_obj_move_foreground(gui.page.checkup.checkupSkipButton);

                                        gui.page.checkup.checkupSkipButtonLabel = lv_label_create(gui.page.checkup.checkupSkipButton);         
                                        lv_label_set_text(gui.page.checkup.checkupSkipButtonLabel, checkupSkip_text); 
                                        lv_obj_set_style_text_font(gui.page.checkup.checkupSkipButtonLabel, &lv_font_montserrat_22, 0);              
                                        lv_obj_align(gui.page.checkup.checkupSkipButtonLabel, LV_ALIGN_CENTER, 0, 0);
                  }


                  if(gui.page.checkup.processStep == 2){
                        gui.page.checkup.checkupTargetTempsContainer = lv_obj_create(gui.page.checkup.checkupStepContainer);
                        lv_obj_remove_flag(gui.page.checkup.checkupTargetTempsContainer, LV_OBJ_FLAG_SCROLLABLE); 
                        lv_obj_align(gui.page.checkup.checkupTargetTempsContainer, LV_ALIGN_TOP_LEFT, -18, -18);
                        lv_obj_set_size(gui.page.checkup.checkupTargetTempsContainer, 240, 265); 
                        //lv_obj_set_style_border_color(gui.page.checkup.checkupTargetTempsContainer, lv_palette_main(LV_PALETTE_GREEN), 0);
                        lv_obj_set_style_border_opa(gui.page.checkup.checkupTargetTempsContainer, LV_OPA_TRANSP, 0);


                                gui.page.checkup.checkupTargetTempContainer = lv_obj_create(gui.page.checkup.checkupStepContainer);
                                lv_obj_remove_flag(gui.page.checkup.checkupTargetTempContainer, LV_OBJ_FLAG_SCROLLABLE); 
                                lv_obj_align(gui.page.checkup.checkupTargetTempContainer, LV_ALIGN_TOP_MID, 0, 0);
                                lv_obj_set_size(gui.page.checkup.checkupTargetTempContainer, 200, 80); 
                                //lv_obj_set_style_border_color(gui.page.checkup.checkupTargetTempContainer, lv_palette_main(LV_PALETTE_GREEN), 0);
                                lv_obj_set_style_border_opa(gui.page.checkup.checkupTargetTempContainer, LV_OPA_TRANSP, 0);

                                        gui.page.checkup.checkupTargetTempLabel = lv_label_create(gui.page.checkup.checkupTargetTempContainer);         
                                        lv_label_set_text(gui.page.checkup.checkupTargetTempLabel, checkupTargetTemp_text); 
                                        lv_obj_set_style_text_font(gui.page.checkup.checkupTargetTempLabel, &lv_font_montserrat_18, 0);              
                                        lv_obj_align(gui.page.checkup.checkupTargetTempLabel, LV_ALIGN_CENTER, 0, -15);

                                        gui.page.checkup.checkupTargetTempValue = lv_label_create(gui.page.checkup.checkupTargetTempContainer);         
                                        lv_label_set_text(gui.page.checkup.checkupTargetTempValue, "20.4°C"); 
                                        lv_obj_set_style_text_font(gui.page.checkup.checkupTargetTempValue, &lv_font_montserrat_20, 0);              
                                        lv_obj_align(gui.page.checkup.checkupTargetTempValue, LV_ALIGN_CENTER, 0, 20);


                                gui.page.checkup.checkupTargetWaterTempContainer = lv_obj_create(gui.page.checkup.checkupStepContainer);
                                lv_obj_remove_flag(gui.page.checkup.checkupTargetWaterTempContainer, LV_OBJ_FLAG_SCROLLABLE); 
                                lv_obj_align(gui.page.checkup.checkupTargetWaterTempContainer, LV_ALIGN_BOTTOM_LEFT, 0, -50);
                                lv_obj_set_size(gui.page.checkup.checkupTargetWaterTempContainer, 100, 80); 
                                //lv_obj_set_style_border_color(gui.page.checkup.checkupTargetWaterTempContainer, lv_palette_main(LV_PALETTE_GREEN), 0);
                                lv_obj_set_style_border_opa(gui.page.checkup.checkupTargetWaterTempContainer, LV_OPA_TRANSP, 0);

                                        gui.page.checkup.checkupTargetWaterTempLabel = lv_label_create(gui.page.checkup.checkupTargetWaterTempContainer);         
                                        lv_label_set_text(gui.page.checkup.checkupTargetWaterTempLabel, checkupWater_text); 
                                        lv_obj_set_style_text_font(gui.page.checkup.checkupTargetWaterTempLabel, &lv_font_montserrat_18, 0);              
                                        lv_obj_align(gui.page.checkup.checkupTargetWaterTempLabel, LV_ALIGN_CENTER, 0, -15);

                                        gui.page.checkup.checkupTargetWaterTempValue = lv_label_create(gui.page.checkup.checkupTargetWaterTempContainer);         
                                        lv_label_set_text(gui.page.checkup.checkupTargetWaterTempValue, "15.4°C"); 
                                        lv_obj_set_style_text_font(gui.page.checkup.checkupTargetWaterTempValue, &lv_font_montserrat_20, 0);              
                                        lv_obj_align(gui.page.checkup.checkupTargetWaterTempValue, LV_ALIGN_CENTER, 0, 20);



                                gui.page.checkup.checkupTargetChemistryTempContainer = lv_obj_create(gui.page.checkup.checkupStepContainer);
                                lv_obj_remove_flag(gui.page.checkup.checkupTargetChemistryTempContainer, LV_OBJ_FLAG_SCROLLABLE); 
                                lv_obj_align(gui.page.checkup.checkupTargetChemistryTempContainer, LV_ALIGN_BOTTOM_RIGHT, 0, -50);
                                lv_obj_set_size(gui.page.checkup.checkupTargetChemistryTempContainer, 100, 80); 
                                //lv_obj_set_style_border_color(gui.page.checkup.checkupTargetChemistryTempContainer, lv_palette_main(LV_PALETTE_GREEN), 0);
                                lv_obj_set_style_border_opa(gui.page.checkup.checkupTargetChemistryTempContainer, LV_OPA_TRANSP, 0);

                                        gui.page.checkup.checkupTargetChemistryTempLabel = lv_label_create(gui.page.checkup.checkupTargetChemistryTempContainer);         
                                        lv_label_set_text(gui.page.checkup.checkupTargetChemistryTempLabel, checkupChemistry_text); 
                                        lv_obj_set_style_text_font(gui.page.checkup.checkupTargetChemistryTempLabel, &lv_font_montserrat_18, 0);              
                                        lv_obj_align(gui.page.checkup.checkupTargetChemistryTempLabel, LV_ALIGN_CENTER, 0, -15);

                                        gui.page.checkup.checkupTargetChemistryTempValue = lv_label_create(gui.page.checkup.checkupTargetChemistryTempContainer);         
                                        lv_label_set_text(gui.page.checkup.checkupTargetChemistryTempValue, "49.8°C"); 
                                        lv_obj_set_style_text_font(gui.page.checkup.checkupTargetChemistryTempValue, &lv_font_montserrat_20, 0);              
                                        lv_obj_align(gui.page.checkup.checkupTargetChemistryTempValue, LV_ALIGN_CENTER, 0, 20);



                                gui.page.checkup.checkupSkipButton = lv_button_create(gui.page.checkup.checkupTargetTempsContainer);
                                lv_obj_set_size(gui.page.checkup.checkupSkipButton, BUTTON_PROCESS_WIDTH, BUTTON_PROCESS_HEIGHT);
                                lv_obj_align(gui.page.checkup.checkupSkipButton, LV_ALIGN_BOTTOM_MID, 0, 10);
                                lv_obj_add_event_cb(gui.page.checkup.checkupSkipButton, event_checkup, LV_EVENT_CLICKED, gui.page.checkup.checkupTargetWaterTempContainer);
                                lv_obj_set_style_bg_color(gui.page.checkup.checkupSkipButton, lv_color_hex(GREEN_DARK), LV_PART_MAIN);
                                lv_obj_move_foreground(gui.page.checkup.checkupSkipButton);

                                        gui.page.checkup.checkupSkipButtonLabel = lv_label_create(gui.page.checkup.checkupSkipButton);         
                                        lv_label_set_text(gui.page.checkup.checkupSkipButtonLabel, checkupSkip_text); 
                                        lv_obj_set_style_text_font(gui.page.checkup.checkupSkipButtonLabel, &lv_font_montserrat_22, 0);              
                                        lv_obj_align(gui.page.checkup.checkupSkipButtonLabel, LV_ALIGN_CENTER, 0, 0);
                  }

                  if(gui.page.checkup.processStep == 3){
                        gui.page.checkup.checkupFilmRotatingContainer = lv_obj_create(gui.page.checkup.checkupStepContainer);
                        lv_obj_remove_flag(gui.page.checkup.checkupFilmRotatingContainer, LV_OBJ_FLAG_SCROLLABLE); 
                        lv_obj_align(gui.page.checkup.checkupFilmRotatingContainer, LV_ALIGN_TOP_LEFT, -18, -18);
                        lv_obj_set_size(gui.page.checkup.checkupFilmRotatingContainer, 240, 265); 
                        //lv_obj_set_style_border_color(gui.page.checkup.checkupFilmRotatingContainer, lv_palette_main(LV_PALETTE_GREEN), 0);
                        lv_obj_set_style_border_opa(gui.page.checkup.checkupFilmRotatingContainer, LV_OPA_TRANSP, 0);
                        
                  
                                gui.page.checkup.checkupTankIsPresentContainer = lv_obj_create(gui.page.checkup.checkupFilmRotatingContainer);
                                lv_obj_remove_flag(gui.page.checkup.checkupTankIsPresentContainer, LV_OBJ_FLAG_SCROLLABLE); 
                                lv_obj_align(gui.page.checkup.checkupTankIsPresentContainer, LV_ALIGN_CENTER, 0, -55);
                                lv_obj_set_size(gui.page.checkup.checkupTankIsPresentContainer, 200, 80); 
                                //lv_obj_set_style_border_color(gui.page.checkup.checkupTankIsPresentContainer, lv_palette_main(LV_PALETTE_GREEN), 0);
                                lv_obj_set_style_border_opa(gui.page.checkup.checkupTankIsPresentContainer, LV_OPA_TRANSP, 0);

                                        gui.page.checkup.checkupTankIsPresentLabel = lv_label_create(gui.page.checkup.checkupTankIsPresentContainer);         
                                        lv_label_set_text(gui.page.checkup.checkupTankIsPresentLabel, checkupTankPosition_text); 
                                        lv_obj_set_style_text_font(gui.page.checkup.checkupTankIsPresentLabel, &lv_font_montserrat_18, 0);              
                                        lv_obj_align(gui.page.checkup.checkupTankIsPresentLabel, LV_ALIGN_CENTER, 0, -15);

                                        gui.page.checkup.checkupTankIsPresentValue = lv_label_create(gui.page.checkup.checkupTankIsPresentContainer);         
                                        lv_label_set_text(gui.page.checkup.checkupTankIsPresentValue, checkupYes_text); 
                                        lv_obj_set_style_text_font(gui.page.checkup.checkupTankIsPresentValue, &lv_font_montserrat_24, 0);              
                                        lv_obj_align(gui.page.checkup.checkupTankIsPresentValue, LV_ALIGN_CENTER, 0, 20);
                  

                                gui.page.checkup.checkupFilmInPositionContainer = lv_obj_create(gui.page.checkup.checkupFilmRotatingContainer);
                                lv_obj_remove_flag(gui.page.checkup.checkupFilmInPositionContainer, LV_OBJ_FLAG_SCROLLABLE); 
                                lv_obj_align(gui.page.checkup.checkupFilmInPositionContainer, LV_ALIGN_CENTER, 0, 40);
                                lv_obj_set_size(gui.page.checkup.checkupFilmInPositionContainer, 200, 80); 
                                //lv_obj_set_style_border_color(gui.page.checkup.checkupFilmInPositionContainer, lv_palette_main(LV_PALETTE_GREEN), 0);
                                lv_obj_set_style_border_opa(gui.page.checkup.checkupFilmInPositionContainer, LV_OPA_TRANSP, 0);

                                        gui.page.checkup.checkupFilmRotatingLabel = lv_label_create(gui.page.checkup.checkupFilmInPositionContainer);         
                                        lv_label_set_text(gui.page.checkup.checkupFilmRotatingLabel, checkupFilmRotation_text); 
                                        lv_obj_set_style_text_font(gui.page.checkup.checkupFilmRotatingLabel, &lv_font_montserrat_18, 0);              
                                        lv_obj_align(gui.page.checkup.checkupFilmRotatingLabel, LV_ALIGN_CENTER, 0, -15);

                                        gui.page.checkup.checkupFilmRotatingValue = lv_label_create(gui.page.checkup.checkupFilmInPositionContainer);         
                                        lv_label_set_text(gui.page.checkup.checkupFilmRotatingValue, checkupChecking_text); 
                                        lv_obj_set_style_text_font(gui.page.checkup.checkupFilmRotatingValue, &lv_font_montserrat_24, 0);              
                                        lv_obj_align(gui.page.checkup.checkupFilmRotatingValue, LV_ALIGN_CENTER, 0, 20);


                        gui.page.checkup.checkupStartButton = lv_button_create(gui.page.checkup.checkupFilmRotatingContainer);
                        lv_obj_set_size(gui.page.checkup.checkupStartButton, BUTTON_PROCESS_WIDTH, BUTTON_PROCESS_HEIGHT);
                        lv_obj_align(gui.page.checkup.checkupStartButton, LV_ALIGN_BOTTOM_MID, 0, 10);
                        lv_obj_add_event_cb(gui.page.checkup.checkupStartButton, event_checkup, LV_EVENT_CLICKED, gui.page.checkup.checkupFilmRotatingContainer);
                        lv_obj_set_style_bg_color(gui.page.checkup.checkupStartButton, lv_color_hex(GREEN_DARK), LV_PART_MAIN);
                        lv_obj_move_foreground(gui.page.checkup.checkupStartButton);

                                gui.page.checkup.checkupStartButtonLabel = lv_label_create(gui.page.checkup.checkupStartButton);         
                                lv_label_set_text(gui.page.checkup.checkupStartButtonLabel, checkupStart_text); 
                                lv_obj_set_style_text_font(gui.page.checkup.checkupStartButtonLabel, &lv_font_montserrat_22, 0);              
                                lv_obj_align(gui.page.checkup.checkupStartButtonLabel, LV_ALIGN_CENTER, 0, 0);
                  }

                  if(gui.page.checkup.processStep == 4){
                        gui.page.checkup.checkupProcessingContainer = lv_obj_create(gui.page.checkup.checkupStepContainer);
                        lv_obj_remove_flag(gui.page.checkup.checkupProcessingContainer, LV_OBJ_FLAG_SCROLLABLE); 
                        lv_obj_align(gui.page.checkup.checkupProcessingContainer, LV_ALIGN_TOP_LEFT, -18, -18);
                        lv_obj_set_size(gui.page.checkup.checkupProcessingContainer, 240, 265); 
                        //lv_obj_set_style_border_color(gui.page.checkup.checkupProcessingContainer, lv_palette_main(LV_PALETTE_GREEN), 0);
                        lv_obj_set_style_border_opa(gui.page.checkup.checkupProcessingContainer, LV_OPA_TRANSP, 0);
                        

                                gui.page.checkup.checkupStepTimeLeftValue = lv_label_create(gui.page.checkup.checkupProcessingContainer);         
                                lv_label_set_text(gui.page.checkup.checkupStepTimeLeftValue, "20sec"); 
                                lv_obj_set_style_text_font(gui.page.checkup.checkupStepTimeLeftValue, &lv_font_montserrat_20, 0);              
                                lv_obj_align(gui.page.checkup.checkupStepTimeLeftValue, LV_ALIGN_CENTER, 0, -60);

                                gui.page.checkup.checkupStepNameValue = lv_label_create(gui.page.checkup.checkupProcessingContainer);         
                                lv_label_set_text(gui.page.checkup.checkupStepNameValue, testLongString); 
                                lv_obj_set_style_text_font(gui.page.checkup.checkupStepNameValue, &lv_font_montserrat_22, 0);              
                                lv_obj_align(gui.page.checkup.checkupStepNameValue, LV_ALIGN_CENTER, 0, -40);
                                lv_obj_set_width(gui.page.checkup.checkupStepNameValue, 130);
                                lv_label_set_long_mode(gui.page.checkup.checkupStepNameValue, LV_LABEL_LONG_SCROLL_CIRCULAR);

                                gui.page.checkup.checkupStepKindValue = lv_label_create(gui.page.checkup.checkupProcessingContainer);         
                                lv_label_set_text(gui.page.checkup.checkupStepKindValue, &currentStep[1][0]); 
                                lv_obj_set_style_text_font(gui.page.checkup.checkupStepKindValue, &lv_font_montserrat_20, 0);              
                                lv_obj_align(gui.page.checkup.checkupStepKindValue, LV_ALIGN_CENTER, 0, 10);

                                gui.page.checkup.checkupProcessTimeLeftValue = lv_label_create(gui.page.checkup.checkupProcessingContainer);         
                                lv_label_set_text(gui.page.checkup.checkupProcessTimeLeftValue, "20min 45sec"); 
                                lv_obj_set_style_text_font(gui.page.checkup.checkupProcessTimeLeftValue, &lv_font_montserrat_24, 0);              
                                lv_obj_align(gui.page.checkup.checkupProcessTimeLeftValue, LV_ALIGN_CENTER, 0, 30);
                              
                                gui.page.checkup.processArc = lv_arc_create(gui.page.checkup.checkupProcessingContainer);
                                lv_obj_set_size(gui.page.checkup.processArc, 220, 220);
                                lv_arc_set_rotation(gui.page.checkup.processArc, 140);
                                lv_arc_set_bg_angles(gui.page.checkup.processArc, 0, 260);
                                lv_arc_set_value(gui.page.checkup.processArc, 10);
                                lv_obj_align(gui.page.checkup.processArc, LV_ALIGN_CENTER, 0, 10);
                                lv_obj_remove_style(gui.page.checkup.processArc, NULL, LV_PART_KNOB);
                                lv_obj_remove_flag(gui.page.checkup.processArc, LV_OBJ_FLAG_CLICKABLE);
                                lv_obj_set_style_arc_color(gui.page.checkup.processArc,lv_color_hex(GREEN) , LV_PART_INDICATOR);
                                lv_obj_set_style_arc_color(gui.page.checkup.processArc, lv_color_hex(GREEN_DARK), LV_PART_MAIN);

                                gui.page.checkup.stepArc = lv_arc_create(gui.page.checkup.checkupProcessingContainer);
                                lv_obj_set_size(gui.page.checkup.stepArc, 220, 220);
                                lv_arc_set_rotation(gui.page.checkup.stepArc, 230);
                                lv_arc_set_bg_angles(gui.page.checkup.stepArc, 0, 80);
                                lv_arc_set_value(gui.page.checkup.stepArc, 20);
                                lv_obj_align(gui.page.checkup.stepArc, LV_ALIGN_CENTER, 0, 160);
                                lv_obj_remove_style(gui.page.checkup.stepArc, NULL, LV_PART_KNOB);
                                lv_obj_remove_flag(gui.page.checkup.stepArc, LV_OBJ_FLAG_CLICKABLE);
                                lv_obj_set_style_arc_color(gui.page.checkup.stepArc,lv_color_hex(ORANGE_LIGHT) , LV_PART_INDICATOR);
                                lv_obj_set_style_arc_color(gui.page.checkup.stepArc, lv_color_hex(ORANGE_DARK), LV_PART_MAIN);
                                lv_obj_move_foreground(gui.page.checkup.stepArc);

                  }
}


