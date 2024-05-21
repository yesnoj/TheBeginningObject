/**
 * @file element_step.c
 *
 */


//ESSENTIAL INCLUDES
#include <lvgl.h>
#include "../../include/definitions.h"

extern struct gui_components gui;

//ACCESSORY INCLUDES

processNode * processNodeReference;

/******************************
*  LINKED LIST IMPLEMENTATION
******************************/
stepNode *addStepElement(stepNode * stepToAdd, processNode * processReference) {
  
	if(processReference->process.processDetails->stepElementsList.size == MAX_STEP_ELEMENTS) return NULL;		// Put some limit on things!
	
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
	return stepToAdd;
}



bool deleteStepElement( stepNode	*stepToDelete, processNode * processReference ) {

	stepNode 	*adjust_y_ptr = NULL;
	lv_coord_t		container_y_prev, container_y_new ;


	if( stepToDelete ) {
		adjust_y_ptr = stepToDelete->next;
		container_y_prev = stepToDelete->step.container_y;
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

		while( adjust_y_ptr ) {
			if( adjust_y_ptr->next ) container_y_new = adjust_y_ptr->step.container_y;
			adjust_y_ptr->step.container_y = container_y_prev;
			lv_obj_set_y(adjust_y_ptr->step.stepElement, adjust_y_ptr->step.container_y);
			if( adjust_y_ptr->next ) container_y_prev = container_y_new;
			adjust_y_ptr = adjust_y_ptr->next;
		}
		lv_obj_delete_async( stepToDelete->step.stepElement );			// Delete all LVGL objects associated with entry
		free( stepToDelete );												// Free the list entry itself
		processReference->process.processDetails->stepElementsList.size--;
		return true;
	}
	return false;
}

stepNode *getStepElementEntryByObject(lv_obj_t *obj, processNode * processReference) {
  
	stepNode	*currentNode  = processReference->process.processDetails->stepElementsList.start;

	while( currentNode  ) {
		if( obj == currentNode->step.stepElement ) break;				// Check all objects if any match return element pointer, not styles! 
    if( obj == currentNode->step.stepElementSummary ) break;
    if( obj == currentNode->step.stepName ) break;
    if( obj == currentNode->step.stepTime ) break;
    if( obj == currentNode->step.stepTimeIcon ) break;
    if( obj == currentNode->step.stepTypeIcon ) break;
		currentNode = currentNode->next;
	}
  
	return currentNode;
}

static bool deleteStepElementByObj( lv_obj_t *obj, processNode * processReference ) {

	stepList	*step_ptr  = getStepElementEntryByObject(obj,processReference);

	return deleteStepElement(step_ptr,processReference);
}


/******************************
*  LVGL ELEMENTS IMPLEMENTATION
******************************/

void event_stepElement(lv_event_t * e){
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t * obj = (lv_obj_t *)lv_event_get_target(e);
  lv_obj_t * data = (lv_obj_t *)lv_event_get_user_data(e);
  stepNode	*currentNode = getStepElementEntryByObject(obj,processNodeReference);
  

  //if(obj == currentNode->step.stepElementSummary){
      if(code == LV_EVENT_CLICKED) {    
        LV_LOG_USER("Process Element Details");
        stepDetail(data);
        return;
      }
      if(code == LV_EVENT_LONG_PRESSED_REPEAT) {    
        if(gui.element.messagePopup.mBoxPopupParent == NULL){
        LV_LOG_USER("Long press element");
        //messagePopupCreate(deletePopupTitle_text,deletePopupBody_text, deleteButton_text, stepDetailCancel_text, currentNode->step.stepElement);
        return;
      }
    }
 // }
  
  if(code == LV_EVENT_DELETE) {
       lv_style_reset(&currentNode->step.stepStyle);
       return;
  }
}


