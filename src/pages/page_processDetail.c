/**
 * @file page_processDetail.c
 *
 */


//ESSENTIAL INCLUDE
#include <lvgl.h>
#include "../../include/definitions.h"

extern struct gui_components gui;

//ACCESSORY INCLUDES


/* PROCESS DETAIL VARIABLES*/
static lv_obj_t * processDetailContainer;
static lv_obj_t * processDetailNameContainer;
static lv_obj_t * processStepsContainer;
static lv_obj_t * processInfoContainer;
static lv_obj_t * processTempControlContainer;
static lv_obj_t * processTempContainer;
static lv_obj_t * processToleranceContainer;
static lv_obj_t * processColorOrBnWContainer;
static lv_obj_t * processTotalTimeContainer;

static lv_obj_t * processDetailName;
static lv_obj_t * processDetailStepsLabel;
static lv_obj_t * processDetailInfoLabel;
static lv_obj_t * processDetailCloseButtonLabel;
static lv_obj_t * processTempControlLabel;
static lv_obj_t * processTempLabel;
static lv_obj_t * processTempControlSwitch;
static lv_obj_t * processTempUnitLabel;
static lv_obj_t * processToleranceLabel;
static lv_obj_t * processColorLabel;
static lv_obj_t * processBnWLabel;
static lv_obj_t * processPreferredLabel;
static lv_obj_t * processDeleteLabel;
static lv_obj_t * processRunLabel;
static lv_obj_t * processSaveLabel;
static lv_obj_t * processNewStepLabel;
static lv_obj_t * processTotalTimeLabel;
static lv_obj_t * processTotalTimeValue;

static lv_obj_t * processDetailCloseButton;
static lv_obj_t * processRunButton;
static lv_obj_t * processDeleteButton;
static lv_obj_t * processNewStepButton;

static uint8_t isColor = 0;
static uint8_t isSaved = 0;
static uint8_t somethingChanged = 0;
static uint8_t stepsCreated = 0;

