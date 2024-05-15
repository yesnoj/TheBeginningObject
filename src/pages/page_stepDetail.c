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



void event_stepDetail(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t * obj = (lv_obj_t *)lv_event_get_target(e);
  lv_obj_t * objCont = (lv_obj_t *)lv_obj_get_parent(obj);
  lv_obj_t * mboxCont = (lv_obj_t *)lv_obj_get_parent(objCont);
  lv_obj_t * mboxParent = (lv_obj_t *)lv_obj_get_parent(mboxCont);
  lv_obj_t * data = (lv_obj_t *)lv_event_get_user_data(e);
 

 if(code == LV_EVENT_CLICKED){
    if(obj == gui.page.stepDetail.stepSaveButton){
      LV_LOG_USER("Pressed gui.page.stepDetail.stepSaveButton");
      lv_msgbox_close(mboxCont);
      lv_obj_delete(mboxCont);
      stepElementCreate(gui.page.stepDetail.stepsContainer);
    }
    if(obj == gui.page.stepDetail.stepCancelButton){
      LV_LOG_USER("Pressed gui.page.stepDetail.stepCancelButton");
      lv_msgbox_close(mboxCont);
      lv_obj_delete(mboxCont);
    }
 }

 if(code == LV_EVENT_FOCUSED){
  if(data == gui.page.stepDetail.stepDetailMinTextArea){
    LV_LOG_USER("Set minutes");
    rollerPopupCreate(gui.element.rollerPopup.minutesOptions, setMinutesPopupTitle_text,gui.page.stepDetail.stepDetailMinTextArea);
  }
  if(data == gui.page.stepDetail.stepDetailSecTextArea){
    LV_LOG_USER("Set seconds");
    rollerPopupCreate(gui.element.rollerPopup.secondsOptions, setMinutesPopupTitle_text,gui.page.stepDetail.stepDetailSecTextArea);
  }
 }

 if(obj == gui.page.stepDetail.stepDetailNamelTextArea){
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
    if(obj == gui.page.stepDetail.stepTypeDropDownList){
      if(lv_dropdown_get_selected(gui.page.stepDetail.stepTypeDropDownList) == 0){
          stepType = 0;
          LV_LOG_USER("Selected gui.page.stepDetail.stepTypeDropDownList chemical_Icon value:%d",stepType);
          lv_label_set_text(gui.page.stepDetail.stepTypeHelpIcon, chemical_Icon);
      }
      if(lv_dropdown_get_selected(gui.page.stepDetail.stepTypeDropDownList) == 1){
          stepType = 1;
          LV_LOG_USER("Selected gui.page.stepDetail.stepTypeDropDownList rinse_Icon value:%d",stepType);
          lv_label_set_text(gui.page.stepDetail.stepTypeHelpIcon, rinse_Icon);
      }
      if(lv_dropdown_get_selected(gui.page.stepDetail.stepTypeDropDownList) == 2){
          stepType = 2;
          LV_LOG_USER("Selected gui.page.stepDetail.stepTypeDropDownList multiRinse_Icon value:%d",stepType);
          lv_label_set_text(gui.page.stepDetail.stepTypeHelpIcon, multiRinse_Icon);
      }
    }

    if(obj == gui.page.stepDetail.stepDiscsardAfterSwitch){
      discardAfter = lv_obj_has_state(obj, LV_STATE_CHECKED);
      LV_LOG_USER("Discard After : %s", discardAfter ? "On" : "Off");
    }

    if(obj == gui.page.stepDetail.stepSourceDropDownList){
         stepSource = lv_dropdown_get_selected(gui.page.stepDetail.stepSourceDropDownList);
         LV_LOG_USER("Selected gui.page.stepDetail.stepSourceDropDownList %d",stepSource);
    }
  }

  if(code == LV_EVENT_DELETE) {
       LV_LOG_USER("Delete Styles");
        //list of all styles to be reset, so clean the memory.
        lv_style_reset(&gui.page.stepDetail.style_mBoxStepPopupTitleLine);
        lv_style_reset(&gui.page.stepDetail.dropDownListStyle);
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
      gui.page.stepDetail.stepsContainer = referenceStep;

      LV_LOG_USER("Step detail creation");

      gui.page.stepDetail.stepDetailParent = lv_obj_class_create_obj(&lv_msgbox_backdrop_class, lv_layer_top());
      lv_obj_class_init_obj(gui.page.stepDetail.stepDetailParent);
      lv_obj_remove_flag(gui.page.stepDetail.stepDetailParent, LV_OBJ_FLAG_IGNORE_LAYOUT);
      lv_obj_set_size(gui.page.stepDetail.stepDetailParent, LV_PCT(100), LV_PCT(100));


            gui.page.stepDetail.stepDetailContainer = lv_obj_create(gui.page.stepDetail.stepDetailParent);
            lv_obj_align(gui.page.stepDetail.stepDetailContainer, LV_ALIGN_CENTER, 0, 0);
            lv_obj_set_size(gui.page.stepDetail.stepDetailContainer, 340, 300); 
            lv_obj_remove_flag(gui.page.stepDetail.stepDetailContainer, LV_OBJ_FLAG_SCROLLABLE); 

                    gui.page.stepDetail.stepDetailLabel = lv_label_create(gui.page.stepDetail.stepDetailContainer);         
                    lv_label_set_text(gui.page.stepDetail.stepDetailLabel, stepDetailTitle_text); 
                    lv_obj_set_style_text_font(gui.page.stepDetail.stepDetailLabel, &lv_font_montserrat_22, 0);              
                    lv_obj_align(gui.page.stepDetail.stepDetailLabel, LV_ALIGN_TOP_MID, 0, - 10);


                          lv_style_init(&gui.page.stepDetail.style_mBoxStepPopupTitleLine);
                          lv_style_set_line_width(&gui.page.stepDetail.style_mBoxStepPopupTitleLine, 2);
                          lv_style_set_line_color(&gui.page.stepDetail.style_mBoxStepPopupTitleLine, lv_palette_main(LV_PALETTE_GREEN));
                          lv_style_set_line_rounded(&gui.page.stepDetail.style_mBoxStepPopupTitleLine, true);


                    gui.page.stepDetail.mBoxStepPopupTitleLine = lv_line_create(gui.page.stepDetail.stepDetailContainer);
                    lv_line_set_points(gui.page.stepDetail.mBoxStepPopupTitleLine, gui.page.stepDetail.titleLinePoints, 2);
                    lv_obj_add_style(gui.page.stepDetail.mBoxStepPopupTitleLine, &gui.page.stepDetail.style_mBoxStepPopupTitleLine, 0);
                    lv_obj_align(gui.page.stepDetail.mBoxStepPopupTitleLine, LV_ALIGN_TOP_MID, 0, 23);


            gui.page.stepDetail.stepDetailNameContainer = lv_obj_create(gui.page.stepDetail.stepDetailContainer);
            lv_obj_remove_flag(gui.page.stepDetail.stepDetailNameContainer, LV_OBJ_FLAG_SCROLLABLE); 
            lv_obj_align(gui.page.stepDetail.stepDetailNameContainer, LV_ALIGN_TOP_LEFT, -15, 30);
            lv_obj_set_size(gui.page.stepDetail.stepDetailNameContainer, 325, 40); 
            lv_obj_set_style_border_opa(gui.page.stepDetail.stepDetailNameContainer, LV_OPA_TRANSP, 0);
            //lv_obj_set_style_border_color(gui.page.stepDetail.stepDetailNameContainer, lv_color_hex(GREEN_DARK), 0);

                  gui.page.stepDetail.stepDetailNamelLabel = lv_label_create(gui.page.stepDetail.stepDetailNameContainer);         
                  lv_label_set_text(gui.page.stepDetail.stepDetailNamelLabel, stepDetailName_text); 
                  lv_obj_set_style_text_font(gui.page.stepDetail.stepDetailNamelLabel, &lv_font_montserrat_22, 0);              
                  lv_obj_align(gui.page.stepDetail.stepDetailNamelLabel, LV_ALIGN_LEFT_MID, -10, 0);
              
                  gui.page.stepDetail.stepDetailNamelTextArea = lv_textarea_create(gui.page.stepDetail.stepDetailNameContainer);
                  lv_textarea_set_one_line(gui.page.stepDetail.stepDetailNamelTextArea, true);
                  lv_textarea_set_placeholder_text(gui.page.stepDetail.stepDetailNamelTextArea, stepDetailPlaceHolder_text);
                  lv_obj_align(gui.page.stepDetail.stepDetailNamelTextArea, LV_ALIGN_LEFT_MID, 70, 0);
                  lv_obj_set_width(gui.page.stepDetail.stepDetailNamelTextArea, 210);
                  lv_obj_add_event_cb(gui.page.stepDetail.stepDetailNamelTextArea, event_stepDetail, LV_EVENT_ALL, gui.page.stepDetail.stepDetailNamelTextArea);
                  lv_obj_add_state(gui.page.stepDetail.stepDetailNamelTextArea, LV_STATE_FOCUSED); 
                  lv_obj_set_style_bg_color(gui.page.stepDetail.stepDetailNamelTextArea, lv_palette_darken(LV_PALETTE_GREY, 3), 0);
                  lv_obj_set_style_border_color(gui.page.stepDetail.stepDetailNamelTextArea, lv_color_hex(WHITE), 0);



            gui.page.stepDetail.stepDurationContainer = lv_obj_create(gui.page.stepDetail.stepDetailContainer);
            lv_obj_remove_flag(gui.page.stepDetail.stepDurationContainer, LV_OBJ_FLAG_SCROLLABLE); 
            lv_obj_align(gui.page.stepDetail.stepDurationContainer, LV_ALIGN_TOP_LEFT, -15, 70);
            lv_obj_set_size(gui.page.stepDetail.stepDurationContainer, 325, 40); 
            lv_obj_set_style_border_opa(gui.page.stepDetail.stepDurationContainer, LV_OPA_TRANSP, 0);
            //lv_obj_set_style_border_color(gui.page.stepDetail.stepDurationContainer, lv_color_hex(GREEN_DARK), 0);

                  gui.page.stepDetail.stepDurationLabel = lv_label_create(gui.page.stepDetail.stepDurationContainer);         
                  lv_label_set_text(gui.page.stepDetail.stepDurationLabel, stepDetailDuration_text); 
                  lv_obj_set_style_text_font(gui.page.stepDetail.stepDurationLabel, &lv_font_montserrat_22, 0);              
                  lv_obj_align(gui.page.stepDetail.stepDurationLabel, LV_ALIGN_LEFT_MID, -10, 0);

                  gui.page.stepDetail.stepDetailMinTextArea = lv_textarea_create(gui.page.stepDetail.stepDurationContainer);
                  lv_textarea_set_one_line(gui.page.stepDetail.stepDetailMinTextArea, true);
                  lv_textarea_set_placeholder_text(gui.page.stepDetail.stepDetailMinTextArea, stepDetailDurationMinPlaceHolder_text);
                  lv_obj_align(gui.page.stepDetail.stepDetailMinTextArea, LV_ALIGN_LEFT_MID, 100, 0);
                  lv_obj_set_width(gui.page.stepDetail.stepDetailMinTextArea, 60);
                  lv_obj_add_event_cb(gui.page.stepDetail.stepDetailMinTextArea, event_stepDetail, LV_EVENT_ALL, gui.page.stepDetail.stepDetailMinTextArea);
                  lv_obj_add_state(gui.page.stepDetail.stepDetailMinTextArea, LV_STATE_FOCUSED); 
                  lv_obj_set_style_bg_color(gui.page.stepDetail.stepDetailMinTextArea, lv_palette_darken(LV_PALETTE_GREY, 3), 0);
                  lv_obj_set_style_text_align(gui.page.stepDetail.stepDetailMinTextArea , LV_TEXT_ALIGN_CENTER, 0);
                  lv_obj_set_style_border_color(gui.page.stepDetail.stepDetailMinTextArea, lv_color_hex(WHITE), 0);


                  gui.page.stepDetail.stepDurationMinLabel = lv_label_create(gui.page.stepDetail.stepDurationContainer);         
                  lv_label_set_text(gui.page.stepDetail.stepDurationMinLabel, stepDetailDurationTimeSep_text); 
                  lv_obj_set_style_text_font(gui.page.stepDetail.stepDurationMinLabel, &lv_font_montserrat_22, 0);              
                  lv_obj_align(gui.page.stepDetail.stepDurationMinLabel, LV_ALIGN_LEFT_MID, 170, 0);

                  gui.page.stepDetail.stepDetailSecTextArea = lv_textarea_create(gui.page.stepDetail.stepDurationContainer);
                  lv_textarea_set_one_line(gui.page.stepDetail.stepDetailSecTextArea, true);
                  lv_textarea_set_placeholder_text(gui.page.stepDetail.stepDetailSecTextArea, stepDetailDurationSecPlaceHolder_text);
                  lv_obj_align(gui.page.stepDetail.stepDetailSecTextArea, LV_ALIGN_LEFT_MID, 187, 0);
                  lv_obj_set_width(gui.page.stepDetail.stepDetailSecTextArea, 60);
                  lv_obj_add_event_cb(gui.page.stepDetail.stepDetailSecTextArea, event_stepDetail, LV_EVENT_ALL, gui.page.stepDetail.stepDetailSecTextArea);
                  lv_obj_add_state(gui.page.stepDetail.stepDetailSecTextArea, LV_STATE_FOCUSED); 
                  lv_obj_set_style_bg_color(gui.page.stepDetail.stepDetailSecTextArea, lv_palette_darken(LV_PALETTE_GREY, 3), 0);
                  lv_obj_set_style_text_align(gui.page.stepDetail.stepDetailSecTextArea , LV_TEXT_ALIGN_CENTER, 0);
                  lv_obj_set_style_border_color(gui.page.stepDetail.stepDetailSecTextArea, lv_color_hex(WHITE), 0);


    
            gui.page.stepDetail.stepTypeContainer = lv_obj_create(gui.page.stepDetail.stepDetailContainer);
            lv_obj_remove_flag(gui.page.stepDetail.stepTypeContainer, LV_OBJ_FLAG_SCROLLABLE); 
            lv_obj_align(gui.page.stepDetail.stepTypeContainer, LV_ALIGN_TOP_LEFT, -15, 110);
            lv_obj_set_size(gui.page.stepDetail.stepTypeContainer, 325, 40); 
            lv_obj_set_style_border_opa(gui.page.stepDetail.stepTypeContainer, LV_OPA_TRANSP, 0);
            //lv_obj_set_style_border_color(gui.page.stepDetail.stepTypeContainer, lv_color_hex(GREEN_DARK), 0);


                  gui.page.stepDetail.stepTypeLabel = lv_label_create(gui.page.stepDetail.stepTypeContainer);         
                  lv_label_set_text(gui.page.stepDetail.stepTypeLabel, stepDetailType_text); 
                  lv_obj_set_style_text_font(gui.page.stepDetail.stepTypeLabel, &lv_font_montserrat_22, 0);              
                  lv_obj_align(gui.page.stepDetail.stepTypeLabel, LV_ALIGN_LEFT_MID, -10, 0);


                  lv_style_init(&gui.page.stepDetail.dropDownListStyle);
                  gui.page.stepDetail.stepTypeDropDownList = lv_dropdown_create(gui.page.stepDetail.stepTypeContainer);
                  lv_obj_set_style_border_opa(gui.page.stepDetail.stepTypeDropDownList, LV_OPA_TRANSP, 0);
                  lv_dropdown_set_options(gui.page.stepDetail.stepTypeDropDownList, stepTypeList);
                  lv_style_set_text_font(&gui.page.stepDetail.dropDownListStyle, &lv_font_montserrat_20);
                  lv_obj_add_style(gui.page.stepDetail.stepTypeDropDownList, &gui.page.stepDetail.dropDownListStyle, 0);
                  lv_obj_align(gui.page.stepDetail.stepTypeDropDownList, LV_ALIGN_LEFT_MID, 50, 2);
                  lv_obj_set_size(gui.page.stepDetail.stepTypeDropDownList, 165, 50); 
                  lv_obj_add_event_cb(gui.page.stepDetail.stepTypeDropDownList, event_stepDetail, LV_EVENT_VALUE_CHANGED, gui.page.stepDetail.stepTypeDropDownList);
                  lv_obj_set_style_bg_color(lv_dropdown_get_list(gui.page.stepDetail.stepTypeDropDownList), lv_palette_main(LV_PALETTE_GREEN), LV_PART_SELECTED | LV_STATE_CHECKED);


                  gui.page.stepDetail.stepTypeHelpIcon = lv_label_create(gui.page.stepDetail.stepTypeContainer);         
                  lv_label_set_text(gui.page.stepDetail.stepTypeHelpIcon, chemical_Icon); 
                  lv_obj_set_style_text_font(gui.page.stepDetail.stepTypeHelpIcon, &FilMachineFontIcons_20, 0);              
                  lv_obj_align(gui.page.stepDetail.stepTypeHelpIcon, LV_ALIGN_LEFT_MID, 210, 0);


            gui.page.stepDetail.stepSourceContainer = lv_obj_create(gui.page.stepDetail.stepDetailContainer);
            lv_obj_remove_flag(gui.page.stepDetail.stepSourceContainer, LV_OBJ_FLAG_SCROLLABLE); 
            lv_obj_align(gui.page.stepDetail.stepSourceContainer, LV_ALIGN_TOP_LEFT, -15, 150);
            lv_obj_set_size(gui.page.stepDetail.stepSourceContainer, 325, 40); 
            lv_obj_set_style_border_opa(gui.page.stepDetail.stepSourceContainer, LV_OPA_TRANSP, 0);
            //lv_obj_set_style_border_color(gui.page.stepDetail.stepSourceContainer, lv_color_hex(GREEN_DARK), 0);


                  gui.page.stepDetail.stepSourceLabel = lv_label_create(gui.page.stepDetail.stepSourceContainer);         
                  lv_label_set_text(gui.page.stepDetail.stepSourceLabel, stepDetailSource_text); 
                  lv_obj_set_style_text_font(gui.page.stepDetail.stepSourceLabel, &lv_font_montserrat_22, 0);              
                  lv_obj_align(gui.page.stepDetail.stepSourceLabel, LV_ALIGN_LEFT_MID, -10, 0);


                  lv_style_init(&gui.page.stepDetail.dropDownListStyle);
                  gui.page.stepDetail.stepSourceDropDownList = lv_dropdown_create(gui.page.stepDetail.stepSourceContainer);
                  lv_obj_set_style_border_opa(gui.page.stepDetail.stepSourceDropDownList, LV_OPA_TRANSP, 0);
                  lv_dropdown_set_options(gui.page.stepDetail.stepSourceDropDownList, stepSourceList);
                  lv_style_set_text_font(&gui.page.stepDetail.dropDownListStyle, &lv_font_montserrat_20);
                  lv_obj_add_style(gui.page.stepDetail.stepSourceDropDownList, &gui.page.stepDetail.dropDownListStyle, 0);
                  lv_obj_align(gui.page.stepDetail.stepSourceDropDownList, LV_ALIGN_LEFT_MID, 71, 2);
                  lv_obj_set_size(gui.page.stepDetail.stepSourceDropDownList, 83, 50); 
                  lv_obj_add_event_cb(gui.page.stepDetail.stepSourceDropDownList, event_stepDetail, LV_EVENT_VALUE_CHANGED, gui.page.stepDetail.stepSourceDropDownList);
                  lv_obj_set_style_bg_color(lv_dropdown_get_list(gui.page.stepDetail.stepSourceDropDownList), lv_palette_main(LV_PALETTE_GREEN), LV_PART_SELECTED | LV_STATE_CHECKED); 


                  gui.page.stepDetail.stepSourceTempLabel = lv_label_create(gui.page.stepDetail.stepSourceContainer);         
                  lv_label_set_text(gui.page.stepDetail.stepSourceTempLabel, stepDetailCurrentTemp_text); 
                  lv_obj_set_style_text_font(gui.page.stepDetail.stepSourceTempLabel, &lv_font_montserrat_18, 0);              
                  lv_obj_align(gui.page.stepDetail.stepSourceTempLabel, LV_ALIGN_LEFT_MID, 155, 0);

                  gui.page.stepDetail.stepSourceTempHelpIcon = lv_label_create(gui.page.stepDetail.stepSourceContainer);         
                  lv_label_set_text(gui.page.stepDetail.stepSourceTempHelpIcon, temp_icon); 
                  lv_obj_set_style_text_font(gui.page.stepDetail.stepSourceTempHelpIcon, &FilMachineFontIcons_20, 0);              
                  lv_obj_align(gui.page.stepDetail.stepSourceTempHelpIcon, LV_ALIGN_LEFT_MID, 227, 0);

                  //itoa(stepSourceTemp, tempBuffer, 10);
                  gui.page.stepDetail.stepSourceTempValue = lv_label_create(gui.page.stepDetail.stepSourceContainer);         
                  //lv_label_set_text(gui.page.stepDetail.stepSourceTempValue, convertFloatToTemp(stepSourceTemp)); //THIS NOT WORKS!
                  lv_label_set_text(gui.page.stepDetail.stepSourceTempValue, "20.4°C");  
                  lv_obj_set_style_text_font(gui.page.stepDetail.stepSourceTempValue, &lv_font_montserrat_18, 0);              
                  lv_obj_align(gui.page.stepDetail.stepSourceTempValue, LV_ALIGN_LEFT_MID, 245, 1);



            gui.page.stepDetail.stepDiscardAfterContainer = lv_obj_create(gui.page.stepDetail.stepDetailContainer);
            lv_obj_remove_flag(gui.page.stepDetail.stepDiscardAfterContainer, LV_OBJ_FLAG_SCROLLABLE); 
            lv_obj_align(gui.page.stepDetail.stepDiscardAfterContainer, LV_ALIGN_TOP_LEFT, -15, 190);
            lv_obj_set_size(gui.page.stepDetail.stepDiscardAfterContainer, 315, 40); 
            lv_obj_set_style_border_opa(gui.page.stepDetail.stepDiscardAfterContainer, LV_OPA_TRANSP, 0);
            //lv_obj_set_style_border_color(gui.page.stepDetail.stepDiscardAfterContainer, lv_color_hex(GREEN_DARK), 0);


                  gui.page.stepDetail.stepDiscsardAfterLabel = lv_label_create(gui.page.stepDetail.stepDiscardAfterContainer);         
                  lv_label_set_text(gui.page.stepDetail.stepDiscsardAfterLabel, stepDetailDiscardAfter_text); 
                  lv_obj_set_style_text_font(gui.page.stepDetail.stepDiscsardAfterLabel, &lv_font_montserrat_22, 0);              
                  lv_obj_align(gui.page.stepDetail.stepDiscsardAfterLabel, LV_ALIGN_LEFT_MID, -10, 0);


                  gui.page.stepDetail.stepDiscsardAfterSwitch = lv_switch_create(gui.page.stepDetail.stepDiscardAfterContainer);
                  lv_obj_add_event_cb(gui.page.stepDetail.stepDiscsardAfterSwitch, event_stepDetail, LV_EVENT_VALUE_CHANGED, gui.page.stepDetail.stepDiscsardAfterSwitch);
                  lv_obj_align(gui.page.stepDetail.stepDiscsardAfterSwitch, LV_ALIGN_LEFT_MID, 140, 0);
                  lv_obj_set_style_bg_color(gui.page.stepDetail.stepDiscsardAfterSwitch, lv_palette_darken(LV_PALETTE_GREY, 3), LV_STATE_DEFAULT);
                  lv_obj_set_style_bg_color(gui.page.stepDetail.stepDiscsardAfterSwitch,  lv_palette_main(LV_PALETTE_GREEN), LV_PART_KNOB | LV_STATE_DEFAULT);
                  lv_obj_set_style_bg_color(gui.page.stepDetail.stepDiscsardAfterSwitch, lv_color_hex(GREEN_DARK) , LV_PART_INDICATOR | LV_STATE_CHECKED);



      gui.page.stepDetail.stepSaveButton = lv_button_create(gui.page.stepDetail.stepDetailContainer);
      lv_obj_set_size(gui.page.stepDetail.stepSaveButton, BUTTON_PROCESS_WIDTH, BUTTON_PROCESS_HEIGHT);
      lv_obj_align(gui.page.stepDetail.stepSaveButton, LV_ALIGN_BOTTOM_LEFT, 10 , 10);
      lv_obj_add_event_cb(gui.page.stepDetail.stepSaveButton, event_stepDetail, LV_EVENT_CLICKED, gui.element.filterPopup.mBoxResetFilterButton);
      lv_obj_add_event_cb(gui.page.stepDetail.stepSaveButton, event_stepDetail, LV_EVENT_DELETE, NULL);
      lv_obj_set_style_bg_color(gui.page.stepDetail.stepSaveButton, lv_color_hex(GREEN_DARK), LV_PART_MAIN);

          gui.page.stepDetail.stepSaveLabel = lv_label_create(gui.page.stepDetail.stepSaveButton);
          lv_label_set_text(gui.page.stepDetail.stepSaveLabel, stepDetailSave_text);
          lv_obj_set_style_text_font(gui.page.stepDetail.stepSaveLabel, &lv_font_montserrat_22, 0);
          lv_obj_align(gui.page.stepDetail.stepSaveLabel, LV_ALIGN_CENTER, 0, 0);


      gui.page.stepDetail.stepCancelButton = lv_button_create(gui.page.stepDetail.stepDetailContainer);
      lv_obj_set_size(gui.page.stepDetail.stepCancelButton, BUTTON_PROCESS_WIDTH, BUTTON_PROCESS_HEIGHT);
      lv_obj_align(gui.page.stepDetail.stepCancelButton, LV_ALIGN_BOTTOM_RIGHT, - 10 , 10);
      lv_obj_add_event_cb(gui.page.stepDetail.stepCancelButton, event_stepDetail, LV_EVENT_CLICKED, gui.page.stepDetail.stepCancelButton);
      lv_obj_add_event_cb(gui.page.stepDetail.stepCancelButton, event_stepDetail, LV_EVENT_DELETE, NULL);
      lv_obj_set_style_bg_color(gui.page.stepDetail.stepCancelButton, lv_color_hex(GREEN_DARK), LV_PART_MAIN);

            gui.page.stepDetail.stepCancelLabel = lv_label_create(gui.page.stepDetail.stepCancelButton);
            lv_label_set_text(gui.page.stepDetail.stepCancelLabel, stepDetailCancel_text);
            lv_obj_set_style_text_font(gui.page.stepDetail.stepCancelLabel, &lv_font_montserrat_22, 0);
            lv_obj_align(gui.page.stepDetail.stepCancelLabel, LV_ALIGN_CENTER, 0, 0);

if(keyboardStep == NULL){
  LV_LOG_USER("create_keyboard");
    //create_keyboard(keyboardStep, gui.page.stepDetail.stepDetailParent);
    }
}

