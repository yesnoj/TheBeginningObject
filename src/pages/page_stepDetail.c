#include "misc/lv_event.h"
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

stepNode *newStep;

void event_stepDetail(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t * obj = (lv_obj_t *)lv_event_get_target(e);
  lv_obj_t * objCont = (lv_obj_t *)lv_obj_get_parent(obj);
  lv_obj_t * mboxCont = (lv_obj_t *)lv_obj_get_parent(objCont);
  lv_obj_t * mboxParent = (lv_obj_t *)lv_obj_get_parent(mboxCont);
  processNode * data = (processNode *)lv_event_get_user_data(e);
 

 if(code == LV_EVENT_CLICKED){
    if(obj == newStep->step.stepDetails->stepSaveButton){
      LV_LOG_USER("Pressed stepSaveButton");

                // for testing
          static char 			  	name[80];	// Test Code

                if(addStepElement(newStep, data) != NULL){
                    LV_LOG_USER("Step %p element created!Now process %p has n: %d steps",newStep,data, ((processNode *)data)->process.processDetails->stepElementsList.size);
                    stepElementCreate(newStep , data , -1);
                    //lv_obj_send_event(fakeObject, LV_EVENT_REFRESH, NULL);
                }
                else{
                    LV_LOG_USER("Step element creation failed, maximum entries reached" );
                }

      data->process.processDetails->somethingChanged = 1;
      lv_obj_send_event(data->process.processDetails->processSaveButton, LV_EVENT_REFRESH, NULL);
      calcolateTotalTime(data);
      updateStepElement(data, newStep);
      lv_msgbox_close(mboxCont);
      lv_obj_delete(mboxCont);

    }
    if(obj == newStep->step.stepDetails->stepCancelButton){
      LV_LOG_USER("Pressed newStep->step.stepDetails->stepCancelButton");
      lv_msgbox_close(mboxCont);
      lv_obj_delete(mboxCont);
    }
 }


 if(obj == newStep->step.stepDetails->stepDetailNamelTextArea){
     LV_LOG_USER("Set Name");
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
    if(obj == newStep->step.stepDetails->stepTypeDropDownList){
      if(lv_dropdown_get_selected(newStep->step.stepDetails->stepTypeDropDownList) == 0){
          stepType = 0;
          LV_LOG_USER("Selected newStep->step.stepDetails->stepTypeDropDownList chemical_icon value:%d",stepType);
          lv_label_set_text(newStep->step.stepDetails->stepTypeHelpIcon, chemical_icon);
      }
      if(lv_dropdown_get_selected(newStep->step.stepDetails->stepTypeDropDownList) == 1){
          stepType = 1;
          LV_LOG_USER("Selected newStep->step.stepDetails->stepTypeDropDownList rinse_icon value:%d",stepType);
          lv_label_set_text(newStep->step.stepDetails->stepTypeHelpIcon, rinse_icon);
      }
      if(lv_dropdown_get_selected(newStep->step.stepDetails->stepTypeDropDownList) == 2){
          stepType = 2;
          LV_LOG_USER("Selected newStep->step.stepDetails->stepTypeDropDownList multiRinse_icon value:%d",stepType);
          lv_label_set_text(newStep->step.stepDetails->stepTypeHelpIcon, multiRinse_icon);
      }
      newStep->step.stepDetails->type = stepType;
    }

    if(obj == newStep->step.stepDetails->stepDiscardAfterSwitch){
      discardAfter = lv_obj_has_state(obj, LV_STATE_CHECKED);
      newStep->step.stepDetails->discardAfterProc = discardAfter;
      LV_LOG_USER("Discard After : %s", discardAfter ? "On" : "Off");
    }

    if(obj == newStep->step.stepDetails->stepSourceDropDownList){
         stepSource = lv_dropdown_get_selected(newStep->step.stepDetails->stepSourceDropDownList);
         newStep->step.stepDetails->source = stepSource;

         LV_LOG_USER("Selected newStep->step.stepDetails->stepSourceDropDownList %d",stepSource);
    }
  }

    if(code == LV_EVENT_FOCUSED) {
      tempStepNode = newStep;
        if(data == newStep->step.stepDetails->stepDetailMinTextArea){
            LV_LOG_USER("Set minutes");
            rollerPopupCreate(gui.element.rollerPopup.minutesOptions, setMinutesPopupTitle_text, newStep->step.stepDetails->stepDetailMinTextArea, 0);
        }
        if(data == newStep->step.stepDetails->stepDetailSecTextArea){
            LV_LOG_USER("Set seconds");
            rollerPopupCreate(gui.element.rollerPopup.secondsOptions, setSecondsPopupTitle_text, newStep->step.stepDetails->stepDetailSecTextArea, 0);
        }
    }


  if(code == LV_EVENT_DELETE) {
       LV_LOG_USER("Delete Styles");
        //list of all styles to be reset, so clean the memory.
        lv_style_reset(&newStep->step.stepDetails->style_mBoxStepPopupTitleLine);
        //lv_style_reset(&newStep->step.stepDetails->dropDownListStyle);
        if(keyboardStep == NULL){
          LV_LOG_USER("create_keyboard");
           }
  }
}

