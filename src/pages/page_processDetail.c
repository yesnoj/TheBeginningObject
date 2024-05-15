/**
 * @file page_processDetail.c
 *
 */


//ESSENTIAL INCLUDE
#include <lvgl.h>
#include "../../include/definitions.h"

extern struct gui_components gui;

//ACCESSORY INCLUDES


/******************************
  *    PROCESS EVENT
******************************/

void event_processDetail(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t * obj = (lv_obj_t *)lv_event_get_target(e);
  lv_obj_t * objCont = (lv_obj_t *)lv_obj_get_parent(obj);
  lv_obj_t * mboxCont = (lv_obj_t *)lv_obj_get_parent(objCont);
  lv_obj_t * mboxParent = (lv_obj_t *)lv_obj_get_parent(mboxCont);
  lv_obj_t * data = (lv_obj_t *)lv_event_get_user_data(e);


  if(code == LV_EVENT_CLICKED) {
    if(data == gui.page.processDetail.processDetailCloseButton){
        gui.element.step.stepCounter = 0;
        lv_msgbox_close(mboxCont);
        lv_obj_delete(mboxCont);
        LV_LOG_USER("Close Process Detail");
    }
    if(data == gui.page.processDetail.processColorLabel){
        lv_obj_set_style_text_color(gui.page.processDetail.processColorLabel, lv_color_hex(GREEN_DARK), LV_PART_MAIN);
        lv_obj_set_style_text_color(gui.page.processDetail.processBnWLabel, lv_color_hex(WHITE), LV_PART_MAIN);
        gui.page.processDetail.isColor = 1;
        gui.page.processDetail.somethingChanged = 1;

        if(gui.page.processDetail.stepsCreated > 0){
          lv_obj_clear_state(gui.page.processDetail.processSaveLabel, LV_STATE_DISABLED);
          lv_obj_set_style_text_color(gui.page.processDetail.processSaveLabel, lv_color_hex(WHITE), LV_PART_MAIN);
        }

        LV_LOG_USER("Pressed gui.page.processDetail.processColorLabel");
    }
    if(data == gui.page.processDetail.processBnWLabel){
        lv_obj_set_style_text_color(gui.page.processDetail.processBnWLabel, lv_color_hex(GREEN_DARK), LV_PART_MAIN);
        lv_obj_set_style_text_color(gui.page.processDetail.processColorLabel, lv_color_hex(WHITE), LV_PART_MAIN);
        gui.page.processDetail.isColor = 0;
        gui.page.processDetail.somethingChanged = 1;
        if(gui.page.processDetail.stepsCreated > 0){
          lv_obj_clear_state(gui.page.processDetail.processSaveLabel, LV_STATE_DISABLED);
          lv_obj_set_style_text_color(gui.page.processDetail.processSaveLabel, lv_color_hex(WHITE), LV_PART_MAIN);
        }
        LV_LOG_USER("Pressed gui.page.processDetail.processBnWLabel");
    }

    if(data == gui.page.processDetail.processPreferredLabel){
        if(isPreferred == 0){
          lv_obj_set_style_text_color(gui.page.processDetail.processPreferredLabel, lv_color_hex(RED), LV_PART_MAIN);
          isPreferred = 1;
          gui.page.processDetail.somethingChanged = 1;

          if(gui.page.processDetail.stepsCreated > 0){
            lv_obj_clear_state(gui.page.processDetail.processSaveLabel, LV_STATE_DISABLED);
            lv_obj_set_style_text_color(gui.page.processDetail.processSaveLabel, lv_color_hex(WHITE), LV_PART_MAIN);
          }
        }
        else{
          lv_obj_set_style_text_color(gui.page.processDetail.processPreferredLabel, lv_color_hex(WHITE), LV_PART_MAIN);
          isPreferred = 0;
        }
        LV_LOG_USER("Process is preferred :%d",isPreferred);
    }
    if(data == gui.page.processDetail.processSaveLabel && gui.page.processDetail.stepsCreated > 0){
        if(gui.page.processDetail.isSaved == 0){
          gui.page.processDetail.isSaved = 1;
          gui.page.processDetail.somethingChanged = 0;
          lv_obj_clear_state(gui.page.processDetail.processRunButton, LV_STATE_DISABLED);

          lv_obj_set_style_text_color(gui.page.processDetail.processSaveLabel, lv_color_hex(GREY), LV_PART_MAIN);
          lv_obj_add_state(gui.page.processDetail.processSaveLabel, LV_STATE_DISABLED);
          processElementCreate(gui.page.processDetail.processesContainer);
        }
        else{
          gui.page.processDetail.isSaved = 0;
        }
        LV_LOG_USER("Pressed gui.page.processDetail.processSaveLabel");
    }
    if(data == gui.page.processDetail.processDeleteButton){
        gui.element.step.stepCounter = 0;
        messagePopupCreate(deletePopupTitle_text,deletePopupBody_text, deleteButton_text, stepDetailCancel_text, gui.page.processDetail.processDetailParent);
        LV_LOG_USER("Pressed gui.page.processDetail.processDeleteButton");
    }
    if(data == gui.page.processDetail.processRunButton){
        gui.element.step.stepCounter = 0;
        lv_msgbox_close(mboxCont);
        lv_obj_delete(mboxCont);
        LV_LOG_USER("Pressed gui.page.processDetail.processRunButton");
        checkup();
    }
    if(data == gui.page.processDetail.processNewStepButton){
        LV_LOG_USER("Pressed gui.page.processDetail.processNewStepButton");
        gui.page.processDetail.stepsCreated += 1;
        gui.page.processDetail.somethingChanged = 1;
        lv_obj_clear_state(gui.page.processDetail.processSaveLabel, LV_STATE_DISABLED);

        if(gui.page.processDetail.stepsCreated > 0){
          lv_obj_clear_state(gui.page.processDetail.processSaveLabel, LV_STATE_DISABLED);
          lv_obj_set_style_text_color(gui.page.processDetail.processSaveLabel, lv_color_hex(WHITE), LV_PART_MAIN);
        }

        stepDetail(gui.page.processDetail.processStepsContainer);
    }
  }

  if(code == LV_EVENT_LONG_PRESSED_REPEAT){
    
  }

  if(code == LV_EVENT_VALUE_CHANGED) {
      if(data == gui.page.processDetail.processTempControlSwitch){
          LV_LOG_USER("Temperature controlled : %s", lv_obj_has_state(obj, LV_STATE_CHECKED) ? "On" : "Off");
          gui.page.processDetail.somethingChanged = 1;

          if(gui.page.processDetail.stepsCreated > 0){
            lv_obj_clear_state(gui.page.processDetail.processSaveLabel, LV_STATE_DISABLED);
            lv_obj_set_style_text_color(gui.page.processDetail.processSaveLabel, lv_color_hex(WHITE), LV_PART_MAIN);
          }
      }
  }

  if(code == LV_EVENT_FOCUSED) {
      if(data == gui.page.processDetail.processTempTextArea){
          LV_LOG_USER("Set Temperature");
          rollerPopupCreate(gui.element.rollerPopup.tempCelsiusOptions,tuneTempPopupTitle_text,gui.page.processDetail.processTempTextArea);
      }
      if(data == gui.page.processDetail.processToleranceTextArea){
          LV_LOG_USER("Set Tolerance");
          rollerPopupCreate(gui.element.rollerPopup.tempCelsiusToleranceOptions,tuneTempPopupTitle_text,gui.page.processDetail.processToleranceTextArea);
      }
  }
}
 
