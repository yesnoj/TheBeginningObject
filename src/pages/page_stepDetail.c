
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
  processNode * data = (processNode *)lv_event_get_user_data(e);
 

 if(code == LV_EVENT_CLICKED){
    if(obj == gui.tempStepNode->step.stepDetails->stepSaveButton){
      LV_LOG_USER("Pressed stepSaveButton");

      strcpy(gui.tempStepNode->step.stepDetails->stepNameString, lv_textarea_get_text(gui.tempStepNode->step.stepDetails->stepDetailNamelTextArea));
      if(addStepElement(gui.tempStepNode, data) != NULL){
          LV_LOG_USER("Step %p element created!Now process %p has n: %d steps",gui.tempStepNode,data, ((processNode *)data)->process.processDetails->stepElementsList.size);
          stepElementCreate(gui.tempStepNode , data , -1);
          
      }
      else{
          LV_LOG_USER("Step element creation failed, maximum entries reached" );
      }

      data->process.processDetails->somethingChanged = 1;
      lv_obj_send_event(data->process.processDetails->processSaveButton, LV_EVENT_REFRESH, NULL);
      
      if(gui.tempStepNode->step.swipedLeft == 1 && gui.tempStepNode->step.swipedRight == 0){
        uint32_t  x = lv_obj_get_x_aligned(gui.tempStepNode->step.stepElement) + 50;
        uint32_t  y = lv_obj_get_y_aligned(gui.tempStepNode->step.stepElement);
        lv_obj_set_pos(gui.tempStepNode->step.stepElement, x, y);
        gui.tempStepNode->step.swipedLeft = 0;
        gui.tempStepNode->step.swipedRight = 0;
        lv_obj_add_flag(gui.tempStepNode->step.deleteButton, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(gui.tempStepNode->step.editButton, LV_OBJ_FLAG_HIDDEN);
      }
      calcolateTotalTime(data);
      updateStepElement(data, gui.tempStepNode);
      lv_msgbox_close(mboxCont);
//      lv_obj_delete(mboxCont);
      
    }
    if(obj == gui.tempStepNode->step.stepDetails->stepCancelButton){
      LV_LOG_USER("Pressed gui.tempStepNode->step.stepDetails->stepCancelButton");
      
      if(gui.tempStepNode->step.swipedLeft == 1 && gui.tempStepNode->step.swipedRight == 0){
        uint32_t  x = lv_obj_get_x_aligned(gui.tempStepNode->step.stepElement) + 50;
        uint32_t  y = lv_obj_get_y_aligned(gui.tempStepNode->step.stepElement);
        lv_obj_set_pos(gui.tempStepNode->step.stepElement, x, y);
        gui.tempStepNode->step.swipedLeft = 0;
        gui.tempStepNode->step.swipedRight = 0;
        lv_obj_add_flag(gui.tempStepNode->step.deleteButton, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(gui.tempStepNode->step.editButton, LV_OBJ_FLAG_HIDDEN);
      }

      lv_msgbox_close(mboxCont);
//      lv_obj_delete(mboxCont);
    }
 }
  
 
  if(code == LV_EVENT_VALUE_CHANGED){
    if(obj == gui.tempStepNode->step.stepDetails->stepTypeDropDownList){
      if(lv_dropdown_get_selected(gui.tempStepNode->step.stepDetails->stepTypeDropDownList) == 0){
          stepType = 0;
          LV_LOG_USER("Selected gui.tempStepNode->step.stepDetails->stepTypeDropDownList chemical_icon value:%d",stepType);
          lv_label_set_text(gui.tempStepNode->step.stepDetails->stepTypeHelpIcon, chemical_icon);
      }
      if(lv_dropdown_get_selected(gui.tempStepNode->step.stepDetails->stepTypeDropDownList) == 1){
          stepType = 1;
          LV_LOG_USER("Selected gui.tempStepNode->step.stepDetails->stepTypeDropDownList rinse_icon value:%d",stepType);
          lv_label_set_text(gui.tempStepNode->step.stepDetails->stepTypeHelpIcon, rinse_icon);
      }
      if(lv_dropdown_get_selected(gui.tempStepNode->step.stepDetails->stepTypeDropDownList) == 2){
          stepType = 2;
          LV_LOG_USER("Selected gui.tempStepNode->step.stepDetails->stepTypeDropDownList multiRinse_icon value:%d",stepType);
          lv_label_set_text(gui.tempStepNode->step.stepDetails->stepTypeHelpIcon, multiRinse_icon);
      }
      gui.tempStepNode->step.stepDetails->type = stepType;
    }

    if(obj == gui.tempStepNode->step.stepDetails->stepDiscardAfterSwitch){
      discardAfter = lv_obj_has_state(obj, LV_STATE_CHECKED);
      gui.tempStepNode->step.stepDetails->discardAfterProc = discardAfter;
      LV_LOG_USER("Discard After : %s", discardAfter ? "On" : "Off");
    }

    if(obj == gui.tempStepNode->step.stepDetails->stepSourceDropDownList){
         stepSource = lv_dropdown_get_selected(gui.tempStepNode->step.stepDetails->stepSourceDropDownList);
         gui.tempStepNode->step.stepDetails->source = stepSource;

         LV_LOG_USER("Selected gui.tempStepNode->step.stepDetails->stepSourceDropDownList %d",stepSource);
    }
  }

    if(code == LV_EVENT_FOCUSED) {
        
        if((lv_obj_t*)data == gui.tempStepNode->step.stepDetails->stepDetailMinTextArea){
            LV_LOG_USER("Set minutes");
            rollerPopupCreate(gui.element.rollerPopup.minutesOptions,setMinutesPopupTitle_text,gui.tempStepNode->step.stepDetails->stepDetailMinTextArea, findRolleStringIndex(lv_textarea_get_text(gui.tempStepNode->step.stepDetails->stepDetailMinTextArea),gui.element.rollerPopup.minutesOptions));
        }
        if((lv_obj_t*)data == gui.tempStepNode->step.stepDetails->stepDetailSecTextArea){
            LV_LOG_USER("Set seconds");
            rollerPopupCreate(gui.element.rollerPopup.secondsOptions,setSecondsPopupTitle_text,gui.tempStepNode->step.stepDetails->stepDetailSecTextArea, findRolleStringIndex(lv_textarea_get_text(gui.tempStepNode->step.stepDetails->stepDetailSecTextArea),gui.element.rollerPopup.secondsOptions));
        }
    }
  
  if(code == LV_EVENT_REFRESH){
    
    if(strlen(lv_textarea_get_text(gui.tempStepNode->step.stepDetails->stepDetailNamelTextArea)) > 0 && (atoi(lv_textarea_get_text(gui.tempStepNode->step.stepDetails->stepDetailSecTextArea)) > 0 || atoi(lv_textarea_get_text(gui.tempStepNode->step.stepDetails->stepDetailMinTextArea)) > 0 )) {
        lv_obj_clear_state(gui.tempStepNode->step.stepDetails->stepSaveButton, LV_STATE_DISABLED);
    }
    else{
      lv_obj_add_state(gui.tempStepNode->step.stepDetails->stepSaveButton, LV_STATE_DISABLED);
    }
  }

  if(code == LV_EVENT_DELETE) {
       LV_LOG_USER("Delete Styles");
        //list of all styles to be reset, so clean the memory.
        lv_style_reset(&gui.tempStepNode->step.stepDetails->style_mBoxStepPopupTitleLine);
        //lv_style_reset(&gui.tempStepNode->step.stepDetails->dropDownListStyle);
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

      char formatted_string[20];

      stepNode* existingStep = (stepNode*)isNodeInList((void*)&(referenceNode->process.processDetails->stepElementsList), currentNode, STEP_NODE);
      if(existingStep != NULL) {
          LV_LOG_USER("Step already exist with address 0x%p", currentNode);
          gui.tempStepNode = existingStep; // Usa il nodo già presente anziché allocarne uno nuovo

      } else {
          gui.tempStepNode = (stepNode*)allocateAndInitializeNode(STEP_NODE);
          LV_LOG_USER("New stepNode created at address 0x%p", gui.tempStepNode);
      }
      

      LV_LOG_USER("Step detail creation");

     // if(gui.tempStepNode->step.stepDetails->dropDownListStyle->values_and_props == NULL ) {		/* Only initialise the style once! */
         // lv_style_init(&gui.tempStepNode->step.stepDetails->dropDownListStyle);
         // lv_style_set_text_font(&gui.tempStepNode->step.stepDetails->dropDownListStyle, &lv_font_montserrat_20);
       
      //  }


      gui.tempStepNode->step.stepDetails->stepDetailParent = lv_obj_class_create_obj(&lv_msgbox_backdrop_class, lv_layer_top());
      lv_obj_class_init_obj(gui.tempStepNode->step.stepDetails->stepDetailParent);
      lv_obj_remove_flag(gui.tempStepNode->step.stepDetails->stepDetailParent, LV_OBJ_FLAG_IGNORE_LAYOUT);
      lv_obj_set_size(gui.tempStepNode->step.stepDetails->stepDetailParent, LV_PCT(100), LV_PCT(100));


            gui.tempStepNode->step.stepDetails->stepDetailContainer = lv_obj_create(gui.tempStepNode->step.stepDetails->stepDetailParent);
            lv_obj_align(gui.tempStepNode->step.stepDetails->stepDetailContainer, LV_ALIGN_CENTER, 0, 0);
            lv_obj_set_size(gui.tempStepNode->step.stepDetails->stepDetailContainer, 350, 300); 
            lv_obj_remove_flag(gui.tempStepNode->step.stepDetails->stepDetailContainer, LV_OBJ_FLAG_SCROLLABLE); 

                    gui.tempStepNode->step.stepDetails->stepDetailLabel = lv_label_create(gui.tempStepNode->step.stepDetails->stepDetailContainer);         
                    lv_label_set_text(gui.tempStepNode->step.stepDetails->stepDetailLabel, stepDetailTitle_text); 
                    lv_obj_set_style_text_font(gui.tempStepNode->step.stepDetails->stepDetailLabel, &lv_font_montserrat_22, 0);              
                    lv_obj_align(gui.tempStepNode->step.stepDetails->stepDetailLabel, LV_ALIGN_TOP_MID, 0, - 10);


                          lv_style_init(&gui.tempStepNode->step.stepDetails->style_mBoxStepPopupTitleLine);
                          lv_style_set_line_width(&gui.tempStepNode->step.stepDetails->style_mBoxStepPopupTitleLine, 2);
                          lv_style_set_line_color(&gui.tempStepNode->step.stepDetails->style_mBoxStepPopupTitleLine, lv_palette_main(LV_PALETTE_GREEN));
                          lv_style_set_line_rounded(&gui.tempStepNode->step.stepDetails->style_mBoxStepPopupTitleLine, true);


                    gui.tempStepNode->step.stepDetails->mBoxStepPopupTitleLine = lv_line_create(gui.tempStepNode->step.stepDetails->stepDetailContainer);
                    gui.tempStepNode->step.stepDetails->titleLinePoints[1].x = 310;
                    lv_line_set_points(gui.tempStepNode->step.stepDetails->mBoxStepPopupTitleLine, gui.tempStepNode->step.stepDetails->titleLinePoints, 2);
                    lv_obj_add_style(gui.tempStepNode->step.stepDetails->mBoxStepPopupTitleLine, &gui.tempStepNode->step.stepDetails->style_mBoxStepPopupTitleLine, 0);
                    lv_obj_align(gui.tempStepNode->step.stepDetails->mBoxStepPopupTitleLine, LV_ALIGN_TOP_MID, 0, 23);


            gui.tempStepNode->step.stepDetails->stepDetailNameContainer = lv_obj_create(gui.tempStepNode->step.stepDetails->stepDetailContainer);
            lv_obj_remove_flag(gui.tempStepNode->step.stepDetails->stepDetailNameContainer, LV_OBJ_FLAG_SCROLLABLE); 
            lv_obj_align(gui.tempStepNode->step.stepDetails->stepDetailNameContainer, LV_ALIGN_TOP_LEFT, -15, 30);
            lv_obj_set_size(gui.tempStepNode->step.stepDetails->stepDetailNameContainer, 325, 40); 
            lv_obj_set_style_border_opa(gui.tempStepNode->step.stepDetails->stepDetailNameContainer, LV_OPA_TRANSP, 0);
            //lv_obj_set_style_border_color(gui.tempStepNode->step.stepDetails->stepDetailNameContainer, lv_color_hex(GREEN_DARK), 0);

                  gui.tempStepNode->step.stepDetails->stepDetailNamelLabel = lv_label_create(gui.tempStepNode->step.stepDetails->stepDetailNameContainer);         
                  lv_label_set_text(gui.tempStepNode->step.stepDetails->stepDetailNamelLabel, stepDetailName_text); 
                  lv_obj_set_style_text_font(gui.tempStepNode->step.stepDetails->stepDetailNamelLabel, &lv_font_montserrat_22, 0);              
                  lv_obj_align(gui.tempStepNode->step.stepDetails->stepDetailNamelLabel, LV_ALIGN_LEFT_MID, -10, 0);
              
                  gui.tempStepNode->step.stepDetails->stepDetailNamelTextArea = lv_textarea_create(gui.tempStepNode->step.stepDetails->stepDetailNameContainer);
                  lv_textarea_set_one_line(gui.tempStepNode->step.stepDetails->stepDetailNamelTextArea, true);
                  lv_textarea_set_placeholder_text(gui.tempStepNode->step.stepDetails->stepDetailNamelTextArea, stepDetailPlaceHolder_text);
                  lv_obj_align(gui.tempStepNode->step.stepDetails->stepDetailNamelTextArea, LV_ALIGN_LEFT_MID, 70, 0);
                  lv_obj_set_width(gui.tempStepNode->step.stepDetails->stepDetailNamelTextArea, 210);
                  lv_obj_add_event_cb(gui.tempStepNode->step.stepDetails->stepDetailNamelTextArea, event_keyboard, LV_EVENT_CLICKED, NULL);
                  lv_obj_add_event_cb(gui.tempStepNode->step.stepDetails->stepDetailNamelTextArea, event_keyboard, LV_EVENT_DEFOCUSED, NULL);
                  lv_obj_add_event_cb(gui.tempStepNode->step.stepDetails->stepDetailNamelTextArea, event_keyboard, LV_EVENT_CANCEL, NULL);
                  lv_obj_add_event_cb(gui.tempStepNode->step.stepDetails->stepDetailNamelTextArea, event_keyboard, LV_EVENT_READY, NULL);
                  lv_obj_add_state(gui.tempStepNode->step.stepDetails->stepDetailNamelTextArea, LV_STATE_FOCUSED); 
                  lv_obj_set_style_bg_color(gui.tempStepNode->step.stepDetails->stepDetailNamelTextArea, lv_palette_darken(LV_PALETTE_GREY, 3), 0);
                  lv_obj_set_style_border_color(gui.tempStepNode->step.stepDetails->stepDetailNamelTextArea, lv_color_hex(WHITE), 0);
                  lv_textarea_set_max_length(gui.tempStepNode->step.stepDetails->stepDetailNamelTextArea, MAX_PROC_NAME_LEN);
                  //gui.tempStepNode->step.stepDetails->stepNameString = generateRandomCharArray(10);
                  lv_textarea_set_text(gui.tempStepNode->step.stepDetails->stepDetailNamelTextArea, gui.tempStepNode->step.stepDetails->stepNameString);



            gui.tempStepNode->step.stepDetails->stepDurationContainer = lv_obj_create(gui.tempStepNode->step.stepDetails->stepDetailContainer);
            lv_obj_remove_flag(gui.tempStepNode->step.stepDetails->stepDurationContainer, LV_OBJ_FLAG_SCROLLABLE); 
            lv_obj_align(gui.tempStepNode->step.stepDetails->stepDurationContainer, LV_ALIGN_TOP_LEFT, -15, 70);
            lv_obj_set_size(gui.tempStepNode->step.stepDetails->stepDurationContainer, 325, 40); 
            lv_obj_set_style_border_opa(gui.tempStepNode->step.stepDetails->stepDurationContainer, LV_OPA_TRANSP, 0);
            //lv_obj_set_style_border_color(gui.tempStepNode->step.stepDetails->stepDurationContainer, lv_color_hex(GREEN_DARK), 0);

                  gui.tempStepNode->step.stepDetails->stepDurationLabel = lv_label_create(gui.tempStepNode->step.stepDetails->stepDurationContainer);         
                  lv_label_set_text(gui.tempStepNode->step.stepDetails->stepDurationLabel, stepDetailDuration_text); 
                  lv_obj_set_style_text_font(gui.tempStepNode->step.stepDetails->stepDurationLabel, &lv_font_montserrat_22, 0);              
                  lv_obj_align(gui.tempStepNode->step.stepDetails->stepDurationLabel, LV_ALIGN_LEFT_MID, -10, 0);

                  gui.tempStepNode->step.stepDetails->stepDetailMinTextArea = lv_textarea_create(gui.tempStepNode->step.stepDetails->stepDurationContainer);
                  lv_textarea_set_one_line(gui.tempStepNode->step.stepDetails->stepDetailMinTextArea, true);
                  lv_textarea_set_placeholder_text(gui.tempStepNode->step.stepDetails->stepDetailMinTextArea, stepDetailDurationMinPlaceHolder_text);
                  lv_obj_align(gui.tempStepNode->step.stepDetails->stepDetailMinTextArea, LV_ALIGN_LEFT_MID, 100, 0);
                  lv_obj_set_width(gui.tempStepNode->step.stepDetails->stepDetailMinTextArea, 60);
                 
                  lv_obj_add_event_cb(gui.tempStepNode->step.stepDetails->stepDetailMinTextArea, event_stepDetail, LV_EVENT_CLICKED, gui.tempStepNode->step.stepDetails->stepDetailMinTextArea);
                  lv_obj_add_event_cb(gui.tempStepNode->step.stepDetails->stepDetailMinTextArea, event_stepDetail, LV_EVENT_VALUE_CHANGED, gui.tempStepNode->step.stepDetails->stepDetailMinTextArea);
                  lv_obj_add_event_cb(gui.tempStepNode->step.stepDetails->stepDetailMinTextArea, event_stepDetail, LV_EVENT_REFRESH, gui.tempStepNode->step.stepDetails->stepDetailMinTextArea);
                  lv_obj_add_event_cb(gui.tempStepNode->step.stepDetails->stepDetailMinTextArea, event_stepDetail, LV_EVENT_DELETE, gui.tempStepNode->step.stepDetails->stepDetailMinTextArea);
                  lv_obj_add_event_cb(gui.tempStepNode->step.stepDetails->stepDetailMinTextArea, event_stepDetail, LV_EVENT_FOCUSED, gui.tempStepNode->step.stepDetails->stepDetailMinTextArea);
//                  lv_textarea_set_cursor_hidden(gui.tempStepNode->step.stepDetails->stepDetailMinTextArea, true);
//                  lv_obj_add_state(gui.tempStepNode->step.stepDetails->stepDetailMinTextArea, LV_STATE_FOCUSED); 
                  lv_obj_set_style_bg_color(gui.tempStepNode->step.stepDetails->stepDetailMinTextArea, lv_palette_darken(LV_PALETTE_GREY, 3), 0);
                  lv_obj_set_style_text_align(gui.tempStepNode->step.stepDetails->stepDetailMinTextArea , LV_TEXT_ALIGN_CENTER, 0);
                  lv_obj_set_style_border_color(gui.tempStepNode->step.stepDetails->stepDetailMinTextArea, lv_color_hex(WHITE), 0);
                  sprintf(formatted_string, "%d", gui.tempStepNode->step.stepDetails->timeMins);
                  lv_textarea_set_text(gui.tempStepNode->step.stepDetails->stepDetailMinTextArea, formatted_string); 


                  gui.tempStepNode->step.stepDetails->stepDetailSecTextArea = lv_textarea_create(gui.tempStepNode->step.stepDetails->stepDurationContainer);
//                  lv_textarea_set_cursor_hidden(gui.tempStepNode->step.stepDetails->stepDetailSecTextArea, true);
                  lv_textarea_set_one_line(gui.tempStepNode->step.stepDetails->stepDetailSecTextArea, true);
                  lv_textarea_set_placeholder_text(gui.tempStepNode->step.stepDetails->stepDetailSecTextArea, stepDetailDurationSecPlaceHolder_text);
                  lv_obj_align(gui.tempStepNode->step.stepDetails->stepDetailSecTextArea, LV_ALIGN_LEFT_MID, 187, 0);
                  lv_obj_set_width(gui.tempStepNode->step.stepDetails->stepDetailSecTextArea, 60);

                  lv_obj_add_event_cb(gui.tempStepNode->step.stepDetails->stepDetailSecTextArea, event_stepDetail, LV_EVENT_CLICKED, gui.tempStepNode->step.stepDetails->stepDetailSecTextArea);
                  lv_obj_add_event_cb(gui.tempStepNode->step.stepDetails->stepDetailSecTextArea, event_stepDetail, LV_EVENT_VALUE_CHANGED, gui.tempStepNode->step.stepDetails->stepDetailSecTextArea);
                  lv_obj_add_event_cb(gui.tempStepNode->step.stepDetails->stepDetailSecTextArea, event_stepDetail, LV_EVENT_REFRESH, gui.tempStepNode->step.stepDetails->stepDetailSecTextArea);
                  lv_obj_add_event_cb(gui.tempStepNode->step.stepDetails->stepDetailSecTextArea, event_stepDetail, LV_EVENT_DELETE, gui.tempStepNode->step.stepDetails->stepDetailSecTextArea);
                  lv_obj_add_event_cb(gui.tempStepNode->step.stepDetails->stepDetailSecTextArea, event_stepDetail, LV_EVENT_FOCUSED, gui.tempStepNode->step.stepDetails->stepDetailSecTextArea);
//                  lv_obj_add_state(gui.tempStepNode->step.stepDetails->stepDetailSecTextArea, LV_STATE_FOCUSED); 
                  lv_obj_set_style_bg_color(gui.tempStepNode->step.stepDetails->stepDetailSecTextArea, lv_palette_darken(LV_PALETTE_GREY, 3), 0);
                  lv_obj_set_style_text_align(gui.tempStepNode->step.stepDetails->stepDetailSecTextArea , LV_TEXT_ALIGN_CENTER, 0);
                  lv_obj_set_style_border_color(gui.tempStepNode->step.stepDetails->stepDetailSecTextArea, lv_color_hex(WHITE), 0);
                  sprintf(formatted_string, "%d", gui.tempStepNode->step.stepDetails->timeSecs);
                  lv_textarea_set_text(gui.tempStepNode->step.stepDetails->stepDetailSecTextArea, formatted_string); 

    

            gui.tempStepNode->step.stepDetails->stepTypeContainer = lv_obj_create(gui.tempStepNode->step.stepDetails->stepDetailContainer);
            lv_obj_remove_flag(gui.tempStepNode->step.stepDetails->stepTypeContainer, LV_OBJ_FLAG_SCROLLABLE); 
            lv_obj_align(gui.tempStepNode->step.stepDetails->stepTypeContainer, LV_ALIGN_TOP_LEFT, -15, 110);
            lv_obj_set_size(gui.tempStepNode->step.stepDetails->stepTypeContainer, 325, 40); 
            lv_obj_set_style_border_opa(gui.tempStepNode->step.stepDetails->stepTypeContainer, LV_OPA_TRANSP, 0);
            //lv_obj_set_style_border_color(gui.tempStepNode->step.stepDetails->stepTypeContainer, lv_color_hex(GREEN_DARK), 0);


                  gui.tempStepNode->step.stepDetails->stepTypeLabel = lv_label_create(gui.tempStepNode->step.stepDetails->stepTypeContainer);         
                  lv_label_set_text(gui.tempStepNode->step.stepDetails->stepTypeLabel, stepDetailType_text); 
                  lv_obj_set_style_text_font(gui.tempStepNode->step.stepDetails->stepTypeLabel, &lv_font_montserrat_22, 0);              
                  lv_obj_align(gui.tempStepNode->step.stepDetails->stepTypeLabel, LV_ALIGN_LEFT_MID, -10, 0);


                  //lv_style_init(&gui.tempStepNode->step.stepDetails->dropDownListStyle);
                  gui.tempStepNode->step.stepDetails->stepTypeDropDownList = lv_dropdown_create(gui.tempStepNode->step.stepDetails->stepTypeContainer);
                  lv_obj_set_style_border_opa(gui.tempStepNode->step.stepDetails->stepTypeDropDownList, LV_OPA_TRANSP, 0);
                  lv_dropdown_set_options(gui.tempStepNode->step.stepDetails->stepTypeDropDownList, stepTypeList);
                  //lv_style_set_text_font(&gui.tempStepNode->step.stepDetails->dropDownListStyle, &lv_font_montserrat_20);
                  //lv_obj_add_style(gui.tempStepNode->step.stepDetails->stepTypeDropDownList, &gui.tempStepNode->step.stepDetails->dropDownListStyle, 0);
                  lv_obj_align(gui.tempStepNode->step.stepDetails->stepTypeDropDownList, LV_ALIGN_LEFT_MID, 50, 2);
                  lv_obj_set_size(gui.tempStepNode->step.stepDetails->stepTypeDropDownList, 165, 50); 
                  lv_obj_add_event_cb(gui.tempStepNode->step.stepDetails->stepTypeDropDownList, event_stepDetail, LV_EVENT_VALUE_CHANGED, gui.tempStepNode->step.stepDetails->stepTypeDropDownList);
                  lv_obj_set_style_bg_color(lv_dropdown_get_list(gui.tempStepNode->step.stepDetails->stepTypeDropDownList), lv_palette_main(LV_PALETTE_GREEN), LV_PART_SELECTED | LV_STATE_CHECKED);
                  lv_dropdown_set_selected(gui.tempStepNode->step.stepDetails->stepTypeDropDownList, gui.tempStepNode->step.stepDetails->type);

                  gui.tempStepNode->step.stepDetails->stepTypeHelpIcon = lv_label_create(gui.tempStepNode->step.stepDetails->stepTypeContainer);         
                  lv_label_set_text(gui.tempStepNode->step.stepDetails->stepTypeHelpIcon, chemical_icon); 
                  lv_obj_set_style_text_font(gui.tempStepNode->step.stepDetails->stepTypeHelpIcon, &FilMachineFontIcons_20, 0);              
                  lv_obj_align(gui.tempStepNode->step.stepDetails->stepTypeHelpIcon, LV_ALIGN_LEFT_MID, 210, 0);

   

            gui.tempStepNode->step.stepDetails->stepSourceContainer = lv_obj_create(gui.tempStepNode->step.stepDetails->stepDetailContainer);
            lv_obj_remove_flag(gui.tempStepNode->step.stepDetails->stepSourceContainer, LV_OBJ_FLAG_SCROLLABLE); 
            lv_obj_align(gui.tempStepNode->step.stepDetails->stepSourceContainer, LV_ALIGN_TOP_LEFT, -15, 150);
            lv_obj_set_size(gui.tempStepNode->step.stepDetails->stepSourceContainer, 340, 40); 
            lv_obj_set_style_border_opa(gui.tempStepNode->step.stepDetails->stepSourceContainer, LV_OPA_TRANSP, 0);
            //lv_obj_set_style_border_color(gui.tempStepNode->step.stepDetails->stepSourceContainer, lv_color_hex(GREEN_DARK), 0);


                  gui.tempStepNode->step.stepDetails->stepSourceLabel = lv_label_create(gui.tempStepNode->step.stepDetails->stepSourceContainer);         
                  lv_label_set_text(gui.tempStepNode->step.stepDetails->stepSourceLabel, stepDetailSource_text); 
                  lv_obj_set_style_text_font(gui.tempStepNode->step.stepDetails->stepSourceLabel, &lv_font_montserrat_22, 0);              
                  lv_obj_align(gui.tempStepNode->step.stepDetails->stepSourceLabel, LV_ALIGN_LEFT_MID, -10, 0);


                  //lv_style_init(&gui.tempStepNode->step.stepDetails->dropDownListStyle);
                  gui.tempStepNode->step.stepDetails->stepSourceDropDownList = lv_dropdown_create(gui.tempStepNode->step.stepDetails->stepSourceContainer);
                  lv_obj_set_style_border_opa(gui.tempStepNode->step.stepDetails->stepSourceDropDownList, LV_OPA_TRANSP, 0);
                  lv_dropdown_set_options(gui.tempStepNode->step.stepDetails->stepSourceDropDownList, stepSourceList);
                  //lv_style_set_text_font(&gui.tempStepNode->step.stepDetails->dropDownListStyle, &lv_font_montserrat_20);
                  lv_obj_align(gui.tempStepNode->step.stepDetails->stepSourceDropDownList, LV_ALIGN_LEFT_MID, 71, 2);
                  lv_obj_set_size(gui.tempStepNode->step.stepDetails->stepSourceDropDownList, 83, 50); 
                  lv_obj_add_event_cb(gui.tempStepNode->step.stepDetails->stepSourceDropDownList, event_stepDetail, LV_EVENT_VALUE_CHANGED, gui.tempStepNode->step.stepDetails->stepSourceDropDownList);
                  lv_obj_set_style_bg_color(lv_dropdown_get_list(gui.tempStepNode->step.stepDetails->stepSourceDropDownList), lv_palette_main(LV_PALETTE_GREEN), LV_PART_SELECTED | LV_STATE_CHECKED); 
                  lv_dropdown_set_selected(gui.tempStepNode->step.stepDetails->stepSourceDropDownList, gui.tempStepNode->step.stepDetails->source);

                  gui.tempStepNode->step.stepDetails->stepSourceTempLabel = lv_label_create(gui.tempStepNode->step.stepDetails->stepSourceContainer);         
                  lv_label_set_text(gui.tempStepNode->step.stepDetails->stepSourceTempLabel, stepDetailCurrentTemp_text); 
                  lv_obj_set_style_text_font(gui.tempStepNode->step.stepDetails->stepSourceTempLabel, &lv_font_montserrat_22, 0);              
                  lv_obj_align(gui.tempStepNode->step.stepDetails->stepSourceTempLabel, LV_ALIGN_LEFT_MID, 165, 0);

                  gui.tempStepNode->step.stepDetails->stepSourceTempHelpIcon = lv_label_create(gui.tempStepNode->step.stepDetails->stepSourceContainer);         
                  lv_label_set_text(gui.tempStepNode->step.stepDetails->stepSourceTempHelpIcon, temp_icon); 
                  lv_obj_set_style_text_font(gui.tempStepNode->step.stepDetails->stepSourceTempHelpIcon, &FilMachineFontIcons_20, 0);              
                  lv_obj_align(gui.tempStepNode->step.stepDetails->stepSourceTempHelpIcon, LV_ALIGN_LEFT_MID, 227, 0);

                  //itoa(stepSourceTemp, tempBuffer, 10);
                  gui.tempStepNode->step.stepDetails->stepSourceTempValue = lv_label_create(gui.tempStepNode->step.stepDetails->stepSourceContainer);         
                  //lv_label_set_text(gui.tempStepNode->step.stepDetails->stepSourceTempValue, convertFloatToTemp(stepSourceTemp)); //THIS NOT WORKS!
                  lv_label_set_text(gui.tempStepNode->step.stepDetails->stepSourceTempValue, "20.4°C");  
                  lv_obj_set_style_text_font(gui.tempStepNode->step.stepDetails->stepSourceTempValue, &lv_font_montserrat_22, 0);              
                  lv_obj_align(gui.tempStepNode->step.stepDetails->stepSourceTempValue, LV_ALIGN_LEFT_MID, 245, 1);


            gui.tempStepNode->step.stepDetails->stepDiscardAfterContainer = lv_obj_create(gui.tempStepNode->step.stepDetails->stepDetailContainer);
            lv_obj_remove_flag(gui.tempStepNode->step.stepDetails->stepDiscardAfterContainer, LV_OBJ_FLAG_SCROLLABLE); 
            lv_obj_align(gui.tempStepNode->step.stepDetails->stepDiscardAfterContainer, LV_ALIGN_TOP_LEFT, -15, 190);
            lv_obj_set_size(gui.tempStepNode->step.stepDetails->stepDiscardAfterContainer, 315, 40); 
            lv_obj_set_style_border_opa(gui.tempStepNode->step.stepDetails->stepDiscardAfterContainer, LV_OPA_TRANSP, 0);
            //lv_obj_set_style_border_color(gui.tempStepNode->step.stepDetails->stepDiscardAfterContainer, lv_color_hex(GREEN_DARK), 0);


                  gui.tempStepNode->step.stepDetails->stepDiscardAfterLabel = lv_label_create(gui.tempStepNode->step.stepDetails->stepDiscardAfterContainer);         
                  lv_label_set_text(gui.tempStepNode->step.stepDetails->stepDiscardAfterLabel, stepDetailDiscardAfter_text); 
                  lv_obj_set_style_text_font(gui.tempStepNode->step.stepDetails->stepDiscardAfterLabel, &lv_font_montserrat_22, 0);              
                  lv_obj_align(gui.tempStepNode->step.stepDetails->stepDiscardAfterLabel, LV_ALIGN_LEFT_MID, -10, 0);


                  gui.tempStepNode->step.stepDetails->stepDiscardAfterSwitch = lv_switch_create(gui.tempStepNode->step.stepDetails->stepDiscardAfterContainer);
                  lv_obj_add_event_cb(gui.tempStepNode->step.stepDetails->stepDiscardAfterSwitch, event_stepDetail, LV_EVENT_VALUE_CHANGED, gui.tempStepNode->step.stepDetails->stepDiscardAfterSwitch);
                  lv_obj_align(gui.tempStepNode->step.stepDetails->stepDiscardAfterSwitch, LV_ALIGN_LEFT_MID, 140, 0);
                  lv_obj_set_style_bg_color(gui.tempStepNode->step.stepDetails->stepDiscardAfterSwitch, lv_palette_darken(LV_PALETTE_GREY, 3), LV_STATE_DEFAULT);
                  lv_obj_set_style_bg_color(gui.tempStepNode->step.stepDetails->stepDiscardAfterSwitch,  lv_palette_main(LV_PALETTE_GREEN), LV_PART_KNOB | LV_STATE_DEFAULT);
                  lv_obj_set_style_bg_color(gui.tempStepNode->step.stepDetails->stepDiscardAfterSwitch, lv_color_hex(GREEN_DARK) , LV_PART_INDICATOR | LV_STATE_CHECKED);
                  lv_obj_add_state(gui.tempStepNode->step.stepDetails->stepDiscardAfterSwitch, gui.tempStepNode->step.stepDetails->discardAfterProc);


      gui.tempStepNode->step.stepDetails->stepSaveButton = lv_button_create(gui.tempStepNode->step.stepDetails->stepDetailContainer);
      lv_obj_set_size(gui.tempStepNode->step.stepDetails->stepSaveButton, BUTTON_PROCESS_WIDTH, BUTTON_PROCESS_HEIGHT);
      lv_obj_align(gui.tempStepNode->step.stepDetails->stepSaveButton, LV_ALIGN_BOTTOM_LEFT, 10 , 10);
      lv_obj_add_event_cb(gui.tempStepNode->step.stepDetails->stepSaveButton, event_stepDetail, LV_EVENT_CLICKED, referenceNode);
      lv_obj_add_event_cb(gui.tempStepNode->step.stepDetails->stepSaveButton, event_stepDetail, LV_EVENT_REFRESH, NULL);
      lv_obj_set_style_bg_color(gui.tempStepNode->step.stepDetails->stepSaveButton, lv_color_hex(GREEN_DARK), LV_PART_MAIN);
      lv_obj_add_state(gui.tempStepNode->step.stepDetails->stepSaveButton, LV_STATE_DISABLED);

          gui.tempStepNode->step.stepDetails->stepSaveLabel = lv_label_create(gui.tempStepNode->step.stepDetails->stepSaveButton);
          lv_label_set_text(gui.tempStepNode->step.stepDetails->stepSaveLabel, stepDetailSave_text);
          lv_obj_set_style_text_font(gui.tempStepNode->step.stepDetails->stepSaveLabel, &lv_font_montserrat_22, 0);
          lv_obj_align(gui.tempStepNode->step.stepDetails->stepSaveLabel, LV_ALIGN_CENTER, 0, 0);


      gui.tempStepNode->step.stepDetails->stepCancelButton = lv_button_create(gui.tempStepNode->step.stepDetails->stepDetailContainer);
      lv_obj_set_size(gui.tempStepNode->step.stepDetails->stepCancelButton, BUTTON_PROCESS_WIDTH, BUTTON_PROCESS_HEIGHT);
      lv_obj_align(gui.tempStepNode->step.stepDetails->stepCancelButton, LV_ALIGN_BOTTOM_RIGHT, - 10 , 10);
      lv_obj_add_event_cb(gui.tempStepNode->step.stepDetails->stepCancelButton, event_stepDetail, LV_EVENT_CLICKED, gui.tempStepNode->step.stepDetails->stepCancelButton);
      lv_obj_set_style_bg_color(gui.tempStepNode->step.stepDetails->stepCancelButton, lv_color_hex(GREEN_DARK), LV_PART_MAIN);

            gui.tempStepNode->step.stepDetails->stepCancelLabel = lv_label_create(gui.tempStepNode->step.stepDetails->stepCancelButton);
            lv_label_set_text(gui.tempStepNode->step.stepDetails->stepCancelLabel, stepDetailCancel_text);
            lv_obj_set_style_text_font(gui.tempStepNode->step.stepDetails->stepCancelLabel, &lv_font_montserrat_22, 0);
            lv_obj_align(gui.tempStepNode->step.stepDetails->stepCancelLabel, LV_ALIGN_CENTER, 0, 0);
  
}