/*********************
*    STEP DETAIL
*********************/

void stepDetail(processNode * referenceNode, stepNode * currentNode)
{   
/*********************
  *    PAGE ELEMENTS
*********************/


      stepNode* existingStep = (stepNode*)isNodeInList((void*)&(referenceNode->process.processDetails->stepElementsList), currentNode, STEP_NODE);
      if(existingStep != NULL) {
          LV_LOG_USER("Step already exist with address 0x%p", currentNode);
          newStep = existingStep; // Usa il nodo già presente anziché allocarne uno nuovo
      } else {
          newStep = (stepNode*)allocateAndInitializeNode(STEP_NODE);
          LV_LOG_USER("New stepNode created at address 0x%p", newStep);
      }
      
      tempStepNode = newStep;

      LV_LOG_USER("Step detail creation");

     // if(newStep->step.stepDetails->dropDownListStyle->values_and_props == NULL ) {		/* Only initialise the style once! */
         // lv_style_init(&newStep->step.stepDetails->dropDownListStyle);
         // lv_style_set_text_font(&newStep->step.stepDetails->dropDownListStyle, &lv_font_montserrat_20);
       
      //  }


      newStep->step.stepDetails->stepDetailParent = lv_obj_class_create_obj(&lv_msgbox_backdrop_class, lv_layer_top());
      lv_obj_class_init_obj(newStep->step.stepDetails->stepDetailParent);
      lv_obj_remove_flag(newStep->step.stepDetails->stepDetailParent, LV_OBJ_FLAG_IGNORE_LAYOUT);
      lv_obj_set_size(newStep->step.stepDetails->stepDetailParent, LV_PCT(100), LV_PCT(100));


            newStep->step.stepDetails->stepDetailContainer = lv_obj_create(newStep->step.stepDetails->stepDetailParent);
            lv_obj_align(newStep->step.stepDetails->stepDetailContainer, LV_ALIGN_CENTER, 0, 0);
            lv_obj_set_size(newStep->step.stepDetails->stepDetailContainer, 350, 300); 
            lv_obj_remove_flag(newStep->step.stepDetails->stepDetailContainer, LV_OBJ_FLAG_SCROLLABLE); 

                    newStep->step.stepDetails->stepDetailLabel = lv_label_create(newStep->step.stepDetails->stepDetailContainer);         
                    lv_label_set_text(newStep->step.stepDetails->stepDetailLabel, stepDetailTitle_text); 
                    lv_obj_set_style_text_font(newStep->step.stepDetails->stepDetailLabel, &lv_font_montserrat_22, 0);              
                    lv_obj_align(newStep->step.stepDetails->stepDetailLabel, LV_ALIGN_TOP_MID, 0, - 10);


                          lv_style_init(&newStep->step.stepDetails->style_mBoxStepPopupTitleLine);
                          lv_style_set_line_width(&newStep->step.stepDetails->style_mBoxStepPopupTitleLine, 2);
                          lv_style_set_line_color(&newStep->step.stepDetails->style_mBoxStepPopupTitleLine, lv_palette_main(LV_PALETTE_GREEN));
                          lv_style_set_line_rounded(&newStep->step.stepDetails->style_mBoxStepPopupTitleLine, true);


                    newStep->step.stepDetails->mBoxStepPopupTitleLine = lv_line_create(newStep->step.stepDetails->stepDetailContainer);
                    newStep->step.stepDetails->titleLinePoints[1].x = 310;
                    lv_line_set_points(newStep->step.stepDetails->mBoxStepPopupTitleLine, newStep->step.stepDetails->titleLinePoints, 2);
                    lv_obj_add_style(newStep->step.stepDetails->mBoxStepPopupTitleLine, &newStep->step.stepDetails->style_mBoxStepPopupTitleLine, 0);
                    lv_obj_align(newStep->step.stepDetails->mBoxStepPopupTitleLine, LV_ALIGN_TOP_MID, 0, 23);


            newStep->step.stepDetails->stepDetailNameContainer = lv_obj_create(newStep->step.stepDetails->stepDetailContainer);
            lv_obj_remove_flag(newStep->step.stepDetails->stepDetailNameContainer, LV_OBJ_FLAG_SCROLLABLE); 
            lv_obj_align(newStep->step.stepDetails->stepDetailNameContainer, LV_ALIGN_TOP_LEFT, -15, 30);
            lv_obj_set_size(newStep->step.stepDetails->stepDetailNameContainer, 325, 40); 
            lv_obj_set_style_border_opa(newStep->step.stepDetails->stepDetailNameContainer, LV_OPA_TRANSP, 0);
            //lv_obj_set_style_border_color(newStep->step.stepDetails->stepDetailNameContainer, lv_color_hex(GREEN_DARK), 0);

                  newStep->step.stepDetails->stepDetailNamelLabel = lv_label_create(newStep->step.stepDetails->stepDetailNameContainer);         
                  lv_label_set_text(newStep->step.stepDetails->stepDetailNamelLabel, stepDetailName_text); 
                  lv_obj_set_style_text_font(newStep->step.stepDetails->stepDetailNamelLabel, &lv_font_montserrat_22, 0);              
                  lv_obj_align(newStep->step.stepDetails->stepDetailNamelLabel, LV_ALIGN_LEFT_MID, -10, 0);
              
                  newStep->step.stepDetails->stepDetailNamelTextArea = lv_textarea_create(newStep->step.stepDetails->stepDetailNameContainer);
                  lv_textarea_set_one_line(newStep->step.stepDetails->stepDetailNamelTextArea, true);
                  lv_textarea_set_placeholder_text(newStep->step.stepDetails->stepDetailNamelTextArea, stepDetailPlaceHolder_text);
                  lv_obj_align(newStep->step.stepDetails->stepDetailNamelTextArea, LV_ALIGN_LEFT_MID, 70, 0);
                  lv_obj_set_width(newStep->step.stepDetails->stepDetailNamelTextArea, 210);
                  lv_obj_add_event_cb(newStep->step.stepDetails->stepDetailNamelTextArea, event_stepDetail, LV_EVENT_FOCUSED, newStep->step.stepDetails->stepDetailNamelTextArea);
                  lv_obj_add_event_cb(newStep->step.stepDetails->stepDetailNamelTextArea, event_stepDetail, LV_EVENT_DEFOCUSED, newStep->step.stepDetails->stepDetailNamelTextArea);
                  lv_obj_add_state(newStep->step.stepDetails->stepDetailNamelTextArea, LV_STATE_FOCUSED); 
                  lv_obj_set_style_bg_color(newStep->step.stepDetails->stepDetailNamelTextArea, lv_palette_darken(LV_PALETTE_GREY, 3), 0);
                  lv_obj_set_style_border_color(newStep->step.stepDetails->stepDetailNamelTextArea, lv_color_hex(WHITE), 0);
                  newStep->step.stepDetails->stepNameString = "TBD";
                  lv_textarea_set_text(newStep->step.stepDetails->stepDetailNamelTextArea, newStep->step.stepDetails->stepNameString);


            newStep->step.stepDetails->stepDurationContainer = lv_obj_create(newStep->step.stepDetails->stepDetailContainer);
            lv_obj_remove_flag(newStep->step.stepDetails->stepDurationContainer, LV_OBJ_FLAG_SCROLLABLE); 
            lv_obj_align(newStep->step.stepDetails->stepDurationContainer, LV_ALIGN_TOP_LEFT, -15, 70);
            lv_obj_set_size(newStep->step.stepDetails->stepDurationContainer, 325, 40); 
            lv_obj_set_style_border_opa(newStep->step.stepDetails->stepDurationContainer, LV_OPA_TRANSP, 0);
            //lv_obj_set_style_border_color(newStep->step.stepDetails->stepDurationContainer, lv_color_hex(GREEN_DARK), 0);

                  newStep->step.stepDetails->stepDurationLabel = lv_label_create(newStep->step.stepDetails->stepDurationContainer);         
                  lv_label_set_text(newStep->step.stepDetails->stepDurationLabel, stepDetailDuration_text); 
                  lv_obj_set_style_text_font(newStep->step.stepDetails->stepDurationLabel, &lv_font_montserrat_22, 0);              
                  lv_obj_align(newStep->step.stepDetails->stepDurationLabel, LV_ALIGN_LEFT_MID, -10, 0);

                  newStep->step.stepDetails->stepDetailMinTextArea = lv_textarea_create(newStep->step.stepDetails->stepDurationContainer);
                  lv_textarea_set_one_line(newStep->step.stepDetails->stepDetailMinTextArea, true);
                  lv_textarea_set_placeholder_text(newStep->step.stepDetails->stepDetailMinTextArea, stepDetailDurationMinPlaceHolder_text);
                  lv_obj_align(newStep->step.stepDetails->stepDetailMinTextArea, LV_ALIGN_LEFT_MID, 100, 0);
                  lv_obj_set_width(newStep->step.stepDetails->stepDetailMinTextArea, 60);
                 
                  lv_obj_add_event_cb(newStep->step.stepDetails->stepDetailMinTextArea, event_stepDetail, LV_EVENT_ALL, newStep->step.stepDetails->stepDetailMinTextArea);
                  lv_obj_add_state(newStep->step.stepDetails->stepDetailMinTextArea, LV_STATE_FOCUSED); 
                  lv_obj_set_style_bg_color(newStep->step.stepDetails->stepDetailMinTextArea, lv_palette_darken(LV_PALETTE_GREY, 3), 0);
                  lv_obj_set_style_text_align(newStep->step.stepDetails->stepDetailMinTextArea , LV_TEXT_ALIGN_CENTER, 0);
                  lv_obj_set_style_border_color(newStep->step.stepDetails->stepDetailMinTextArea, lv_color_hex(WHITE), 0);
                  sprintf(formatted_string, "%d", newStep->step.stepDetails->timeMins);
                  lv_textarea_set_text(newStep->step.stepDetails->stepDetailMinTextArea, formatted_string); 


                  newStep->step.stepDetails->stepDetailSecTextArea = lv_textarea_create(newStep->step.stepDetails->stepDurationContainer);
                  lv_textarea_set_one_line(newStep->step.stepDetails->stepDetailSecTextArea, true);
                  lv_textarea_set_placeholder_text(newStep->step.stepDetails->stepDetailSecTextArea, stepDetailDurationSecPlaceHolder_text);
                  lv_obj_align(newStep->step.stepDetails->stepDetailSecTextArea, LV_ALIGN_LEFT_MID, 187, 0);
                  lv_obj_set_width(newStep->step.stepDetails->stepDetailSecTextArea, 60);

                  lv_obj_add_event_cb(newStep->step.stepDetails->stepDetailSecTextArea, event_stepDetail, LV_EVENT_ALL, newStep->step.stepDetails->stepDetailSecTextArea);
                  lv_obj_add_state(newStep->step.stepDetails->stepDetailSecTextArea, LV_STATE_FOCUSED); 
                  lv_obj_set_style_bg_color(newStep->step.stepDetails->stepDetailSecTextArea, lv_palette_darken(LV_PALETTE_GREY, 3), 0);
                  lv_obj_set_style_text_align(newStep->step.stepDetails->stepDetailSecTextArea , LV_TEXT_ALIGN_CENTER, 0);
                  lv_obj_set_style_border_color(newStep->step.stepDetails->stepDetailSecTextArea, lv_color_hex(WHITE), 0);
                  sprintf(formatted_string, "%d", newStep->step.stepDetails->timeSecs);
                  lv_textarea_set_text(newStep->step.stepDetails->stepDetailSecTextArea, formatted_string); 

    

            newStep->step.stepDetails->stepTypeContainer = lv_obj_create(newStep->step.stepDetails->stepDetailContainer);
            lv_obj_remove_flag(newStep->step.stepDetails->stepTypeContainer, LV_OBJ_FLAG_SCROLLABLE); 
            lv_obj_align(newStep->step.stepDetails->stepTypeContainer, LV_ALIGN_TOP_LEFT, -15, 110);
            lv_obj_set_size(newStep->step.stepDetails->stepTypeContainer, 325, 40); 
            lv_obj_set_style_border_opa(newStep->step.stepDetails->stepTypeContainer, LV_OPA_TRANSP, 0);
            //lv_obj_set_style_border_color(newStep->step.stepDetails->stepTypeContainer, lv_color_hex(GREEN_DARK), 0);


                  newStep->step.stepDetails->stepTypeLabel = lv_label_create(newStep->step.stepDetails->stepTypeContainer);         
                  lv_label_set_text(newStep->step.stepDetails->stepTypeLabel, stepDetailType_text); 
                  lv_obj_set_style_text_font(newStep->step.stepDetails->stepTypeLabel, &lv_font_montserrat_22, 0);              
                  lv_obj_align(newStep->step.stepDetails->stepTypeLabel, LV_ALIGN_LEFT_MID, -10, 0);


                  //lv_style_init(&newStep->step.stepDetails->dropDownListStyle);
                  newStep->step.stepDetails->stepTypeDropDownList = lv_dropdown_create(newStep->step.stepDetails->stepTypeContainer);
                  lv_obj_set_style_border_opa(newStep->step.stepDetails->stepTypeDropDownList, LV_OPA_TRANSP, 0);
                  lv_dropdown_set_options(newStep->step.stepDetails->stepTypeDropDownList, stepTypeList);
                  //lv_style_set_text_font(&newStep->step.stepDetails->dropDownListStyle, &lv_font_montserrat_20);
                  //lv_obj_add_style(newStep->step.stepDetails->stepTypeDropDownList, &newStep->step.stepDetails->dropDownListStyle, 0);
                  lv_obj_align(newStep->step.stepDetails->stepTypeDropDownList, LV_ALIGN_LEFT_MID, 50, 2);
                  lv_obj_set_size(newStep->step.stepDetails->stepTypeDropDownList, 165, 50); 
                  lv_obj_add_event_cb(newStep->step.stepDetails->stepTypeDropDownList, event_stepDetail, LV_EVENT_VALUE_CHANGED, newStep->step.stepDetails->stepTypeDropDownList);
                  lv_obj_set_style_bg_color(lv_dropdown_get_list(newStep->step.stepDetails->stepTypeDropDownList), lv_palette_main(LV_PALETTE_GREEN), LV_PART_SELECTED | LV_STATE_CHECKED);
                  lv_dropdown_set_selected(newStep->step.stepDetails->stepTypeDropDownList, newStep->step.stepDetails->type);

                  newStep->step.stepDetails->stepTypeHelpIcon = lv_label_create(newStep->step.stepDetails->stepTypeContainer);         
                  lv_label_set_text(newStep->step.stepDetails->stepTypeHelpIcon, chemical_icon); 
                  lv_obj_set_style_text_font(newStep->step.stepDetails->stepTypeHelpIcon, &FilMachineFontIcons_20, 0);              
                  lv_obj_align(newStep->step.stepDetails->stepTypeHelpIcon, LV_ALIGN_LEFT_MID, 210, 0);

   

            newStep->step.stepDetails->stepSourceContainer = lv_obj_create(newStep->step.stepDetails->stepDetailContainer);
            lv_obj_remove_flag(newStep->step.stepDetails->stepSourceContainer, LV_OBJ_FLAG_SCROLLABLE); 
            lv_obj_align(newStep->step.stepDetails->stepSourceContainer, LV_ALIGN_TOP_LEFT, -15, 150);
            lv_obj_set_size(newStep->step.stepDetails->stepSourceContainer, 340, 40); 
            lv_obj_set_style_border_opa(newStep->step.stepDetails->stepSourceContainer, LV_OPA_TRANSP, 0);
            //lv_obj_set_style_border_color(newStep->step.stepDetails->stepSourceContainer, lv_color_hex(GREEN_DARK), 0);


                  newStep->step.stepDetails->stepSourceLabel = lv_label_create(newStep->step.stepDetails->stepSourceContainer);         
                  lv_label_set_text(newStep->step.stepDetails->stepSourceLabel, stepDetailSource_text); 
                  lv_obj_set_style_text_font(newStep->step.stepDetails->stepSourceLabel, &lv_font_montserrat_22, 0);              
                  lv_obj_align(newStep->step.stepDetails->stepSourceLabel, LV_ALIGN_LEFT_MID, -10, 0);


                  //lv_style_init(&newStep->step.stepDetails->dropDownListStyle);
                  newStep->step.stepDetails->stepSourceDropDownList = lv_dropdown_create(newStep->step.stepDetails->stepSourceContainer);
                  lv_obj_set_style_border_opa(newStep->step.stepDetails->stepSourceDropDownList, LV_OPA_TRANSP, 0);
                  lv_dropdown_set_options(newStep->step.stepDetails->stepSourceDropDownList, stepSourceList);
                  //lv_style_set_text_font(&newStep->step.stepDetails->dropDownListStyle, &lv_font_montserrat_20);
                  lv_obj_align(newStep->step.stepDetails->stepSourceDropDownList, LV_ALIGN_LEFT_MID, 71, 2);
                  lv_obj_set_size(newStep->step.stepDetails->stepSourceDropDownList, 83, 50); 
                  lv_obj_add_event_cb(newStep->step.stepDetails->stepSourceDropDownList, event_stepDetail, LV_EVENT_VALUE_CHANGED, newStep->step.stepDetails->stepSourceDropDownList);
                  lv_obj_set_style_bg_color(lv_dropdown_get_list(newStep->step.stepDetails->stepSourceDropDownList), lv_palette_main(LV_PALETTE_GREEN), LV_PART_SELECTED | LV_STATE_CHECKED); 
                  lv_dropdown_set_selected(newStep->step.stepDetails->stepSourceDropDownList, newStep->step.stepDetails->source);

                  newStep->step.stepDetails->stepSourceTempLabel = lv_label_create(newStep->step.stepDetails->stepSourceContainer);         
                  lv_label_set_text(newStep->step.stepDetails->stepSourceTempLabel, stepDetailCurrentTemp_text); 
                  lv_obj_set_style_text_font(newStep->step.stepDetails->stepSourceTempLabel, &lv_font_montserrat_22, 0);              
                  lv_obj_align(newStep->step.stepDetails->stepSourceTempLabel, LV_ALIGN_LEFT_MID, 165, 0);

                  newStep->step.stepDetails->stepSourceTempHelpIcon = lv_label_create(newStep->step.stepDetails->stepSourceContainer);         
                  lv_label_set_text(newStep->step.stepDetails->stepSourceTempHelpIcon, temp_icon); 
                  lv_obj_set_style_text_font(newStep->step.stepDetails->stepSourceTempHelpIcon, &FilMachineFontIcons_20, 0);              
                  lv_obj_align(newStep->step.stepDetails->stepSourceTempHelpIcon, LV_ALIGN_LEFT_MID, 227, 0);

                  //itoa(stepSourceTemp, tempBuffer, 10);
                  newStep->step.stepDetails->stepSourceTempValue = lv_label_create(newStep->step.stepDetails->stepSourceContainer);         
                  //lv_label_set_text(newStep->step.stepDetails->stepSourceTempValue, convertFloatToTemp(stepSourceTemp)); //THIS NOT WORKS!
                  lv_label_set_text(newStep->step.stepDetails->stepSourceTempValue, "20.4°C");  
                  lv_obj_set_style_text_font(newStep->step.stepDetails->stepSourceTempValue, &lv_font_montserrat_22, 0);              
                  lv_obj_align(newStep->step.stepDetails->stepSourceTempValue, LV_ALIGN_LEFT_MID, 245, 1);


            newStep->step.stepDetails->stepDiscardAfterContainer = lv_obj_create(newStep->step.stepDetails->stepDetailContainer);
            lv_obj_remove_flag(newStep->step.stepDetails->stepDiscardAfterContainer, LV_OBJ_FLAG_SCROLLABLE); 
            lv_obj_align(newStep->step.stepDetails->stepDiscardAfterContainer, LV_ALIGN_TOP_LEFT, -15, 190);
            lv_obj_set_size(newStep->step.stepDetails->stepDiscardAfterContainer, 315, 40); 
            lv_obj_set_style_border_opa(newStep->step.stepDetails->stepDiscardAfterContainer, LV_OPA_TRANSP, 0);
            //lv_obj_set_style_border_color(newStep->step.stepDetails->stepDiscardAfterContainer, lv_color_hex(GREEN_DARK), 0);


                  newStep->step.stepDetails->stepDiscardAfterLabel = lv_label_create(newStep->step.stepDetails->stepDiscardAfterContainer);         
                  lv_label_set_text(newStep->step.stepDetails->stepDiscardAfterLabel, stepDetailDiscardAfter_text); 
                  lv_obj_set_style_text_font(newStep->step.stepDetails->stepDiscardAfterLabel, &lv_font_montserrat_22, 0);              
                  lv_obj_align(newStep->step.stepDetails->stepDiscardAfterLabel, LV_ALIGN_LEFT_MID, -10, 0);


                  newStep->step.stepDetails->stepDiscardAfterSwitch = lv_switch_create(newStep->step.stepDetails->stepDiscardAfterContainer);
                  lv_obj_add_event_cb(newStep->step.stepDetails->stepDiscardAfterSwitch, event_stepDetail, LV_EVENT_VALUE_CHANGED, newStep->step.stepDetails->stepDiscardAfterSwitch);
                  lv_obj_align(newStep->step.stepDetails->stepDiscardAfterSwitch, LV_ALIGN_LEFT_MID, 140, 0);
                  lv_obj_set_style_bg_color(newStep->step.stepDetails->stepDiscardAfterSwitch, lv_palette_darken(LV_PALETTE_GREY, 3), LV_STATE_DEFAULT);
                  lv_obj_set_style_bg_color(newStep->step.stepDetails->stepDiscardAfterSwitch,  lv_palette_main(LV_PALETTE_GREEN), LV_PART_KNOB | LV_STATE_DEFAULT);
                  lv_obj_set_style_bg_color(newStep->step.stepDetails->stepDiscardAfterSwitch, lv_color_hex(GREEN_DARK) , LV_PART_INDICATOR | LV_STATE_CHECKED);
                  lv_obj_add_state(newStep->step.stepDetails->stepDiscardAfterSwitch, newStep->step.stepDetails->discardAfterProc);


      newStep->step.stepDetails->stepSaveButton = lv_button_create(newStep->step.stepDetails->stepDetailContainer);
      lv_obj_set_size(newStep->step.stepDetails->stepSaveButton, BUTTON_PROCESS_WIDTH, BUTTON_PROCESS_HEIGHT);
      lv_obj_align(newStep->step.stepDetails->stepSaveButton, LV_ALIGN_BOTTOM_LEFT, 10 , 10);
      lv_obj_add_event_cb(newStep->step.stepDetails->stepSaveButton, event_stepDetail, LV_EVENT_CLICKED, referenceNode);
      lv_obj_add_event_cb(newStep->step.stepDetails->stepSaveButton, event_stepDetail, LV_EVENT_DELETE, NULL);
      lv_obj_set_style_bg_color(newStep->step.stepDetails->stepSaveButton, lv_color_hex(GREEN_DARK), LV_PART_MAIN);

          newStep->step.stepDetails->stepSaveLabel = lv_label_create(newStep->step.stepDetails->stepSaveButton);
          lv_label_set_text(newStep->step.stepDetails->stepSaveLabel, stepDetailSave_text);
          lv_obj_set_style_text_font(newStep->step.stepDetails->stepSaveLabel, &lv_font_montserrat_22, 0);
          lv_obj_align(newStep->step.stepDetails->stepSaveLabel, LV_ALIGN_CENTER, 0, 0);


      newStep->step.stepDetails->stepCancelButton = lv_button_create(newStep->step.stepDetails->stepDetailContainer);
      lv_obj_set_size(newStep->step.stepDetails->stepCancelButton, BUTTON_PROCESS_WIDTH, BUTTON_PROCESS_HEIGHT);
      lv_obj_align(newStep->step.stepDetails->stepCancelButton, LV_ALIGN_BOTTOM_RIGHT, - 10 , 10);
      lv_obj_add_event_cb(newStep->step.stepDetails->stepCancelButton, event_stepDetail, LV_EVENT_CLICKED, newStep->step.stepDetails->stepCancelButton);
      lv_obj_add_event_cb(newStep->step.stepDetails->stepCancelButton, event_stepDetail, LV_EVENT_DELETE, NULL);
      lv_obj_set_style_bg_color(newStep->step.stepDetails->stepCancelButton, lv_color_hex(GREEN_DARK), LV_PART_MAIN);

            newStep->step.stepDetails->stepCancelLabel = lv_label_create(newStep->step.stepDetails->stepCancelButton);
            lv_label_set_text(newStep->step.stepDetails->stepCancelLabel, stepDetailCancel_text);
            lv_obj_set_style_text_font(newStep->step.stepDetails->stepCancelLabel, &lv_font_montserrat_22, 0);
            lv_obj_align(newStep->step.stepDetails->stepCancelLabel, LV_ALIGN_CENTER, 0, 0);

if(keyboardStep == NULL){
  LV_LOG_USER("create_keyboard");
    //create_keyboard(keyboardStep, newStep->step.stepDetails->stepDetailParent);
    }
  
}

