/**
 * @file page_stepDetail.c
 *
 */


//ESSENTIAL INCLUDE
#include <lvgl.h>
#include "../../include/definitions.h"

extern struct gui_components gui;


//ACCESSORY INCLUDES

/* STEP DETAIL VARIABLES*/
static lv_obj_t * stepDetailNameContainer;
static lv_obj_t * stepDetailContainer;
static lv_obj_t * stepDurationContainer;
static lv_obj_t * stepTypeContainer;
static lv_obj_t * stepSourceContainer;
static lv_obj_t * stepDiscardAfterContainer;

static lv_obj_t * stepDetailLabel;
static lv_obj_t * stepDetailNamelLabel;
static lv_obj_t * stepDurationLabel;
static lv_obj_t * stepDurationMinLabel;
static lv_obj_t * stepSaveLabel;
static lv_obj_t * stepCancelLabel;
static lv_obj_t * stepTypeLabel;
static lv_obj_t * stepSourceLabel;
static lv_obj_t * stepTypeHelpIcon;
static lv_obj_t * stepSourceTempLabel;
static lv_obj_t * stepDiscsardAfterLabel;
static lv_obj_t * stepSourceTempHelpIcon;
static lv_obj_t * stepSourceTempValue;

static lv_obj_t * stepDiscsardAfterSwitch;

static lv_obj_t * stepSaveButton;
static lv_obj_t * stepCancelButton;

static lv_obj_t * stepSourceDropDownList;
static lv_obj_t * stepTypeDropDownList;
static lv_style_t dropDownListStyle;

static lv_obj_t * stepsContainer;




