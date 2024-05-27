#include "misc/lv_event.h"
/**
 * @file page_processDetail.c
 *
 */


//ESSENTIAL INCLUDE
#include <lvgl.h>
#include "../../include/definitions.h"

extern struct gui_components gui;

//ACCESSORY INCLUDES

processNode *newProcess;

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
    if(data == newProcess->process.processDetails->processDetailCloseButton){
        newProcess->process.processDetails->stepElementsList.size = 0;
        lv_obj_send_event(newProcess->process.processDetails->processSaveButton, LV_EVENT_REFRESH, NULL);
        lv_msgbox_close(mboxCont);
        lv_obj_delete(mboxCont);
        LV_LOG_USER("Close Process Detail");
    }
    if(data == newProcess->process.processDetails->processColorLabel){
        lv_obj_set_style_text_color(newProcess->process.processDetails->processColorLabel, lv_color_hex(GREEN_DARK), LV_PART_MAIN);
        lv_obj_set_style_text_color(newProcess->process.processDetails->processBnWLabel, lv_color_hex(WHITE), LV_PART_MAIN);
        newProcess->process.processDetails->filmType = COLOR_FILM;
        newProcess->process.processDetails->somethingChanged = 1;

        lv_obj_send_event(newProcess->process.processDetails->processSaveButton, LV_EVENT_REFRESH, NULL);

        LV_LOG_USER("Pressed newProcess->process.processDetails->processColorLabel");
    }
    if(data == newProcess->process.processDetails->processBnWLabel){
        lv_obj_set_style_text_color(newProcess->process.processDetails->processBnWLabel, lv_color_hex(GREEN_DARK), LV_PART_MAIN);
        lv_obj_set_style_text_color(newProcess->process.processDetails->processColorLabel, lv_color_hex(WHITE), LV_PART_MAIN);
        newProcess->process.processDetails->filmType = BLACK_AND_WHITE_FILM;
        newProcess->process.processDetails->somethingChanged = 1;
        
        lv_obj_send_event(newProcess->process.processDetails->processSaveButton, LV_EVENT_REFRESH, NULL);     
        LV_LOG_USER("Pressed newProcess->process.processDetails->processBnWLabel");
    }

    if(data == newProcess->process.processDetails->processPreferredLabel){
        if(newProcess->process.processDetails->isPreferred == 0){
          lv_obj_set_style_text_color(newProcess->process.processDetails->processPreferredLabel, lv_color_hex(RED), LV_PART_MAIN);
          newProcess->process.processDetails->isPreferred = 1;
          newProcess->process.processDetails->somethingChanged = 1;
          lv_obj_send_event(newProcess->process.processDetails->processSaveButton, LV_EVENT_REFRESH, NULL);
        }
        else{
          lv_obj_set_style_text_color(newProcess->process.processDetails->processPreferredLabel, lv_color_hex(WHITE), LV_PART_MAIN);
          newProcess->process.processDetails->isPreferred = 0;
        }
        LV_LOG_USER("Process is preferred :%d",isPreferred);
    }
    if(data == newProcess->process.processDetails->processSaveButton && newProcess->process.processDetails->stepElementsList.size > 0){
          newProcess->process.processDetails->somethingChanged = 0;
          lv_obj_clear_state(newProcess->process.processDetails->processRunButton, LV_STATE_DISABLED);
          lv_obj_add_state(newProcess->process.processDetails->processSaveButton, LV_STATE_DISABLED);

                // for testing
                static char 				name[80];	// Test Code
                static uint16_t			test_index = 1; // Test Code
                static filmType_t 	type = BLACK_AND_WHITE_FILM; // Test Code
                static uint32_t			temp = 38;	// Test Code
                lv_snprintf( name, sizeof(name), "A Test Process creation index %02d", test_index ); // Test code
                if(isNodeInList(&gui.page.processes.processElementsList, newProcess, PROCESS_NODE) == NULL)
                {
                  LV_LOG_USER("Process not present yet, let's create!");
                  if( !processElementCreate(newProcess, name, temp, type) ){	// Needs to be called with user populated values eventually
                      LV_LOG_USER("Process element not created!");
                    } 
                  else {
                      type = !type; // flip type every time for testing
                      temp ++;	// for test increase temp each time
                      test_index ++;	// for test increase index for name generation

                      if(addProcessElement(newProcess) != NULL)
                        {
                            LV_LOG_USER("Process address 0x%p, with n:%d steps",newProcess, newProcess->process.processDetails->stepElementsList.size); 
                            lv_obj_send_event(fakeObject, LV_EVENT_REFRESH, NULL);
                        }
                      else
                        {
                          LV_LOG_USER("Process element creation failed, maximum entries reached" );
                        }
                    }
                }
                else{
                   LV_LOG_USER("Process already present in list!");
                }
        LV_LOG_USER("Pressed newProcess->process.processDetails->processSaveButton");
    }

    if(data == newProcess->process.processDetails->processRunButton){
        newProcess->process.processDetails->stepElementsList.size = 0;
        lv_msgbox_close(mboxCont);
        lv_obj_delete(mboxCont);
        LV_LOG_USER("Pressed newProcess->process.processDetails->processRunButton");
        checkup(newProcess);
    }
    if(data == newProcess->process.processDetails->processNewStepButton){
        LV_LOG_USER("Pressed newProcess->process.processDetails->processNewStepButton");
        stepDetail(newProcess, NULL);
    }
  }

  if(code == LV_EVENT_LONG_PRESSED_REPEAT){
    //TBD
  }

  if(code == LV_EVENT_REFRESH){
    if(obj == newProcess->process.processDetails->processSaveButton){
        if(newProcess->process.processDetails->stepElementsList.size > 0){
              lv_obj_clear_state(newProcess->process.processDetails->processSaveButton, LV_STATE_DISABLED);
              LV_LOG_USER("Updated SAVE button: ENABLED");
        }
        else{
              lv_obj_add_state(newProcess->process.processDetails->processSaveButton, LV_STATE_DISABLED);
              lv_obj_add_state(newProcess->process.processDetails->processRunButton, LV_STATE_DISABLED);
              LV_LOG_USER("Updated SAVE button : DISABLED");   
        }
        if(newProcess->process.processDetails->somethingChanged == 1 && newProcess->process.processDetails->stepElementsList.size > 0){
              lv_obj_clear_state(newProcess->process.processDetails->processSaveButton, LV_STATE_DISABLED);
              LV_LOG_USER("Updated SAVE button : ENABLED");
        }
    }
  }

  if(code == LV_EVENT_VALUE_CHANGED) {
      if(data == newProcess->process.processDetails->processTempControlSwitch){
          LV_LOG_USER("Temperature controlled : %s", lv_obj_has_state(obj, LV_STATE_CHECKED) ? "On" : "Off");
          newProcess->process.processDetails->somethingChanged = 1;
          newProcess->process.processDetails->isTempControlled = lv_obj_has_state(obj, LV_STATE_CHECKED);
          lv_obj_send_event(newProcess->process.processDetails->processSaveButton, LV_EVENT_REFRESH, NULL);
      }
  }

  if(code == LV_EVENT_FOCUSED) { 
      tempProcessNode = newProcess;
      if(data == newProcess->process.processDetails->processTempTextArea){
          LV_LOG_USER("Set Temperature");
          rollerPopupCreate(gui.element.rollerPopup.tempCelsiusOptions,tuneTempPopupTitle_text,newProcess->process.processDetails->processTempTextArea, 0);
      }
      if(data == newProcess->process.processDetails->processToleranceTextArea){
          LV_LOG_USER("Set Tolerance");
          rollerPopupCreate(gui.element.rollerPopup.tempCelsiusToleranceOptions,tuneTempPopupTitle_text,newProcess->process.processDetails->processToleranceTextArea, 0);
      }
  }
}
 
