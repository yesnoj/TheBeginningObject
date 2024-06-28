
/**
 * @file element_messagePopup.c
 *
 */


//ESSENTIAL INCLUDES
#include <lvgl.h>
#include "../../include/definitions.h"

extern struct gui_components gui;

//ACCESSORY INCLUDES




void event_messagePopup(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = (lv_obj_t *)lv_event_get_target(e);
    lv_obj_t *objCont = (lv_obj_t *)lv_obj_get_parent(obj);
    lv_obj_t *mboxCont = (lv_obj_t *)lv_obj_get_parent(objCont);
    lv_obj_t *mboxParent = (lv_obj_t *)lv_obj_get_parent(mboxCont);

    lv_obj_t *btn = (lv_obj_t *)lv_event_get_current_target(e);
    lv_obj_t *mbox = (lv_obj_t *)lv_obj_get_parent(lv_obj_get_parent(btn));

    if (code == LV_EVENT_CLICKED)
    {
        if (obj == gui.element.messagePopup.mBoxPopupButtonClose)
        {
            LV_LOG_USER("Pressed gui.element.messagePopup.mBoxPopupButtonClose");
            lv_style_reset(&gui.element.messagePopup.style_mBoxPopupTitleLine);
            lv_msgbox_close(mboxCont);
            //lv_obj_delete(mboxCont);
        }
        if (obj == gui.element.messagePopup.mBoxPopupButton1)
        {
            LV_LOG_USER("Pressed gui.element.messagePopup.mBoxPopupButton1");
            if (gui.element.messagePopup.whoCallMe == gui.tempProcessNode && gui.tempProcessNode->process.swipedRight == true && gui.tempProcessNode->process.swipedLeft == false)//manage the delete by pressing button with gesture
            {
                LV_LOG_USER("Delete process!");

                if( !deleteProcessElement( gui.element.messagePopup.whoCallMe ) ) {
                  LV_LOG_USER("Delete process element instance at address 0x%p Failed!", gui.element.messagePopup.whoCallMe);
                } else {
                  LV_LOG_USER("Delete process element instance at address 0x%p", gui.element.messagePopup.whoCallMe);
                  qSysAction( SAVE_PROCESS_CONFIG );
                }

                lv_style_reset(&gui.element.messagePopup.style_mBoxPopupTitleLine);
                lv_msgbox_close(mboxCont);
//                lv_obj_delete(mboxCont);  // Not required! Can cause crash!
                gui.element.messagePopup.mBoxPopupParent = NULL;
            }
            if (gui.element.messagePopup.whoCallMe == gui.tempProcessNode && gui.tempProcessNode->process.swipedRight == false && gui.tempProcessNode->process.swipedLeft == true)//manage duplicate 
            {
                LV_LOG_USER("Cancel duplicate");
                lv_style_reset(&gui.element.messagePopup.style_mBoxPopupTitleLine);
                lv_msgbox_close(mboxCont);
                gui.element.messagePopup.mBoxPopupParent = NULL;
                gui.tempProcessNode->process.longPressHandled = false;
            }
            if (gui.element.messagePopup.whoCallMe == gui.tempStepNode)
            {
                LV_LOG_USER("Delete step!");
                
                if( !deleteStepElement( gui.element.messagePopup.whoCallMe , gui.tempProcessNode, false) ) {
                  LV_LOG_USER("Delete step element instance at address 0x%p Failed!", gui.element.messagePopup.whoCallMe);
                } else {
                  calculateTotalTime(gui.tempProcessNode);
                  LV_LOG_USER("Delete step element instance at address 0x%p", gui.element.messagePopup.whoCallMe);
                  gui.tempProcessNode->process.processDetails->somethingChanged = true;
                  lv_obj_send_event(gui.tempProcessNode->process.processDetails->processSaveButton, LV_EVENT_REFRESH, NULL);
                  gui.tempStepNode->step.longPressHandled = false;
                  qSysAction( SAVE_PROCESS_CONFIG );
                }
                
                lv_style_reset(&gui.element.messagePopup.style_mBoxPopupTitleLine);
                lv_msgbox_close(mboxCont);
                //lv_obj_delete(mboxCont);
                gui.element.messagePopup.mBoxPopupParent = NULL;
            }
            if (gui.element.messagePopup.whoCallMe == gui.tempProcessNode->process.processDetails->checkup->checkupStopNowButton)
            {
                LV_LOG_USER("Stop process NOW!");
                lv_style_reset(&gui.element.messagePopup.style_mBoxPopupTitleLine);
                lv_msgbox_close(mboxCont);
                //lv_obj_delete(mboxCont);
                gui.element.messagePopup.mBoxPopupParent = NULL;
                gui.tempProcessNode->process.processDetails->checkup->stopNow = true;
                gui.tempProcessNode->process.processDetails->checkup->stopAfter = false;
                lv_obj_add_state(gui.tempProcessNode->process.processDetails->checkup->checkupStopAfterButton, LV_STATE_DISABLED);  
                lv_obj_add_state(gui.tempProcessNode->process.processDetails->checkup->checkupStopNowButton, LV_STATE_DISABLED);  


                lv_label_set_text(gui.tempProcessNode->process.processDetails->checkup->checkupProcessCompleteLabel, checkupProcessStopped_text);
                lv_obj_remove_flag(gui.tempProcessNode->process.processDetails->checkup->checkupProcessCompleteLabel, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(gui.tempProcessNode->process.processDetails->checkup->checkupStepNameValue, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(gui.tempProcessNode->process.processDetails->checkup->checkupStepTimeLeftValue, LV_OBJ_FLAG_HIDDEN);
                lv_obj_add_flag(gui.tempProcessNode->process.processDetails->checkup->checkupProcessTimeLeftValue, LV_OBJ_FLAG_HIDDEN);

                gui.page.tools.machineStats.stopped++;
                if(gui.tempProcessNode->process.processDetails->checkup->processTimer != NULL)
                  lv_timer_delete(gui.tempProcessNode->process.processDetails->checkup->processTimer);
                lv_timer_resume(gui.tempProcessNode->process.processDetails->checkup->pumpTimer);
                qSysAction( SAVE_MACHINE_STATS );
            }
            if (gui.element.messagePopup.whoCallMe == gui.tempProcessNode->process.processDetails->checkup->checkupStopAfterButton)
            {
                LV_LOG_USER("Stop process AFTER!");
                lv_style_reset(&gui.element.messagePopup.style_mBoxPopupTitleLine);
                lv_msgbox_close(mboxCont);
                //lv_obj_delete(mboxCont);
                gui.element.messagePopup.mBoxPopupParent = NULL;

                gui.tempProcessNode->process.processDetails->checkup->stopAfter = true;
                lv_obj_add_state(gui.tempProcessNode->process.processDetails->checkup->checkupStopAfterButton, LV_STATE_DISABLED); 
                lv_obj_clear_state(gui.tempProcessNode->process.processDetails->checkup->checkupStopNowButton, LV_STATE_DISABLED);  
                
                gui.page.tools.machineStats.stopped++;
                qSysAction( SAVE_MACHINE_STATS );
            }
            if (gui.element.messagePopup.whoCallMe == &gui)
            {
                lv_style_reset(&gui.element.messagePopup.style_mBoxPopupTitleLine);
                lv_msgbox_close(mboxCont);
                //lv_obj_delete(mboxCont);
                gui.element.messagePopup.mBoxPopupParent = NULL;

                //if(gui.page.processes.processElementsList.size > 0){
                  LV_LOG_USER("Delete ALL PROCESS!"); 
                  lv_obj_clean(gui.page.processes.processesListContainer);
                  processList *processElementsList = &(gui.page.processes.processElementsList);
                  emptyList((void *)processElementsList, PROCESS_NODE);                     
                  //gui.page.processes.processElementsList.start = NULL;
                  //gui.page.processes.processElementsList.size = 0;
                  
                  qSysAction( SAVE_PROCESS_CONFIG );
            }
            if (gui.element.messagePopup.whoCallMe == gui.page.tools.toolsImportButton)
            {
                LV_LOG_USER("Cancel import from SD");
                lv_style_reset(&gui.element.messagePopup.style_mBoxPopupTitleLine);
                lv_msgbox_close(mboxCont);
                gui.element.messagePopup.mBoxPopupParent = NULL;
            }
        }
        if (obj == gui.element.messagePopup.mBoxPopupButton2)
        {
            LV_LOG_USER("Pressed gui.element.messagePopup.mBoxPopupButton2");
            if (gui.element.messagePopup.whoCallMe == gui.tempProcessNode || gui.element.messagePopup.whoCallMe == gui.tempStepNode || gui.element.messagePopup.whoCallMe == gui.tempProcessNode->process.processDetails->checkup->checkupStopAfterButton || gui.element.messagePopup.whoCallMe == gui.tempProcessNode->process.processDetails->checkup->checkupStopNowButton)
            {
                if(gui.element.messagePopup.whoCallMe == gui.tempStepNode && gui.tempStepNode->step.swipedLeft == false && gui.tempStepNode->step.swipedRight == true){
                    if(gui.tempStepNode->step.swipedLeft == false && gui.tempStepNode->step.swipedRight == true){
                      LV_LOG_USER("Cancel delete step element!");
                      uint32_t  x = lv_obj_get_x_aligned(gui.tempStepNode->step.stepElement) - 50;
                      uint32_t  y = lv_obj_get_y_aligned(gui.tempStepNode->step.stepElement);
                      lv_obj_set_pos(gui.tempStepNode->step.stepElement, x, y);
                      gui.tempStepNode->step.swipedLeft = false;
                      gui.tempStepNode->step.swipedRight = false;
                      lv_obj_add_flag(gui.tempStepNode->step.deleteButton, LV_OBJ_FLAG_HIDDEN);
                      lv_obj_add_flag(gui.tempStepNode->step.editButton, LV_OBJ_FLAG_HIDDEN);
                      gui.tempStepNode->step.longPressHandled = false;
                    }
                }
                if(gui.element.messagePopup.whoCallMe == gui.tempProcessNode && gui.tempProcessNode->process.swipedRight == true && gui.tempProcessNode->process.swipedLeft == false){
                    if(gui.tempProcessNode->process.swipedLeft == false && gui.tempProcessNode->process.swipedRight == true){
                      LV_LOG_USER("Cancel delete process element!");
                      uint32_t  x = lv_obj_get_x_aligned(gui.tempProcessNode->process.processElement) - 50;
                      uint32_t  y = lv_obj_get_y_aligned(gui.tempProcessNode->process.processElement);
                      lv_obj_set_pos(gui.tempProcessNode->process.processElement, x, y);
                      gui.tempProcessNode->process.swipedLeft = true;
                      gui.tempProcessNode->process.swipedRight = false;
                      lv_obj_add_flag(gui.tempProcessNode->process.deleteButton, LV_OBJ_FLAG_HIDDEN);
                      gui.tempProcessNode->process.gestureHandled = false;
                    }
                }

                if (gui.element.messagePopup.whoCallMe == gui.tempProcessNode && gui.tempProcessNode->process.swipedRight == false && gui.tempProcessNode->process.swipedLeft == true && gui.tempProcessNode->process.longPressHandled == true)
                {
                    LV_LOG_USER("Duplicate process");
                    char* newProcessName = generateRandomSuffix(gui.tempProcessNode->process.processDetails->processNameString);
                    LV_LOG_USER("New name %s",newProcessName);

                    struct processNode *duplicatedNode = deepCopyProcessNode(gui.element.messagePopup.whoCallMe);
                        if (duplicatedNode == NULL) {
                            fprintf(stderr, "Failed to allocate memory for duplicatedNode\n");
                            free(gui.element.messagePopup.whoCallMe);
                            return 1;
                        }

                        strncpy(duplicatedNode->process.processDetails->processNameString, newProcessName, sizeof(duplicatedNode->process.processDetails->processNameString) - 1);
                        duplicatedNode->process.processDetails->processNameString[sizeof(duplicatedNode->process.processDetails->processNameString) - 1] = '\0';

                        if(addProcessElement(duplicatedNode) != NULL){
                        LV_LOG_USER("Create GUI entry");
                        processElementCreate(duplicatedNode, -1);
                        qSysAction( SAVE_PROCESS_CONFIG );
                    }

                    duplicatedNode->process.longPressHandled = false;
                    ((processNode *)(gui.element.messagePopup.whoCallMe))->process.longPressHandled = false;
                    //gui.tempProcessNode->process.gestureHandled = false;
                    
                }

                lv_style_reset(&gui.element.messagePopup.style_mBoxPopupTitleLine);
                lv_msgbox_close(mboxCont);
                gui.element.messagePopup.mBoxPopupParent = NULL;
            }


            if (gui.element.messagePopup.whoCallMe == gui.page.tools.toolsImportButton)
            {
                lv_style_reset(&gui.element.messagePopup.style_mBoxPopupTitleLine);
                lv_msgbox_close(mboxCont);
                gui.element.messagePopup.mBoxPopupParent = NULL;

                  LV_LOG_USER("Import process from SD"); 
                  qSysAction( RELOAD_CFG );
            }
            if(gui.element.messagePopup.whoCallMe == &gui){
                LV_LOG_USER("Cancel delete all process!");
                lv_style_reset(&gui.element.messagePopup.style_mBoxPopupTitleLine);
                lv_msgbox_close(mboxCont);
                gui.element.messagePopup.mBoxPopupParent = NULL;
            }
        }
    }
}       

void messagePopupCreate(const char * popupTitleText,const char * popupText,const char * textButton1, const char * textButton2, void * whoCallMe){
  /*********************
  *    PAGE HEADER
  *********************/
   gui.element.messagePopup.whoCallMe = whoCallMe;

   LV_LOG_USER("Message popup create");  
   gui.element.messagePopup.mBoxPopupParent = lv_obj_class_create_obj(&lv_msgbox_backdrop_class, lv_layer_top());
   lv_obj_class_init_obj(gui.element.messagePopup.mBoxPopupParent);
   lv_obj_remove_flag(gui.element.messagePopup.mBoxPopupParent, LV_OBJ_FLAG_IGNORE_LAYOUT);
   lv_obj_set_size(gui.element.messagePopup.mBoxPopupParent, LV_PCT(100), LV_PCT(100));

         gui.element.messagePopup.mBoxPopupContainer = lv_obj_create(gui.element.messagePopup.mBoxPopupParent);
         lv_obj_align(gui.element.messagePopup.mBoxPopupContainer, LV_ALIGN_CENTER, 0, 0);
         lv_obj_set_size(gui.element.messagePopup.mBoxPopupContainer, 320, 240); 
         lv_obj_remove_flag(gui.element.messagePopup.mBoxPopupContainer, LV_OBJ_FLAG_SCROLLABLE); 

         gui.element.messagePopup.mBoxPopupTitle = lv_label_create(gui.element.messagePopup.mBoxPopupContainer);         
         lv_label_set_text(gui.element.messagePopup.mBoxPopupTitle, popupTitleText); 
         lv_obj_set_style_text_font(gui.element.messagePopup.mBoxPopupTitle, &lv_font_montserrat_22, 0);              
         lv_obj_align(gui.element.messagePopup.mBoxPopupTitle, LV_ALIGN_TOP_MID, 0, - 10);


   /*Create style*/
   lv_style_init(&gui.element.messagePopup.style_mBoxPopupTitleLine);
   lv_style_set_line_width(&gui.element.messagePopup.style_mBoxPopupTitleLine, 2);
   lv_style_set_line_rounded(&gui.element.messagePopup.style_mBoxPopupTitleLine, true);

   /*Create a line and apply the new style*/
   gui.element.messagePopup.mBoxPopupTitleLine = lv_line_create(gui.element.messagePopup.mBoxPopupContainer);
   lv_line_set_points(gui.element.messagePopup.mBoxPopupTitleLine, gui.element.messagePopup.titleLinePoints, 2);
   lv_obj_add_style(gui.element.messagePopup.mBoxPopupTitleLine, &gui.element.messagePopup.style_mBoxPopupTitleLine, 0);
   lv_obj_align(gui.element.messagePopup.mBoxPopupTitleLine, LV_ALIGN_TOP_MID, 0, 23);


  /*********************
  *    PAGE ELEMENTS
  *********************/

   gui.element.messagePopup.mBoxPopupTextContainer = lv_obj_create(gui.element.messagePopup.mBoxPopupContainer);
   lv_obj_align(gui.element.messagePopup.mBoxPopupTextContainer, LV_ALIGN_TOP_MID, 0, 30);
   lv_obj_set_size(gui.element.messagePopup.mBoxPopupTextContainer, 305, 180); 
   lv_obj_set_style_border_opa(gui.element.messagePopup.mBoxPopupTextContainer, LV_OPA_TRANSP, 0);
   //lv_obj_set_style_border_color(gui.element.messagePopup.mBoxPopupTextContainer, lv_color_hex(GREEN_DARK), 0);
   lv_obj_set_scroll_dir(gui.element.messagePopup.mBoxPopupTextContainer, LV_DIR_VER);

         gui.element.messagePopup.mBoxPopupText = lv_label_create(gui.element.messagePopup.mBoxPopupTextContainer);         
         lv_label_set_text(gui.element.messagePopup.mBoxPopupText, popupText); 
         lv_obj_set_style_text_font(gui.element.messagePopup.mBoxPopupText, &lv_font_montserrat_20, 0);              
         lv_obj_align(gui.element.messagePopup.mBoxPopupText, LV_ALIGN_CENTER, 0, -18);
         lv_obj_set_size(gui.element.messagePopup.mBoxPopupText, 295, LV_SIZE_CONTENT);
         lv_label_set_long_mode(gui.element.messagePopup.mBoxPopupText, LV_LABEL_LONG_WRAP);
         lv_obj_set_style_text_align(gui.element.messagePopup.mBoxPopupText , LV_TEXT_ALIGN_CENTER, 0);

  if(gui.element.messagePopup.whoCallMe == NULL){
      gui.element.messagePopup.mBoxPopupButtonClose = lv_button_create(gui.element.messagePopup.mBoxPopupContainer);
      lv_obj_set_size(gui.element.messagePopup.mBoxPopupButtonClose, BUTTON_POPUP_CLOSE_WIDTH, BUTTON_POPUP_CLOSE_HEIGHT);
      lv_obj_align(gui.element.messagePopup.mBoxPopupButtonClose, LV_ALIGN_TOP_RIGHT, 7 , -10);
      lv_obj_add_event_cb(gui.element.messagePopup.mBoxPopupButtonClose, event_messagePopup, LV_EVENT_CLICKED, gui.element.filterPopup.mBoxApplyFilterButton);

            gui.element.messagePopup.mBoxPopupButtonLabel = lv_label_create(gui.element.messagePopup.mBoxPopupButtonClose);         
            lv_label_set_text(gui.element.messagePopup.mBoxPopupButtonLabel, closePopup_icon); 
            lv_obj_set_style_text_font(gui.element.messagePopup.mBoxPopupButtonLabel, &FilMachineFontIcons_20, 0);              
            lv_obj_align(gui.element.messagePopup.mBoxPopupButtonLabel, LV_ALIGN_CENTER, 0, 0);
  }
  else{
      gui.element.messagePopup.mBoxPopupButton1 = lv_button_create(gui.element.messagePopup.mBoxPopupContainer);
      lv_obj_set_size(gui.element.messagePopup.mBoxPopupButton1, BUTTON_MBOX_WIDTH, BUTTON_MBOX_HEIGHT);
      lv_obj_align(gui.element.messagePopup.mBoxPopupButton1, LV_ALIGN_BOTTOM_LEFT, 10 , 10);
      lv_obj_add_event_cb(gui.element.messagePopup.mBoxPopupButton1, event_messagePopup, LV_EVENT_CLICKED, gui.element.messagePopup.mBoxPopupButton1);
      lv_obj_set_style_bg_color(gui.element.messagePopup.mBoxPopupButton1, lv_color_hex(RED_DARK), LV_PART_MAIN);

          gui.element.messagePopup.mBoxPopupButton1Label = lv_label_create(gui.element.messagePopup.mBoxPopupButton1);
          lv_label_set_text(gui.element.messagePopup.mBoxPopupButton1Label, textButton1);
          lv_obj_set_style_text_font(gui.element.messagePopup.mBoxPopupButton1Label, &lv_font_montserrat_24, 0);
          lv_obj_align(gui.element.messagePopup.mBoxPopupButton1Label, LV_ALIGN_CENTER, 0, 0);


      gui.element.messagePopup.mBoxPopupButton2 = lv_button_create(gui.element.messagePopup.mBoxPopupContainer);
      lv_obj_set_size(gui.element.messagePopup.mBoxPopupButton2, BUTTON_MBOX_WIDTH, BUTTON_MBOX_HEIGHT);
      lv_obj_align(gui.element.messagePopup.mBoxPopupButton2, LV_ALIGN_BOTTOM_RIGHT, - 10 , 10);
      lv_obj_add_event_cb(gui.element.messagePopup.mBoxPopupButton2, event_messagePopup, LV_EVENT_CLICKED, gui.element.messagePopup.mBoxPopupButton2);
      lv_obj_set_style_bg_color(gui.element.messagePopup.mBoxPopupButton2, lv_color_hex(GREEN_DARK), LV_PART_MAIN);

            gui.element.messagePopup.mBoxPopupButton2Label = lv_label_create(gui.element.messagePopup.mBoxPopupButton2);
            lv_label_set_text(gui.element.messagePopup.mBoxPopupButton2Label, textButton2);
            lv_obj_set_style_text_font(gui.element.messagePopup.mBoxPopupButton2Label, &lv_font_montserrat_24, 0);
            lv_obj_align(gui.element.messagePopup.mBoxPopupButton2Label, LV_ALIGN_CENTER, 0, 0);
  }
}

