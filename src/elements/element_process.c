/**
 * @file element_process.c
 *
 */


//ESSENTIAL INCLUDES
#include <lvgl.h>
#include "../../include/definitions.h"

extern struct gui_components gui;

//ACCESSORY INCLUDES



/******************************
*  LINKED LIST IMPLEMENTATION
******************************/

processNode *addProcessElement(processNode	*processToAdd) {
	if( gui.page.processes.processElementsList.size == MAX_PROC_ELEMENTS ) {
    messagePopupCreate(warningPopupTitle_text, maxNumberEntryProcessPopupBody_text, NULL, NULL, NULL);
    return NULL;
    }		// Put some limit on things!

  LV_LOG_USER("Processes available %d first",gui.page.processes.processElementsList.size);
      if( gui.page.processes.processElementsList.start == NULL) {					/* Deals with the first entry */
        gui.page.processes.processElementsList.start = processToAdd;
        processToAdd->prev = NULL;
      } else {
        gui.page.processes.processElementsList.end->next = processToAdd;				/* Do this after the first */
        processToAdd->prev = gui.page.processes.processElementsList.end;
      }
      gui.page.processes.processElementsList.end = processToAdd;
      gui.page.processes.processElementsList.end->next = NULL;
      gui.page.processes.processElementsList.size++;

      LV_LOG_USER("Processes available %d after",gui.page.processes.processElementsList.size -1); 
      return processToAdd;
}



bool deleteProcessElement( processNode	*processToDelete ) {

	processNode 	*adjust_y_ptr = NULL;
	lv_coord_t		container_y_prev, container_y_new ;

	if( processToDelete ) {
		adjust_y_ptr = processToDelete->next;
		container_y_prev = processToDelete->process.container_y;
		if( processToDelete == gui.page.processes.processElementsList.start ) {
			if( processToDelete->next ) {
				gui.page.processes.processElementsList.start = processToDelete->next;
			} else gui.page.processes.processElementsList.start = gui.page.processes.processElementsList.end = NULL;

		} else if( processToDelete == gui.page.processes.processElementsList.end ) {

			if( processToDelete->prev ) {		// Check the end is not the beginning!
				processToDelete->prev->next = NULL;
				gui.page.processes.processElementsList.end = processToDelete->prev;
			}

		} else if( processToDelete->prev ) {
			processToDelete->prev->next = processToDelete->next;	// Re-join the linked list if not at beginning
			processToDelete->next->prev = processToDelete->prev;
		}

		while( adjust_y_ptr ) {
			if( adjust_y_ptr->next ) container_y_new = adjust_y_ptr->process.container_y;
			adjust_y_ptr->process.container_y = container_y_prev;
			lv_obj_set_y(adjust_y_ptr->process.processElement, adjust_y_ptr->process.container_y);
			if( adjust_y_ptr->next ) container_y_prev = container_y_new;
			adjust_y_ptr = adjust_y_ptr->next;
		}
	  if(processToDelete->process.processDetails->stepElementsList.size > 0) {   // If there are steps defined free them too

       while(processToDelete->process.processDetails->stepElementsList.start != NULL) {

            deleteStepElement( processToDelete->process.processDetails->stepElementsList.start, processToDelete , true);
      }
    }
		lv_obj_delete_async( processToDelete->process.processElement );			// Delete all LVGL objects associated with entry
  	free( processToDelete );												                    // Free the list entry itself
		gui.page.processes.processElementsList.size--;

    LV_LOG_USER("Processes available %d",gui.page.processes.processElementsList.size); 
		return true;
	}
	return false;
}