void event_stepDetail(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t * obj = (lv_obj_t *)lv_event_get_target(e);
  lv_obj_t * objCont = (lv_obj_t *)lv_obj_get_parent(obj);
  lv_obj_t * mboxCont = (lv_obj_t *)lv_obj_get_parent(objCont);
  lv_obj_t * mboxParent = (lv_obj_t *)lv_obj_get_parent(mboxCont);
  lv_obj_t * data = (lv_obj_t *)lv_event_get_user_data(e);

 if(code == LV_EVENT_CLICKED){
    if(obj == stepSaveButton){
      LV_LOG_USER("Pressed stepSaveButton");
      lv_msgbox_close(mboxCont);
      lv_obj_delete(mboxCont);
      stepElementCreate(stepsContainer);
    }
    if(obj == stepCancelButton){
      LV_LOG_USER("Pressed stepCancelButton");
      lv_msgbox_close(mboxCont);
      lv_obj_delete(mboxCont);
    }
 }

 if(code == LV_EVENT_FOCUSED){
  if(obj == stepDetailMinTextArea){
    LV_LOG_USER("Set minutes");
    rollerPopupCreate(minutesOptions, setMinutesPopupTitle_text,stepDetailMinTextArea);
  }
  if(obj == stepDetailSecTextArea){
    LV_LOG_USER("Set seconds");
    rollerPopupCreate(secondsOptions, setMinutesPopupTitle_text,stepDetailSecTextArea);
  }
 }

 if(obj == stepDetailNamelTextArea){
     if(code == LV_EVENT_FOCUSED || code == LV_EVENT_CLICKED) {
        lv_obj_add_flag(mboxParent, LV_OBJ_FLAG_HIDDEN);
        lv_obj_remove_flag(keyBoardParent, LV_OBJ_FLAG_HIDDEN);
      }

      if(code == LV_EVENT_DEFOCUSED) {
        lv_obj_remove_flag(mboxParent, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(keyBoardParent, LV_OBJ_FLAG_HIDDEN);
      }
    }

  if(code == LV_EVENT_VALUE_CHANGED){
    if(obj == stepTypeDropDownList){
      if(lv_dropdown_get_selected(stepTypeDropDownList) == 0){
          stepType = 0;
          LV_LOG_USER("Selected stepTypeDropDownList chemical_Icon value:%d",stepType);
          lv_label_set_text(stepTypeHelpIcon, chemical_Icon);
      }
      if(lv_dropdown_get_selected(stepTypeDropDownList) == 1){
          stepType = 1;
          LV_LOG_USER("Selected stepTypeDropDownList rinse_Icon value:%d",stepType);
          lv_label_set_text(stepTypeHelpIcon, rinse_Icon);
      }
      if(lv_dropdown_get_selected(stepTypeDropDownList) == 2){
          stepType = 2;
          LV_LOG_USER("Selected stepTypeDropDownList multiRinse_Icon value:%d",stepType);
          lv_label_set_text(stepTypeHelpIcon, multiRinse_Icon);
      }
    }

    if(obj == stepDiscsardAfterSwitch){
      discardAfter = lv_obj_has_state(obj, LV_STATE_CHECKED);
      LV_LOG_USER("Discard After : %s", discardAfter ? "On" : "Off");
    }

    if(obj == stepSourceDropDownList){
         stepSource = lv_dropdown_get_selected(stepSourceDropDownList);
         LV_LOG_USER("Selected stepSourceDropDownList %d",stepSource);
    }
  }

  if(code == LV_EVENT_DELETE) {
       LV_LOG_USER("Delete Styles");
        //list of all styles to be reset, so clean the memory.
        lv_style_reset(&style_mBoxStepPopupTitleLine);
        lv_style_reset(&dropDownListStyle);
        if(keyboardStep == NULL){
          LV_LOG_USER("create_keyboard");
           }
  }
}
 



/*********************
*    STEP DETAIL
*********************/

void stepDetail(lv_obj_t * referenceStep)
{   
/*********************
  *    PAGE ELEMENTS
*********************/
      stepsContainer = referenceStep;

      LV_LOG_USER("Step detail creation");

      stepDetailParent = lv_obj_class_create_obj(&lv_msgbox_backdrop_class, lv_layer_top());
      lv_obj_class_init_obj(stepDetailParent);
      lv_obj_remove_flag(stepDetailParent, LV_OBJ_FLAG_IGNORE_LAYOUT);
      lv_obj_set_size(stepDetailParent, LV_PCT(100), LV_PCT(100));


            stepDetailContainer = lv_obj_create(stepDetailParent);
            lv_obj_align(stepDetailContainer, LV_ALIGN_CENTER, 0, 0);
            lv_obj_set_size(stepDetailContainer, 340, 300); 
            lv_obj_remove_flag(stepDetailContainer, LV_OBJ_FLAG_SCROLLABLE); 

                    stepDetailLabel = lv_label_create(stepDetailContainer);         
                    lv_label_set_text(stepDetailLabel, stepDetailTitle_text); 
                    lv_obj_set_style_text_font(stepDetailLabel, &lv_font_montserrat_22, 0);              
                    lv_obj_align(stepDetailLabel, LV_ALIGN_TOP_MID, 0, - 10);


                          lv_style_init(&style_mBoxStepPopupTitleLine);
                          lv_style_set_line_width(&style_mBoxStepPopupTitleLine, 2);
                          lv_style_set_line_color(&style_mBoxStepPopupTitleLine, lv_palette_main(LV_PALETTE_GREEN));
                          lv_style_set_line_rounded(&style_mBoxStepPopupTitleLine, true);


                    mBoxStepPopupTitleLine = lv_line_create(stepDetailContainer);
                    lv_line_set_points(mBoxStepPopupTitleLine, mBoxTitleLine_points, 2);
                    lv_obj_add_style(mBoxStepPopupTitleLine, &style_mBoxStepPopupTitleLine, 0);
                    lv_obj_align(mBoxStepPopupTitleLine, LV_ALIGN_TOP_MID, 0, 23);


            stepDetailNameContainer = lv_obj_create(stepDetailContainer);
            lv_obj_remove_flag(stepDetailNameContainer, LV_OBJ_FLAG_SCROLLABLE); 
            lv_obj_align(stepDetailNameContainer, LV_ALIGN_TOP_LEFT, -15, 30);
            lv_obj_set_size(stepDetailNameContainer, 325, 40); 
            lv_obj_set_style_border_opa(stepDetailNameContainer, LV_OPA_TRANSP, 0);
            //lv_obj_set_style_border_color(stepDetailNameContainer, lv_color_hex(GREEN_DARK), 0);

                  stepDetailNamelLabel = lv_label_create(stepDetailNameContainer);         
                  lv_label_set_text(stepDetailNamelLabel, stepDetailName_text); 
                  lv_obj_set_style_text_font(stepDetailNamelLabel, &lv_font_montserrat_22, 0);              
                  lv_obj_align(stepDetailNamelLabel, LV_ALIGN_LEFT_MID, -10, 0);
              
                  stepDetailNamelTextArea = lv_textarea_create(stepDetailNameContainer);
                  lv_textarea_set_one_line(stepDetailNamelTextArea, true);
                  lv_textarea_set_placeholder_text(stepDetailNamelTextArea, stepDetailPlaceHolder_text);
                  lv_obj_align(stepDetailNamelTextArea, LV_ALIGN_LEFT_MID, 70, 0);
                  lv_obj_set_width(stepDetailNamelTextArea, 210);
                  lv_obj_add_event_cb(stepDetailNamelTextArea, event_stepDetail, LV_EVENT_ALL, stepDetailNamelTextArea);
                  lv_obj_add_state(stepDetailNamelTextArea, LV_STATE_FOCUSED); 
                  lv_obj_set_style_bg_color(stepDetailNamelTextArea, lv_palette_darken(LV_PALETTE_GREY, 3), 0);
                  lv_obj_set_style_border_color(stepDetailNamelTextArea, lv_color_hex(WHITE), 0);



            stepDurationContainer = lv_obj_create(stepDetailContainer);
            lv_obj_remove_flag(stepDurationContainer, LV_OBJ_FLAG_SCROLLABLE); 
            lv_obj_align(stepDurationContainer, LV_ALIGN_TOP_LEFT, -15, 70);
            lv_obj_set_size(stepDurationContainer, 325, 40); 
            lv_obj_set_style_border_opa(stepDurationContainer, LV_OPA_TRANSP, 0);
            //lv_obj_set_style_border_color(stepDurationContainer, lv_color_hex(GREEN_DARK), 0);

                  stepDurationLabel = lv_label_create(stepDurationContainer);         
                  lv_label_set_text(stepDurationLabel, stepDetailDuration_text); 
                  lv_obj_set_style_text_font(stepDurationLabel, &lv_font_montserrat_22, 0);              
                  lv_obj_align(stepDurationLabel, LV_ALIGN_LEFT_MID, -10, 0);

                  stepDetailMinTextArea = lv_textarea_create(stepDurationContainer);
                  lv_textarea_set_one_line(stepDetailMinTextArea, true);
                  lv_textarea_set_placeholder_text(stepDetailMinTextArea, stepDetailDurationMinPlaceHolder_text);
                  lv_obj_align(stepDetailMinTextArea, LV_ALIGN_LEFT_MID, 100, 0);
                  lv_obj_set_width(stepDetailMinTextArea, 60);
                  lv_obj_add_event_cb(stepDetailMinTextArea, event_stepDetail, LV_EVENT_ALL, stepDetailMinTextArea);
                  lv_obj_add_state(stepDetailMinTextArea, LV_STATE_FOCUSED); 
                  lv_obj_set_style_bg_color(stepDetailMinTextArea, lv_palette_darken(LV_PALETTE_GREY, 3), 0);
                  lv_obj_set_style_text_align(stepDetailMinTextArea , LV_TEXT_ALIGN_CENTER, 0);
                  lv_obj_set_style_border_color(stepDetailMinTextArea, lv_color_hex(WHITE), 0);


                  stepDurationMinLabel = lv_label_create(stepDurationContainer);         
                  lv_label_set_text(stepDurationMinLabel, stepDetailDurationTimeSep_text); 
                  lv_obj_set_style_text_font(stepDurationMinLabel, &lv_font_montserrat_22, 0);              
                  lv_obj_align(stepDurationMinLabel, LV_ALIGN_LEFT_MID, 170, 0);

                  stepDetailSecTextArea = lv_textarea_create(stepDurationContainer);
                  lv_textarea_set_one_line(stepDetailSecTextArea, true);
                  lv_textarea_set_placeholder_text(stepDetailSecTextArea, stepDetailDurationSecPlaceHolder_text);
                  lv_obj_align(stepDetailSecTextArea, LV_ALIGN_LEFT_MID, 187, 0);
                  lv_obj_set_width(stepDetailSecTextArea, 60);
                  lv_obj_add_event_cb(stepDetailSecTextArea, event_stepDetail, LV_EVENT_ALL, stepDetailSecTextArea);
                  lv_obj_add_state(stepDetailSecTextArea, LV_STATE_FOCUSED); 
                  lv_obj_set_style_bg_color(stepDetailSecTextArea, lv_palette_darken(LV_PALETTE_GREY, 3), 0);
                  lv_obj_set_style_text_align(stepDetailSecTextArea , LV_TEXT_ALIGN_CENTER, 0);
                  lv_obj_set_style_border_color(stepDetailSecTextArea, lv_color_hex(WHITE), 0);


    
            stepTypeContainer = lv_obj_create(stepDetailContainer);
            lv_obj_remove_flag(stepTypeContainer, LV_OBJ_FLAG_SCROLLABLE); 
            lv_obj_align(stepTypeContainer, LV_ALIGN_TOP_LEFT, -15, 110);
            lv_obj_set_size(stepTypeContainer, 325, 40); 
            lv_obj_set_style_border_opa(stepTypeContainer, LV_OPA_TRANSP, 0);
            //lv_obj_set_style_border_color(stepTypeContainer, lv_color_hex(GREEN_DARK), 0);


                  stepTypeLabel = lv_label_create(stepTypeContainer);         
                  lv_label_set_text(stepTypeLabel, stepDetailType_text); 
                  lv_obj_set_style_text_font(stepTypeLabel, &lv_font_montserrat_22, 0);              
                  lv_obj_align(stepTypeLabel, LV_ALIGN_LEFT_MID, -10, 0);


                  lv_style_init(&dropDownListStyle);
                  stepTypeDropDownList = lv_dropdown_create(stepTypeContainer);
                  lv_obj_set_style_border_opa(stepTypeDropDownList, LV_OPA_TRANSP, 0);
                  lv_dropdown_set_options(stepTypeDropDownList, stepTypeList);
                  lv_style_set_text_font(&dropDownListStyle, &lv_font_montserrat_20);
                  lv_obj_add_style(stepTypeDropDownList, &dropDownListStyle, 0);
                  lv_obj_align(stepTypeDropDownList, LV_ALIGN_LEFT_MID, 50, 2);
                  lv_obj_set_size(stepTypeDropDownList, 165, 50); 
                  lv_obj_add_event_cb(stepTypeDropDownList, event_stepDetail, LV_EVENT_VALUE_CHANGED, stepTypeDropDownList);
                  lv_obj_set_style_bg_color(lv_dropdown_get_list(stepTypeDropDownList), lv_palette_main(LV_PALETTE_GREEN), LV_PART_SELECTED | LV_STATE_CHECKED);


                  stepTypeHelpIcon = lv_label_create(stepTypeContainer);         
                  lv_label_set_text(stepTypeHelpIcon, chemical_Icon); 
                  lv_obj_set_style_text_font(stepTypeHelpIcon, &FilMachineFontIcons_20, 0);              
                  lv_obj_align(stepTypeHelpIcon, LV_ALIGN_LEFT_MID, 210, 0);


            stepSourceContainer = lv_obj_create(stepDetailContainer);
            lv_obj_remove_flag(stepSourceContainer, LV_OBJ_FLAG_SCROLLABLE); 
            lv_obj_align(stepSourceContainer, LV_ALIGN_TOP_LEFT, -15, 150);
            lv_obj_set_size(stepSourceContainer, 325, 40); 
            lv_obj_set_style_border_opa(stepSourceContainer, LV_OPA_TRANSP, 0);
            //lv_obj_set_style_border_color(stepSourceContainer, lv_color_hex(GREEN_DARK), 0);


                  stepSourceLabel = lv_label_create(stepSourceContainer);         
                  lv_label_set_text(stepSourceLabel, stepDetailSource_text); 
                  lv_obj_set_style_text_font(stepSourceLabel, &lv_font_montserrat_22, 0);              
                  lv_obj_align(stepSourceLabel, LV_ALIGN_LEFT_MID, -10, 0);


                  lv_style_init(&dropDownListStyle);
                  stepSourceDropDownList = lv_dropdown_create(stepSourceContainer);
                  lv_obj_set_style_border_opa(stepSourceDropDownList, LV_OPA_TRANSP, 0);
                  lv_dropdown_set_options(stepSourceDropDownList, stepSourceList);
                  lv_style_set_text_font(&dropDownListStyle, &lv_font_montserrat_20);
                  lv_obj_add_style(stepSourceDropDownList, &dropDownListStyle, 0);
                  lv_obj_align(stepSourceDropDownList, LV_ALIGN_LEFT_MID, 71, 2);
                  lv_obj_set_size(stepSourceDropDownList, 83, 50); 
                  lv_obj_add_event_cb(stepSourceDropDownList, event_stepDetail, LV_EVENT_VALUE_CHANGED, stepSourceDropDownList);
                  lv_obj_set_style_bg_color(lv_dropdown_get_list(stepSourceDropDownList), lv_palette_main(LV_PALETTE_GREEN), LV_PART_SELECTED | LV_STATE_CHECKED); 


                  stepSourceTempLabel = lv_label_create(stepSourceContainer);         
                  lv_label_set_text(stepSourceTempLabel, stepDetailCurrentTemp_text); 
                  lv_obj_set_style_text_font(stepSourceTempLabel, &lv_font_montserrat_18, 0);              
                  lv_obj_align(stepSourceTempLabel, LV_ALIGN_LEFT_MID, 155, 0);

                  stepSourceTempHelpIcon = lv_label_create(stepSourceContainer);         
                  lv_label_set_text(stepSourceTempHelpIcon, temp_icon); 
                  lv_obj_set_style_text_font(stepSourceTempHelpIcon, &FilMachineFontIcons_20, 0);              
                  lv_obj_align(stepSourceTempHelpIcon, LV_ALIGN_LEFT_MID, 227, 0);

                  //itoa(stepSourceTemp, tempBuffer, 10);
                  stepSourceTempValue = lv_label_create(stepSourceContainer);         
                  //lv_label_set_text(stepSourceTempValue, convertFloatToTemp(stepSourceTemp)); //THIS NOT WORKS!
                  lv_label_set_text(stepSourceTempValue, "20.4°C");  
                  lv_obj_set_style_text_font(stepSourceTempValue, &lv_font_montserrat_18, 0);              
                  lv_obj_align(stepSourceTempValue, LV_ALIGN_LEFT_MID, 245, 1);



            stepDiscardAfterContainer = lv_obj_create(stepDetailContainer);
            lv_obj_remove_flag(stepDiscardAfterContainer, LV_OBJ_FLAG_SCROLLABLE); 
            lv_obj_align(stepDiscardAfterContainer, LV_ALIGN_TOP_LEFT, -15, 190);
            lv_obj_set_size(stepDiscardAfterContainer, 315, 40); 
            lv_obj_set_style_border_opa(stepDiscardAfterContainer, LV_OPA_TRANSP, 0);
            //lv_obj_set_style_border_color(stepDiscardAfterContainer, lv_color_hex(GREEN_DARK), 0);


                  stepDiscsardAfterLabel = lv_label_create(stepDiscardAfterContainer);         
                  lv_label_set_text(stepDiscsardAfterLabel, stepDetailDiscardAfter_text); 
                  lv_obj_set_style_text_font(stepDiscsardAfterLabel, &lv_font_montserrat_22, 0);              
                  lv_obj_align(stepDiscsardAfterLabel, LV_ALIGN_LEFT_MID, -10, 0);


                  stepDiscsardAfterSwitch = lv_switch_create(stepDiscardAfterContainer);
                  lv_obj_add_event_cb(stepDiscsardAfterSwitch, event_stepDetail, LV_EVENT_VALUE_CHANGED, stepDiscsardAfterSwitch);
                  lv_obj_align(stepDiscsardAfterSwitch, LV_ALIGN_LEFT_MID, 140, 0);
                  lv_obj_set_style_bg_color(stepDiscsardAfterSwitch, lv_palette_darken(LV_PALETTE_GREY, 3), LV_STATE_DEFAULT);
                  lv_obj_set_style_bg_color(stepDiscsardAfterSwitch,  lv_palette_main(LV_PALETTE_GREEN), LV_PART_KNOB | LV_STATE_DEFAULT);
                  lv_obj_set_style_bg_color(stepDiscsardAfterSwitch, lv_color_hex(GREEN_DARK) , LV_PART_INDICATOR | LV_STATE_CHECKED);



      stepSaveButton = lv_button_create(stepDetailContainer);
      lv_obj_set_size(stepSaveButton, BUTTON_PROCESS_WIDTH, BUTTON_PROCESS_HEIGHT);
      lv_obj_align(stepSaveButton, LV_ALIGN_BOTTOM_LEFT, 10 , 10);
      lv_obj_add_event_cb(stepSaveButton, event_stepDetail, LV_EVENT_CLICKED, mBoxResetFilterButton);
      lv_obj_add_event_cb(stepSaveButton, event_stepDetail, LV_EVENT_DELETE, NULL);
      lv_obj_set_style_bg_color(stepSaveButton, lv_color_hex(GREEN_DARK), LV_PART_MAIN);

          stepSaveLabel = lv_label_create(stepSaveButton);
          lv_label_set_text(stepSaveLabel, stepDetailSave_text);
          lv_obj_set_style_text_font(stepSaveLabel, &lv_font_montserrat_22, 0);
          lv_obj_align(stepSaveLabel, LV_ALIGN_CENTER, 0, 0);


      stepCancelButton = lv_button_create(stepDetailContainer);
      lv_obj_set_size(stepCancelButton, BUTTON_PROCESS_WIDTH, BUTTON_PROCESS_HEIGHT);
      lv_obj_align(stepCancelButton, LV_ALIGN_BOTTOM_RIGHT, - 10 , 10);
      lv_obj_add_event_cb(stepCancelButton, event_stepDetail, LV_EVENT_CLICKED, stepCancelButton);
      lv_obj_add_event_cb(stepCancelButton, event_stepDetail, LV_EVENT_DELETE, NULL);
      lv_obj_set_style_bg_color(stepCancelButton, lv_color_hex(GREEN_DARK), LV_PART_MAIN);

            stepCancelLabel = lv_label_create(stepCancelButton);
            lv_label_set_text(stepCancelLabel, stepDetailCancel_text);
            lv_obj_set_style_text_font(stepCancelLabel, &lv_font_montserrat_22, 0);
            lv_obj_align(stepCancelLabel, LV_ALIGN_CENTER, 0, 0);

if(keyboardStep == NULL){
  LV_LOG_USER("create_keyboard");
    //create_keyboard(keyboardStep, stepDetailParent);
    }
}

