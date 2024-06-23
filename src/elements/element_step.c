#include "misc/lv_palette.h"
#include <sys/_stdint.h>
#include "core/lv_obj_pos.h"


/**
 * @file element_step.c
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
stepNode *addStepElement(stepNode * stepToAdd, processNode * processReference) {
  if(processReference->process.processDetails->stepElementsList.size == MAX_STEP_ELEMENTS){
      messagePopupCreate(warningPopupTitle_text, maxNumberEntryStepsPopupBody_text, NULL, NULL, NULL);
      return NULL;
  }
	if(isNodeInList((void*)&(processReference->process.processDetails->stepElementsList), stepToAdd, STEP_NODE) != NULL) 
      return NULL;		// Put some limit on things!
	
  if(processReference->process.processDetails->stepElementsList.start == NULL) {					/* Deals with the first entry */
		processReference->process.processDetails->stepElementsList.start = stepToAdd;
		stepToAdd->prev = NULL;
	} else {
		processReference->process.processDetails->stepElementsList.end->next = stepToAdd;				/* Do this after the first */
		stepToAdd->prev = processReference->process.processDetails->stepElementsList.end;
	}
	processReference->process.processDetails->stepElementsList.end = stepToAdd;
	processReference->process.processDetails->stepElementsList.end->next = NULL;
	processReference->process.processDetails->stepElementsList.size++;
  
  LV_LOG_USER("stepElementsList.size: %d", processReference->process.processDetails->stepElementsList.size);

  processReference->process.processDetails->somethingChanged = 1;
  lv_obj_send_event(processReference->process.processDetails->processSaveButton, LV_EVENT_REFRESH, NULL);

  LV_LOG_USER("Process address 0x%p, with n:%d steps",processReference, processReference->process.processDetails->stepElementsList.size); 
	return stepToAdd;
}



bool deleteStepElement( stepNode	*stepToDelete, processNode * processReference , bool isDeleteProcess) {

	stepNode 	*adjust_y_ptr = NULL;
	lv_coord_t		container_y_prev, container_y_new ;

	if( stepToDelete ) {
    if(stepToDelete->step.stepElement != NULL ) {
		  adjust_y_ptr = stepToDelete->next;
		  container_y_prev = stepToDelete->step.container_y;
    }
		if( stepToDelete == processReference->process.processDetails->stepElementsList.start ) {
			if( stepToDelete->next ) {
				processReference->process.processDetails->stepElementsList.start = stepToDelete->next;
			} else processReference->process.processDetails->stepElementsList.start = processReference->process.processDetails->stepElementsList.end = NULL;

		} else if( stepToDelete == processReference->process.processDetails->stepElementsList.end ) {

			if( stepToDelete->prev ) {		// Check the end is not the beginning!
				stepToDelete->prev->next = NULL;
				processReference->process.processDetails->stepElementsList.end = stepToDelete->prev;
			}

		} else if( stepToDelete->prev ) {
			stepToDelete->prev->next = stepToDelete->next;	// Re-join the linked list if not at beginning
			stepToDelete->next->prev = stepToDelete->prev;
		}
    if(!isDeleteProcess){
      while( adjust_y_ptr) {
        if( adjust_y_ptr->next ) container_y_new = adjust_y_ptr->step.container_y;
        adjust_y_ptr->step.container_y = container_y_prev;
        lv_obj_set_y(adjust_y_ptr->step.stepElement, adjust_y_ptr->step.container_y);
        if( adjust_y_ptr->next ) container_y_prev = container_y_new;
        adjust_y_ptr = adjust_y_ptr->next;
      }
      /* Only delete all LVGL objects associated with entry if called from process detail screen */
      if(stepToDelete->step.stepElement) lv_obj_delete_async( stepToDelete->step.stepElement );
      /* Free the allocated memory for the list entry*/
      free( stepToDelete );	
      processReference->process.processDetails->stepElementsList.size--;  // Update list size
      lv_obj_send_event(processReference->process.processDetails->processSaveButton, LV_EVENT_REFRESH, NULL); // Refresh Screen and states

    LV_LOG_USER("Process address %p, with n:%d steps",processReference, processReference->process.processDetails->stepElementsList.size); 
    }
		return true;
	}
	return false;
}