processNode* getProcElementEntryByObject(lv_obj_t* obj) {

    processNode* currentNode = gui.page.processes.processElementsList.start;

    while( currentNode !=  NULL ) {
        // Check all objects if any match, return element pointer, not styles!
        if( obj == currentNode->process.processElement || 
            obj == currentNode->process.preferredIcon || 
            obj == currentNode->process.processElementSummary || 
            obj == currentNode->process.processName || 
            obj == currentNode->process.processTemp || 
            obj == currentNode->process.processTempIcon || 
            obj == currentNode->process.processTime || 
            obj == currentNode->process.processTimeIcon || 
            obj == currentNode->process.processTypeIcon ||
            obj == currentNode->process.deleteButton ||
            obj == currentNode->process.deleteButtonLabel ||
            obj == (lv_obj_t*)currentNode) {
              break;
        }
        currentNode = currentNode->next;
    }
    return currentNode;   // Will Return NULL if no matching processNode is found
}

static bool deleteProcessElementByObj( lv_obj_t *obj ) {

	processNode	*proc_ptr  = getProcElementEntryByObject( obj );

	return deleteProcessElement( proc_ptr );
}





/******************************
*  LVGL ELEMENTS IMPLEMENTATION
******************************/

/*
void event_processElement(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = (lv_obj_t *)lv_event_get_target(e);
    lv_obj_t * cont = (lv_obj_t *)lv_event_get_current_target(e);
    lv_obj_t * data = (lv_obj_t *)lv_event_get_user_data(e);
    processNode *currentNode = getProcElementEntryByObject(data);
    lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_active());

    int8_t x;

    if (currentNode == NULL) {
        LV_LOG_USER("Bad object passed to eventProcessElement!"); // This will stop a crash
        return;                                                    // if something is wrong
    }

    if (code == LV_EVENT_RELEASED) {
      LV_LOG_USER("LV_EVENT_RELEASED");
      if(currentNode->process.gestureHandled == true && currentNode->process.swipedLeft == 1){
        currentNode->process.gestureHandled = false;
        return;
      }
    }

    if (code == LV_EVENT_GESTURE && currentNode->process.longPressHandled == false) {
        currentNode->process.gestureHandled = true; // Imposta il flag quando un gesto viene gestito

        switch (dir) {
            case LV_DIR_LEFT:
                if (currentNode->process.swipedLeft == 0 && currentNode->process.swipedRight == 1) {
                    LV_LOG_USER("Left gesture to return");
                    x = lv_obj_get_x_aligned(obj) - 50;
                    lv_obj_set_pos(obj, x, lv_obj_get_y_aligned(obj));
                    currentNode->process.swipedRight = 0;
                    currentNode->process.swipedLeft = 1;
                    lv_obj_add_flag(currentNode->process.deleteButton, LV_OBJ_FLAG_HIDDEN);
                }
                break;

            case LV_DIR_RIGHT:
                if (currentNode->process.swipedLeft == 1 && currentNode->process.swipedRight == 0) {
                    LV_LOG_USER("Right gesture for delete");
                    x = lv_obj_get_x_aligned(obj) + 50;
                    lv_obj_set_pos(obj, x, lv_obj_get_y_aligned(obj));
                    currentNode->process.swipedRight = 1;
                    currentNode->process.swipedLeft = 0;
                    lv_obj_remove_flag(currentNode->process.deleteButton, LV_OBJ_FLAG_HIDDEN);
                }
                break;
        }
    } 
    if (code == LV_EVENT_CLICKED && currentNode->process.longPressHandled == false) {
         LV_LOG_USER("LV_EVENT_CLICKED");


        if (obj == currentNode->process.preferredIcon) {
            if (lv_color_eq(lv_obj_get_style_text_color(currentNode->process.preferredIcon, LV_PART_MAIN), lv_color_hex(RED))) {
                lv_obj_set_style_text_color(currentNode->process.preferredIcon, lv_color_hex(WHITE), LV_PART_MAIN);
                currentNode->process.processDetails->isPreferred = 0;
            } else {
                lv_obj_set_style_text_color(currentNode->process.preferredIcon, lv_color_hex(RED), LV_PART_MAIN);
                currentNode->process.processDetails->isPreferred = 1;
            }
          LV_LOG_USER("Process is preferred : %d", currentNode->process.processDetails->isPreferred);
          if(gui.page.processes.isFiltered == 1)
            filterAndDisplayProcesses();
            qSysAction( SAVE_PROCESS_CONFIG );
        }

        if (obj == currentNode->process.deleteButton) {
            if (gui.element.messagePopup.mBoxPopupParent == NULL) {
                LV_LOG_USER("Process Element click for popup delete");
                gui.tempProcessNode = currentNode;
                messagePopupCreate(deletePopupTitle_text, deletePopupBody_text, deleteButton_text, stepDetailCancel_text, currentNode);
            }
        }

        if (obj == currentNode->process.processElementSummary && currentNode->process.swipedLeft == 1) {
            LV_LOG_USER("Process Element Details address %p", currentNode);
            gui.tempProcessNode = currentNode;
            processDetail(currentNode->process.processDetails->processesContainer); // currentNode
        }
    }
    if (code == LV_EVENT_LONG_PRESSED && currentNode->process.swipedLeft == 1) {
        LV_LOG_USER("LV_EVENT_LONG_PRESSED");
        currentNode->process.longPressHandled = true;
        gui.tempProcessNode = currentNode;    
        LV_LOG_USER("Duplicate process element %p", currentNode);
        messagePopupCreate(duplicatePopupTitle_text, duplicateProcessPopupBody_text, checkupNo_text, checkupYes_text, currentNode);  
     }

    if (code == LV_EVENT_DELETE) {
        lv_style_reset(&currentNode->process.processStyle);
    }
}
*/