void event_processDetail_style_delete(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_DELETE) {
        //list of all styles to be reset, so clean the memory.
        lv_style_reset(&newProcess->process.processDetails->textAreaStyle);
    }
}



/*********************
  *    PROCESS DETAIL
*********************/

void processDetail(lv_obj_t * processContainer)
{   
/*********************
  *    PAGE HEADER
*********************/
        newProcess = (processNode*) allocateAndInitializeNode(PROCESS_NODE);
        tempProcessNode = (processNode*) allocateAndInitializeNode(PROCESS_NODE);
        tempProcessNode = newProcess;



  LV_LOG_USER("Process address 0x%p, with n:%d steps",newProcess, newProcess->process.processDetails->stepElementsList.size); 
  newProcess->process.processDetails->filmType = COLOR_FILM;
  newProcess->process.processDetails->somethingChanged = 0;
  newProcess->process.processDetails->stepElementsList.size = 0;

  newProcess->process.processDetails->processesContainer = processContainer;


  newProcess->process.processDetails->processDetailParent = lv_obj_class_create_obj(&lv_msgbox_backdrop_class, lv_layer_top());
  lv_obj_class_init_obj(newProcess->process.processDetails->processDetailParent);
  lv_obj_remove_flag(newProcess->process.processDetails->processDetailParent, LV_OBJ_FLAG_IGNORE_LAYOUT);
  lv_obj_set_size(newProcess->process.processDetails->processDetailParent, LV_PCT(100), LV_PCT(100));
  
  lv_style_init(&newProcess->process.processDetails->textAreaStyle);
  
  /*********************
  *    PAGE ELEMENTS
  *********************/
      newProcess->process.processDetails->processDetailContainer = lv_obj_create(newProcess->process.processDetails->processDetailParent);
      lv_obj_align(newProcess->process.processDetails->processDetailContainer, LV_ALIGN_CENTER, 0, 0);
      lv_obj_set_size(newProcess->process.processDetails->processDetailContainer, LV_PCT(100), LV_PCT(100));
      lv_obj_remove_flag(newProcess->process.processDetails->processDetailContainer, LV_OBJ_FLAG_SCROLLABLE); 
            
            newProcess->process.processDetails->processDetailCloseButton = lv_button_create(newProcess->process.processDetails->processDetailContainer);
            lv_obj_set_size(newProcess->process.processDetails->processDetailCloseButton, BUTTON_POPUP_CLOSE_WIDTH * 1.2, BUTTON_POPUP_CLOSE_HEIGHT * 1.2);
            lv_obj_align(newProcess->process.processDetails->processDetailCloseButton, LV_ALIGN_TOP_RIGHT, 7 , -10);
            lv_obj_add_event_cb(newProcess->process.processDetails->processDetailCloseButton, event_processDetail, LV_EVENT_CLICKED, newProcess->process.processDetails->processDetailCloseButton);
            lv_obj_set_style_bg_color(newProcess->process.processDetails->processDetailCloseButton, lv_color_hex(GREEN_DARK), LV_PART_MAIN);
            lv_obj_move_foreground(newProcess->process.processDetails->processDetailCloseButton);


                  newProcess->process.processDetails->processDetailCloseButtonLabel = lv_label_create(newProcess->process.processDetails->processDetailCloseButton);         
                  lv_label_set_text(newProcess->process.processDetails->processDetailCloseButtonLabel, closePopup_icon); 
                  lv_obj_set_style_text_font(newProcess->process.processDetails->processDetailCloseButtonLabel, &FilMachineFontIcons_30, 0);              
                  lv_obj_align(newProcess->process.processDetails->processDetailCloseButtonLabel, LV_ALIGN_CENTER, 0, 0);
                  
        

            newProcess->process.processDetails->processDetailNameContainer = lv_obj_create(newProcess->process.processDetails->processDetailContainer);
            lv_obj_remove_flag(newProcess->process.processDetails->processDetailNameContainer, LV_OBJ_FLAG_SCROLLABLE); 
            lv_obj_align(newProcess->process.processDetails->processDetailNameContainer, LV_ALIGN_TOP_LEFT, -10, -15);
            lv_obj_set_size(newProcess->process.processDetails->processDetailNameContainer, 350, 50); 
            //lv_obj_set_style_border_color(newProcess->process.processDetails->processDetailNameContainer, lv_color_hex(GREEN_DARK), 0);
            lv_obj_set_style_border_opa(newProcess->process.processDetails->processDetailNameContainer, LV_OPA_TRANSP, 0);

                  newProcess->process.processDetails->processDetailName = lv_label_create(newProcess->process.processDetails->processDetailNameContainer);         
                  lv_label_set_text(newProcess->process.processDetails->processDetailName, "E6 six baths"); 
                  lv_obj_set_width(newProcess->process.processDetails->processDetailName, 300);
                  lv_obj_set_style_text_font(newProcess->process.processDetails->processDetailName, &lv_font_montserrat_30, 0);              
                  lv_obj_align(newProcess->process.processDetails->processDetailName, LV_ALIGN_TOP_LEFT, -10, -8);
                  lv_label_set_long_mode(newProcess->process.processDetails->processDetailName, LV_LABEL_LONG_SCROLL_CIRCULAR);
                  newProcess->process.processDetails->processNameString = "E6 six baths";

            newProcess->process.processDetails->processDetailStepsLabel = lv_label_create(newProcess->process.processDetails->processDetailContainer);         
            lv_label_set_text(newProcess->process.processDetails->processDetailStepsLabel, processDetailStep_text); 
            lv_obj_set_width(newProcess->process.processDetails->processDetailStepsLabel, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(newProcess->process.processDetails->processDetailStepsLabel, &lv_font_montserrat_24, 0);              
            lv_obj_align(newProcess->process.processDetails->processDetailStepsLabel, LV_ALIGN_TOP_LEFT, 0, 25);

            newProcess->process.processDetails->processStepsContainer = lv_obj_create(newProcess->process.processDetails->processDetailContainer);
            lv_obj_align(newProcess->process.processDetails->processStepsContainer, LV_ALIGN_TOP_LEFT, -10, 55);
            lv_obj_set_size(newProcess->process.processDetails->processStepsContainer, 250, 240);
            lv_obj_set_scroll_dir(newProcess->process.processDetails->processStepsContainer, LV_DIR_VER);
            lv_obj_set_style_border_color(newProcess->process.processDetails->processStepsContainer, lv_palette_main(LV_PALETTE_GREEN), 0);
            //lv_obj_set_style_border_opa(newProcess->process.processDetails->processStepsContainer, LV_OPA_TRANSP, 0);


            newProcess->process.processDetails->processNewStepButton = lv_button_create(newProcess->process.processDetails->processDetailContainer);
            lv_obj_set_size(newProcess->process.processDetails->processNewStepButton, BUTTON_POPUP_CLOSE_WIDTH, BUTTON_POPUP_CLOSE_HEIGHT);
            lv_obj_align(newProcess->process.processDetails->processNewStepButton, LV_ALIGN_TOP_LEFT, 210 , 55);
            lv_obj_add_event_cb(newProcess->process.processDetails->processNewStepButton, event_processDetail, LV_EVENT_CLICKED, newProcess->process.processDetails->processNewStepButton);
            lv_obj_set_style_bg_color(newProcess->process.processDetails->processNewStepButton, lv_palette_main(LV_PALETTE_GREEN), LV_PART_MAIN);
            //lv_obj_move_foreground(newProcess->process.processDetails->processNewStepButton);
  
                    newProcess->process.processDetails->processNewStepLabel = lv_label_create(newProcess->process.processDetails->processNewStepButton);         
                    lv_label_set_text(newProcess->process.processDetails->processNewStepLabel, LV_SYMBOL_PLUS); 
                    lv_obj_set_style_text_font(newProcess->process.processDetails->processNewStepLabel, &lv_font_montserrat_20, 0);              
                    lv_obj_align(newProcess->process.processDetails->processNewStepLabel, LV_ALIGN_CENTER, 0, 0);


            newProcess->process.processDetails->processDetailInfoLabel = lv_label_create(newProcess->process.processDetails->processDetailContainer);         
            lv_label_set_text(newProcess->process.processDetails->processDetailInfoLabel, processDetailInfo_text); 
            lv_obj_set_width(newProcess->process.processDetails->processDetailInfoLabel, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(newProcess->process.processDetails->processDetailInfoLabel, &lv_font_montserrat_24, 0);              
            lv_obj_align(newProcess->process.processDetails->processDetailInfoLabel, LV_ALIGN_TOP_RIGHT, -105, 25);

            newProcess->process.processDetails->processInfoContainer = lv_obj_create(newProcess->process.processDetails->processDetailContainer);
            lv_obj_align(newProcess->process.processDetails->processInfoContainer, LV_ALIGN_TOP_LEFT, 245, 55);
            lv_obj_set_size(newProcess->process.processDetails->processInfoContainer, 210, 195);
            lv_obj_remove_flag(newProcess->process.processDetails->processInfoContainer, LV_OBJ_FLAG_SCROLLABLE); 
            //lv_obj_set_scroll_dir(newProcess->process.processDetails->processInfoContainer, LV_DIR_VER);
            lv_obj_set_style_border_color(newProcess->process.processDetails->processInfoContainer, lv_color_hex(WHITE), 0);
            //lv_obj_set_style_border_opa(newProcess->process.processDetails->processInfoContainer, LV_OPA_TRANSP, 0);

                  
                  newProcess->process.processDetails->processTempControlContainer = lv_obj_create(newProcess->process.processDetails->processInfoContainer);
                  lv_obj_remove_flag(newProcess->process.processDetails->processTempControlContainer, LV_OBJ_FLAG_SCROLLABLE); 
                  lv_obj_align(newProcess->process.processDetails->processTempControlContainer, LV_ALIGN_TOP_LEFT, -15, -17);
                  lv_obj_set_size(newProcess->process.processDetails->processTempControlContainer, 205, 40); 
                  //lv_obj_set_style_border_color(newProcess->process.processDetails->processTempControlContainer, lv_color_hex(GREEN_DARK), 0);
                  lv_obj_set_style_border_opa(newProcess->process.processDetails->processTempControlContainer, LV_OPA_TRANSP, 0);

                          newProcess->process.processDetails->processTempControlLabel = lv_label_create(newProcess->process.processDetails->processTempControlContainer);         
                          lv_label_set_text(newProcess->process.processDetails->processTempControlLabel, processDetailIsTempControl_text); 
                          lv_obj_set_style_text_font(newProcess->process.processDetails->processTempControlLabel, &lv_font_montserrat_16, 0);              
                          lv_obj_align(newProcess->process.processDetails->processTempControlLabel, LV_ALIGN_LEFT_MID, -15, 0);

                          newProcess->process.processDetails->processTempControlSwitch = lv_switch_create(newProcess->process.processDetails->processTempControlContainer);
                          lv_obj_add_event_cb(newProcess->process.processDetails->processTempControlSwitch, event_processDetail, LV_EVENT_VALUE_CHANGED, newProcess->process.processDetails->processTempControlSwitch);
                          lv_obj_align(newProcess->process.processDetails->processTempControlSwitch, LV_ALIGN_RIGHT_MID, 15, 0);
                          lv_obj_set_style_bg_color(newProcess->process.processDetails->processTempControlSwitch, lv_palette_darken(LV_PALETTE_GREY, 3), LV_STATE_DEFAULT);
                          lv_obj_set_style_bg_color(newProcess->process.processDetails->processTempControlSwitch,  lv_palette_main(LV_PALETTE_GREEN), LV_PART_KNOB | LV_STATE_DEFAULT);
                          lv_obj_set_style_bg_color(newProcess->process.processDetails->processTempControlSwitch, lv_color_hex(GREEN_DARK) , LV_PART_INDICATOR | LV_STATE_CHECKED);


                 
                  newProcess->process.processDetails->processTempContainer = lv_obj_create(newProcess->process.processDetails->processInfoContainer);
                  lv_obj_remove_flag(newProcess->process.processDetails->processTempContainer, LV_OBJ_FLAG_SCROLLABLE); 
                  lv_obj_align(newProcess->process.processDetails->processTempContainer, LV_ALIGN_TOP_LEFT, -15, 20);
                  lv_obj_set_size(newProcess->process.processDetails->processTempContainer, 205, 45); 
                  //lv_obj_set_style_border_color(newProcess->process.processDetails->processTempContainer, lv_color_hex(GREEN_DARK), 0);
                  lv_obj_set_style_border_opa(newProcess->process.processDetails->processTempContainer, LV_OPA_TRANSP, 0);   

                          newProcess->process.processDetails->processTempLabel = lv_label_create(newProcess->process.processDetails->processTempContainer);         
                          lv_label_set_text(newProcess->process.processDetails->processTempLabel, processDetailTemp_text); 
                          lv_obj_set_style_text_font(newProcess->process.processDetails->processTempLabel, &lv_font_montserrat_16, 0);              
                          lv_obj_align(newProcess->process.processDetails->processTempLabel, LV_ALIGN_LEFT_MID, -15, 0);

                          newProcess->process.processDetails->processTempTextArea = lv_textarea_create(newProcess->process.processDetails->processTempContainer);
                          lv_textarea_set_one_line(newProcess->process.processDetails->processTempTextArea, true);
                          lv_textarea_set_placeholder_text(newProcess->process.processDetails->processTempTextArea, processDetailTempPlaceHolder_text);
                          lv_obj_align(newProcess->process.processDetails->processTempTextArea, LV_ALIGN_LEFT_MID, 100, 0);
                          lv_obj_set_width(newProcess->process.processDetails->processTempTextArea, 60);
                          lv_obj_add_event_cb(newProcess->process.processDetails->processTempTextArea, event_processDetail, LV_EVENT_ALL, newProcess->process.processDetails->processTempTextArea);
                          lv_obj_add_state(newProcess->process.processDetails->processTempTextArea, LV_STATE_FOCUSED); /*To be sure the cursor is visible*/
                          lv_obj_set_style_bg_color(newProcess->process.processDetails->processTempTextArea, lv_palette_darken(LV_PALETTE_GREY, 3), 0);
                          lv_obj_set_style_text_align(newProcess->process.processDetails->processTempTextArea , LV_TEXT_ALIGN_CENTER, 0);
                          lv_style_set_text_font(&newProcess->process.processDetails->textAreaStyle, &lv_font_montserrat_18);
                          lv_obj_add_style(newProcess->process.processDetails->processTempTextArea, &newProcess->process.processDetails->textAreaStyle, LV_PART_MAIN);
                          lv_obj_set_style_border_color(newProcess->process.processDetails->processTempTextArea, lv_color_hex(WHITE), 0);


                          newProcess->process.processDetails->processTempUnitLabel = lv_label_create(newProcess->process.processDetails->processTempContainer);         
                          lv_label_set_text(newProcess->process.processDetails->processTempUnitLabel, celsius_text); 
                          lv_obj_set_style_text_font(newProcess->process.processDetails->processTempUnitLabel, &lv_font_montserrat_20, 0);              
                          lv_obj_align(newProcess->process.processDetails->processTempUnitLabel, LV_ALIGN_LEFT_MID, 160, 0);


                  newProcess->process.processDetails->processToleranceContainer = lv_obj_create(newProcess->process.processDetails->processInfoContainer);
                  lv_obj_remove_flag(newProcess->process.processDetails->processToleranceContainer, LV_OBJ_FLAG_SCROLLABLE); 
                  lv_obj_align(newProcess->process.processDetails->processToleranceContainer, LV_ALIGN_TOP_LEFT, -15, 65);
                  lv_obj_set_size(newProcess->process.processDetails->processToleranceContainer, 205, 45); 
                  //lv_obj_set_style_border_color(newProcess->process.processDetails->processToleranceContainer, lv_color_hex(GREEN_DARK), 0);
                  lv_obj_set_style_border_opa(newProcess->process.processDetails->processToleranceContainer, LV_OPA_TRANSP, 0);   

                          newProcess->process.processDetails->processToleranceLabel = lv_label_create(newProcess->process.processDetails->processToleranceContainer);         
                          lv_label_set_text(newProcess->process.processDetails->processToleranceLabel, processDetailTempTolerance_text); 
                          lv_obj_set_style_text_font(newProcess->process.processDetails->processToleranceLabel, &lv_font_montserrat_16, 0);              
                          lv_obj_align(newProcess->process.processDetails->processToleranceLabel, LV_ALIGN_LEFT_MID, -15, 0);

                          newProcess->process.processDetails->processToleranceTextArea = lv_textarea_create(newProcess->process.processDetails->processToleranceContainer);
                          lv_textarea_set_one_line(newProcess->process.processDetails->processToleranceTextArea, true);
                          lv_textarea_set_placeholder_text(newProcess->process.processDetails->processToleranceTextArea, processDetailTempPlaceHolder_text);
                          lv_obj_align(newProcess->process.processDetails->processToleranceTextArea, LV_ALIGN_LEFT_MID, 100, 0);
                          lv_obj_set_width(newProcess->process.processDetails->processToleranceTextArea, 60);

                          lv_obj_add_event_cb(newProcess->process.processDetails->processToleranceTextArea, event_processDetail, LV_EVENT_ALL, newProcess->process.processDetails->processToleranceTextArea);
                          lv_obj_add_state(newProcess->process.processDetails->processToleranceTextArea, LV_STATE_FOCUSED); /*To be sure the cursor is visible*/
                          lv_obj_set_style_bg_color(newProcess->process.processDetails->processToleranceTextArea, lv_palette_darken(LV_PALETTE_GREY, 3), 0);
                          lv_obj_set_style_text_align(newProcess->process.processDetails->processToleranceTextArea , LV_TEXT_ALIGN_CENTER, 0);
                          //lv_style_set_text_font(&newProcess->process.processDetails->textAreaStyle, &lv_font_montserrat_18);
                          lv_obj_add_style(newProcess->process.processDetails->processToleranceTextArea, &newProcess->process.processDetails->textAreaStyle, LV_PART_MAIN);
                          lv_obj_set_style_border_color(newProcess->process.processDetails->processToleranceTextArea, lv_color_hex(WHITE), 0);


                          newProcess->process.processDetails->processTempUnitLabel = lv_label_create(newProcess->process.processDetails->processToleranceContainer);         
                          lv_label_set_text(newProcess->process.processDetails->processTempUnitLabel, celsius_text); 
                          lv_obj_set_style_text_font(newProcess->process.processDetails->processTempUnitLabel, &lv_font_montserrat_20, 0);              
                          lv_obj_align(newProcess->process.processDetails->processTempUnitLabel, LV_ALIGN_LEFT_MID, 160, 0);
                          

                  newProcess->process.processDetails->processTotalTimeContainer = lv_obj_create(newProcess->process.processDetails->processInfoContainer);
                  lv_obj_remove_flag(newProcess->process.processDetails->processTotalTimeContainer, LV_OBJ_FLAG_SCROLLABLE); 
                  lv_obj_align(newProcess->process.processDetails->processTotalTimeContainer, LV_ALIGN_TOP_LEFT, -15, 110);
                  lv_obj_set_size(newProcess->process.processDetails->processTotalTimeContainer, 205, 30); 
                  //lv_obj_set_style_border_color(newProcess->process.processDetails->processTotalTimeContainer, lv_color_hex(GREEN_DARK), 0);
                  lv_obj_set_style_border_opa(newProcess->process.processDetails->processTotalTimeContainer, LV_OPA_TRANSP, 0);   

                          newProcess->process.processDetails->processTotalTimeLabel = lv_label_create(newProcess->process.processDetails->processTotalTimeContainer);         
                          lv_label_set_text(newProcess->process.processDetails->processTotalTimeLabel, processDetailTotalTime_text); 
                          lv_obj_set_style_text_font(newProcess->process.processDetails->processTotalTimeLabel, &lv_font_montserrat_16, 0);              
                          lv_obj_align(newProcess->process.processDetails->processTotalTimeLabel, LV_ALIGN_LEFT_MID, -15, 0);

                          newProcess->process.processDetails->processTotalTimeValue = lv_label_create(newProcess->process.processDetails->processTotalTimeContainer);         
                          lv_label_set_text(newProcess->process.processDetails->processTotalTimeValue, "32m20s"); 
                          lv_obj_set_style_text_font(newProcess->process.processDetails->processTotalTimeValue, &lv_font_montserrat_20, 0);              
                          lv_obj_align(newProcess->process.processDetails->processTotalTimeValue, LV_ALIGN_LEFT_MID, 100, 0);
                          newProcess->process.processDetails->totalTime = 666;    


                  newProcess->process.processDetails->processColorOrBnWContainer = lv_obj_create(newProcess->process.processDetails->processInfoContainer);
                  lv_obj_remove_flag(newProcess->process.processDetails->processColorOrBnWContainer, LV_OBJ_FLAG_SCROLLABLE); 
                  lv_obj_align(newProcess->process.processDetails->processColorOrBnWContainer, LV_ALIGN_TOP_LEFT, -18, 137);
                  lv_obj_set_size(newProcess->process.processDetails->processColorOrBnWContainer, 105, 40); 
                  lv_obj_set_style_border_color(newProcess->process.processDetails->processColorOrBnWContainer, lv_color_hex(WHITE), 0);
                  //lv_obj_set_style_border_opa(newProcess->process.processDetails->processColorOrBnWContainer, LV_OPA_TRANSP, 0);   


                          newProcess->process.processDetails->processColorLabel = lv_label_create(newProcess->process.processDetails->processColorOrBnWContainer);         
                          lv_label_set_text(newProcess->process.processDetails->processColorLabel, colorpalette_icon); 
                          lv_obj_set_style_text_font(newProcess->process.processDetails->processColorLabel, &FilMachineFontIcons_30, 0);              
                          lv_obj_align(newProcess->process.processDetails->processColorLabel, LV_ALIGN_LEFT_MID, -5, 0);
                          lv_obj_add_flag(newProcess->process.processDetails->processColorLabel, LV_OBJ_FLAG_CLICKABLE);
                          lv_obj_add_event_cb(newProcess->process.processDetails->processColorLabel, event_processDetail, LV_EVENT_CLICKED, newProcess->process.processDetails->processColorLabel);


                          newProcess->process.processDetails->processBnWLabel = lv_label_create(newProcess->process.processDetails->processColorOrBnWContainer);         
                          lv_label_set_text(newProcess->process.processDetails->processBnWLabel, blackwhite_icon); 
                          lv_obj_set_style_text_font(newProcess->process.processDetails->processBnWLabel, &FilMachineFontIcons_30, 0);              
                          lv_obj_align(newProcess->process.processDetails->processBnWLabel, LV_ALIGN_LEFT_MID, 45, 0);
                          lv_obj_add_flag(newProcess->process.processDetails->processBnWLabel, LV_OBJ_FLAG_CLICKABLE);
                          lv_obj_add_event_cb(newProcess->process.processDetails->processBnWLabel, event_processDetail, LV_EVENT_CLICKED, newProcess->process.processDetails->processBnWLabel);


                  newProcess->process.processDetails->processPreferredLabel = lv_label_create(newProcess->process.processDetails->processInfoContainer);         
                  lv_label_set_text(newProcess->process.processDetails->processPreferredLabel, preferred_icon); 
                  lv_obj_set_style_text_font(newProcess->process.processDetails->processPreferredLabel, &FilMachineFontIcons_30, 0);              
                  lv_obj_align(newProcess->process.processDetails->processPreferredLabel, LV_ALIGN_TOP_LEFT, 120, 140);
                  lv_obj_add_flag(newProcess->process.processDetails->processPreferredLabel, LV_OBJ_FLAG_CLICKABLE);
                  lv_obj_add_event_cb(newProcess->process.processDetails->processPreferredLabel, event_processDetail, LV_EVENT_CLICKED, newProcess->process.processDetails->processPreferredLabel);



                  newProcess->process.processDetails->processSaveButton = lv_button_create(newProcess->process.processDetails->processDetailContainer);
                  lv_obj_set_size(newProcess->process.processDetails->processSaveButton, BUTTON_PROCESS_WIDTH, BUTTON_PROCESS_HEIGHT);
                  lv_obj_align(newProcess->process.processDetails->processSaveButton, LV_ALIGN_BOTTOM_RIGHT, -103, 10);
                  lv_obj_add_event_cb(newProcess->process.processDetails->processSaveButton, event_processDetail, LV_EVENT_REFRESH, NULL);
                  lv_obj_add_event_cb(newProcess->process.processDetails->processSaveButton, event_processDetail, LV_EVENT_CLICKED, newProcess->process.processDetails->processSaveButton);
                  lv_obj_set_style_bg_color(newProcess->process.processDetails->processSaveButton, lv_color_hex(BLUE_DARK), LV_PART_MAIN);
                  lv_obj_add_state(newProcess->process.processDetails->processSaveButton, LV_STATE_DISABLED);


                          newProcess->process.processDetails->processSaveLabel = lv_label_create(newProcess->process.processDetails->processSaveButton);         
                          lv_label_set_text(newProcess->process.processDetails->processSaveLabel, save_Icon); 
                          lv_obj_set_style_text_font(newProcess->process.processDetails->processSaveLabel, &FilMachineFontIcons_30, 0);              
                          lv_obj_align(newProcess->process.processDetails->processSaveLabel, LV_ALIGN_CENTER, 0, 0);

                  
                  newProcess->process.processDetails->processRunButton = lv_button_create(newProcess->process.processDetails->processDetailContainer);
                  lv_obj_set_size(newProcess->process.processDetails->processRunButton, BUTTON_PROCESS_WIDTH, BUTTON_PROCESS_HEIGHT);
                  lv_obj_align(newProcess->process.processDetails->processRunButton, LV_ALIGN_BOTTOM_RIGHT, 10, 10);
                  lv_obj_add_event_cb(newProcess->process.processDetails->processRunButton, event_processDetail, LV_EVENT_CLICKED, newProcess->process.processDetails->processRunButton);
                  lv_obj_set_style_bg_color(newProcess->process.processDetails->processRunButton, lv_color_hex(GREEN_DARK), LV_PART_MAIN);
                  if(newProcess->process.processDetails->stepElementsList.size == 0)
                      lv_obj_add_state(newProcess->process.processDetails->processRunButton, LV_STATE_DISABLED);
                  else
                      lv_obj_clear_state(newProcess->process.processDetails->processRunButton, LV_STATE_DISABLED);
                 
                          newProcess->process.processDetails->processRunLabel = lv_label_create(newProcess->process.processDetails->processRunButton);         
                          lv_label_set_text(newProcess->process.processDetails->processRunLabel, play_Icon); 
                          lv_obj_set_style_text_font(newProcess->process.processDetails->processRunLabel, &FilMachineFontIcons_30, 0);              
                          lv_obj_align(newProcess->process.processDetails->processRunLabel, LV_ALIGN_CENTER, 0, 0);
                          lv_obj_add_flag(newProcess->process.processDetails->processRunLabel, LV_OBJ_FLAG_CLICKABLE);


            if(getProcElementEntryByObject(processContainer) != NULL){
              LV_LOG_USER("Process already exist with address 0x%p with n:%d steps", getProcElementEntryByObject(processContainer),getProcElementEntryByObject(processContainer)->process.processDetails->stepElementsList.size);
              stepNode	*currentNode = (processNode*) allocateAndInitializeNode(PROCESS_NODE);
              currentNode = getProcElementEntryByObject(processContainer)->process.processDetails->stepElementsList.start;

              while(currentNode){
                  stepElementCreate(currentNode, newProcess ,currentNode->step.stepDetails->stepNameString, currentNode->step.stepDetails->timeMins, currentNode->step.stepDetails->timeSecs, currentNode->step.stepDetails->type);
                  currentNode = currentNode->next;
              } 
        }
}
