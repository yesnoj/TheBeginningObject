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
static processNode *addProcessElement( void ) {

	processNode	*processToAdd;

	if( gui.page.processes.processElementsList.size == MAX_PROC_ELEMENTS ) return NULL;		// Put some limit on things!

	processToAdd = (processNode*)malloc( sizeof( processNode ));
	if( processToAdd == NULL ) {
		LV_LOG_USER("Out of heap memory!");
		return processToAdd;
	}
	memset( processToAdd, 0, sizeof( processNode ) );

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
	return processToAdd;
}

static processNode *getProcElementEntryByObject( lv_obj_t *obj ) {

	processNode	*currentNode  = gui.page.processes.processElementsList.start;

	while( currentNode  ) {
		if( obj == currentNode->process.processElement ) break;				/* Check all objects if any match return element pointer, not styles! */
		if( obj == currentNode->process.preferredIcon ) break;
		if( obj == currentNode->process.processElementSummary ) break;
		if( obj == currentNode->process.processName ) break;
		if( obj == currentNode->process.processTemp ) break;
		if( obj == currentNode->process.processTempIcon ) break;
		if( obj == currentNode->process.processTypeIcon ) break;
		currentNode = currentNode->next;
	}
	return currentNode;
}

static bool deleteProcessElementByObj( lv_obj_t *obj ) {

	processList	*proc_ptr  = getProcElementEntryByObject( obj );

	return deleteProcessElement( proc_ptr );
}






/******************************
*  LVGL ELEMENTS IMPLEMENTATION
******************************/

void event_processElement(lv_event_t * e){
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t * obj = (lv_obj_t *)lv_event_get_target(e);
  lv_obj_t * cont = (lv_obj_t *)lv_event_get_current_target(e);
  processNode	*currentNode = getProcElementEntryByObject(obj);

 // if(obj == cont && cont != gui.element.process.processElementSummary && cont != gui.element.process.preferredIcon)
 //   return;

	if( currentNode == NULL ) {
		LV_LOG_USER("Bad object passed to eventProcessElement!");	/* This will stop a crash */
		return;														/* if something is wrong */
	}

  if(obj == currentNode->process.preferredIcon){
      if(code == LV_EVENT_CLICKED) {   
        LV_LOG_USER("Process is preferred : %d",currentNode->process.isPreferred);
          if(  lv_color_eq( lv_obj_get_style_text_color(currentNode->process.preferredIcon, LV_PART_MAIN ), lv_color_hex(RED) ) ) {
            lv_obj_set_style_text_color(currentNode->process.preferredIcon, lv_color_hex(WHITE), LV_PART_MAIN);
            currentNode->process.isPreferred = 0;
          } else {
            lv_obj_set_style_text_color(currentNode->process.preferredIcon, lv_color_hex(RED), LV_PART_MAIN);
            currentNode->process.isPreferred = 1;
          }
      }
  }
  
  if(obj == currentNode->process.processElementSummary){
      if(code == LV_EVENT_SHORT_CLICKED) {    
        LV_LOG_USER("Process Element Details");
        //processDetail(gui.element.process.processElement); //won't work, cause definition in pages.h. Here we want to call processDetail, but this is not defined, because in page.h is defined after element_process.h
      }
      if(code == LV_EVENT_LONG_PRESSED_REPEAT){
        if(gui.element.messagePopup.mBoxPopupParent == NULL){
          LV_LOG_USER("Process Element Long Press for popup delete");
          tempProcessNode = currentNode;
          messagePopupCreate(deletePopupTitle_text,deletePopupBody_text, deleteButton_text, stepDetailCancel_text, currentNode);
        }
      }
  }
   if(code == LV_EVENT_DELETE) {
        lv_style_reset(&currentNode->process.style);
    }
}