void event_processElement(lv_event_t *e) {
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = (lv_obj_t *)lv_event_get_target(e);
    lv_obj_t *cont = (lv_obj_t *)lv_event_get_current_target(e);
    lv_obj_t *data = (lv_obj_t *)lv_event_get_user_data(e);
    processNode *currentNode = getProcElementEntryByObject(data);
    lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_active());

    int8_t x;

    static bool ignore_click = false;  // Flag to ignore click

    if (currentNode == NULL) {
        LV_LOG_USER("Bad object passed to eventProcessElement!");
        return;
    }

    if (code == LV_EVENT_RELEASED) {
        LV_LOG_USER("LV_EVENT_RELEASED");
        if (currentNode->process.gestureHandled == true && currentNode->process.swipedLeft == true) {
            currentNode->process.gestureHandled = false;
            return;
        }
        ignore_click = false;  // Reset ignore click flag on release
    }

    if (code == LV_EVENT_GESTURE && currentNode->process.longPressHandled == false) {
        currentNode->process.gestureHandled = true;

        switch (dir) {
            case LV_DIR_LEFT:
                if (currentNode->process.swipedLeft == false && currentNode->process.swipedRight == true) {
                    LV_LOG_USER("Left gesture to return");
                    x = lv_obj_get_x_aligned(obj) - 50;
                    lv_obj_set_pos(obj, x, lv_obj_get_y_aligned(obj));
                    currentNode->process.swipedRight = false;
                    currentNode->process.swipedLeft = true;
                    lv_obj_add_flag(currentNode->process.deleteButton, LV_OBJ_FLAG_HIDDEN);
                    ignore_click = true;  // Set ignore click flag
                }
                break;

            case LV_DIR_RIGHT:
                if (currentNode->process.swipedLeft == true && currentNode->process.swipedRight == false) {
                    LV_LOG_USER("Right gesture for delete");
                    x = lv_obj_get_x_aligned(obj) + 50;
                    lv_obj_set_pos(obj, x, lv_obj_get_y_aligned(obj));
                    currentNode->process.swipedRight = true;
                    currentNode->process.swipedLeft = false;
                    lv_obj_remove_flag(currentNode->process.deleteButton, LV_OBJ_FLAG_HIDDEN);
                    ignore_click = true;  // Set ignore click flag
                }
                break;
        }
    }

    if (code == LV_EVENT_SHORT_CLICKED && currentNode->process.longPressHandled == false && !ignore_click) {
        LV_LOG_USER("LV_EVENT_CLICKED");

        if (obj == currentNode->process.preferredIcon) {
            if (lv_color_eq(lv_obj_get_style_text_color(currentNode->process.preferredIcon, LV_PART_MAIN), lv_color_hex(RED))) {
                lv_obj_set_style_text_color(currentNode->process.preferredIcon, lv_color_hex(WHITE), LV_PART_MAIN);
                currentNode->process.processDetails->isPreferred = false;
            } else {
                lv_obj_set_style_text_color(currentNode->process.preferredIcon, lv_color_hex(RED), LV_PART_MAIN);
                currentNode->process.processDetails->isPreferred = true;
            }
            LV_LOG_USER("Process is preferred: %d", currentNode->process.processDetails->isPreferred);
            if (gui.page.processes.isFiltered == true)
                filterAndDisplayProcesses();
            qSysAction(SAVE_PROCESS_CONFIG);
        }

        if (obj == currentNode->process.deleteButton) {
            if (gui.element.messagePopup.mBoxPopupParent == NULL) {
                LV_LOG_USER("Process Element click for popup delete");
                gui.tempProcessNode = currentNode;
                messagePopupCreate(deletePopupTitle_text, deletePopupBody_text, deleteButton_text, stepDetailCancel_text, currentNode);
            }
        }

        if (obj == currentNode->process.processElementSummary && currentNode->process.swipedLeft == true) {
            LV_LOG_USER("Process Element Details address %p", currentNode);
            gui.tempProcessNode = currentNode;
            processDetail(currentNode->process.processDetails->processesContainer); // currentNode
        }
    }

    if (code == LV_EVENT_LONG_PRESSED && currentNode->process.swipedLeft == true) {
        LV_LOG_USER("LV_EVENT_LONG_PRESSED");
        currentNode->process.longPressHandled = true;
        gui.tempProcessNode = currentNode;
        LV_LOG_USER("Duplicate process element %p", currentNode);
        messagePopupCreate(duplicatePopupTitle_text, duplicateProcessPopupBody_text, checkupNo_text, checkupYes_text, currentNode);
    }

    if(code == LV_EVENT_REFRESH){
      if(gui.page.settings.settingsParams.tempUnit == CELSIUS_TEMP){
           lv_label_set_text_fmt(currentNode->process.processTemp, "%d°C", currentNode->process.processDetails->temp); 
      } else{
            lv_label_set_text_fmt(currentNode->process.processTemp, "%d°F", convertCelsiusoToFahrenheit(currentNode->process.processDetails->temp)); 
      }
    }

    if (code == LV_EVENT_DELETE) {
        lv_style_reset(&currentNode->process.processStyle);
    }
}