stepNode *getStepElementEntryByObject(lv_obj_t *obj, processNode *processReference) {
  
	stepNode	*currentNode  = processReference->process.processDetails->stepElementsList.start;

	while( currentNode != NULL ) {
		if( obj == currentNode->step.stepElement ||				// Check all objects if any match return element pointer, not styles! 
        obj == currentNode->step.stepElementSummary ||
        obj == currentNode->step.stepName ||
        obj == currentNode->step.stepTime ||
        obj == currentNode->step.stepTimeIcon ||
        obj == currentNode->step.stepTypeIcon ||
        obj == currentNode->step.discardAfterIcon ||
        obj == currentNode->step.sourceLabel ||
        obj == currentNode->step.deleteButton ||
        obj == currentNode->step.deleteButtonLabel ||
        obj == currentNode->step.editButton ||   
        obj == currentNode->step.editButtonLabel ||
        obj == (lv_obj_t*)currentNode ) {
           break;
    }
		currentNode = currentNode->next;
	}
  return currentNode;   // Will Return NULL if no matching stepNode is found
}

static bool deleteStepElementByObj( lv_obj_t *obj, processNode * processReference ) {

	stepNode	*step_ptr  = getStepElementEntryByObject(obj,processReference);

	return deleteStepElement(step_ptr,processReference, false);
}


/******************************
*  LVGL ELEMENTS IMPLEMENTATION
******************************/

void removeStepElementFromList(processNode *data, stepNode *node) {
    // Rimuovi l'elemento dalla lista
    if (node->prev) {
        node->prev->next = node->next;
    } else {
        data->process.processDetails->stepElementsList.start = node->next;
    }
    if (node->next) {
        node->next->prev = node->prev;
    } else {
        data->process.processDetails->stepElementsList.end = node->prev;
    }

    // Aggiorna la dimensione della lista
    data->process.processDetails->stepElementsList.size--;

    // Pulizia dei puntatori del nodo
    node->prev = NULL;
    node->next = NULL;
}

void insertStepElementAfter(processNode *data, stepNode *afterNode, stepNode *node) {
    // Inserisci l'elemento dopo afterNode nella lista
    if (afterNode == NULL) {
        // Inserisci all'inizio della lista
        node->next = data->process.processDetails->stepElementsList.start;
        if (data->process.processDetails->stepElementsList.start != NULL) {
            data->process.processDetails->stepElementsList.start->prev = node;
        }
        data->process.processDetails->stepElementsList.start = node;
    } else {
        node->next = afterNode->next;
        node->prev = afterNode;
        afterNode->next = node;
        if (node->next != NULL) {
            node->next->prev = node;
        }
    }

    // Aggiorna l'elemento finale della lista se necessario
    if (afterNode == data->process.processDetails->stepElementsList.end) {
        data->process.processDetails->stepElementsList.end = node;
    }

    // Aggiorna la dimensione della lista
    data->process.processDetails->stepElementsList.size++;
}

void reorderStepElements(processNode *data) {
    int y_offset = -13; // Imposta l'offset iniziale a -13 pixel
    stepNode *current = data->process.processDetails->stepElementsList.start;
    while (current) {
        lv_obj_set_pos(current->step.stepElement, lv_obj_get_x_aligned(current->step.stepElement), y_offset);
        y_offset += lv_obj_get_height(current->step.stepElement); // Aggiorna l'offset y per il prossimo elemento
        current = current->next;
    }
}

bool hasListChanged(processNode *data) {
    stepNode *current = data->process.processDetails->stepElementsList.start;
    lv_coord_t original_y = 0; // Posizione Y originale prevista
    lv_coord_t current_y = 0;  // Posizione Y attuale

    while (current) {
        // Controlla se la posizione Y è cambiata rispetto alla posizione originale prevista
        current_y = lv_obj_get_y_aligned(current->step.stepElement);
        if (current_y != original_y) {
            // Se c'è una differenza, significa che la lista è cambiata
            return true;
        }

        // Aggiorna la posizione Y originale prevista per il prossimo elemento
        original_y += lv_obj_get_height(current->step.stepElement);

        // Passa all'elemento successivo nella lista
        current = current->next;
    }

    // Se si arriva qui, significa che la lista non è cambiata
    return false;
}