void event_processDetail_style_delete(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_DELETE) {
        //list of all styles to be reset, so clean the memory.
        lv_style_reset(&gui.page.processDetail.textAreaStyle);
    }
}



/*********************
  *    PROCESS DETAIL
*********************/

void processDetail(lv_obj_t * referenceProcess)
{   
/*********************
  *    PAGE HEADER
*********************/
  LV_LOG_USER("Process detail creation");
  gui.page.processDetail.isColor = 0;
  gui.page.processDetail.isSaved = 0;
  gui.page.processDetail.somethingChanged = 0;
  gui.page.processDetail.stepsCreated = 0;

  gui.page.processDetail.processesContainer = referenceProcess;

  gui.page.processDetail.processDetailParent = lv_obj_class_create_obj(&lv_msgbox_backdrop_class, lv_layer_top());
  lv_obj_class_init_obj(gui.page.processDetail.processDetailParent);
  lv_obj_remove_flag(gui.page.processDetail.processDetailParent, LV_OBJ_FLAG_IGNORE_LAYOUT);
  lv_obj_set_size(gui.page.processDetail.processDetailParent, LV_PCT(100), LV_PCT(100));
  
  lv_style_init(&gui.page.processDetail.textAreaStyle);
  
  /*********************
  *    PAGE ELEMENTS
  *********************/
   
      gui.page.processDetail.processDetailContainer = lv_obj_create(gui.page.processDetail.processDetailParent);
      lv_obj_align(gui.page.processDetail.processDetailContainer, LV_ALIGN_CENTER, 0, 0);
      lv_obj_set_size(gui.page.processDetail.processDetailContainer, LV_PCT(100), LV_PCT(100));
      lv_obj_remove_flag(gui.page.processDetail.processDetailContainer, LV_OBJ_FLAG_SCROLLABLE); 

            gui.page.processDetail.processDetailCloseButton = lv_button_create(gui.page.processDetail.processDetailContainer);
            lv_obj_set_size(gui.page.processDetail.processDetailCloseButton, BUTTON_POPUP_CLOSE_WIDTH * 1.2, BUTTON_POPUP_CLOSE_HEIGHT * 1.2);
            lv_obj_align(gui.page.processDetail.processDetailCloseButton, LV_ALIGN_TOP_RIGHT, 7 , -10);
            lv_obj_add_event_cb(gui.page.processDetail.processDetailCloseButton, event_processDetail, LV_EVENT_CLICKED, gui.page.processDetail.processDetailCloseButton);
            lv_obj_set_style_bg_color(gui.page.processDetail.processDetailCloseButton, lv_color_hex(GREEN_DARK), LV_PART_MAIN);
            lv_obj_move_foreground(gui.page.processDetail.processDetailCloseButton);


                  gui.page.processDetail.processDetailCloseButtonLabel = lv_label_create(gui.page.processDetail.processDetailCloseButton);         
                  lv_label_set_text(gui.page.processDetail.processDetailCloseButtonLabel, closePopup_icon); 
                  lv_obj_set_style_text_font(gui.page.processDetail.processDetailCloseButtonLabel, &FilMachineFontIcons_30, 0);              
                  lv_obj_align(gui.page.processDetail.processDetailCloseButtonLabel, LV_ALIGN_CENTER, 0, 0);
                  
        

            gui.page.processDetail.processDetailNameContainer = lv_obj_create(gui.page.processDetail.processDetailContainer);
            lv_obj_remove_flag(gui.page.processDetail.processDetailNameContainer, LV_OBJ_FLAG_SCROLLABLE); 
            lv_obj_align(gui.page.processDetail.processDetailNameContainer, LV_ALIGN_TOP_LEFT, -10, -15);
            lv_obj_set_size(gui.page.processDetail.processDetailNameContainer, 350, 50); 
            //lv_obj_set_style_border_color(gui.page.processDetail.processDetailNameContainer, lv_color_hex(GREEN_DARK), 0);
            lv_obj_set_style_border_opa(gui.page.processDetail.processDetailNameContainer, LV_OPA_TRANSP, 0);

                  gui.page.processDetail.processDetailName = lv_label_create(gui.page.processDetail.processDetailNameContainer);         
                  lv_label_set_text(gui.page.processDetail.processDetailName, "E6 six baths"); 
                  lv_obj_set_width(gui.page.processDetail.processDetailName, 300);
                  lv_obj_set_style_text_font(gui.page.processDetail.processDetailName, &lv_font_montserrat_30, 0);              
                  lv_obj_align(gui.page.processDetail.processDetailName, LV_ALIGN_TOP_LEFT, -10, -8);
                  lv_label_set_long_mode(gui.page.processDetail.processDetailName, LV_LABEL_LONG_SCROLL_CIRCULAR);

            gui.page.processDetail.processDetailStepsLabel = lv_label_create(gui.page.processDetail.processDetailContainer);         
            lv_label_set_text(gui.page.processDetail.processDetailStepsLabel, processDetailStep_text); 
            lv_obj_set_width(gui.page.processDetail.processDetailStepsLabel, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(gui.page.processDetail.processDetailStepsLabel, &lv_font_montserrat_24, 0);              
            lv_obj_align(gui.page.processDetail.processDetailStepsLabel, LV_ALIGN_TOP_LEFT, 0, 25);

            gui.page.processDetail.processStepsContainer = lv_obj_create(gui.page.processDetail.processDetailContainer);
            lv_obj_align(gui.page.processDetail.processStepsContainer, LV_ALIGN_TOP_LEFT, -10, 55);
            lv_obj_set_size(gui.page.processDetail.processStepsContainer, 250, 240);
            lv_obj_set_scroll_dir(gui.page.processDetail.processStepsContainer, LV_DIR_VER);
            lv_obj_set_style_border_color(gui.page.processDetail.processStepsContainer, lv_palette_main(LV_PALETTE_GREEN), 0);
            //lv_obj_set_style_border_opa(gui.page.processDetail.processStepsContainer, LV_OPA_TRANSP, 0);


            gui.page.processDetail.processNewStepButton = lv_button_create(gui.page.processDetail.processDetailContainer);
            lv_obj_set_size(gui.page.processDetail.processNewStepButton, BUTTON_POPUP_CLOSE_WIDTH, BUTTON_POPUP_CLOSE_HEIGHT);
            lv_obj_align(gui.page.processDetail.processNewStepButton, LV_ALIGN_TOP_LEFT, 210 , 55);
            lv_obj_add_event_cb(gui.page.processDetail.processNewStepButton, event_processDetail, LV_EVENT_CLICKED, gui.page.processDetail.processNewStepButton);
            lv_obj_set_style_bg_color(gui.page.processDetail.processNewStepButton, lv_palette_main(LV_PALETTE_GREEN), LV_PART_MAIN);
            //lv_obj_move_foreground(gui.page.processDetail.processNewStepButton);
  
                    gui.page.processDetail.processNewStepLabel = lv_label_create(gui.page.processDetail.processNewStepButton);         
                    lv_label_set_text(gui.page.processDetail.processNewStepLabel, LV_SYMBOL_PLUS); 
                    lv_obj_set_style_text_font(gui.page.processDetail.processNewStepLabel, &lv_font_montserrat_20, 0);              
                    lv_obj_align(gui.page.processDetail.processNewStepLabel, LV_ALIGN_CENTER, 0, 0);


            gui.page.processDetail.processDetailInfoLabel = lv_label_create(gui.page.processDetail.processDetailContainer);         
            lv_label_set_text(gui.page.processDetail.processDetailInfoLabel, processDetailInfo_text); 
            lv_obj_set_width(gui.page.processDetail.processDetailInfoLabel, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(gui.page.processDetail.processDetailInfoLabel, &lv_font_montserrat_24, 0);              
            lv_obj_align(gui.page.processDetail.processDetailInfoLabel, LV_ALIGN_TOP_RIGHT, -105, 25);

            gui.page.processDetail.processInfoContainer = lv_obj_create(gui.page.processDetail.processDetailContainer);
            lv_obj_align(gui.page.processDetail.processInfoContainer, LV_ALIGN_TOP_LEFT, 245, 55);
            lv_obj_set_size(gui.page.processDetail.processInfoContainer, 210, 195);
            lv_obj_remove_flag(gui.page.processDetail.processInfoContainer, LV_OBJ_FLAG_SCROLLABLE); 
            //lv_obj_set_scroll_dir(gui.page.processDetail.processInfoContainer, LV_DIR_VER);
            lv_obj_set_style_border_color(gui.page.processDetail.processInfoContainer, lv_color_hex(WHITE), 0);
            //lv_obj_set_style_border_opa(gui.page.processDetail.processInfoContainer, LV_OPA_TRANSP, 0);


                  gui.page.processDetail.processTempControlContainer = lv_obj_create(gui.page.processDetail.processInfoContainer);
                  lv_obj_remove_flag(gui.page.processDetail.processTempControlContainer, LV_OBJ_FLAG_SCROLLABLE); 
                  lv_obj_align(gui.page.processDetail.processTempControlContainer, LV_ALIGN_TOP_LEFT, -15, -17);
                  lv_obj_set_size(gui.page.processDetail.processTempControlContainer, 205, 40); 
                  //lv_obj_set_style_border_color(gui.page.processDetail.processTempControlContainer, lv_color_hex(GREEN_DARK), 0);
                  lv_obj_set_style_border_opa(gui.page.processDetail.processTempControlContainer, LV_OPA_TRANSP, 0);

                          gui.page.processDetail.processTempControlLabel = lv_label_create(gui.page.processDetail.processTempControlContainer);         
                          lv_label_set_text(gui.page.processDetail.processTempControlLabel, processDetailIsTempControl_text); 
                          lv_obj_set_style_text_font(gui.page.processDetail.processTempControlLabel, &lv_font_montserrat_16, 0);              
                          lv_obj_align(gui.page.processDetail.processTempControlLabel, LV_ALIGN_LEFT_MID, -15, 0);

                          gui.page.processDetail.processTempControlSwitch = lv_switch_create(gui.page.processDetail.processTempControlContainer);
                          lv_obj_add_event_cb(gui.page.processDetail.processTempControlSwitch, event_processDetail, LV_EVENT_VALUE_CHANGED, gui.page.processDetail.processTempControlSwitch);
                          lv_obj_align(gui.page.processDetail.processTempControlSwitch, LV_ALIGN_RIGHT_MID, 15, 0);
                          lv_obj_set_style_bg_color(gui.page.processDetail.processTempControlSwitch, lv_palette_darken(LV_PALETTE_GREY, 3), LV_STATE_DEFAULT);
                          lv_obj_set_style_bg_color(gui.page.processDetail.processTempControlSwitch,  lv_palette_main(LV_PALETTE_GREEN), LV_PART_KNOB | LV_STATE_DEFAULT);
                          lv_obj_set_style_bg_color(gui.page.processDetail.processTempControlSwitch, lv_color_hex(GREEN_DARK) , LV_PART_INDICATOR | LV_STATE_CHECKED);


                 

                  gui.page.processDetail.processTempContainer = lv_obj_create(gui.page.processDetail.processInfoContainer);
                  lv_obj_remove_flag(gui.page.processDetail.processTempContainer, LV_OBJ_FLAG_SCROLLABLE); 
                  lv_obj_align(gui.page.processDetail.processTempContainer, LV_ALIGN_TOP_LEFT, -15, 20);
                  lv_obj_set_size(gui.page.processDetail.processTempContainer, 205, 45); 
                  //lv_obj_set_style_border_color(gui.page.processDetail.processTempContainer, lv_color_hex(GREEN_DARK), 0);
                  lv_obj_set_style_border_opa(gui.page.processDetail.processTempContainer, LV_OPA_TRANSP, 0);   

                          gui.page.processDetail.processTempLabel = lv_label_create(gui.page.processDetail.processTempContainer);         
                          lv_label_set_text(gui.page.processDetail.processTempLabel, processDetailTemp_text); 
                          lv_obj_set_style_text_font(gui.page.processDetail.processTempLabel, &lv_font_montserrat_16, 0);              
                          lv_obj_align(gui.page.processDetail.processTempLabel, LV_ALIGN_LEFT_MID, -15, 0);

                          gui.page.processDetail.processTempTextArea = lv_textarea_create(gui.page.processDetail.processTempContainer);
                          lv_textarea_set_one_line(gui.page.processDetail.processTempTextArea, true);
                          lv_textarea_set_placeholder_text(gui.page.processDetail.processTempTextArea, processDetailTempPlaceHolder_text);
                          lv_obj_align(gui.page.processDetail.processTempTextArea, LV_ALIGN_LEFT_MID, 100, 0);
                          lv_obj_set_width(gui.page.processDetail.processTempTextArea, 60);
                          lv_obj_add_event_cb(gui.page.processDetail.processTempTextArea, event_processDetail, LV_EVENT_ALL, gui.page.processDetail.processTempTextArea);
                          lv_obj_add_state(gui.page.processDetail.processTempTextArea, LV_STATE_FOCUSED); /*To be sure the cursor is visible*/
                          lv_obj_set_style_bg_color(gui.page.processDetail.processTempTextArea, lv_palette_darken(LV_PALETTE_GREY, 3), 0);
                          lv_obj_set_style_text_align(gui.page.processDetail.processTempTextArea , LV_TEXT_ALIGN_CENTER, 0);
                          lv_style_set_text_font(&gui.page.processDetail.textAreaStyle, &lv_font_montserrat_18);
                          lv_obj_add_style(gui.page.processDetail.processTempTextArea, &gui.page.processDetail.textAreaStyle, LV_PART_MAIN);
                          lv_obj_set_style_border_color(gui.page.processDetail.processTempTextArea, lv_color_hex(WHITE), 0);


                          gui.page.processDetail.processTempUnitLabel = lv_label_create(gui.page.processDetail.processTempContainer);         
                          lv_label_set_text(gui.page.processDetail.processTempUnitLabel, celsius_text); 
                          lv_obj_set_style_text_font(gui.page.processDetail.processTempUnitLabel, &lv_font_montserrat_20, 0);              
                          lv_obj_align(gui.page.processDetail.processTempUnitLabel, LV_ALIGN_LEFT_MID, 160, 0);


                  gui.page.processDetail.processToleranceContainer = lv_obj_create(gui.page.processDetail.processInfoContainer);
                  lv_obj_remove_flag(gui.page.processDetail.processToleranceContainer, LV_OBJ_FLAG_SCROLLABLE); 
                  lv_obj_align(gui.page.processDetail.processToleranceContainer, LV_ALIGN_TOP_LEFT, -15, 65);
                  lv_obj_set_size(gui.page.processDetail.processToleranceContainer, 205, 45); 
                  //lv_obj_set_style_border_color(gui.page.processDetail.processToleranceContainer, lv_color_hex(GREEN_DARK), 0);
                  lv_obj_set_style_border_opa(gui.page.processDetail.processToleranceContainer, LV_OPA_TRANSP, 0);   

                          gui.page.processDetail.processToleranceLabel = lv_label_create(gui.page.processDetail.processToleranceContainer);         
                          lv_label_set_text(gui.page.processDetail.processToleranceLabel, processDetailTempTolerance_text); 
                          lv_obj_set_style_text_font(gui.page.processDetail.processToleranceLabel, &lv_font_montserrat_16, 0);              
                          lv_obj_align(gui.page.processDetail.processToleranceLabel, LV_ALIGN_LEFT_MID, -15, 0);

                          gui.page.processDetail.processToleranceTextArea = lv_textarea_create(gui.page.processDetail.processToleranceContainer);
                          lv_textarea_set_one_line(gui.page.processDetail.processToleranceTextArea, true);
                          lv_textarea_set_placeholder_text(gui.page.processDetail.processToleranceTextArea, processDetailTempPlaceHolder_text);
                          lv_obj_align(gui.page.processDetail.processToleranceTextArea, LV_ALIGN_LEFT_MID, 100, 0);
                          lv_obj_set_width(gui.page.processDetail.processToleranceTextArea, 60);

                          lv_obj_add_event_cb(gui.page.processDetail.processToleranceTextArea, event_processDetail, LV_EVENT_ALL, gui.page.processDetail.processToleranceTextArea);
                          lv_obj_add_state(gui.page.processDetail.processToleranceTextArea, LV_STATE_FOCUSED); /*To be sure the cursor is visible*/
                          lv_obj_set_style_bg_color(gui.page.processDetail.processToleranceTextArea, lv_palette_darken(LV_PALETTE_GREY, 3), 0);
                          lv_obj_set_style_text_align(gui.page.processDetail.processToleranceTextArea , LV_TEXT_ALIGN_CENTER, 0);
                          //lv_style_set_text_font(&gui.page.processDetail.textAreaStyle, &lv_font_montserrat_18);
                          lv_obj_add_style(gui.page.processDetail.processToleranceTextArea, &gui.page.processDetail.textAreaStyle, LV_PART_MAIN);
                          lv_obj_set_style_border_color(gui.page.processDetail.processToleranceTextArea, lv_color_hex(WHITE), 0);


                          gui.page.processDetail.processTempUnitLabel = lv_label_create(gui.page.processDetail.processToleranceContainer);         
                          lv_label_set_text(gui.page.processDetail.processTempUnitLabel, celsius_text); 
                          lv_obj_set_style_text_font(gui.page.processDetail.processTempUnitLabel, &lv_font_montserrat_20, 0);              
                          lv_obj_align(gui.page.processDetail.processTempUnitLabel, LV_ALIGN_LEFT_MID, 160, 0);
                          

                  gui.page.processDetail.processTotalTimeContainer = lv_obj_create(gui.page.processDetail.processInfoContainer);
                  lv_obj_remove_flag(gui.page.processDetail.processTotalTimeContainer, LV_OBJ_FLAG_SCROLLABLE); 
                  lv_obj_align(gui.page.processDetail.processTotalTimeContainer, LV_ALIGN_TOP_LEFT, -15, 110);
                  lv_obj_set_size(gui.page.processDetail.processTotalTimeContainer, 205, 30); 
                  //lv_obj_set_style_border_color(gui.page.processDetail.processTotalTimeContainer, lv_color_hex(GREEN_DARK), 0);
                  lv_obj_set_style_border_opa(gui.page.processDetail.processTotalTimeContainer, LV_OPA_TRANSP, 0);   

                          gui.page.processDetail.processTotalTimeLabel = lv_label_create(gui.page.processDetail.processTotalTimeContainer);         
                          lv_label_set_text(gui.page.processDetail.processTotalTimeLabel, processDetailTotalTime_text); 
                          lv_obj_set_style_text_font(gui.page.processDetail.processTotalTimeLabel, &lv_font_montserrat_16, 0);              
                          lv_obj_align(gui.page.processDetail.processTotalTimeLabel, LV_ALIGN_LEFT_MID, -15, 0);

                          gui.page.processDetail.processTotalTimeValue = lv_label_create(gui.page.processDetail.processTotalTimeContainer);         
                          lv_label_set_text(gui.page.processDetail.processTotalTimeValue, "32m20s"); 
                          lv_obj_set_style_text_font(gui.page.processDetail.processTotalTimeValue, &lv_font_montserrat_20, 0);              
                          lv_obj_align(gui.page.processDetail.processTotalTimeValue, LV_ALIGN_LEFT_MID, 100, 0);



                  gui.page.processDetail.processColorOrBnWContainer = lv_obj_create(gui.page.processDetail.processInfoContainer);
                  lv_obj_remove_flag(gui.page.processDetail.processColorOrBnWContainer, LV_OBJ_FLAG_SCROLLABLE); 
                  lv_obj_align(gui.page.processDetail.processColorOrBnWContainer, LV_ALIGN_TOP_LEFT, -18, 137);
                  lv_obj_set_size(gui.page.processDetail.processColorOrBnWContainer, 105, 40); 
                  lv_obj_set_style_border_color(gui.page.processDetail.processColorOrBnWContainer, lv_color_hex(WHITE), 0);
                  //lv_obj_set_style_border_opa(gui.page.processDetail.processColorOrBnWContainer, LV_OPA_TRANSP, 0);   


                          gui.page.processDetail.processColorLabel = lv_label_create(gui.page.processDetail.processColorOrBnWContainer);         
                          lv_label_set_text(gui.page.processDetail.processColorLabel, colorpalette_icon); 
                          lv_obj_set_style_text_font(gui.page.processDetail.processColorLabel, &FilMachineFontIcons_30, 0);              
                          lv_obj_align(gui.page.processDetail.processColorLabel, LV_ALIGN_LEFT_MID, -5, 0);
                          lv_obj_add_flag(gui.page.processDetail.processColorLabel, LV_OBJ_FLAG_CLICKABLE);
                          lv_obj_add_event_cb(gui.page.processDetail.processColorLabel, event_processDetail, LV_EVENT_CLICKED, gui.page.processDetail.processColorLabel);


                          gui.page.processDetail.processBnWLabel = lv_label_create(gui.page.processDetail.processColorOrBnWContainer);         
                          lv_label_set_text(gui.page.processDetail.processBnWLabel, blackwhite_icon); 
                          lv_obj_set_style_text_font(gui.page.processDetail.processBnWLabel, &FilMachineFontIcons_30, 0);              
                          lv_obj_align(gui.page.processDetail.processBnWLabel, LV_ALIGN_LEFT_MID, 45, 0);
                          lv_obj_add_flag(gui.page.processDetail.processBnWLabel, LV_OBJ_FLAG_CLICKABLE);
                          lv_obj_add_event_cb(gui.page.processDetail.processBnWLabel, event_processDetail, LV_EVENT_CLICKED, gui.page.processDetail.processBnWLabel);


                  gui.page.processDetail.processPreferredLabel = lv_label_create(gui.page.processDetail.processInfoContainer);         
                  lv_label_set_text(gui.page.processDetail.processPreferredLabel, preferred_icon); 
                  lv_obj_set_style_text_font(gui.page.processDetail.processPreferredLabel, &FilMachineFontIcons_30, 0);              
                  lv_obj_align(gui.page.processDetail.processPreferredLabel, LV_ALIGN_TOP_LEFT, 100, 140);
                  lv_obj_add_flag(gui.page.processDetail.processPreferredLabel, LV_OBJ_FLAG_CLICKABLE);
                  lv_obj_add_event_cb(gui.page.processDetail.processPreferredLabel, event_processDetail, LV_EVENT_CLICKED, gui.page.processDetail.processPreferredLabel);


                  gui.page.processDetail.processSaveLabel = lv_label_create(gui.page.processDetail.processInfoContainer);         
                  lv_label_set_text(gui.page.processDetail.processSaveLabel, save_Icon); 
                  lv_obj_set_style_text_font(gui.page.processDetail.processSaveLabel, &FilMachineFontIcons_30, 0);              
                  lv_obj_align(gui.page.processDetail.processSaveLabel, LV_ALIGN_TOP_LEFT, 150, 140);
                  lv_obj_add_flag(gui.page.processDetail.processSaveLabel, LV_OBJ_FLAG_CLICKABLE);
                  lv_obj_add_state(gui.page.processDetail.processSaveLabel, LV_STATE_DISABLED);
                  lv_obj_add_event_cb(gui.page.processDetail.processSaveLabel, event_processDetail, LV_EVENT_CLICKED, gui.page.processDetail.processSaveLabel);
                  lv_obj_set_style_text_color(gui.page.processDetail.processSaveLabel, lv_color_hex(GREY), LV_PART_MAIN);

                  gui.page.processDetail.processDeleteButton = lv_button_create(gui.page.processDetail.processDetailContainer);
                  lv_obj_set_size(gui.page.processDetail.processDeleteButton, BUTTON_PROCESS_WIDTH, BUTTON_PROCESS_HEIGHT);
                  lv_obj_align(gui.page.processDetail.processDeleteButton, LV_ALIGN_BOTTOM_RIGHT, -103, 10);
                  lv_obj_add_event_cb(gui.page.processDetail.processDeleteButton, event_processDetail, LV_EVENT_CLICKED, gui.page.processDetail.processDeleteButton);
                  lv_obj_set_style_bg_color(gui.page.processDetail.processDeleteButton, lv_color_hex(RED_DARK), LV_PART_MAIN);
                  lv_obj_add_state(gui.page.processDetail.processDeleteButton, LV_STATE_DISABLED);


                          gui.page.processDetail.processDeleteLabel = lv_label_create(gui.page.processDetail.processDeleteButton);         
                          lv_label_set_text(gui.page.processDetail.processDeleteLabel, trash_Icon); 
                          lv_obj_set_style_text_font(gui.page.processDetail.processDeleteLabel, &FilMachineFontIcons_20, 0);              
                          lv_obj_align(gui.page.processDetail.processDeleteLabel, LV_ALIGN_CENTER, 0, 0);
                          lv_obj_add_flag(gui.page.processDetail.processDeleteLabel, LV_OBJ_FLAG_CLICKABLE);

                  
                  gui.page.processDetail.processRunButton = lv_button_create(gui.page.processDetail.processDetailContainer);
                  lv_obj_set_size(gui.page.processDetail.processRunButton, BUTTON_PROCESS_WIDTH, BUTTON_PROCESS_HEIGHT);
                  lv_obj_align(gui.page.processDetail.processRunButton, LV_ALIGN_BOTTOM_RIGHT, 10, 10);
                  lv_obj_add_event_cb(gui.page.processDetail.processRunButton, event_processDetail, LV_EVENT_CLICKED, gui.page.processDetail.processRunButton);
                  lv_obj_set_style_bg_color(gui.page.processDetail.processRunButton, lv_color_hex(GREEN_DARK), LV_PART_MAIN);
                  if(gui.page.processDetail.stepsCreated == 0)
                      lv_obj_add_state(gui.page.processDetail.processRunButton, LV_STATE_DISABLED);
                  else
                      lv_obj_clear_state(gui.page.processDetail.processRunButton, LV_STATE_DISABLED);

                          gui.page.processDetail.processRunLabel = lv_label_create(gui.page.processDetail.processRunButton);         
                          lv_label_set_text(gui.page.processDetail.processRunLabel, play_Icon); 
                          lv_obj_set_style_text_font(gui.page.processDetail.processRunLabel, &FilMachineFontIcons_20, 0);              
                          lv_obj_align(gui.page.processDetail.processRunLabel, LV_ALIGN_CENTER, 0, 0);
                          lv_obj_add_flag(gui.page.processDetail.processRunLabel, LV_OBJ_FLAG_CLICKABLE);
}