bool stepElementCreate(stepNode * newStep,processNode * processReference, char *name, uint32_t timeMins, uint32_t timeSecs, chemicalType type){
  /*********************
  *    PAGE HEADER
  *********************/

  LV_LOG_USER("Step Creation");

  processNodeReference = (processNode*) allocateAndInitializeNode(PROCESS_NODE);
  processNodeReference = processReference;
  
  LV_LOG_USER("Step element created with address 0x%p", newStep);
  LV_LOG_USER("Process element associated with address 0x%p", processReference);

	if( newStep->step.stepStyle.values_and_props == NULL ) {		/* Only initialise the style once! */
		lv_style_init(&newStep->step.stepStyle);

    lv_style_set_bg_opa(&newStep->step.stepStyle, LV_OPA_60);
    lv_style_set_bg_color(&newStep->step.stepStyle, lv_color_hex(WHITE));
    lv_style_set_border_color(&newStep->step.stepStyle, lv_color_hex(GREEN_DARK));
    lv_style_set_border_width(&newStep->step.stepStyle, 4);
    lv_style_set_border_opa(&newStep->step.stepStyle, LV_OPA_50);
    lv_style_set_border_side(&newStep->step.stepStyle, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT);
		LV_LOG_USER("First call to processElementCreate style now initialised");
	}


  newStep->step.stepElement = lv_obj_create(processReference->process.processDetails->processStepsContainer);
  
  newStep->step.container_y = -13 + ((processReference->process.processDetails->stepElementsList.size) * 70);
  lv_obj_set_pos(newStep->step.stepElement, -13, newStep->step.container_y);        
  lv_obj_set_size(newStep->step.stepElement, 240, 70);
  lv_obj_remove_flag(newStep->step.stepElement, LV_OBJ_FLAG_SCROLLABLE); 
  lv_obj_set_style_border_opa(newStep->step.stepElement, LV_OPA_TRANSP, 0);

  /*********************
  *    PAGE ELEMENTS
  *********************/
        newStep->step.stepElementSummary = lv_obj_create(newStep->step.stepElement);
        //lv_obj_set_style_border_color(newStep->step.stepElementSummary, lv_color_hex(LV_PALETTE_ORANGE), 0);
        lv_obj_set_size(newStep->step.stepElementSummary, 235, 66);
        lv_obj_align(newStep->step.stepElementSummary, LV_ALIGN_TOP_LEFT, -16, -16);
        lv_obj_remove_flag(newStep->step.stepElementSummary, LV_OBJ_FLAG_SCROLLABLE);  
        lv_obj_add_event_cb(newStep->step.stepElementSummary, event_stepElement, LV_EVENT_CLICKED, processReference);  
        lv_obj_add_event_cb(newStep->step.stepElementSummary, event_stepElement, LV_EVENT_LONG_PRESSED_REPEAT, processReference);

        lv_obj_add_style(newStep->step.stepElementSummary, &newStep->step.stepStyle, 0);

        newStep->step.stepName = lv_label_create(newStep->step.stepElementSummary);         
        lv_label_set_text(newStep->step.stepName, "Precipitevolissimevolmente"); 
        lv_obj_set_style_text_font(newStep->step.stepName, &lv_font_montserrat_22, 0);      
        lv_label_set_long_mode(newStep->step.stepName, LV_LABEL_LONG_SCROLL_CIRCULAR);
        lv_obj_set_width(newStep->step.stepName, 190);        
        lv_obj_align(newStep->step.stepName, LV_ALIGN_LEFT_MID, -10, -10);
        lv_obj_remove_flag(newStep->step.stepName, LV_OBJ_FLAG_SCROLLABLE); 

        newStep->step.stepTimeIcon = lv_label_create(newStep->step.stepElementSummary);          
        lv_label_set_text(newStep->step.stepTimeIcon, clock_Icon);                  
        lv_obj_set_style_text_font(newStep->step.stepTimeIcon, &FilMachineFontIcons_20, 0);
        //lv_obj_set_style_text_color(newStep->step.stepTimeIcon, lv_color_hex(GREY), LV_PART_MAIN);
        lv_obj_align(newStep->step.stepTimeIcon, LV_ALIGN_LEFT_MID, -10, 17);
        
        newStep->step.stepTime = lv_label_create(newStep->step.stepElementSummary);         
        lv_label_set_text(newStep->step.stepTime, "6m30s"); 
        lv_obj_set_style_text_font(newStep->step.stepTime, &lv_font_montserrat_18, 0);              
        lv_obj_align(newStep->step.stepTime, LV_ALIGN_LEFT_MID, 10, 17);

        newStep->step.stepTypeIcon = lv_label_create(newStep->step.stepElementSummary);     
        lv_label_set_text(newStep->step.stepTypeIcon, chemical_Icon);     
        lv_obj_set_style_text_font(newStep->step.stepTypeIcon, &FilMachineFontIcons_20, 0);              
        lv_obj_align(newStep->step.stepTypeIcon, LV_ALIGN_RIGHT_MID, 9, 0);


return true;
}