void event_stepElement(lv_event_t *e) {
    int8_t x;

    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = (lv_obj_t *)lv_event_get_target(e);
    lv_obj_t *objElement = (lv_obj_t *)lv_obj_get_parent(obj);
    processNode *data = (processNode *)lv_event_get_user_data(e);
    stepNode *currentNode = getStepElementEntryByObject(obj, data);

    lv_indev_t *indev = lv_indev_active();
    lv_dir_t dir = lv_indev_get_gesture_dir(lv_indev_active());

    static lv_point_t last_point;
    static bool dragging = false;

    if (indev == NULL)
        return;

    if (currentNode == NULL) {
        LV_LOG_USER("Bad object passed to eventProcessElement!");
        return;
    }

    if (code == LV_EVENT_LONG_PRESSED && currentNode->step.gestureHandled == false) {
       currentNode->step.longPressHandled = true;
        // Gestione LV_EVENT_LONG_PRESSED
        LV_LOG_USER("LV_EVENT_LONG_PRESSED");
        lv_obj_move_foreground(objElement);
        lv_indev_get_point(lv_indev_get_act(), &last_point);
        //lv_style_set_border_color(&currentNode->step.stepStyle, lv_color_hex(RED));
        lv_style_set_shadow_spread(&currentNode->step.stepStyle, 3);
        lv_obj_remove_flag(lv_obj_get_parent(objElement), LV_OBJ_FLAG_SCROLLABLE);
        dragging = true;
    } 
     
    if (code == LV_EVENT_RELEASED && currentNode->step.longPressHandled == true) {
      LV_LOG_USER("LV_EVENT_RELEASED");
      if(currentNode->step.gestureHandled == true){
        currentNode->step.gestureHandled = false;
        return;
      }
        currentNode->step.longPressHandled = false;
        
        
        //lv_style_set_border_color(&currentNode->step.stepStyle, lv_color_hex(GREEN_DARK));
        lv_style_set_shadow_spread(&currentNode->step.stepStyle, 0);
        lv_obj_add_flag(lv_obj_get_parent(objElement), LV_OBJ_FLAG_SCROLLABLE);

        // Inserisci l'elemento nella nuova posizione
        stepNode *previous = NULL;
        stepNode *next = data->process.processDetails->stepElementsList.start;
        lv_coord_t obj_y = lv_obj_get_y_aligned(objElement);

        // Determina se stiamo trascinando verso l'alto o verso il basso
        bool moveUp = true; // Flag per indicare il movimento verso l'alto

        // Controlla la direzione del movimento
        lv_indev_get_point(lv_indev_get_act(), &last_point);

        if (last_point.y > obj_y) {
                moveUp = false; // Stiamo trascinando verso il basso
        }

        // Cerca la posizione corretta nella lista in base alla direzione del movimento
        while (next) {
            lv_coord_t next_y = lv_obj_get_y_aligned(next->step.stepElement);
            if ((moveUp && next_y >= obj_y) || (!moveUp && next_y > obj_y)) {
                break;
            }
            previous = next;
            next = next->next;
        }

        removeStepElementFromList(data, currentNode);

        if (previous == NULL) {
            insertStepElementAfter(data, NULL, currentNode);
        } else {
            insertStepElementAfter(data, previous, currentNode);
        }

        reorderStepElements(data);
        lv_obj_invalidate(objElement);

        if (hasListChanged(data)) {
            gui.tempProcessNode->process.processDetails->somethingChanged = 1;
            lv_obj_send_event(gui.tempProcessNode->process.processDetails->processSaveButton, LV_EVENT_REFRESH, NULL);
        }

        dragging = false;
    }  
    if (code == LV_EVENT_LONG_PRESSED_REPEAT && currentNode->step.gestureHandled == false) {
        currentNode->step.longPressHandled = true;
        // Gestione LV_EVENT_LONG_PRESSED_REPEAT
     //   if (!currentNode->step.longPressHandled) {
            // Gestisci long press solo se non è stato già gestito
       //     currentNode->step.longPressHandled = true;
            // Esegui le azioni per il long press
       // }

        LV_LOG_USER("LV_EVENT_LONG_PRESSED_REPEAT");

        if (gui.tempProcessNode->process.processDetails->stepElementsList.size > 1) {
            if (dragging) {
                lv_point_t current_point;
                lv_indev_get_point(lv_indev_get_act(), &current_point);

                lv_coord_t dy = current_point.y - last_point.y;
                
                if (gui.tempProcessNode->process.processDetails->stepElementsList.start == currentNode) {
                    LV_LOG_USER("IS FIRST STEP IN LIST %d %d", dy, lv_obj_get_y_aligned(objElement));
                    if ((dy + lv_obj_get_y_aligned(objElement)) >= -16) {
                        lv_obj_set_pos(objElement, lv_obj_get_x_aligned(objElement), lv_obj_get_y_aligned(objElement) + dy);
                        last_point = current_point;
                        lv_obj_invalidate(objElement);
                    }
                } else if (gui.tempProcessNode->process.processDetails->stepElementsList.end == currentNode) {
                    LV_LOG_USER("IS LAST STEP IN LIST %d %d", dy, lv_obj_get_y_aligned(objElement));
                    if ((dy + lv_obj_get_y_aligned(objElement)) <= (((gui.tempProcessNode->process.processDetails->stepElementsList.size) * 70) - 53)) {
                        lv_obj_set_pos(objElement, lv_obj_get_x_aligned(objElement), lv_obj_get_y_aligned(objElement) + dy);
                        last_point = current_point;
                        lv_obj_invalidate(objElement);
                    }
                } else {
                  
                    LV_LOG_USER("IS MIDDLE STEP IN LIST %d %d", dy, lv_obj_get_y_aligned(objElement));
                    lv_obj_set_pos(objElement, lv_obj_get_x_aligned(objElement), lv_obj_get_y_aligned(objElement) + dy);
                    last_point = current_point;
                    lv_obj_invalidate(objElement);
                }
            }
        }
    }  

    if (code == LV_EVENT_GESTURE && currentNode->step.longPressHandled == false) {
        // Gestione LV_EVENT_GESTURE
        currentNode->step.gestureHandled = true;
        //currentNode->step.longPressHandled = false;
        switch (dir) {
            case LV_DIR_LEFT:
                if (currentNode->step.swipedLeft == 0 && currentNode->step.swipedRight == 0) {
                    LV_LOG_USER("Left gesture for edit");
                    x = lv_obj_get_x_aligned(currentNode->step.stepElement) - 50;
                    lv_obj_set_pos(currentNode->step.stepElement, x, lv_obj_get_y_aligned(currentNode->step.stepElement));
                    currentNode->step.swipedLeft = 1;
                    currentNode->step.swipedRight = 0;
                    lv_obj_remove_flag(currentNode->step.editButton, LV_OBJ_FLAG_HIDDEN);
                    break;
                }
                if (currentNode->step.swipedLeft == 0 && currentNode->step.swipedRight == 1) {
                    LV_LOG_USER("Left gesture to return");
                    x = lv_obj_get_x_aligned(currentNode->step.stepElement) - 50;
                    lv_obj_set_pos(currentNode->step.stepElement, x, lv_obj_get_y_aligned(currentNode->step.stepElement));
                    currentNode->step.swipedLeft = 0;
                    currentNode->step.swipedRight = 0;
                    lv_obj_add_flag(currentNode->step.deleteButton, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_add_flag(currentNode->step.editButton, LV_OBJ_FLAG_HIDDEN);
                    break;
                }

            case LV_DIR_RIGHT:
                if (currentNode->step.swipedLeft == 1 && currentNode->step.swipedRight == 0) {
                    LV_LOG_USER("Right gesture to return");
                    x = lv_obj_get_x_aligned(currentNode->step.stepElement) + 50;
                    lv_obj_set_pos(currentNode->step.stepElement, x, lv_obj_get_y_aligned(currentNode->step.stepElement));
                    currentNode->step.swipedLeft = 0;
                    currentNode->step.swipedRight = 0;
                    lv_obj_add_flag(currentNode->step.deleteButton, LV_OBJ_FLAG_HIDDEN);
                    lv_obj_add_flag(currentNode->step.editButton, LV_OBJ_FLAG_HIDDEN);
                    currentNode->step.gestureHandled = false;
                    break;
                }
                if (currentNode->step.swipedLeft == 0 && currentNode->step.swipedRight == 0) {
                    LV_LOG_USER("Right gesture for delete");
                    x = lv_obj_get_x_aligned(currentNode->step.stepElement) + 50;
                    lv_obj_set_pos(currentNode->step.stepElement, x, lv_obj_get_y_aligned(currentNode->step.stepElement));
                    currentNode->step.swipedRight = 1;
                    currentNode->step.swipedLeft = 0;
                    lv_obj_remove_flag(currentNode->step.deleteButton, LV_OBJ_FLAG_HIDDEN);
                    break;
                }
                
        }
    }  
    if (code == LV_EVENT_CLICKED) {
        // Gestione LV_EVENT_CLICKED
        if (obj == currentNode->step.editButton) {
            LV_LOG_USER("Click Edit button step address 0x%p", currentNode);
            stepDetail(data, currentNode);
            return;
        }
        if (obj == currentNode->step.deleteButton) {
            if (gui.element.messagePopup.mBoxPopupParent == NULL) {
                LV_LOG_USER("Click Delete button step address %p", currentNode);
                gui.tempStepNode = currentNode;
                messagePopupCreate(deletePopupTitle_text, deletePopupBody_text, deleteButton_text, stepDetailCancel_text, gui.tempStepNode);
                return;
            }
        }
    }  
    if (code == LV_EVENT_DELETE) {
        // Gestione LV_EVENT_DELETE
        lv_style_reset(&currentNode->step.stepStyle);
        return;
    }
}

void stepElementCreate(stepNode * newStep,processNode * processReference, int8_t tempSize){
  /*********************
  *    PAGE HEADER
  *********************/

  LV_LOG_USER("Step Creation");


  gui.tempProcessNode = processReference;
  
  calcolateTotalTime(processReference);
  
  LV_LOG_USER("Step element created with address 0x%p", newStep);
  LV_LOG_USER("Process element associated with address 0x%p", processReference);

	if( newStep->step.stepStyle.values_and_props == NULL ) {		//Only initialise the style once! 
		lv_style_init(&newStep->step.stepStyle);

    lv_style_set_bg_color(&newStep->step.stepStyle, lv_color_hex(GREY));
    lv_style_set_border_color(&newStep->step.stepStyle, lv_color_hex(GREEN_DARK));
    lv_style_set_border_width(&newStep->step.stepStyle, 4);
    lv_style_set_border_opa(&newStep->step.stepStyle, LV_OPA_50);
    lv_style_set_border_side(&newStep->step.stepStyle, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT);
    lv_style_set_shadow_width(&newStep->step.stepStyle, 5);
    lv_style_set_shadow_spread(&newStep->step.stepStyle, 0);
    lv_style_set_shadow_color(&newStep->step.stepStyle, lv_palette_main(LV_PALETTE_RED));
		LV_LOG_USER("First call to processElementCreate style now initialised");
	}

  newStep->step.swipedLeft = 0;
  newStep->step.swipedRight = 0;
  newStep->step.gestureHandled = false;
  newStep->step.longPressHandled = false;
  newStep->step.stepElement = lv_obj_create(processReference->process.processDetails->processStepsContainer);
  
  if(tempSize == -1){
		LV_LOG_USER("New Step");
    newStep->step.container_y = -13 + ((processReference->process.processDetails->stepElementsList.size - 1) * 70);
  }
  else{
		LV_LOG_USER("Previous Step");
    newStep->step.container_y = -13 + ((tempSize-1) * 70);
  }
  lv_obj_set_pos(newStep->step.stepElement, -13, newStep->step.container_y);        
  lv_obj_set_size(newStep->step.stepElement, 340, 70);
  lv_obj_remove_flag(newStep->step.stepElement, LV_OBJ_FLAG_SCROLLABLE); 
  lv_obj_set_style_border_opa(newStep->step.stepElement, LV_OPA_TRANSP, 0);
  lv_obj_remove_flag(newStep->step.stepElement, LV_OBJ_FLAG_GESTURE_BUBBLE);
  lv_obj_set_style_bg_opa(newStep->step.stepElement, LV_OPA_TRANSP, 0);

  lv_obj_add_event_cb(newStep->step.stepElement, event_stepElement, LV_EVENT_GESTURE, processReference);
  lv_obj_add_event_cb(newStep->step.stepElement, event_stepElement, LV_EVENT_LONG_PRESSED_REPEAT, processReference);
  lv_obj_add_event_cb(newStep->step.stepElement, event_stepElement, LV_EVENT_RELEASED, processReference);
  lv_obj_add_event_cb(newStep->step.stepElement, event_stepElement, LV_EVENT_LONG_PRESSED, processReference);


  lv_obj_set_pos(newStep->step.stepElement,lv_obj_get_x_aligned(newStep->step.stepElement) - 50,lv_obj_get_y_aligned(newStep->step.stepElement));

  /*********************
  *    PAGE ELEMENTS
  *********************/


        newStep->step.deleteButton = lv_obj_create(newStep->step.stepElement);
        lv_obj_set_style_bg_color(newStep->step.deleteButton, lv_color_hex(RED_DARK), LV_PART_MAIN);
        lv_obj_set_size(newStep->step.deleteButton, 60, 70);
        lv_obj_align(newStep->step.deleteButton, LV_ALIGN_TOP_LEFT, -16, -18);
        lv_obj_add_flag(newStep->step.deleteButton, LV_OBJ_FLAG_HIDDEN);
        lv_obj_remove_flag(newStep->step.deleteButton, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_add_event_cb(newStep->step.deleteButton, event_stepElement, LV_EVENT_CLICKED, processReference);
        lv_obj_add_flag(newStep->step.deleteButton, LV_OBJ_FLAG_CLICKABLE);

                newStep->step.deleteButtonLabel = lv_label_create(newStep->step.deleteButton);         
                lv_label_set_text(newStep->step.deleteButtonLabel, trash_icon); 
                lv_obj_set_style_text_font(newStep->step.deleteButtonLabel, &FilMachineFontIcons_30, 0);              
                lv_obj_align(newStep->step.deleteButtonLabel, LV_ALIGN_CENTER, -5 , 0);


        newStep->step.editButton = lv_obj_create(newStep->step.stepElement);
        lv_obj_set_style_bg_color(newStep->step.editButton, lv_color_hex(LIGHT_BLUE_DARK), LV_PART_MAIN);
        lv_obj_set_size(newStep->step.editButton, 60, 70);
        lv_obj_align(newStep->step.editButton, LV_ALIGN_TOP_LEFT, 260, -18);
        lv_obj_add_flag(newStep->step.editButton, LV_OBJ_FLAG_HIDDEN);
        lv_obj_remove_flag(newStep->step.editButton, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_add_event_cb(newStep->step.editButton, event_stepElement, LV_EVENT_CLICKED, processReference);
        lv_obj_add_flag(newStep->step.editButton, LV_OBJ_FLAG_CLICKABLE);

                newStep->step.editButtonLabel = lv_label_create(newStep->step.editButton);         
                lv_label_set_text(newStep->step.editButtonLabel, edit_icon); 
                lv_obj_set_style_text_font(newStep->step.editButtonLabel, &FilMachineFontIcons_30, 0);              
                lv_obj_align(newStep->step.editButtonLabel, LV_ALIGN_CENTER, 5, 0);

        newStep->step.stepElementSummary = lv_obj_create(newStep->step.stepElement);
        //lv_obj_set_style_border_color(newStep->step.stepElementSummary, lv_color_hex(LV_PALETTE_ORANGE), 0);
        lv_obj_set_size(newStep->step.stepElementSummary, 235, 66);
        lv_obj_align(newStep->step.stepElementSummary, LV_ALIGN_TOP_LEFT, 34, -16);
        lv_obj_remove_flag(newStep->step.stepElementSummary, LV_OBJ_FLAG_SCROLLABLE);  
        lv_obj_add_style(newStep->step.stepElementSummary, &newStep->step.stepStyle, 0);
        lv_obj_add_flag(newStep->step.stepElementSummary, LV_OBJ_FLAG_EVENT_BUBBLE);
                
                newStep->step.stepTypeIcon = lv_label_create(newStep->step.stepElementSummary);

                if(newStep->step.stepDetails->type == CHEMISTRY)
                    lv_label_set_text(newStep->step.stepTypeIcon, chemical_icon);
                if(newStep->step.stepDetails->type == RINSE)
                    lv_label_set_text(newStep->step.stepTypeIcon, rinse_icon);           
                if(newStep->step.stepDetails->type == MULTI_RINSE)
                    lv_label_set_text(newStep->step.stepTypeIcon, multiRinse_icon); 

                lv_obj_set_style_text_font(newStep->step.stepTypeIcon, &FilMachineFontIcons_20, 0);              
                lv_obj_align(newStep->step.stepTypeIcon, LV_ALIGN_LEFT_MID, -9, -12);


                newStep->step.stepName = lv_label_create(newStep->step.stepElementSummary);         
                lv_label_set_text(newStep->step.stepName, newStep->step.stepDetails->stepNameString); 
                lv_obj_set_style_text_font(newStep->step.stepName, &lv_font_montserrat_22, 0);      
                lv_label_set_long_mode(newStep->step.stepName, LV_LABEL_LONG_SCROLL_CIRCULAR);
                lv_obj_set_width(newStep->step.stepName, 175);        
                lv_obj_align(newStep->step.stepName, LV_ALIGN_LEFT_MID, 12, -12);
                lv_obj_remove_flag(newStep->step.stepName, LV_OBJ_FLAG_SCROLLABLE); 

                newStep->step.stepTimeIcon = lv_label_create(newStep->step.stepElementSummary);          
                lv_label_set_text(newStep->step.stepTimeIcon, clock_icon);                  
                lv_obj_set_style_text_font(newStep->step.stepTimeIcon, &FilMachineFontIcons_20, 0);
                //lv_obj_set_style_text_color(newStep->step.stepTimeIcon, lv_color_hex(GREY), LV_PART_MAIN);
                lv_obj_align(newStep->step.stepTimeIcon, LV_ALIGN_LEFT_MID, -10, 17);
                
                newStep->step.stepTime = lv_label_create(newStep->step.stepElementSummary);    
//                sprintf(formatted_string, "%dm%ds", newStep->step.stepDetails->timeMins, newStep->step.stepDetails->timeSecs);
                lv_label_set_text_fmt(newStep->step.stepTime, "%dm%ds", newStep->step.stepDetails->timeMins, newStep->step.stepDetails->timeSecs); 
                lv_obj_set_style_text_font(newStep->step.stepTime, &lv_font_montserrat_18, 0);              
                lv_obj_align(newStep->step.stepTime, LV_ALIGN_LEFT_MID, 12, 17);

                newStep->step.sourceLabel = lv_label_create(newStep->step.stepElementSummary); 
//                sprintf(formatted_string, "From:%s", processSourceList[newStep->step.stepDetails->source]);        
                lv_label_set_text_fmt(newStep->step.sourceLabel, "From:%s", processSourceList[newStep->step.stepDetails->source]); 
                lv_obj_set_style_text_font(newStep->step.sourceLabel, &lv_font_montserrat_18, 0);      
                lv_obj_set_width(newStep->step.sourceLabel, 120);        
                lv_obj_align(newStep->step.sourceLabel, LV_ALIGN_LEFT_MID, 85, 17);
                lv_obj_remove_flag(newStep->step.sourceLabel, LV_OBJ_FLAG_SCROLLABLE); 

                newStep->step.discardAfterIcon = lv_label_create(newStep->step.stepElementSummary);        
                lv_label_set_text(newStep->step.discardAfterIcon, discardAfter_icon); 
                lv_obj_set_style_text_font(newStep->step.discardAfterIcon, &FilMachineFontIcons_20, 0);            
                lv_obj_align(newStep->step.discardAfterIcon, LV_ALIGN_RIGHT_MID, 13, 17);

                if(newStep->step.stepDetails->discardAfterProc){
                    lv_obj_set_style_text_color(newStep->step.discardAfterIcon, lv_color_hex(WHITE), LV_PART_MAIN);
                  } else {
                    lv_obj_set_style_text_color(newStep->step.discardAfterIcon, lv_color_hex(GREY), LV_PART_MAIN);
                  }


}

