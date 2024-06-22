
/**
 * @file page_processDetail.c
 *
 */


//ESSENTIAL INCLUDE
#include <lvgl.h>
#include "../../include/definitions.h"

extern struct gui_components gui;

//ACCESSORY INCLUDES

processNode *existingProcess;


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
    
    if(data == gui.tempProcessNode->process.processDetails->processDetailCloseButton){
        //gui.tempProcessNode->process.processDetails->stepElementsList.size = 0;
        //lv_obj_send_event(gui.tempProcessNode->process.processDetails->processSaveButton, LV_EVENT_REFRESH, NULL);
        //lv_msgbox_close(mboxCont);
//        lv_obj_delete(mboxCont);
        lv_obj_delete(mboxCont);
        //list of all styles to be reset, so clean the memory.
        lv_style_reset(&gui.tempProcessNode->process.processDetails->textAreaStyle);
        lv_scr_load(gui.page.menu.screen_mainMenu);

        LV_LOG_USER("Close Process Detail");
    }
    if(data == gui.tempProcessNode->process.processDetails->processColorLabel){
        lv_obj_set_style_text_color(gui.tempProcessNode->process.processDetails->processColorLabel, lv_color_hex(GREEN_DARK), LV_PART_MAIN);
        lv_obj_set_style_text_color(gui.tempProcessNode->process.processDetails->processBnWLabel, lv_color_hex(WHITE), LV_PART_MAIN);
        gui.tempProcessNode->process.processDetails->filmType = COLOR_FILM;
        gui.tempProcessNode->process.processDetails->somethingChanged = 1;

        lv_obj_send_event(gui.tempProcessNode->process.processDetails->processSaveButton, LV_EVENT_REFRESH, NULL);

        LV_LOG_USER("Pressed processColorLabel %d",gui.tempProcessNode->process.processDetails->filmType);
    }
    if(data == gui.tempProcessNode->process.processDetails->processBnWLabel){
        lv_obj_set_style_text_color(gui.tempProcessNode->process.processDetails->processBnWLabel, lv_color_hex(GREEN_DARK), LV_PART_MAIN);
        lv_obj_set_style_text_color(gui.tempProcessNode->process.processDetails->processColorLabel, lv_color_hex(WHITE), LV_PART_MAIN);
        gui.tempProcessNode->process.processDetails->filmType = BLACK_AND_WHITE_FILM;
        gui.tempProcessNode->process.processDetails->somethingChanged = 1;
        
        lv_obj_send_event(gui.tempProcessNode->process.processDetails->processSaveButton, LV_EVENT_REFRESH, NULL);    
        LV_LOG_USER("Pressed processBnWLabel %d",gui.tempProcessNode->process.processDetails->filmType);
    }

    if(data == gui.tempProcessNode->process.processDetails->processPreferredLabel){
			if(  lv_color_eq( lv_obj_get_style_text_color(gui.tempProcessNode->process.processDetails->processPreferredLabel, LV_PART_MAIN ), lv_color_hex(RED) ) ) {
				  lv_obj_set_style_text_color(gui.tempProcessNode->process.processDetails->processPreferredLabel, lv_color_hex(WHITE), LV_PART_MAIN);
				  gui.tempProcessNode->process.processDetails->isPreferred = 0;
          gui.tempProcessNode->process.processDetails->somethingChanged = 1;
			} else {
				lv_obj_set_style_text_color(gui.tempProcessNode->process.processDetails->processPreferredLabel, lv_color_hex(RED), LV_PART_MAIN);
				gui.tempProcessNode->process.processDetails->isPreferred = 1;
        gui.tempProcessNode->process.processDetails->somethingChanged = 1;
			}
      lv_obj_send_event(gui.tempProcessNode->process.processDetails->processSaveButton, LV_EVENT_REFRESH, NULL);
      //  LV_LOG_USER("Process is preferred :%d",isPreferred);
    }
    if(data == gui.tempProcessNode->process.processDetails->processSaveButton && gui.tempProcessNode->process.processDetails->stepElementsList.size > 0){
          gui.tempProcessNode->process.processDetails->somethingChanged = 0;
          
          strcpy( gui.tempProcessNode->process.processDetails->processNameString, lv_textarea_get_text(gui.tempProcessNode->process.processDetails->processDetailNameTextArea));
          lv_obj_clear_state(gui.tempProcessNode->process.processDetails->processRunButton, LV_STATE_DISABLED);
          lv_obj_clear_state(gui.tempProcessNode->process.processDetails->processDetailCloseButton, LV_STATE_DISABLED);
          lv_obj_add_state(gui.tempProcessNode->process.processDetails->processSaveButton, LV_STATE_DISABLED);

          if(isNodeInList((void*)&(gui.page.processes.processElementsList), gui.tempProcessNode, PROCESS_NODE) == NULL ) { 
            LV_LOG_USER("Process not in list");
            if(addProcessElement(gui.tempProcessNode) != NULL){
             LV_LOG_USER("Create GUI entry");
             processElementCreate(gui.tempProcessNode, -1);
            } else LV_LOG_USER("Process list is full");
          } else{
            LV_LOG_USER("Process element exists so edited" );
          }
          qSysAction( SAVE_PROCESS_CONFIG );
          
        updateProcessElement(gui.tempProcessNode);
        if(gui.page.processes.isFiltered == 1)
            filterAndDisplayProcesses();
        LV_LOG_USER("Pressed processSaveButton");
    }
    if(data == gui.tempProcessNode->process.processDetails->processSaveButton && gui.tempProcessNode->process.processDetails->stepElementsList.size == 0){
       lv_obj_add_state(gui.tempProcessNode->process.processDetails->processSaveButton, LV_STATE_DISABLED);
       lv_obj_add_state(gui.tempProcessNode->process.processDetails->processRunButton, LV_STATE_DISABLED);
       lv_obj_clear_state(gui.tempProcessNode->process.processDetails->processDetailCloseButton, LV_STATE_DISABLED);
    }

    if(data == gui.tempProcessNode->process.processDetails->processRunButton){
        //gui.tempProcessNode->process.processDetails->stepElementsList.size = 0;
//        lv_msgbox_close(mboxCont);
//        lv_obj_delete(mboxCont);
        LV_LOG_USER("Pressed processRunButton");
        gui.tempProcessNode->process.processDetails->checkup->checkupParent = NULL;
        lv_style_reset(&gui.tempProcessNode->process.processDetails->textAreaStyle);
        checkup(gui.tempProcessNode);
    }
    if(data == gui.tempProcessNode->process.processDetails->processNewStepButton){
        LV_LOG_USER("Pressed processNewStepButton");
        stepDetail(gui.tempProcessNode, NULL);
    }
  }


  if(code == LV_EVENT_REFRESH){
    
    if(obj == gui.tempProcessNode->process.processDetails->processSaveButton){
        if(gui.tempProcessNode->process.processDetails->stepElementsList.size > 0){
              lv_obj_clear_state(gui.tempProcessNode->process.processDetails->processSaveButton, LV_STATE_DISABLED);
              LV_LOG_USER("Updated SAVE button: ENABLED");
        }
        else{
              lv_obj_add_state(gui.tempProcessNode->process.processDetails->processSaveButton, LV_STATE_DISABLED);
              lv_obj_add_state(gui.tempProcessNode->process.processDetails->processRunButton, LV_STATE_DISABLED);
              LV_LOG_USER("Updated SAVE button : DISABLED");   
        }
        /*
        if(gui.tempProcessNode->process.processDetails->somethingChanged == 1 && gui.tempProcessNode->process.processDetails->stepElementsList.size == 0){
              lv_obj_clear_state(gui.tempProcessNode->process.processDetails->processSaveButton, LV_STATE_DISABLED);
              lv_obj_add_state(gui.tempProcessNode->process.processDetails->processDetailCloseButton, LV_STATE_DISABLED);
              lv_obj_add_state(gui.tempProcessNode->process.processDetails->processRunButton, LV_STATE_DISABLED);
              LV_LOG_USER("Updated SAVE button : ENABLED");
        }
        */
        if(gui.tempProcessNode->process.processDetails->somethingChanged == 1 && gui.tempProcessNode->process.processDetails->stepElementsList.size > 0){
              lv_obj_clear_state(gui.tempProcessNode->process.processDetails->processSaveButton, LV_STATE_DISABLED);
              lv_obj_add_state(gui.tempProcessNode->process.processDetails->processDetailCloseButton, LV_STATE_DISABLED);
              lv_obj_add_state(gui.tempProcessNode->process.processDetails->processRunButton, LV_STATE_DISABLED);
              LV_LOG_USER("Updated SAVE button : ENABLED");
        }
    }
  }

  if(code == LV_EVENT_VALUE_CHANGED) {
      
      if(data == gui.tempProcessNode->process.processDetails->processTempControlSwitch){
          LV_LOG_USER("Temperature controlled : %s", lv_obj_has_state(obj, LV_STATE_CHECKED) ? "On" : "Off");
          gui.tempProcessNode->process.processDetails->somethingChanged = 1;
          gui.tempProcessNode->process.processDetails->isTempControlled = lv_obj_has_state(obj, LV_STATE_CHECKED);
          lv_obj_send_event(gui.tempProcessNode->process.processDetails->processSaveButton, LV_EVENT_REFRESH, NULL);
      }
  }


  if(code == LV_EVENT_FOCUSED) { 
      
      if(data == gui.tempProcessNode->process.processDetails->processTempTextArea){
          LV_LOG_USER("Set Temperature");
          
          rollerPopupCreate(gui.element.rollerPopup.tempCelsiusOptions,tuneTempPopupTitle_text,gui.tempProcessNode->process.processDetails->processTempTextArea, findRolleStringIndex(lv_textarea_get_text(gui.tempProcessNode->process.processDetails->processTempTextArea),gui.element.rollerPopup.tempCelsiusOptions));
      }
      if(data == gui.tempProcessNode->process.processDetails->processToleranceTextArea){
          LV_LOG_USER("Set Tolerance");
          rollerPopupCreate(gui.element.rollerPopup.tempCelsiusToleranceOptions,tuneTempPopupTitle_text,gui.tempProcessNode->process.processDetails->processToleranceTextArea, findRolleStringIndex(lv_textarea_get_text(gui.tempProcessNode->process.processDetails->processToleranceTextArea),gui.element.rollerPopup.tempCelsiusToleranceOptions));
      }
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
  char formatted_string[20];

  processNode* existingProcess;


  existingProcess = (processNode*)isNodeInList((void*)&(gui.page.processes.processElementsList), gui.tempProcessNode, PROCESS_NODE);


if(existingProcess != NULL) {
    LV_LOG_USER("Process already present");
    gui.tempProcessNode = existingProcess; // Usa il nodo già presente anziché allocarne uno nuovo
    
  } else {
      gui.tempProcessNode = (processNode*)allocateAndInitializeNode(PROCESS_NODE);
      gui.tempProcessNode->process.processDetails->filmType = BLACK_AND_WHITE_FILM;
      gui.tempProcessNode->process.isFiltered = false;  
  }

  gui.tempProcessNode->process.processDetails->processesContainer = processContainer;
  
 
  LV_LOG_USER("Processes available %d",gui.page.processes.processElementsList.size);
  LV_LOG_USER("Process address 0x%p, with n:%d steps",gui.tempProcessNode, gui.tempProcessNode->process.processDetails->stepElementsList.size); 

  gui.tempProcessNode->process.processDetails->processDetailParent = lv_obj_create(NULL);
      lv_scr_load(gui.tempProcessNode->process.processDetails->processDetailParent);

  //gui.tempProcessNode->process.processDetails->processDetailParent = lv_obj_class_create_obj(&lv_msgbox_backdrop_class, lv_layer_top());
  //lv_obj_class_init_obj(gui.tempProcessNode->process.processDetails->processDetailParent);
  //lv_obj_remove_flag(gui.tempProcessNode->process.processDetails->processDetailParent, LV_OBJ_FLAG_IGNORE_LAYOUT);
  //lv_obj_set_size(gui.tempProcessNode->process.processDetails->processDetailParent, LV_PCT(100), LV_PCT(100));
  
  lv_style_init(&gui.tempProcessNode->process.processDetails->textAreaStyle);
  

      gui.tempProcessNode->process.processDetails->processDetailContainer = lv_obj_create(gui.tempProcessNode->process.processDetails->processDetailParent);
      lv_obj_align(gui.tempProcessNode->process.processDetails->processDetailContainer, LV_ALIGN_CENTER, 0, 0);
      lv_obj_set_size(gui.tempProcessNode->process.processDetails->processDetailContainer, LV_PCT(100), LV_PCT(100));
      lv_obj_remove_flag(gui.tempProcessNode->process.processDetails->processDetailContainer, LV_OBJ_FLAG_SCROLLABLE); 
            
            gui.tempProcessNode->process.processDetails->processDetailCloseButton = lv_button_create(gui.tempProcessNode->process.processDetails->processDetailContainer);
            lv_obj_set_size(gui.tempProcessNode->process.processDetails->processDetailCloseButton, BUTTON_POPUP_CLOSE_WIDTH * 1.2, BUTTON_POPUP_CLOSE_HEIGHT * 1.2);
            lv_obj_align(gui.tempProcessNode->process.processDetails->processDetailCloseButton, LV_ALIGN_TOP_RIGHT, 7 , -10);
            lv_obj_add_event_cb(gui.tempProcessNode->process.processDetails->processDetailCloseButton, event_processDetail, LV_EVENT_CLICKED, gui.tempProcessNode->process.processDetails->processDetailCloseButton);
            lv_obj_set_style_bg_color(gui.tempProcessNode->process.processDetails->processDetailCloseButton, lv_color_hex(GREEN_DARK), LV_PART_MAIN);
            lv_obj_move_foreground(gui.tempProcessNode->process.processDetails->processDetailCloseButton);


                  gui.tempProcessNode->process.processDetails->processDetailCloseButtonLabel = lv_label_create(gui.tempProcessNode->process.processDetails->processDetailCloseButton);         
                  lv_label_set_text(gui.tempProcessNode->process.processDetails->processDetailCloseButtonLabel, closePopup_icon); 
                  lv_obj_set_style_text_font(gui.tempProcessNode->process.processDetails->processDetailCloseButtonLabel, &FilMachineFontIcons_30, 0);              
                  lv_obj_align(gui.tempProcessNode->process.processDetails->processDetailCloseButtonLabel, LV_ALIGN_CENTER, 0, 0);
                  
        

            gui.tempProcessNode->process.processDetails->processDetailNameContainer = lv_obj_create(gui.tempProcessNode->process.processDetails->processDetailContainer);
            lv_obj_remove_flag(gui.tempProcessNode->process.processDetails->processDetailNameContainer, LV_OBJ_FLAG_SCROLLABLE); 
            lv_obj_align(gui.tempProcessNode->process.processDetails->processDetailNameContainer, LV_ALIGN_TOP_LEFT, -10, -15);
            lv_obj_set_size(gui.tempProcessNode->process.processDetails->processDetailNameContainer, 420, 50); 
            //lv_obj_set_style_border_color(gui.tempProcessNode->process.processDetails->processDetailNameContainer, lv_color_hex(GREEN_DARK), 0);
            lv_obj_set_style_border_opa(gui.tempProcessNode->process.processDetails->processDetailNameContainer, LV_OPA_TRANSP, 0);


                  gui.tempProcessNode->process.processDetails->processDetailNameTextArea = lv_textarea_create(gui.tempProcessNode->process.processDetails->processDetailNameContainer); 
                  lv_textarea_set_one_line(gui.tempProcessNode->process.processDetails->processDetailNameTextArea, true);
                  lv_textarea_set_placeholder_text(gui.tempProcessNode->process.processDetails->processDetailNameTextArea, processDetailPlaceHolder_text);
                  lv_obj_set_width(gui.tempProcessNode->process.processDetails->processDetailNameTextArea, 420);
                  lv_obj_set_style_text_font(gui.tempProcessNode->process.processDetails->processDetailNameTextArea, &lv_font_montserrat_30, 0);              
                  lv_obj_align(gui.tempProcessNode->process.processDetails->processDetailNameTextArea, LV_ALIGN_TOP_LEFT, -20, -25);
                  lv_obj_add_event_cb(gui.tempProcessNode->process.processDetails->processDetailNameTextArea, event_keyboard, LV_EVENT_CLICKED, NULL);
                  lv_obj_add_event_cb(gui.tempProcessNode->process.processDetails->processDetailNameTextArea, event_keyboard, LV_EVENT_DEFOCUSED, NULL);
                  lv_obj_add_event_cb(gui.tempProcessNode->process.processDetails->processDetailNameTextArea, event_keyboard, LV_EVENT_CANCEL, NULL);
                  lv_obj_add_event_cb(gui.tempProcessNode->process.processDetails->processDetailNameTextArea, event_keyboard, LV_EVENT_READY, NULL);
                  lv_obj_add_state(gui.tempProcessNode->process.processDetails->processDetailNameTextArea, LV_STATE_FOCUSED); 
                  lv_obj_set_style_border_opa(gui.tempProcessNode->process.processDetails->processDetailNameTextArea, LV_OPA_TRANSP, 0);
                  lv_textarea_set_max_length(gui.tempProcessNode->process.processDetails->processDetailNameTextArea, MAX_PROC_NAME_LEN);
                  //gui.tempProcessNode->process.processDetails->processNameString = generateRandomCharArray(10);
                  lv_textarea_set_text(gui.tempProcessNode->process.processDetails->processDetailNameTextArea, gui.tempProcessNode->process.processDetails->processNameString); 


            gui.tempProcessNode->process.processDetails->processDetailStepsLabel = lv_label_create(gui.tempProcessNode->process.processDetails->processDetailContainer);         
            lv_label_set_text(gui.tempProcessNode->process.processDetails->processDetailStepsLabel, processDetailStep_text); 
            lv_obj_set_width(gui.tempProcessNode->process.processDetails->processDetailStepsLabel, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(gui.tempProcessNode->process.processDetails->processDetailStepsLabel, &lv_font_montserrat_24, 0);              
            lv_obj_align(gui.tempProcessNode->process.processDetails->processDetailStepsLabel, LV_ALIGN_TOP_LEFT, 0, 25);

            gui.tempProcessNode->process.processDetails->processStepsContainer = lv_obj_create(gui.tempProcessNode->process.processDetails->processDetailContainer);
            lv_obj_align(gui.tempProcessNode->process.processDetails->processStepsContainer, LV_ALIGN_TOP_LEFT, -10, 55);
            lv_obj_set_size(gui.tempProcessNode->process.processDetails->processStepsContainer, 250, 240);
            lv_obj_set_scroll_dir(gui.tempProcessNode->process.processDetails->processStepsContainer, LV_DIR_VER);
            lv_obj_set_style_border_color(gui.tempProcessNode->process.processDetails->processStepsContainer, lv_palette_main(LV_PALETTE_GREEN), 0);
            //lv_obj_set_style_border_opa(gui.tempProcessNode->process.processDetails->processStepsContainer, LV_OPA_TRANSP, 0);


            gui.tempProcessNode->process.processDetails->processNewStepButton = lv_button_create(gui.tempProcessNode->process.processDetails->processDetailContainer);
            lv_obj_set_size(gui.tempProcessNode->process.processDetails->processNewStepButton, BUTTON_POPUP_CLOSE_WIDTH * 0.8, BUTTON_POPUP_CLOSE_HEIGHT * 0.8);
            lv_obj_align(gui.tempProcessNode->process.processDetails->processNewStepButton, LV_ALIGN_TOP_LEFT, 216 , 55);
            lv_obj_add_event_cb(gui.tempProcessNode->process.processDetails->processNewStepButton, event_processDetail, LV_EVENT_CLICKED, gui.tempProcessNode->process.processDetails->processNewStepButton);
            lv_obj_set_style_bg_color(gui.tempProcessNode->process.processDetails->processNewStepButton, lv_palette_main(LV_PALETTE_GREEN), LV_PART_MAIN);
            //lv_obj_move_foreground(gui.tempProcessNode->process.processDetails->processNewStepButton);
  
                    gui.tempProcessNode->process.processDetails->processNewStepLabel = lv_label_create(gui.tempProcessNode->process.processDetails->processNewStepButton);         
                    lv_label_set_text(gui.tempProcessNode->process.processDetails->processNewStepLabel, LV_SYMBOL_PLUS); 
                    lv_obj_set_style_text_font(gui.tempProcessNode->process.processDetails->processNewStepLabel, &lv_font_montserrat_20, 0);              
                    lv_obj_align(gui.tempProcessNode->process.processDetails->processNewStepLabel, LV_ALIGN_CENTER, 0, 0);


            gui.tempProcessNode->process.processDetails->processDetailInfoLabel = lv_label_create(gui.tempProcessNode->process.processDetails->processDetailContainer);         
            lv_label_set_text(gui.tempProcessNode->process.processDetails->processDetailInfoLabel, processDetailInfo_text); 
            lv_obj_set_width(gui.tempProcessNode->process.processDetails->processDetailInfoLabel, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(gui.tempProcessNode->process.processDetails->processDetailInfoLabel, &lv_font_montserrat_24, 0);              
            lv_obj_align(gui.tempProcessNode->process.processDetails->processDetailInfoLabel, LV_ALIGN_TOP_RIGHT, -105, 25);

            gui.tempProcessNode->process.processDetails->processInfoContainer = lv_obj_create(gui.tempProcessNode->process.processDetails->processDetailContainer);
            lv_obj_align(gui.tempProcessNode->process.processDetails->processInfoContainer, LV_ALIGN_TOP_LEFT, 245, 55);
            lv_obj_set_size(gui.tempProcessNode->process.processDetails->processInfoContainer, 210, 195);
            lv_obj_remove_flag(gui.tempProcessNode->process.processDetails->processInfoContainer, LV_OBJ_FLAG_SCROLLABLE); 
            //lv_obj_set_scroll_dir(gui.tempProcessNode->process.processDetails->processInfoContainer, LV_DIR_VER);
            lv_obj_set_style_border_color(gui.tempProcessNode->process.processDetails->processInfoContainer, lv_color_hex(WHITE), 0);
            //lv_obj_set_style_border_opa(gui.tempProcessNode->process.processDetails->processInfoContainer, LV_OPA_TRANSP, 0);

                  
                  gui.tempProcessNode->process.processDetails->processTempControlContainer = lv_obj_create(gui.tempProcessNode->process.processDetails->processInfoContainer);
                  lv_obj_remove_flag(gui.tempProcessNode->process.processDetails->processTempControlContainer, LV_OBJ_FLAG_SCROLLABLE); 
                  lv_obj_align(gui.tempProcessNode->process.processDetails->processTempControlContainer, LV_ALIGN_TOP_LEFT, -15, -17);
                  lv_obj_set_size(gui.tempProcessNode->process.processDetails->processTempControlContainer, 205, 40); 
                  //lv_obj_set_style_border_color(gui.tempProcessNode->process.processDetails->processTempControlContainer, lv_color_hex(GREEN_DARK), 0);
                  lv_obj_set_style_border_opa(gui.tempProcessNode->process.processDetails->processTempControlContainer, LV_OPA_TRANSP, 0);

                          gui.tempProcessNode->process.processDetails->processTempControlLabel = lv_label_create(gui.tempProcessNode->process.processDetails->processTempControlContainer);         
                          lv_label_set_text(gui.tempProcessNode->process.processDetails->processTempControlLabel, processDetailIsTempControl_text); 
                          lv_obj_set_style_text_font(gui.tempProcessNode->process.processDetails->processTempControlLabel, &lv_font_montserrat_16, 0);              
                          lv_obj_align(gui.tempProcessNode->process.processDetails->processTempControlLabel, LV_ALIGN_LEFT_MID, -15, 0);

                          gui.tempProcessNode->process.processDetails->processTempControlSwitch = lv_switch_create(gui.tempProcessNode->process.processDetails->processTempControlContainer);
                          lv_obj_add_event_cb(gui.tempProcessNode->process.processDetails->processTempControlSwitch, event_processDetail, LV_EVENT_VALUE_CHANGED, gui.tempProcessNode->process.processDetails->processTempControlSwitch);
                          lv_obj_align(gui.tempProcessNode->process.processDetails->processTempControlSwitch, LV_ALIGN_RIGHT_MID, 15, 0);
                          lv_obj_set_style_bg_color(gui.tempProcessNode->process.processDetails->processTempControlSwitch, lv_palette_darken(LV_PALETTE_GREY, 3), LV_STATE_DEFAULT);
                          lv_obj_set_style_bg_color(gui.tempProcessNode->process.processDetails->processTempControlSwitch,  lv_palette_main(LV_PALETTE_GREEN), LV_PART_KNOB | LV_STATE_DEFAULT);
                          lv_obj_set_style_bg_color(gui.tempProcessNode->process.processDetails->processTempControlSwitch, lv_color_hex(GREEN_DARK) , LV_PART_INDICATOR | LV_STATE_CHECKED);
                          lv_obj_add_state(gui.tempProcessNode->process.processDetails->processTempControlSwitch, gui.tempProcessNode->process.processDetails->isTempControlled);

                 
                  gui.tempProcessNode->process.processDetails->processTempContainer = lv_obj_create(gui.tempProcessNode->process.processDetails->processInfoContainer);
                  lv_obj_remove_flag(gui.tempProcessNode->process.processDetails->processTempContainer, LV_OBJ_FLAG_SCROLLABLE); 
                  lv_obj_align(gui.tempProcessNode->process.processDetails->processTempContainer, LV_ALIGN_TOP_LEFT, -15, 20);
                  lv_obj_set_size(gui.tempProcessNode->process.processDetails->processTempContainer, 205, 45); 
                  //lv_obj_set_style_border_color(gui.tempProcessNode->process.processDetails->processTempContainer, lv_color_hex(GREEN_DARK), 0);
                  lv_obj_set_style_border_opa(gui.tempProcessNode->process.processDetails->processTempContainer, LV_OPA_TRANSP, 0);   

                          gui.tempProcessNode->process.processDetails->processTempLabel = lv_label_create(gui.tempProcessNode->process.processDetails->processTempContainer);         
                          lv_label_set_text(gui.tempProcessNode->process.processDetails->processTempLabel, processDetailTemp_text); 
                          lv_obj_set_style_text_font(gui.tempProcessNode->process.processDetails->processTempLabel, &lv_font_montserrat_16, 0);              
                          lv_obj_align(gui.tempProcessNode->process.processDetails->processTempLabel, LV_ALIGN_LEFT_MID, -15, 0);

                          gui.tempProcessNode->process.processDetails->processTempTextArea = lv_textarea_create(gui.tempProcessNode->process.processDetails->processTempContainer);
                          lv_textarea_set_one_line(gui.tempProcessNode->process.processDetails->processTempTextArea, true);
                          lv_textarea_set_placeholder_text(gui.tempProcessNode->process.processDetails->processTempTextArea, processDetailTempPlaceHolder_text);
                          lv_obj_align(gui.tempProcessNode->process.processDetails->processTempTextArea, LV_ALIGN_LEFT_MID, 100, 0);
                          lv_obj_set_width(gui.tempProcessNode->process.processDetails->processTempTextArea, 60);
                          lv_obj_add_event_cb(gui.tempProcessNode->process.processDetails->processTempTextArea, event_processDetail, LV_EVENT_CLICKED, gui.tempProcessNode->process.processDetails->processTempTextArea);
                          lv_obj_add_event_cb(gui.tempProcessNode->process.processDetails->processTempTextArea, event_processDetail, LV_EVENT_REFRESH, gui.tempProcessNode->process.processDetails->processTempTextArea);
                          lv_obj_add_event_cb(gui.tempProcessNode->process.processDetails->processTempTextArea, event_processDetail, LV_EVENT_VALUE_CHANGED, gui.tempProcessNode->process.processDetails->processTempTextArea);
                          lv_obj_add_event_cb(gui.tempProcessNode->process.processDetails->processTempTextArea, event_processDetail, LV_EVENT_FOCUSED, gui.tempProcessNode->process.processDetails->processTempTextArea);
//                          lv_obj_add_state(gui.tempProcessNode->process.processDetails->processTempTextArea, LV_STATE_FOCUSED);
                          lv_obj_set_style_bg_color(gui.tempProcessNode->process.processDetails->processTempTextArea, lv_palette_darken(LV_PALETTE_GREY, 3), 0);
                          lv_obj_set_style_text_align(gui.tempProcessNode->process.processDetails->processTempTextArea , LV_TEXT_ALIGN_CENTER, 0);
                          lv_style_set_text_font(&gui.tempProcessNode->process.processDetails->textAreaStyle, &lv_font_montserrat_18);
                          lv_obj_add_style(gui.tempProcessNode->process.processDetails->processTempTextArea, &gui.tempProcessNode->process.processDetails->textAreaStyle, LV_PART_MAIN);
                          lv_obj_set_style_border_color(gui.tempProcessNode->process.processDetails->processTempTextArea, lv_color_hex(WHITE), 0);
                           

                          gui.tempProcessNode->process.processDetails->processTempUnitLabel = lv_label_create(gui.tempProcessNode->process.processDetails->processTempContainer);  
                          if(gui.page.settings.settingsParams.tempUnit == CELSIUS_TEMP){
                              lv_label_set_text(gui.tempProcessNode->process.processDetails->processTempUnitLabel, celsius_text); 
                              sprintf(formatted_string, "%d", gui.tempProcessNode->process.processDetails->temp);
                              lv_textarea_set_text(gui.tempProcessNode->process.processDetails->processTempTextArea, formatted_string);
                          } else{
                              lv_label_set_text(gui.tempProcessNode->process.processDetails->processTempUnitLabel, fahrenheit_text);
                              sprintf(formatted_string, "%d", convertCelsiusoToFahrenheit(gui.tempProcessNode->process.processDetails->temp));
                              lv_textarea_set_text(gui.tempProcessNode->process.processDetails->processTempTextArea, formatted_string);
                          }  
                          
                          lv_obj_set_style_text_font(gui.tempProcessNode->process.processDetails->processTempUnitLabel, &lv_font_montserrat_20, 0);              
                          lv_obj_align(gui.tempProcessNode->process.processDetails->processTempUnitLabel, LV_ALIGN_LEFT_MID, 160, 0);


                  gui.tempProcessNode->process.processDetails->processToleranceContainer = lv_obj_create(gui.tempProcessNode->process.processDetails->processInfoContainer);
                  lv_obj_remove_flag(gui.tempProcessNode->process.processDetails->processToleranceContainer, LV_OBJ_FLAG_SCROLLABLE); 
                  lv_obj_align(gui.tempProcessNode->process.processDetails->processToleranceContainer, LV_ALIGN_TOP_LEFT, -15, 65);
                  lv_obj_set_size(gui.tempProcessNode->process.processDetails->processToleranceContainer, 205, 45); 
                  //lv_obj_set_style_border_color(gui.tempProcessNode->process.processDetails->processToleranceContainer, lv_color_hex(GREEN_DARK), 0);
                  lv_obj_set_style_border_opa(gui.tempProcessNode->process.processDetails->processToleranceContainer, LV_OPA_TRANSP, 0);   

                          gui.tempProcessNode->process.processDetails->processToleranceLabel = lv_label_create(gui.tempProcessNode->process.processDetails->processToleranceContainer);         
                          lv_label_set_text(gui.tempProcessNode->process.processDetails->processToleranceLabel, processDetailTempTolerance_text); 
                          lv_obj_set_style_text_font(gui.tempProcessNode->process.processDetails->processToleranceLabel, &lv_font_montserrat_16, 0);              
                          lv_obj_align(gui.tempProcessNode->process.processDetails->processToleranceLabel, LV_ALIGN_LEFT_MID, -15, 0);

                          gui.tempProcessNode->process.processDetails->processToleranceTextArea = lv_textarea_create(gui.tempProcessNode->process.processDetails->processToleranceContainer);
                          lv_textarea_set_one_line(gui.tempProcessNode->process.processDetails->processToleranceTextArea, true);
                          lv_textarea_set_placeholder_text(gui.tempProcessNode->process.processDetails->processToleranceTextArea, processDetailTempPlaceHolder_text);
                          lv_obj_align(gui.tempProcessNode->process.processDetails->processToleranceTextArea, LV_ALIGN_LEFT_MID, 100, 0);
                          lv_obj_set_width(gui.tempProcessNode->process.processDetails->processToleranceTextArea, 60);

                          lv_obj_add_event_cb(gui.tempProcessNode->process.processDetails->processToleranceTextArea, event_processDetail, LV_EVENT_CLICKED, gui.tempProcessNode->process.processDetails->processToleranceTextArea);
                          lv_obj_add_event_cb(gui.tempProcessNode->process.processDetails->processToleranceTextArea, event_processDetail, LV_EVENT_REFRESH, gui.tempProcessNode->process.processDetails->processToleranceTextArea);
                          lv_obj_add_event_cb(gui.tempProcessNode->process.processDetails->processToleranceTextArea, event_processDetail, LV_EVENT_VALUE_CHANGED, gui.tempProcessNode->process.processDetails->processToleranceTextArea);
                          lv_obj_add_event_cb(gui.tempProcessNode->process.processDetails->processToleranceTextArea, event_processDetail, LV_EVENT_FOCUSED, gui.tempProcessNode->process.processDetails->processToleranceTextArea);
//                          lv_obj_add_state(gui.tempProcessNode->process.processDetails->processToleranceTextArea, LV_STATE_FOCUSED);
                          lv_obj_set_style_bg_color(gui.tempProcessNode->process.processDetails->processToleranceTextArea, lv_palette_darken(LV_PALETTE_GREY, 3), 0);
                          lv_obj_set_style_text_align(gui.tempProcessNode->process.processDetails->processToleranceTextArea , LV_TEXT_ALIGN_CENTER, 0);
                          lv_style_set_text_font(&gui.tempProcessNode->process.processDetails->textAreaStyle, &lv_font_montserrat_18);
                          lv_obj_add_style(gui.tempProcessNode->process.processDetails->processToleranceTextArea, &gui.tempProcessNode->process.processDetails->textAreaStyle, LV_PART_MAIN);
                          lv_obj_set_style_border_color(gui.tempProcessNode->process.processDetails->processToleranceTextArea, lv_color_hex(WHITE), 0);


                          gui.tempProcessNode->process.processDetails->processTempUnitLabel = lv_label_create(gui.tempProcessNode->process.processDetails->processToleranceContainer);         
                          lv_label_set_text(gui.tempProcessNode->process.processDetails->processTempUnitLabel, celsius_text); 
                          lv_obj_set_style_text_font(gui.tempProcessNode->process.processDetails->processTempUnitLabel, &lv_font_montserrat_20, 0);              
                          lv_obj_align(gui.tempProcessNode->process.processDetails->processTempUnitLabel, LV_ALIGN_LEFT_MID, 160, 0);
                          if(gui.page.settings.settingsParams.tempUnit == CELSIUS_TEMP){
                              lv_label_set_text(gui.tempProcessNode->process.processDetails->processTempUnitLabel, celsius_text); 
                              sprintf(formatted_string, "%d", gui.tempProcessNode->process.processDetails->tempTolerance);
                              lv_textarea_set_text(gui.tempProcessNode->process.processDetails->processToleranceTextArea, getRollerStringIndex(gui.tempProcessNode->process.processDetails->tempTolerance,gui.element.rollerPopup.tempCelsiusToleranceOptions));
                          } else{
                              lv_label_set_text(gui.tempProcessNode->process.processDetails->processTempUnitLabel, fahrenheit_text);
                              sprintf(formatted_string, "%d", convertCelsiusoToFahrenheit(gui.tempProcessNode->process.processDetails->tempTolerance));
                              lv_textarea_set_text(gui.tempProcessNode->process.processDetails->processToleranceTextArea, getRollerStringIndex(gui.tempProcessNode->process.processDetails->tempTolerance,gui.element.rollerPopup.tempCelsiusToleranceOptions));
                          }  


                  gui.tempProcessNode->process.processDetails->processTotalTimeContainer = lv_obj_create(gui.tempProcessNode->process.processDetails->processInfoContainer);
                  lv_obj_remove_flag(gui.tempProcessNode->process.processDetails->processTotalTimeContainer, LV_OBJ_FLAG_SCROLLABLE); 
                  lv_obj_align(gui.tempProcessNode->process.processDetails->processTotalTimeContainer, LV_ALIGN_TOP_LEFT, -15, 110);
                  lv_obj_set_size(gui.tempProcessNode->process.processDetails->processTotalTimeContainer, 205, 30); 
                  //lv_obj_set_style_border_color(gui.tempProcessNode->process.processDetails->processTotalTimeContainer, lv_color_hex(GREEN_DARK), 0);
                  lv_obj_set_style_border_opa(gui.tempProcessNode->process.processDetails->processTotalTimeContainer, LV_OPA_TRANSP, 0);   

                          gui.tempProcessNode->process.processDetails->processTotalTimeLabel = lv_label_create(gui.tempProcessNode->process.processDetails->processTotalTimeContainer);         
                          lv_label_set_text(gui.tempProcessNode->process.processDetails->processTotalTimeLabel, processDetailTotalTime_text); 
                          lv_obj_set_style_text_font(gui.tempProcessNode->process.processDetails->processTotalTimeLabel, &lv_font_montserrat_16, 0);              
                          lv_obj_align(gui.tempProcessNode->process.processDetails->processTotalTimeLabel, LV_ALIGN_LEFT_MID, -15, 0);

                          gui.tempProcessNode->process.processDetails->processTotalTimeValue = lv_label_create(gui.tempProcessNode->process.processDetails->processTotalTimeContainer);         
                          lv_obj_set_style_text_font(gui.tempProcessNode->process.processDetails->processTotalTimeValue, &lv_font_montserrat_20, 0);              
                          lv_obj_align(gui.tempProcessNode->process.processDetails->processTotalTimeValue, LV_ALIGN_LEFT_MID, 100, 0);   
//                          sprintf(formatted_string, "%dm%ds", gui.tempProcessNode->process.processDetails->timeMins, gui.tempProcessNode->process.processDetails->timeSecs);
                          lv_label_set_text_fmt(gui.tempProcessNode->process.processDetails->processTotalTimeValue, "%dm%ds", 
                            gui.tempProcessNode->process.processDetails->timeMins, gui.tempProcessNode->process.processDetails->timeSecs); 


                  gui.tempProcessNode->process.processDetails->processColorOrBnWContainer = lv_obj_create(gui.tempProcessNode->process.processDetails->processInfoContainer);
                  lv_obj_remove_flag(gui.tempProcessNode->process.processDetails->processColorOrBnWContainer, LV_OBJ_FLAG_SCROLLABLE); 
                  lv_obj_align(gui.tempProcessNode->process.processDetails->processColorOrBnWContainer, LV_ALIGN_TOP_LEFT, -18, 137);
                  lv_obj_set_size(gui.tempProcessNode->process.processDetails->processColorOrBnWContainer, 105, 40); 
                  lv_obj_set_style_border_color(gui.tempProcessNode->process.processDetails->processColorOrBnWContainer, lv_color_hex(WHITE), 0);
                  //lv_obj_set_style_border_opa(gui.tempProcessNode->process.processDetails->processColorOrBnWContainer, LV_OPA_TRANSP, 0);   


                          gui.tempProcessNode->process.processDetails->processColorLabel = lv_label_create(gui.tempProcessNode->process.processDetails->processColorOrBnWContainer);         
                          lv_label_set_text(gui.tempProcessNode->process.processDetails->processColorLabel, colorpalette_icon); 
                          lv_obj_set_style_text_font(gui.tempProcessNode->process.processDetails->processColorLabel, &FilMachineFontIcons_30, 0);              
                          lv_obj_align(gui.tempProcessNode->process.processDetails->processColorLabel, LV_ALIGN_LEFT_MID, 45, 0);
                          lv_obj_add_flag(gui.tempProcessNode->process.processDetails->processColorLabel, LV_OBJ_FLAG_CLICKABLE);
                          lv_obj_add_event_cb(gui.tempProcessNode->process.processDetails->processColorLabel, event_processDetail, LV_EVENT_CLICKED, gui.tempProcessNode->process.processDetails->processColorLabel);


                          gui.tempProcessNode->process.processDetails->processBnWLabel = lv_label_create(gui.tempProcessNode->process.processDetails->processColorOrBnWContainer);         
                          lv_label_set_text(gui.tempProcessNode->process.processDetails->processBnWLabel, blackwhite_icon); 
                          lv_obj_set_style_text_font(gui.tempProcessNode->process.processDetails->processBnWLabel, &FilMachineFontIcons_30, 0);              
                          lv_obj_align(gui.tempProcessNode->process.processDetails->processBnWLabel, LV_ALIGN_LEFT_MID, -5, 0);
                          lv_obj_add_flag(gui.tempProcessNode->process.processDetails->processBnWLabel, LV_OBJ_FLAG_CLICKABLE);
                          lv_obj_add_event_cb(gui.tempProcessNode->process.processDetails->processBnWLabel, event_processDetail, LV_EVENT_CLICKED, gui.tempProcessNode->process.processDetails->processBnWLabel);
                          if(gui.tempProcessNode->process.processDetails->filmType == COLOR_FILM){
                              lv_obj_set_style_text_color(gui.tempProcessNode->process.processDetails->processColorLabel, lv_color_hex(GREEN_DARK), LV_PART_MAIN);
                              lv_obj_set_style_text_color(gui.tempProcessNode->process.processDetails->processBnWLabel, lv_color_hex(WHITE), LV_PART_MAIN);
                          }
                          else if(gui.tempProcessNode->process.processDetails->filmType == BLACK_AND_WHITE_FILM){
                              lv_obj_set_style_text_color(gui.tempProcessNode->process.processDetails->processBnWLabel, lv_color_hex(GREEN_DARK), LV_PART_MAIN);
                              lv_obj_set_style_text_color(gui.tempProcessNode->process.processDetails->processColorLabel, lv_color_hex(WHITE), LV_PART_MAIN);
                          }
                          else if(gui.tempProcessNode->process.processDetails->filmType == FILM_TYPE_NA){
                              lv_obj_set_style_text_color(gui.tempProcessNode->process.processDetails->processBnWLabel, lv_color_hex(WHITE), LV_PART_MAIN);
                              lv_obj_set_style_text_color(gui.tempProcessNode->process.processDetails->processColorLabel, lv_color_hex(WHITE), LV_PART_MAIN);
                          }

                  gui.tempProcessNode->process.processDetails->processPreferredLabel = lv_label_create(gui.tempProcessNode->process.processDetails->processInfoContainer);         
                  lv_label_set_text(gui.tempProcessNode->process.processDetails->processPreferredLabel, preferred_icon); 
                  lv_obj_set_style_text_font(gui.tempProcessNode->process.processDetails->processPreferredLabel, &FilMachineFontIcons_30, 0);              
                  lv_obj_align(gui.tempProcessNode->process.processDetails->processPreferredLabel, LV_ALIGN_TOP_LEFT, 120, 140);
                  lv_obj_add_flag(gui.tempProcessNode->process.processDetails->processPreferredLabel, LV_OBJ_FLAG_CLICKABLE);
                  lv_obj_add_event_cb(gui.tempProcessNode->process.processDetails->processPreferredLabel, event_processDetail, LV_EVENT_CLICKED, gui.tempProcessNode->process.processDetails->processPreferredLabel);
                  if(gui.tempProcessNode->process.processDetails->isPreferred == 0){
                    lv_obj_set_style_text_color(gui.tempProcessNode->process.processDetails->processPreferredLabel, lv_color_hex(WHITE), LV_PART_MAIN);
                  }
                  else{
                    lv_obj_set_style_text_color(gui.tempProcessNode->process.processDetails->processPreferredLabel, lv_color_hex(RED), LV_PART_MAIN);
                  }

                  gui.tempProcessNode->process.processDetails->processSaveButton = lv_button_create(gui.tempProcessNode->process.processDetails->processDetailContainer);
                  lv_obj_set_size(gui.tempProcessNode->process.processDetails->processSaveButton, BUTTON_PROCESS_WIDTH, BUTTON_PROCESS_HEIGHT);
                  lv_obj_align(gui.tempProcessNode->process.processDetails->processSaveButton, LV_ALIGN_BOTTOM_RIGHT, -103, 10);
                  lv_obj_add_event_cb(gui.tempProcessNode->process.processDetails->processSaveButton, event_processDetail, LV_EVENT_REFRESH, NULL);
                  lv_obj_add_event_cb(gui.tempProcessNode->process.processDetails->processSaveButton, event_processDetail, LV_EVENT_CLICKED, gui.tempProcessNode->process.processDetails->processSaveButton);
                  lv_obj_set_style_bg_color(gui.tempProcessNode->process.processDetails->processSaveButton, lv_color_hex(BLUE_DARK), LV_PART_MAIN);
                  lv_obj_add_state(gui.tempProcessNode->process.processDetails->processSaveButton, LV_STATE_DISABLED);


                          gui.tempProcessNode->process.processDetails->processSaveLabel = lv_label_create(gui.tempProcessNode->process.processDetails->processSaveButton);         
                          lv_label_set_text(gui.tempProcessNode->process.processDetails->processSaveLabel, save_icon); 
                          lv_obj_set_style_text_font(gui.tempProcessNode->process.processDetails->processSaveLabel, &FilMachineFontIcons_30, 0);              
                          lv_obj_align(gui.tempProcessNode->process.processDetails->processSaveLabel, LV_ALIGN_CENTER, 0, 0);

                  
                  gui.tempProcessNode->process.processDetails->processRunButton = lv_button_create(gui.tempProcessNode->process.processDetails->processDetailContainer);
                  lv_obj_set_size(gui.tempProcessNode->process.processDetails->processRunButton, BUTTON_PROCESS_WIDTH, BUTTON_PROCESS_HEIGHT);
                  lv_obj_align(gui.tempProcessNode->process.processDetails->processRunButton, LV_ALIGN_BOTTOM_RIGHT, 10, 10);
                  lv_obj_add_event_cb(gui.tempProcessNode->process.processDetails->processRunButton, event_processDetail, LV_EVENT_CLICKED, gui.tempProcessNode->process.processDetails->processRunButton);
                  lv_obj_set_style_bg_color(gui.tempProcessNode->process.processDetails->processRunButton, lv_color_hex(GREEN_DARK), LV_PART_MAIN);
                  if(gui.tempProcessNode->process.processDetails->stepElementsList.size == 0)
                      lv_obj_add_state(gui.tempProcessNode->process.processDetails->processRunButton, LV_STATE_DISABLED);
                  else
                      lv_obj_clear_state(gui.tempProcessNode->process.processDetails->processRunButton, LV_STATE_DISABLED);
                 
                          gui.tempProcessNode->process.processDetails->processRunLabel = lv_label_create(gui.tempProcessNode->process.processDetails->processRunButton);         
                          lv_label_set_text(gui.tempProcessNode->process.processDetails->processRunLabel, play_icon); 
                          lv_obj_set_style_text_font(gui.tempProcessNode->process.processDetails->processRunLabel, &FilMachineFontIcons_30, 0);              
                          lv_obj_align(gui.tempProcessNode->process.processDetails->processRunLabel, LV_ALIGN_CENTER, 0, 0);
                          lv_obj_add_flag(gui.tempProcessNode->process.processDetails->processRunLabel, LV_OBJ_FLAG_CLICKABLE);

              
              if(gui.tempProcessNode->process.processDetails->stepElementsList.size > 0){
                stepNode *currentNode = gui.tempProcessNode->process.processDetails->stepElementsList.start;
                uint8_t tempSize = 1;
                while(currentNode != NULL){               
                  LV_LOG_USER("Adding to process with address 0x%p n:%d steps", gui.tempProcessNode,gui.tempProcessNode->process.processDetails->stepElementsList.size);
                  stepElementCreate(currentNode, gui.tempProcessNode, tempSize);
                  currentNode = currentNode->next;
                  tempSize ++;
                } 
            }
            /*
            if (gui.tempProcessNode != NULL) {
              stepNode *currentNode = gui.tempProcessNode->process.processDetails->stepElementsList.end; // Parti dall'ultimo nodo
              uint8_t tempSize = 0; // Inizia dalla dimensione - 1
              while (currentNode != NULL) {
                  LV_LOG_USER("Adding to process with address 0x%p n:%d steps", gui.tempProcessNode, gui.tempProcessNode->process.processDetails->stepElementsList.size);
                  stepElementCreate(currentNode, gui.tempProcessNode ,currentNode->step.stepDetails->stepNameString, currentNode->step.stepDetails->timeMins, currentNode->step.stepDetails->timeSecs, currentNode->step.stepDetails->type, tempSize);
                  currentNode = currentNode->prev; // Passa al nodo precedente
                  tempSize++;
              }
          }  */           
}