bool processElementCreate( char *name, uint32_t temp, filmType type ) {

	processNode *newProcess = addProcessElement();			/* Create new process list element storage */

	if( newProcess == NULL ) {
		LV_LOG_USER("Process element creation failed, maximum entries reached" );
		return false;
	}

	LV_LOG_USER("Process element created with address 0x%p", newProcess);

	if( gui.element.process.style.values_and_props == NULL ) {		/* Only initialise the style once! */
		lv_style_init(&gui.element.process.style);

		lv_style_set_bg_opa(&gui.element.process.style, LV_OPA_60);
		lv_style_set_bg_color(&gui.element.process.style, lv_color_hex(WHITE));
		lv_style_set_border_color(&gui.element.process.style, lv_color_hex(GREEN_DARK));
		lv_style_set_border_width(&gui.element.process.style, 4);
		lv_style_set_border_opa(&gui.element.process.style, LV_OPA_50);
		lv_style_set_border_side(&gui.element.process.style, LV_BORDER_SIDE_BOTTOM | LV_BORDER_SIDE_RIGHT);
		LV_LOG_USER("First call to processElementCreate style now initialised");
	}

	newProcess->process.processElement = lv_obj_create(gui.page.processes.processesList);
	newProcess->process.container_y = -10 + ((gui.page.processes.processElementsList.size-1) * 70);
	lv_obj_set_pos(newProcess->process.processElement, -10, newProcess->process.container_y);
	lv_obj_set_size(newProcess->process.processElement, 315, 70);
	lv_obj_remove_flag(newProcess->process.processElement, LV_OBJ_FLAG_SCROLLABLE);
	lv_obj_set_style_border_opa(newProcess->process.processElement, LV_OPA_TRANSP, 0);

	/*********************
	*    PAGE ELEMENTS			
	*********************/
        newProcess->process.processElementSummary = lv_obj_create(newProcess->process.processElement);
        //lv_obj_set_style_border_color(proc_ptr->process.processElementSummary, lv_color_hex(LV_PALETTE_ORANGE), 0);
        lv_obj_set_size(newProcess->process.processElementSummary, 270, 66);
        lv_obj_align(newProcess->process.processElementSummary, LV_ALIGN_TOP_LEFT, -16, -16);
        lv_obj_remove_flag(newProcess->process.processElementSummary, LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_add_event_cb(newProcess->process.processElementSummary, event_processElement, LV_EVENT_SHORT_CLICKED, newProcess->process.processElementSummary);
        lv_obj_add_event_cb(newProcess->process.processElementSummary, event_processElement, LV_EVENT_LONG_PRESSED_REPEAT, newProcess->process.processElementSummary);

        lv_obj_add_style(newProcess->process.processElementSummary, &gui.element.process.style, 0);

        newProcess->process.processName = lv_label_create(newProcess->process.processElementSummary);
        lv_label_set_text(newProcess->process.processName, name );
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
        lv_label_set_text_fmt(newProcess->process.processTemp, "%d °C", temp );
        newProcess->process.temp = temp;
        lv_obj_set_style_text_font(newProcess->process.processTemp, &lv_font_montserrat_18, 0);
        lv_obj_align(newProcess->process.processTemp, LV_ALIGN_LEFT_MID, 7, 17);

        newProcess->process.processTypeIcon = lv_label_create(newProcess->process.processElementSummary);
        lv_label_set_text(newProcess->process.processTypeIcon, type == BLACK_AND_WHITE_FILM ? blackwhite_icon : colorpalette_icon);
        newProcess->process.filmType = type;
        lv_obj_set_style_text_font(newProcess->process.processTypeIcon, &FilMachineFontIcons_20, 0);
        lv_obj_align(newProcess->process.processTypeIcon, LV_ALIGN_RIGHT_MID, 7, 0);
        
        newProcess->process.preferredIcon = lv_label_create(newProcess->process.processElement);
        lv_label_set_text(newProcess->process.preferredIcon, preferred_icon);
        lv_obj_add_flag(newProcess->process.preferredIcon, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_set_style_text_font(newProcess->process.preferredIcon, &FilMachineFontIcons_30, 0);
        lv_obj_set_style_text_color(newProcess->process.preferredIcon, lv_color_hex(WHITE), LV_PART_MAIN);
        lv_obj_align(newProcess->process.preferredIcon, LV_ALIGN_RIGHT_MID, 15, 0);
        lv_obj_add_event_cb(newProcess->process.preferredIcon, event_processElement, LV_EVENT_CLICKED, newProcess->process.preferredIcon);
        return true;
}