static lv_obj_t * processesContainer;



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
    if(data == processDetailCloseButton){
        stepCounter = 0;
        lv_msgbox_close(mboxCont);
        lv_obj_delete(mboxCont);
        LV_LOG_USER("Close Process Detail");
    }
    if(data == processColorLabel){
        lv_obj_set_style_text_color(processColorLabel, lv_color_hex(GREEN_DARK), LV_PART_MAIN);
        lv_obj_set_style_text_color(processBnWLabel, lv_color_hex(WHITE), LV_PART_MAIN);
        isColor = 1;
        somethingChanged = 1;

        if(stepsCreated > 0){
          lv_obj_clear_state(processSaveLabel, LV_STATE_DISABLED);
          lv_obj_set_style_text_color(processSaveLabel, lv_color_hex(WHITE), LV_PART_MAIN);
        }

        LV_LOG_USER("Pressed processColorLabel");
    }
    if(data == processBnWLabel){
        lv_obj_set_style_text_color(processBnWLabel, lv_color_hex(GREEN_DARK), LV_PART_MAIN);
        lv_obj_set_style_text_color(processColorLabel, lv_color_hex(WHITE), LV_PART_MAIN);
        isColor = 0;
        somethingChanged = 1;
        if(stepsCreated > 0){
          lv_obj_clear_state(processSaveLabel, LV_STATE_DISABLED);
          lv_obj_set_style_text_color(processSaveLabel, lv_color_hex(WHITE), LV_PART_MAIN);
        }
        LV_LOG_USER("Pressed processBnWLabel");
    }

    if(data == processPreferredLabel){
        if(isPreferred == 0){
          lv_obj_set_style_text_color(processPreferredLabel, lv_color_hex(RED), LV_PART_MAIN);
          isPreferred = 1;
          somethingChanged = 1;

          if(stepsCreated > 0){
            lv_obj_clear_state(processSaveLabel, LV_STATE_DISABLED);
            lv_obj_set_style_text_color(processSaveLabel, lv_color_hex(WHITE), LV_PART_MAIN);
          }
        }
        else{
          lv_obj_set_style_text_color(processPreferredLabel, lv_color_hex(WHITE), LV_PART_MAIN);
          isPreferred = 0;
        }
        LV_LOG_USER("Process is preferred :%d",isPreferred);
    }
    if(data == processSaveLabel && stepsCreated > 0){
        if(isSaved == 0){
          isSaved = 1;
          somethingChanged = 0;
          lv_obj_clear_state(processRunButton, LV_STATE_DISABLED);

          lv_obj_set_style_text_color(processSaveLabel, lv_color_hex(GREY), LV_PART_MAIN);
          lv_obj_add_state(processSaveLabel, LV_STATE_DISABLED);
          processElementCreate(processesContainer);
        }
        else{
          isSaved = 0;
        }
        LV_LOG_USER("Pressed processSaveLabel");
    }
    if(data == processDeleteButton){
        stepCounter = 0;
        messagePopupCreate(deletePopupTitle_text,deletePopupBody_text, deleteButton_text, stepDetailCancel_text, processDetailParent);
        LV_LOG_USER("Pressed processDeleteButton");
    }
    if(data == processRunButton){
        stepCounter = 0;
        lv_msgbox_close(mboxCont);
        lv_obj_delete(mboxCont);
        LV_LOG_USER("Pressed processRunButton");
        checkup();
    }
    if(data == processNewStepButton){
        LV_LOG_USER("Pressed processNewStepButton");
        stepsCreated += 1;
        somethingChanged = 1;
        lv_obj_clear_state(processSaveLabel, LV_STATE_DISABLED);

        if(stepsCreated > 0){
          lv_obj_clear_state(processSaveLabel, LV_STATE_DISABLED);
          lv_obj_set_style_text_color(processSaveLabel, lv_color_hex(WHITE), LV_PART_MAIN);
        }

        stepDetail(processStepsContainer);
    }
  }

  if(code == LV_EVENT_LONG_PRESSED_REPEAT){
    
  }

  if(code == LV_EVENT_VALUE_CHANGED) {
      if(data == processTempControlSwitch){
          LV_LOG_USER("Temperature controlled : %s", lv_obj_has_state(obj, LV_STATE_CHECKED) ? "On" : "Off");
          somethingChanged = 1;

          if(stepsCreated > 0){
            lv_obj_clear_state(processSaveLabel, LV_STATE_DISABLED);
            lv_obj_set_style_text_color(processSaveLabel, lv_color_hex(WHITE), LV_PART_MAIN);
          }
      }
  }

  if(code == LV_EVENT_FOCUSED) {
      if(data == processTempTextArea){
          LV_LOG_USER("Set Temperature");
          rollerPopupCreate(tempCelsiusOptions,tuneTempPopupTitle_text,processTempTextArea);
      }
      if(data == processToleranceTextArea){
          LV_LOG_USER("Set Tolerance");
          rollerPopupCreate(tempCelsiusToleranceOptions,tuneTempPopupTitle_text,processToleranceTextArea);
      }
  }
}
 