void processElementCreate(processNode *newProcess, int32_t tempSize) {
  gui.tempProcessNode = newProcess;

	if(newProcess->process.processStyle.values_and_props == NULL ) {		/* Only initialise the style once! */
		lv_style_init(&newProcess->process.processStyle);

		lv_style_set_bg_color(&newProcess->process.processStyle, lv_color_hex(GREY));
		lv_style_set_border_color(&newProcess->process.processStyle, lv_color_hex(GREEN_DARK));
		lv_style_set_border_width(&newProcess->process.processStyle, 4);
		lv_style_set_border_opa(&newProcess->process.processStyle, LV_OPA_50);
		lv_style_set_border_side(&newProcess->process.processStyle, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT);
		LV_LOG_USER("First call to processElementCreate style now initialised");
	}
  int32_t positionIndex;
  
  if(tempSize == -1)
    positionIndex = gui.page.processes.processElementsList.size; // New entry add to the end of the list
  else 
    positionIndex = tempSize; // Use the index position passed into the function
        
  LV_LOG_USER("Process size :%d",gui.page.processes.processElementsList.size);
 
  newProcess->process.swipedLeft = true;
  newProcess->process.swipedRight = false;
  newProcess->process.gestureHandled = false;

	newProcess->process.processElement = lv_obj_create(gui.page.processes.processesListContainer);
	newProcess->process.container_y = -10 + ((positionIndex - 1) * 70);
	lv_obj_set_pos(newProcess->process.processElement, -10, newProcess->process.container_y);
  lv_obj_set_size(newProcess->process.processElement, 365, 70);
	lv_obj_remove_flag(newProcess->process.processElement, LV_OBJ_FLAG_SCROLLABLE);
	lv_obj_set_style_border_opa(newProcess->process.processElement, LV_OPA_TRANSP, 0);
  lv_obj_add_event_cb(newProcess->process.processElement, event_processElement, LV_EVENT_GESTURE, newProcess);
  lv_obj_add_event_cb(newProcess->process.processElement, event_processElement, LV_EVENT_REFRESH, newProcess);
  lv_obj_add_event_cb(newProcess->process.processElement, event_processElement, LV_EVENT_RELEASED, newProcess);
  lv_obj_add_event_cb(newProcess->process.processElement, event_processElement, LV_EVENT_SHORT_CLICKED, newProcess);
  lv_obj_add_event_cb(newProcess->process.processElement, event_processElement, LV_EVENT_LONG_PRESSED, newProcess);

  lv_obj_remove_flag(newProcess->process.processElement, LV_OBJ_FLAG_GESTURE_BUBBLE);
  lv_obj_set_pos(newProcess->process.processElement,lv_obj_get_x_aligned(newProcess->process.processElement) - 50,lv_obj_get_y_aligned(newProcess->process.processElement));


  /*********************
	*    PAGE ELEMENTS			
	*********************/
        newProcess->process.deleteButton = lv_obj_create(newProcess->process.processElement);
        lv_obj_set_style_bg_color(newProcess->process.deleteButton, lv_color_hex(RED_DARK), LV_PART_MAIN);
        lv_obj_set_size(newProcess->process.deleteButton, 60, 70);
        lv_obj_align(newProcess->process.deleteButton, LV_ALIGN_TOP_LEFT, -16, -18);
        lv_obj_add_flag(newProcess->process.deleteButton, LV_OBJ_FLAG_HIDDEN);
        lv_obj_remove_flag(newProcess->process.deleteButton, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_add_event_cb(newProcess->process.deleteButton, event_processElement, LV_EVENT_SHORT_CLICKED, newProcess);

                newProcess->process.deleteButtonLabel = lv_label_create(newProcess->process.deleteButton);         
                lv_label_set_text(newProcess->process.deleteButtonLabel, trash_icon); 
                lv_obj_set_style_text_font(newProcess->process.deleteButtonLabel, &FilMachineFontIcons_30, 0);              
                lv_obj_align(newProcess->process.deleteButtonLabel, LV_ALIGN_CENTER, -5 , 0);


        newProcess->process.processElementSummary = lv_obj_create(newProcess->process.processElement);
        //lv_obj_set_style_border_color(proc_ptr->process.processElementSummary, lv_color_hex(LV_PALETTE_ORANGE), 0);
        lv_obj_set_size(newProcess->process.processElementSummary, 270, 66);
        lv_obj_align(newProcess->process.processElementSummary, LV_ALIGN_TOP_LEFT, 34, -16);
        lv_obj_remove_flag(newProcess->process.processElementSummary, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_add_flag(newProcess->process.processElementSummary, LV_OBJ_FLAG_EVENT_BUBBLE);
        lv_obj_add_style(newProcess->process.processElementSummary, &newProcess->process.processStyle, 0);

        newProcess->process.processName = lv_label_create(newProcess->process.processElementSummary);
        lv_label_set_text(newProcess->process.processName, newProcess->process.processDetails->processNameString);
        lv_obj_set_style_text_font(newProcess->process.processName, &lv_font_montserrat_22, 0);
        lv_label_set_long_mode(newProcess->process.processName, LV_LABEL_LONG_SCROLL_CIRCULAR);
        lv_obj_set_width(newProcess->process.processName, 220);
        lv_obj_align(newProcess->process.processName, LV_ALIGN_LEFT_MID, -10, -10);
        lv_obj_remove_flag(newProcess->process.processName, LV_OBJ_FLAG_SCROLLABLE);

        newProcess->process.processTempIcon = lv_label_create(newProcess->process.processElementSummary);
        lv_label_set_text(newProcess->process.processTempIcon, temp_icon);
        lv_obj_set_style_text_font(newProcess->process.processTempIcon, &FilMachineFontIcons_20, 0);
        //lv_obj_set_style_text_color(newProcess->process.tempIcon, lv_color_hex(GREY), LV_PART_MAIN);
        lv_obj_align(newProcess->process.processTempIcon, LV_ALIGN_LEFT_MID, -10, 17);

        newProcess->process.processTemp = lv_label_create(newProcess->process.processElementSummary);

        if(gui.page.settings.settingsParams.tempUnit == CELSIUS_TEMP){
           lv_label_set_text_fmt(newProcess->process.processTemp, "%d°C", newProcess->process.processDetails->temp); 
        } else{
            lv_label_set_text_fmt(newProcess->process.processTemp, "%d°F", convertCelsiusoToFahrenheit(newProcess->process.processDetails->temp)); 
        }
        
        lv_obj_set_style_text_font(newProcess->process.processTemp, &lv_font_montserrat_18, 0);
        lv_obj_align(newProcess->process.processTemp, LV_ALIGN_LEFT_MID, 7, 17);

        newProcess->process.processTimeIcon = lv_label_create(newProcess->process.processElementSummary);          
        lv_label_set_text(newProcess->process.processTimeIcon, clock_icon);                  
        lv_obj_set_style_text_font(newProcess->process.processTimeIcon, &FilMachineFontIcons_20, 0);
        //lv_obj_set_style_text_color(newStep->step.stepTimeIcon, lv_color_hex(GREY), LV_PART_MAIN);
        lv_obj_align(newProcess->process.processTimeIcon, LV_ALIGN_LEFT_MID, 65, 17);

        newProcess->process.processTime = lv_label_create(newProcess->process.processElementSummary);    
        lv_label_set_text_fmt(newProcess->process.processTime, "%dm%ds", newProcess->process.processDetails->timeMins, 
          newProcess->process.processDetails->timeSecs); 
        lv_obj_set_style_text_font(newProcess->process.processTime, &lv_font_montserrat_18, 0);              
        lv_obj_align(newProcess->process.processTime, LV_ALIGN_LEFT_MID, 87, 17);


        newProcess->process.processTypeIcon = lv_label_create(newProcess->process.processElementSummary);
        lv_label_set_text(newProcess->process.processTypeIcon, newProcess->process.processDetails->filmType == BLACK_AND_WHITE_FILM ? blackwhite_icon : colorpalette_icon);
        newProcess->process.processDetails->filmType = newProcess->process.processDetails->filmType;
        lv_obj_set_style_text_font(newProcess->process.processTypeIcon, &FilMachineFontIcons_30, 0);
        lv_obj_align(newProcess->process.processTypeIcon, LV_ALIGN_RIGHT_MID, 7, 0);
        
        newProcess->process.preferredIcon = lv_label_create(newProcess->process.processElement);
        lv_label_set_text(newProcess->process.preferredIcon, preferred_icon);
        lv_obj_add_flag(newProcess->process.preferredIcon, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_set_style_text_font(newProcess->process.preferredIcon, &FilMachineFontIcons_30, 0);
        lv_obj_set_style_text_color(newProcess->process.preferredIcon, lv_color_hex(WHITE), LV_PART_MAIN);
        lv_obj_align(newProcess->process.preferredIcon, LV_ALIGN_RIGHT_MID, 15, 0);
        
        if(newProcess->process.processDetails->isPreferred == true){
            lv_obj_set_style_text_color(newProcess->process.preferredIcon, lv_color_hex(RED), LV_PART_MAIN);
        }
        else{
            lv_obj_set_style_text_color(newProcess->process.preferredIcon, lv_color_hex(WHITE), LV_PART_MAIN);
        }
        
        lv_obj_add_event_cb(newProcess->process.preferredIcon, event_processElement, LV_EVENT_SHORT_CLICKED, newProcess);
}