void event_processDetail_style_delete(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_DELETE) {
        //list of all styles to be reset, so clean the memory.
        lv_style_reset(&textAreaStyle);
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
  isColor = 0;
  isSaved = 0;
  somethingChanged = 0;
  stepsCreated = 0;

  processesContainer = referenceProcess;

  processDetailParent = lv_obj_class_create_obj(&lv_msgbox_backdrop_class, lv_layer_top());
  lv_obj_class_init_obj(processDetailParent);
  lv_obj_remove_flag(processDetailParent, LV_OBJ_FLAG_IGNORE_LAYOUT);
  lv_obj_set_size(processDetailParent, LV_PCT(100), LV_PCT(100));
  
  lv_style_init(&textAreaStyle);
  
  /*********************
  *    PAGE ELEMENTS
  *********************/
   
      processDetailContainer = lv_obj_create(processDetailParent);
      lv_obj_align(processDetailContainer, LV_ALIGN_CENTER, 0, 0);
      lv_obj_set_size(processDetailContainer, LV_PCT(100), LV_PCT(100));
      lv_obj_remove_flag(processDetailContainer, LV_OBJ_FLAG_SCROLLABLE); 

            processDetailCloseButton = lv_button_create(processDetailContainer);
            lv_obj_set_size(processDetailCloseButton, BUTTON_POPUP_CLOSE_WIDTH * 1.2, BUTTON_POPUP_CLOSE_HEIGHT * 1.2);
            lv_obj_align(processDetailCloseButton, LV_ALIGN_TOP_RIGHT, 7 , -10);
            lv_obj_add_event_cb(processDetailCloseButton, event_processDetail, LV_EVENT_CLICKED, processDetailCloseButton);
            lv_obj_set_style_bg_color(processDetailCloseButton, lv_color_hex(GREEN_DARK), LV_PART_MAIN);
            lv_obj_move_foreground(processDetailCloseButton);


                  processDetailCloseButtonLabel = lv_label_create(processDetailCloseButton);         
                  lv_label_set_text(processDetailCloseButtonLabel, closePopup_icon); 
                  lv_obj_set_style_text_font(processDetailCloseButtonLabel, &FilMachineFontIcons_30, 0);              
                  lv_obj_align(processDetailCloseButtonLabel, LV_ALIGN_CENTER, 0, 0);
                  
        

            processDetailNameContainer = lv_obj_create(processDetailContainer);
            lv_obj_remove_flag(processDetailNameContainer, LV_OBJ_FLAG_SCROLLABLE); 
            lv_obj_align(processDetailNameContainer, LV_ALIGN_TOP_LEFT, -10, -15);
            lv_obj_set_size(processDetailNameContainer, 350, 50); 
            //lv_obj_set_style_border_color(processDetailNameContainer, lv_color_hex(GREEN_DARK), 0);
            lv_obj_set_style_border_opa(processDetailNameContainer, LV_OPA_TRANSP, 0);

                  processDetailName = lv_label_create(processDetailNameContainer);         
                  lv_label_set_text(processDetailName, "E6 six baths"); 
                  lv_obj_set_width(processDetailName, 300);
                  lv_obj_set_style_text_font(processDetailName, &lv_font_montserrat_30, 0);              
                  lv_obj_align(processDetailName, LV_ALIGN_TOP_LEFT, -10, -8);
                  lv_label_set_long_mode(processDetailName, LV_LABEL_LONG_SCROLL_CIRCULAR);

            processDetailStepsLabel = lv_label_create(processDetailContainer);         
            lv_label_set_text(processDetailStepsLabel, processDetailStep_text); 
            lv_obj_set_width(processDetailStepsLabel, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(processDetailStepsLabel, &lv_font_montserrat_24, 0);              
            lv_obj_align(processDetailStepsLabel, LV_ALIGN_TOP_LEFT, 0, 25);

            processStepsContainer = lv_obj_create(processDetailContainer);
            lv_obj_align(processStepsContainer, LV_ALIGN_TOP_LEFT, -10, 55);
            lv_obj_set_size(processStepsContainer, 250, 240);
            lv_obj_set_scroll_dir(processStepsContainer, LV_DIR_VER);
            lv_obj_set_style_border_color(processStepsContainer, lv_palette_main(LV_PALETTE_GREEN), 0);
            //lv_obj_set_style_border_opa(processStepsContainer, LV_OPA_TRANSP, 0);


            processNewStepButton = lv_button_create(processDetailContainer);
            lv_obj_set_size(processNewStepButton, BUTTON_POPUP_CLOSE_WIDTH, BUTTON_POPUP_CLOSE_HEIGHT);
            lv_obj_align(processNewStepButton, LV_ALIGN_TOP_LEFT, 210 , 55);
            lv_obj_add_event_cb(processNewStepButton, event_processDetail, LV_EVENT_CLICKED, processNewStepButton);
            lv_obj_set_style_bg_color(processNewStepButton, lv_palette_main(LV_PALETTE_GREEN), LV_PART_MAIN);
            //lv_obj_move_foreground(processNewStepButton);
  
                    processNewStepLabel = lv_label_create(processNewStepButton);         
                    lv_label_set_text(processNewStepLabel, LV_SYMBOL_PLUS); 
                    lv_obj_set_style_text_font(processNewStepLabel, &lv_font_montserrat_20, 0);              
                    lv_obj_align(processNewStepLabel, LV_ALIGN_CENTER, 0, 0);


            processDetailInfoLabel = lv_label_create(processDetailContainer);         
            lv_label_set_text(processDetailInfoLabel, processDetailInfo_text); 
            lv_obj_set_width(processDetailInfoLabel, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(processDetailInfoLabel, &lv_font_montserrat_24, 0);              
            lv_obj_align(processDetailInfoLabel, LV_ALIGN_TOP_RIGHT, -105, 25);

            processInfoContainer = lv_obj_create(processDetailContainer);
            lv_obj_align(processInfoContainer, LV_ALIGN_TOP_LEFT, 245, 55);
            lv_obj_set_size(processInfoContainer, 210, 195);
            lv_obj_remove_flag(processInfoContainer, LV_OBJ_FLAG_SCROLLABLE); 
            //lv_obj_set_scroll_dir(processInfoContainer, LV_DIR_VER);
            lv_obj_set_style_border_color(processInfoContainer, lv_color_hex(WHITE), 0);
            //lv_obj_set_style_border_opa(processInfoContainer, LV_OPA_TRANSP, 0);


                  processTempControlContainer = lv_obj_create(processInfoContainer);
                  lv_obj_remove_flag(processTempControlContainer, LV_OBJ_FLAG_SCROLLABLE); 
                  lv_obj_align(processTempControlContainer, LV_ALIGN_TOP_LEFT, -15, -17);
                  lv_obj_set_size(processTempControlContainer, 205, 40); 
                  //lv_obj_set_style_border_color(processTempControlContainer, lv_color_hex(GREEN_DARK), 0);
                  lv_obj_set_style_border_opa(processTempControlContainer, LV_OPA_TRANSP, 0);

                          processTempControlLabel = lv_label_create(processTempControlContainer);         
                          lv_label_set_text(processTempControlLabel, processDetailIsTempControl_text); 
                          lv_obj_set_style_text_font(processTempControlLabel, &lv_font_montserrat_16, 0);              
                          lv_obj_align(processTempControlLabel, LV_ALIGN_LEFT_MID, -15, 0);

                          processTempControlSwitch = lv_switch_create(processTempControlContainer);
                          lv_obj_add_event_cb(processTempControlSwitch, event_processDetail, LV_EVENT_VALUE_CHANGED, processTempControlSwitch);
                          lv_obj_align(processTempControlSwitch, LV_ALIGN_RIGHT_MID, 15, 0);
                          lv_obj_set_style_bg_color(processTempControlSwitch, lv_palette_darken(LV_PALETTE_GREY, 3), LV_STATE_DEFAULT);
                          lv_obj_set_style_bg_color(processTempControlSwitch,  lv_palette_main(LV_PALETTE_GREEN), LV_PART_KNOB | LV_STATE_DEFAULT);
                          lv_obj_set_style_bg_color(processTempControlSwitch, lv_color_hex(GREEN_DARK) , LV_PART_INDICATOR | LV_STATE_CHECKED);


                 

                  processTempContainer = lv_obj_create(processInfoContainer);
                  lv_obj_remove_flag(processTempContainer, LV_OBJ_FLAG_SCROLLABLE); 
                  lv_obj_align(processTempContainer, LV_ALIGN_TOP_LEFT, -15, 20);
                  lv_obj_set_size(processTempContainer, 205, 45); 
                  //lv_obj_set_style_border_color(processTempContainer, lv_color_hex(GREEN_DARK), 0);
                  lv_obj_set_style_border_opa(processTempContainer, LV_OPA_TRANSP, 0);   

                          processTempLabel = lv_label_create(processTempContainer);         
                          lv_label_set_text(processTempLabel, processDetailTemp_text); 
                          lv_obj_set_style_text_font(processTempLabel, &lv_font_montserrat_16, 0);              
                          lv_obj_align(processTempLabel, LV_ALIGN_LEFT_MID, -15, 0);

                          processTempTextArea = lv_textarea_create(processTempContainer);
                          lv_textarea_set_one_line(processTempTextArea, true);
                          lv_textarea_set_placeholder_text(processTempTextArea, processDetailTempPlaceHolder_text);
                          lv_obj_align(processTempTextArea, LV_ALIGN_LEFT_MID, 100, 0);
                          lv_obj_set_width(processTempTextArea, 60);
                          lv_obj_add_event_cb(processTempTextArea, event_processDetail, LV_EVENT_ALL, processTempTextArea);
                          lv_obj_add_state(processTempTextArea, LV_STATE_FOCUSED); /*To be sure the cursor is visible*/
                          lv_obj_set_style_bg_color(processTempTextArea, lv_palette_darken(LV_PALETTE_GREY, 3), 0);
                          lv_obj_set_style_text_align(processTempTextArea , LV_TEXT_ALIGN_CENTER, 0);
                          lv_style_set_text_font(&textAreaStyle, &lv_font_montserrat_18);
                          lv_obj_add_style(processTempTextArea, &textAreaStyle, LV_PART_MAIN);
                          lv_obj_set_style_border_color(processTempTextArea, lv_color_hex(WHITE), 0);


                          processTempUnitLabel = lv_label_create(processTempContainer);         
                          lv_label_set_text(processTempUnitLabel, celsius_text); 
                          lv_obj_set_style_text_font(processTempUnitLabel, &lv_font_montserrat_20, 0);              
                          lv_obj_align(processTempUnitLabel, LV_ALIGN_LEFT_MID, 160, 0);


                  processToleranceContainer = lv_obj_create(processInfoContainer);
                  lv_obj_remove_flag(processToleranceContainer, LV_OBJ_FLAG_SCROLLABLE); 
                  lv_obj_align(processToleranceContainer, LV_ALIGN_TOP_LEFT, -15, 65);
                  lv_obj_set_size(processToleranceContainer, 205, 45); 
                  //lv_obj_set_style_border_color(processToleranceContainer, lv_color_hex(GREEN_DARK), 0);
                  lv_obj_set_style_border_opa(processToleranceContainer, LV_OPA_TRANSP, 0);   

                          processToleranceLabel = lv_label_create(processToleranceContainer);         
                          lv_label_set_text(processToleranceLabel, processDetailTempTolerance_text); 
                          lv_obj_set_style_text_font(processToleranceLabel, &lv_font_montserrat_16, 0);              
                          lv_obj_align(processToleranceLabel, LV_ALIGN_LEFT_MID, -15, 0);

                          processToleranceTextArea = lv_textarea_create(processToleranceContainer);
                          lv_textarea_set_one_line(processToleranceTextArea, true);
                          lv_textarea_set_placeholder_text(processToleranceTextArea, processDetailTempPlaceHolder_text);
                          lv_obj_align(processToleranceTextArea, LV_ALIGN_LEFT_MID, 100, 0);
                          lv_obj_set_width(processToleranceTextArea, 60);

                          lv_obj_add_event_cb(processToleranceTextArea, event_processDetail, LV_EVENT_ALL, processToleranceTextArea);
                          lv_obj_add_state(processToleranceTextArea, LV_STATE_FOCUSED); /*To be sure the cursor is visible*/
                          lv_obj_set_style_bg_color(processToleranceTextArea, lv_palette_darken(LV_PALETTE_GREY, 3), 0);
                          lv_obj_set_style_text_align(processToleranceTextArea , LV_TEXT_ALIGN_CENTER, 0);
                          //lv_style_set_text_font(&textAreaStyle, &lv_font_montserrat_18);
                          lv_obj_add_style(processToleranceTextArea, &textAreaStyle, LV_PART_MAIN);
                          lv_obj_set_style_border_color(processToleranceTextArea, lv_color_hex(WHITE), 0);


                          processTempUnitLabel = lv_label_create(processToleranceContainer);         
                          lv_label_set_text(processTempUnitLabel, celsius_text); 
                          lv_obj_set_style_text_font(processTempUnitLabel, &lv_font_montserrat_20, 0);              
                          lv_obj_align(processTempUnitLabel, LV_ALIGN_LEFT_MID, 160, 0);
                          

                  processTotalTimeContainer = lv_obj_create(processInfoContainer);
                  lv_obj_remove_flag(processTotalTimeContainer, LV_OBJ_FLAG_SCROLLABLE); 
                  lv_obj_align(processTotalTimeContainer, LV_ALIGN_TOP_LEFT, -15, 110);
                  lv_obj_set_size(processTotalTimeContainer, 205, 30); 
                  //lv_obj_set_style_border_color(processTotalTimeContainer, lv_color_hex(GREEN_DARK), 0);
                  lv_obj_set_style_border_opa(processTotalTimeContainer, LV_OPA_TRANSP, 0);   

                          processTotalTimeLabel = lv_label_create(processTotalTimeContainer);         
                          lv_label_set_text(processTotalTimeLabel, processDetailTotalTime_text); 
                          lv_obj_set_style_text_font(processTotalTimeLabel, &lv_font_montserrat_16, 0);              
                          lv_obj_align(processTotalTimeLabel, LV_ALIGN_LEFT_MID, -15, 0);

                          processTotalTimeValue = lv_label_create(processTotalTimeContainer);         
                          lv_label_set_text(processTotalTimeValue, "32m20s"); 
                          lv_obj_set_style_text_font(processTotalTimeValue, &lv_font_montserrat_20, 0);              
                          lv_obj_align(processTotalTimeValue, LV_ALIGN_LEFT_MID, 100, 0);



                  processColorOrBnWContainer = lv_obj_create(processInfoContainer);
                  lv_obj_remove_flag(processColorOrBnWContainer, LV_OBJ_FLAG_SCROLLABLE); 
                  lv_obj_align(processColorOrBnWContainer, LV_ALIGN_TOP_LEFT, -18, 137);
                  lv_obj_set_size(processColorOrBnWContainer, 105, 40); 
                  lv_obj_set_style_border_color(processColorOrBnWContainer, lv_color_hex(WHITE), 0);
                  //lv_obj_set_style_border_opa(processColorOrBnWContainer, LV_OPA_TRANSP, 0);   


                          processColorLabel = lv_label_create(processColorOrBnWContainer);         
                          lv_label_set_text(processColorLabel, colorpalette_icon); 
                          lv_obj_set_style_text_font(processColorLabel, &FilMachineFontIcons_30, 0);              
                          lv_obj_align(processColorLabel, LV_ALIGN_LEFT_MID, -5, 0);
                          lv_obj_add_flag(processColorLabel, LV_OBJ_FLAG_CLICKABLE);
                          lv_obj_add_event_cb(processColorLabel, event_processDetail, LV_EVENT_CLICKED, processColorLabel);


                          processBnWLabel = lv_label_create(processColorOrBnWContainer);         
                          lv_label_set_text(processBnWLabel, blackwhite_icon); 
                          lv_obj_set_style_text_font(processBnWLabel, &FilMachineFontIcons_30, 0);              
                          lv_obj_align(processBnWLabel, LV_ALIGN_LEFT_MID, 45, 0);
                          lv_obj_add_flag(processBnWLabel, LV_OBJ_FLAG_CLICKABLE);
                          lv_obj_add_event_cb(processBnWLabel, event_processDetail, LV_EVENT_CLICKED, processBnWLabel);


                  processPreferredLabel = lv_label_create(processInfoContainer);         
                  lv_label_set_text(processPreferredLabel, preferred_icon); 
                  lv_obj_set_style_text_font(processPreferredLabel, &FilMachineFontIcons_30, 0);              
                  lv_obj_align(processPreferredLabel, LV_ALIGN_TOP_LEFT, 100, 140);
                  lv_obj_add_flag(processPreferredLabel, LV_OBJ_FLAG_CLICKABLE);
                  lv_obj_add_event_cb(processPreferredLabel, event_processDetail, LV_EVENT_CLICKED, processPreferredLabel);


                  processSaveLabel = lv_label_create(processInfoContainer);         
                  lv_label_set_text(processSaveLabel, save_Icon); 
                  lv_obj_set_style_text_font(processSaveLabel, &FilMachineFontIcons_30, 0);              
                  lv_obj_align(processSaveLabel, LV_ALIGN_TOP_LEFT, 150, 140);
                  lv_obj_add_flag(processSaveLabel, LV_OBJ_FLAG_CLICKABLE);
                  lv_obj_add_state(processSaveLabel, LV_STATE_DISABLED);
                  lv_obj_add_event_cb(processSaveLabel, event_processDetail, LV_EVENT_CLICKED, processSaveLabel);
                  lv_obj_set_style_text_color(processSaveLabel, lv_color_hex(GREY), LV_PART_MAIN);

                  processDeleteButton = lv_button_create(processDetailContainer);
                  lv_obj_set_size(processDeleteButton, BUTTON_PROCESS_WIDTH, BUTTON_PROCESS_HEIGHT);
                  lv_obj_align(processDeleteButton, LV_ALIGN_BOTTOM_RIGHT, -103, 10);
                  lv_obj_add_event_cb(processDeleteButton, event_processDetail, LV_EVENT_CLICKED, processDeleteButton);
                  lv_obj_set_style_bg_color(processDeleteButton, lv_color_hex(RED_DARK), LV_PART_MAIN);
                  lv_obj_add_state(processDeleteButton, LV_STATE_DISABLED);


                          processDeleteLabel = lv_label_create(processDeleteButton);         
                          lv_label_set_text(processDeleteLabel, trash_Icon); 
                          lv_obj_set_style_text_font(processDeleteLabel, &FilMachineFontIcons_20, 0);              
                          lv_obj_align(processDeleteLabel, LV_ALIGN_CENTER, 0, 0);
                          lv_obj_add_flag(processDeleteLabel, LV_OBJ_FLAG_CLICKABLE);

                  
                  processRunButton = lv_button_create(processDetailContainer);
                  lv_obj_set_size(processRunButton, BUTTON_PROCESS_WIDTH, BUTTON_PROCESS_HEIGHT);
                  lv_obj_align(processRunButton, LV_ALIGN_BOTTOM_RIGHT, 10, 10);
                  lv_obj_add_event_cb(processRunButton, event_processDetail, LV_EVENT_CLICKED, processRunButton);
                  lv_obj_set_style_bg_color(processRunButton, lv_color_hex(GREEN_DARK), LV_PART_MAIN);
                  if(stepsCreated == 0)
                      lv_obj_add_state(processRunButton, LV_STATE_DISABLED);
                  else
                      lv_obj_clear_state(processRunButton, LV_STATE_DISABLED);

                          processRunLabel = lv_label_create(processRunButton);         
                          lv_label_set_text(processRunLabel, play_Icon); 
                          lv_obj_set_style_text_font(processRunLabel, &FilMachineFontIcons_20, 0);              
                          lv_obj_align(processRunLabel, LV_ALIGN_CENTER, 0, 0);
                          lv_obj_add_flag(processRunLabel, LV_OBJ_FLAG_CLICKABLE);



}

