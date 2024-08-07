#include <sys/_stdint.h>
#include "esp32-hal.h"
/**
 * @file accessory.c
 *
 */


/*********************
 *      INCLUDES
 *********************/
#include "lvgl.h"
#include "definitions.h"
//#include <ArduinoJson.h>

#include <FS.h>
#include <SPI.h>
#include <Wire.h>

#include "sdkconfig.h"
#include "esp_attr.h"
/**********************
 *      MACROS
 **********************/


#define LESS_THAN_10 1
#define GREATER_THAN_9 2
#define GREATER_THAN_99 3

struct gui_components   gui;
Preferences preferences;

extern LGFX lcd;
void (*rebootBoard)(void) = 0;

/* Put a system request in the queue returns true if succesful false if queue is full */
uint8_t qSysAction( uint16_t msg ) {
  
  return xQueueSend( gui.sysActionQ, &msg, 0 );
}

void event_cb(lv_event_t * e)
{
  lv_obj_t * obj = (lv_obj_t *)lv_event_get_target(e);
  lv_obj_t * objCont = (lv_obj_t *)lv_obj_get_parent(obj);
  lv_obj_t * mboxCont = (lv_obj_t *)lv_obj_get_parent(objCont);

    //const char *label = lv_msgbox_get_active_btn(mbox);
    //LV_UNUSED(label);
    lv_msgbox_close(mboxCont);
    //lv_obj_delete(mboxCont);
    //LV_LOG_USER("Button %s clicked", lv_label_get(label));
}



lv_obj_t * create_radiobutton(lv_obj_t * mBoxParent, const char * txt, const int32_t x, const int32_t y, const int32_t size, const lv_font_t * font, const lv_color_t borderColor, const lv_color_t bgColor)
{
    lv_obj_t * obj = (lv_obj_t *)lv_checkbox_create(mBoxParent);
    lv_obj_add_flag(obj, LV_OBJ_FLAG_EVENT_BUBBLE);
    lv_obj_set_style_text_font(obj, font, LV_PART_MAIN);
    lv_checkbox_set_text(obj, txt);
    lv_obj_align(obj, LV_ALIGN_RIGHT_MID, x, y);


    lv_coord_t font_h = lv_font_get_line_height(font);
    lv_coord_t pad = (size - font_h) / 2;
    lv_obj_set_style_pad_left(obj, pad, LV_PART_INDICATOR);
    lv_obj_set_style_pad_right(obj, pad, LV_PART_INDICATOR);
    lv_obj_set_style_pad_top(obj, pad, LV_PART_INDICATOR);
    lv_obj_set_style_pad_bottom(obj, pad, LV_PART_INDICATOR);

  lv_obj_set_style_border_color(obj, borderColor, LV_PART_INDICATOR | LV_STATE_DEFAULT);
  lv_obj_set_style_border_color(obj, borderColor, LV_PART_INDICATOR | LV_STATE_CHECKED);
  lv_obj_set_style_bg_color(obj, bgColor, LV_PART_INDICATOR | LV_STATE_CHECKED);

    lv_obj_update_layout(obj);
    return obj;
}

void createMessageBox(lv_obj_t * messageBox,char *title, char *text, char *button1Text, char *button2Text)
{
    
    messageBox = lv_msgbox_create(NULL);
    lv_msgbox_add_title(messageBox, title);
    lv_obj_set_style_bg_color(messageBox, lv_palette_main(LV_PALETTE_YELLOW), LV_PART_MAIN);
    //lv_msgbox_add_header_button(messageBox, NULL);
    lv_msgbox_add_text(messageBox, text); 
    

    lv_obj_t * btn;
    btn = lv_msgbox_add_footer_button(messageBox, button1Text);
    lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, NULL);
    btn = lv_msgbox_add_footer_button(messageBox, button2Text);
    lv_obj_add_event_cb(btn, event_cb, LV_EVENT_CLICKED, NULL);
    
}

void event_checkbox_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = (lv_obj_t *)lv_event_get_target(e);
    if(code == LV_EVENT_VALUE_CHANGED) {
        LV_UNUSED(obj);
        const char * txt = lv_checkbox_get_text(obj);
        const char * state = lv_obj_get_state(obj) & LV_STATE_CHECKED ? "Checked" : "Unchecked";
        LV_UNUSED(txt);
        LV_UNUSED(state);
        LV_LOG_USER("%s: %s", txt, state);
    }
}

void event_keyboard(lv_event_t* e)
 {
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t * obj = (lv_obj_t *)lv_event_get_target(e);

   if(code == LV_EVENT_CLICKED){ 
      if(obj == gui.element.filterPopup.mBoxNameTextArea){
          LV_LOG_USER("LV_EVENT_FOCUSED on filterPopup.mBoxNameTextArea");
          lv_obj_set_user_data(gui.element.keyboardPopup.keyboard,obj);

          showKeyboard(gui.element.filterPopup.mBoxFilterPopupParent, obj);
      }
      if(obj == gui.tempProcessNode->process.processDetails->processDetailNameTextArea){
          LV_LOG_USER("LV_EVENT_FOCUSED on processDetailNameTextArea");
          lv_obj_set_user_data(gui.element.keyboardPopup.keyboard,obj);

          showKeyboard(gui.tempProcessNode->process.processDetails->processDetailParent, obj);
      }
      if(obj == gui.tempStepNode->step.stepDetails->stepDetailNamelTextArea){
          LV_LOG_USER("LV_EVENT_FOCUSED on stepDetailNamelTextArea");
          lv_obj_set_user_data(gui.element.keyboardPopup.keyboard,obj);

          showKeyboard(gui.tempStepNode->step.stepDetails->stepDetailParent, obj);
      }
   }
  
  if(code == LV_EVENT_DEFOCUSED){
      if(obj == gui.element.filterPopup.mBoxNameTextArea){
          LV_LOG_USER("LV_EVENT_DEFOCUSED on filterPopup.mBoxNameTextArea");
          hideKeyboard(gui.element.filterPopup.mBoxFilterPopupParent);
      }
      if(obj == gui.tempProcessNode->process.processDetails->processDetailNameTextArea){
          LV_LOG_USER("LV_EVENT_DEFOCUSED on processDetailNameTextArea");
          hideKeyboard(gui.tempProcessNode->process.processDetails->processDetailParent);
      }
      if(obj == gui.tempStepNode->step.stepDetails->stepDetailNamelTextArea){
          LV_LOG_USER("LV_EVENT_DEFOCUSED on stepDetailNamelTextArea");
          hideKeyboard(gui.tempStepNode->step.stepDetails->stepDetailParent);
      }
   }
    
  if (code == LV_EVENT_CANCEL) {
      //after the LV_EVENT_FOCUSED, the caller send it self to the keyboard as userData
      LV_LOG_USER("LV_EVENT_CANCEL PRESSED");
      if(lv_obj_get_user_data(gui.element.keyboardPopup.keyboard) == gui.element.filterPopup.mBoxNameTextArea){
          LV_LOG_USER("LV_EVENT_DEFOCUSED on filterPopup.mBoxNameTextArea");
          hideKeyboard(gui.element.filterPopup.mBoxFilterPopupParent);
      }
      if(lv_obj_get_user_data(gui.element.keyboardPopup.keyboard) == gui.tempProcessNode->process.processDetails->processDetailNameTextArea){
          LV_LOG_USER("LV_EVENT_DEFOCUSED on processDetailNameTextArea");
          hideKeyboard(gui.tempProcessNode->process.processDetails->processDetailParent);
      }
      if(lv_obj_get_user_data(gui.element.keyboardPopup.keyboard) == gui.tempStepNode->step.stepDetails->stepDetailNamelTextArea){
          LV_LOG_USER("LV_EVENT_DEFOCUSED on stepDetailNamelTextArea");
          hideKeyboard(gui.tempStepNode->step.stepDetails->stepDetailParent);
      }
    }
    if (code == LV_EVENT_READY) {
      LV_LOG_USER("LV_EVENT_READY PRESSED");

            if(lv_obj_get_user_data(gui.element.keyboardPopup.keyboard) == gui.element.filterPopup.mBoxNameTextArea){
              LV_LOG_USER("Press ok from filterPopup.mBoxFilterPopupParent");
              lv_textarea_set_text(gui.element.filterPopup.mBoxNameTextArea, lv_textarea_get_text(gui.element.keyboardPopup.keyboardTextArea));
              if(strlen(lv_textarea_get_text(gui.element.keyboardPopup.keyboardTextArea)) > 0) {
              	if(gui.element.filterPopup.filterName != NULL ) free( gui.element.filterPopup.filterName );
                gui.element.filterPopup.filterName = (char *)malloc(sizeof(char) * (strlen(lv_textarea_get_text(gui.element.keyboardPopup.keyboardTextArea)) + 1)); // Alloca memoria per la stringa, lunghezza_stringa è la lunghezza della stringa da assegnare

                if(gui.element.filterPopup.filterName != NULL ) 
                  strcpy(gui.element.filterPopup.filterName, lv_textarea_get_text(gui.element.keyboardPopup.keyboardTextArea));
              } else if(gui.element.filterPopup.filterName != NULL ) {
                free( gui.element.filterPopup.filterName );
                gui.element.filterPopup.filterName = NULL;
              }             
              hideKeyboard(gui.element.filterPopup.mBoxFilterPopupParent);
            }
            if(lv_obj_get_user_data(gui.element.keyboardPopup.keyboard) == gui.tempProcessNode->process.processDetails->processDetailNameTextArea){
              LV_LOG_USER("Press ok from processDetailNameTextArea");
              lv_textarea_set_text(gui.tempProcessNode->process.processDetails->processDetailNameTextArea, lv_textarea_get_text(gui.element.keyboardPopup.keyboardTextArea));

              gui.tempProcessNode->process.processDetails->somethingChanged = true;
              lv_obj_send_event(gui.tempProcessNode->process.processDetails->processSaveButton, LV_EVENT_REFRESH, NULL);
              hideKeyboard(gui.tempProcessNode->process.processDetails->processDetailParent);
            }
            if(lv_obj_get_user_data(gui.element.keyboardPopup.keyboard) == gui.tempStepNode->step.stepDetails->stepDetailNamelTextArea){
              LV_LOG_USER("Press ok from stepDetailNamelTextArea");
              lv_textarea_set_text(gui.tempStepNode->step.stepDetails->stepDetailNamelTextArea, lv_textarea_get_text(gui.element.keyboardPopup.keyboardTextArea));
              gui.tempStepNode->step.stepDetails->somethingChanged = true;
              lv_obj_send_event( gui.tempStepNode->step.stepDetails->stepSaveButton, LV_EVENT_REFRESH, NULL);
              hideKeyboard(gui.tempStepNode->step.stepDetails->stepDetailParent);
            } 
      }
 }



void create_keyboard()
{   
    gui.element.keyboardPopup.keyBoardParent = lv_obj_class_create_obj(&lv_msgbox_backdrop_class, lv_layer_top());
    lv_obj_class_init_obj(gui.element.keyboardPopup.keyBoardParent);
    lv_obj_remove_flag(gui.element.keyboardPopup.keyBoardParent, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_obj_set_size(gui.element.keyboardPopup.keyBoardParent, LV_PCT(100), LV_PCT(100));

    gui.element.keyboardPopup.keyboard = lv_keyboard_create(gui.element.keyboardPopup.keyBoardParent);
    lv_obj_add_event_cb(gui.element.keyboardPopup.keyboard, event_keyboard, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(gui.element.keyboardPopup.keyboard, event_keyboard, LV_EVENT_DEFOCUSED, NULL);
    lv_obj_add_event_cb(gui.element.keyboardPopup.keyboard, event_keyboard, LV_EVENT_CANCEL, NULL);
    lv_obj_add_event_cb(gui.element.keyboardPopup.keyboard, event_keyboard, LV_EVENT_READY, NULL);
    lv_obj_add_flag(gui.element.keyboardPopup.keyboard, LV_OBJ_FLAG_EVENT_BUBBLE);


    static const char * kb_map[] = {"Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", " ", "\n",
                                    "A", "S", "D", "F", "G", "H", "J", "K", "L",  " ", "\n",
                                    " "," ", "Z", "X", "C", "V", "B", "N", "M"," "," ","\n",
                                    LV_SYMBOL_CLOSE, LV_SYMBOL_BACKSPACE,  " ", " ", LV_SYMBOL_OK, NULL
                                   };
 
    /*Set the relative width of the buttons and other controls*/
    static const lv_buttonmatrix_ctrl_t kb_ctrl[] = {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, LV_BUTTONMATRIX_CTRL_HIDDEN,
                                                     4, 4, 4, 4, 4, 4, 4, 4, 4, LV_BUTTONMATRIX_CTRL_HIDDEN,
                                                     LV_BUTTONMATRIX_CTRL_HIDDEN, LV_BUTTONMATRIX_CTRL_HIDDEN, 4, 4, 4, 4, 4, 4, 4, LV_BUTTONMATRIX_CTRL_HIDDEN, LV_BUTTONMATRIX_CTRL_HIDDEN,
                                                     2, 2 | 2, 6, LV_BUTTONMATRIX_CTRL_HIDDEN | 2, 2
                                                    };

    /*Create a keyboard and add the new map as USER_1 mode*/

    lv_keyboard_set_map(gui.element.keyboardPopup.keyboard, LV_KEYBOARD_MODE_USER_1, kb_map, kb_ctrl);
    lv_keyboard_set_mode(gui.element.keyboardPopup.keyboard, LV_KEYBOARD_MODE_USER_1);
    
    gui.element.keyboardPopup.keyboardTextArea = lv_textarea_create(gui.element.keyboardPopup.keyBoardParent);
    lv_obj_align(gui.element.keyboardPopup.keyboardTextArea, LV_ALIGN_TOP_MID, 0, 10);
    lv_textarea_set_placeholder_text(gui.element.keyboardPopup.keyboardTextArea, keyboard_placeholder_text);
    lv_obj_set_size(gui.element.keyboardPopup.keyboardTextArea, lv_pct(90), 80);
    lv_obj_add_state(gui.element.keyboardPopup.keyboardTextArea, LV_STATE_FOCUSED);
    lv_textarea_set_max_length(gui.element.keyboardPopup.keyboardTextArea, MAX_PROC_NAME_LEN);
    lv_keyboard_set_textarea(gui.element.keyboardPopup.keyboard, gui.element.keyboardPopup.keyboardTextArea);

    lv_obj_set_style_text_font(gui.element.keyboardPopup.keyboard, &lv_font_montserrat_26, 0);
    lv_obj_set_style_text_font(gui.element.keyboardPopup.keyboardTextArea, &lv_font_montserrat_30, 0);
    
    lv_obj_add_flag(gui.element.keyboardPopup.keyBoardParent, LV_OBJ_FLAG_HIDDEN);
}


lv_obj_t * create_text(lv_obj_t * parent, const char * icon, const char * txt)
{
    lv_obj_t * obj =  (lv_obj_t *)lv_menu_cont_create(parent);

    lv_obj_t * img = NULL;
    lv_obj_t * label = NULL;

    if(icon) {
        img = lv_image_create(obj);
        lv_image_set_src(img, icon);
        //lv_obj_set_size(img, 46, 46);
    }

    if(txt) {
        label = lv_label_create(obj);
        lv_label_set_text(label, txt);
        lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL_CIRCULAR);
        lv_obj_set_flex_grow(label, 1);
        lv_obj_align(label, LV_ALIGN_OUT_BOTTOM_MID, 0, 0);
    }


    return obj;
}


lv_obj_t * create_slider(lv_obj_t * parent, const char * icon, const char * txt, int32_t min, int32_t max,
                                int32_t val)
{
    lv_obj_t * obj =  (lv_obj_t *) create_text(parent, icon, txt);

    lv_obj_t * slider =  (lv_obj_t *) lv_slider_create(obj);
    lv_obj_set_flex_grow(slider, 1);
    lv_slider_set_range(slider, min, max);
    lv_slider_set_value(slider, val, LV_ANIM_OFF);

    if(icon == NULL) {
        lv_obj_add_flag(slider, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK);
    }

    return obj;
}

lv_obj_t * create_switch(lv_obj_t * parent, const char * icon, const char * txt, bool chk)
{
    lv_obj_t * obj =  (lv_obj_t *) create_text(parent, icon, txt);

    lv_obj_t * sw = (lv_obj_t *)lv_switch_create(obj);
    lv_obj_add_state(sw, chk ? LV_STATE_CHECKED : 0);

    return obj;
}


void createQuestionMark(lv_obj_t * parent,lv_obj_t * element,lv_event_cb_t e, const int32_t x, const int32_t y){
    questionMark = lv_label_create(parent);
    lv_obj_set_size(questionMark, lv_font_get_line_height(&FilMachineFontIcons_15) * 1.5, lv_font_get_line_height(&FilMachineFontIcons_15) * 1.5);       
    lv_label_set_text(questionMark, questionMark_icon);
    lv_obj_add_event_cb(questionMark, e, LV_EVENT_CLICKED, element);
    lv_obj_set_style_text_font(questionMark, &FilMachineFontIcons_15, 0);                     
    lv_obj_align_to(questionMark, element, LV_ALIGN_OUT_RIGHT_MID, x, y);
    lv_obj_add_flag(questionMark, LV_OBJ_FLAG_CLICKABLE);
}

uint32_t convertCelsiusoToFahrenheit(uint32_t tempC){
  return static_cast<uint32_t>(tempC * 1.8 + 32 + 0.5); // Aggiunge 0.5 per l'approssimazione
}

uint32_t calc_buf_len( uint32_t maxVal, uint8_t extra_len ) {
    uint8_t caseVal = LESS_THAN_10;
    uint32_t len = 0, remainder = maxVal;
    
    if( maxVal > 999 ) return 0; // Limit to 999 although this is alot!
    if( maxVal > 99 ) caseVal = GREATER_THAN_99;
    else if( maxVal > 9 ) caseVal = GREATER_THAN_9;
    switch( caseVal ) {
        case GREATER_THAN_99:
          len += ( (4 + extra_len) * (remainder - 99)); // Add storage length for the 3 digit numbers + a '\n' charater
          remainder -= (remainder - 99); // Deduct what we have accounted for so far
        case GREATER_THAN_9: // Fall through case (no break; required!)
          len += ( (3 + extra_len) * (remainder - 9)); // Add the storage length for the 2 digit numbers + a '\n' character
          remainder -= (remainder - 9); // Deduct what we have accounted for so far
        case LESS_THAN_10:
        default:
          len +=( (2 + extra_len) * remainder); // Add strorage length for the 1 digit numbers + a '\n'
          remainder -= remainder; // Deduct what we have finally accounted for... Remainder should now be zero!
        break;
        }
    if( !remainder ) {
        return len; // All well return length
    } else {
        return 0; // Something went wrong, so we return 0 length for buffer to show error!
    }
}

char *createRollerValues(uint32_t minVal, uint32_t maxVal, const char* extra_str, bool isFahrenheit) {
    // Calcola la lunghezza necessaria del buffer
    uint32_t buf_len = 0;
    for (uint32_t i = minVal; i <= maxVal; i++) {
        int value = isFahrenheit ? static_cast<int>(i * 1.8 + 32) : i;
        buf_len += snprintf(NULL, 0, "%s%d\n", extra_str, value);
    }
    buf_len -= 1; // Rimuovi l'ultimo '\n' per evitare lo spazio vuoto

    // Alloca memoria per il buffer
    char *buf = (char *)malloc(buf_len + 1); // +1 per il terminatore nullo
    if (!buf) {
        return NULL; // Gestione del fallimento della malloc
    }

    // Popola il buffer con i valori
    uint32_t buf_ptr = 0;
    for (uint32_t i = minVal; i <= maxVal; i++) {
        int value = isFahrenheit ? static_cast<int>(i * 1.8 + 32) : i;
        if (i == maxVal) {
            buf_ptr += snprintf(&buf[buf_ptr], (buf_len - buf_ptr + 1), "%s%d", extra_str, value);
        } else {
            buf_ptr += snprintf(&buf[buf_ptr], (buf_len - buf_ptr + 1), "%s%d\n", extra_str, value);
        }
    }
    //LV_LOG_USER("Roller values :%s",buf);
    return buf;
}

/*
char *createRollerValues(uint32_t minVal, uint32_t maxVal, const char* extra_str, bool isFahrenheit) {
    // Calcola la lunghezza necessaria del buffer
    uint32_t buf_len = 0;
    for (uint32_t i = minVal; i <= maxVal; i++) {
        buf_len += snprintf(NULL, 0, "%s%d\n", extra_str, i);
    }
    buf_len -= 1; // Rimuovi l'ultimo '\n' per evitare lo spazio vuoto

    // Alloca memoria per il buffer
    char *buf = (char *)malloc(buf_len + 1); // +1 per il terminatore nullo
    if (!buf) {
        return NULL; // Gestione del fallimento della malloc
    }

    // Popola il buffer con i valori
    uint32_t buf_ptr = 0;
    for (uint32_t i = minVal; i <= maxVal; i++) {
        if (i == maxVal) {
            buf_ptr += lv_snprintf(&buf[buf_ptr], (buf_len - buf_ptr + 1), "%s%d", extra_str, i);
        } else {
            buf_ptr += lv_snprintf(&buf[buf_ptr], (buf_len - buf_ptr + 1), "%s%d\n", extra_str, i);
        }
    }

    return buf;
}
*/

/*
char *createRollerValues(uint32_t minVal, uint32_t maxVal, const char* extra_str) {
    uint32_t buf_len = calc_buf_len(maxVal, strlen(extra_str));
    uint32_t buf_ptr = 0;
    char *buf = (char *)malloc(buf_len);

    if (!buf) {
        return NULL; // Handle malloc failure
    }

    for (uint32_t i = minVal; i <= maxVal; i++) {
        if (i == maxVal) {
            buf_ptr += lv_snprintf(&buf[buf_ptr], (buf_len - buf_ptr), "%s%d", extra_str, i);
        } else {
            buf_ptr += lv_snprintf(&buf[buf_ptr], (buf_len - buf_ptr), "%s%d\n", extra_str, i);
        }
    }
    return buf;
}

*/
char * convertFloatToChar(float temp){
  char *buf =(char*)malloc(sizeof(temp));
  
  buf[0] = '\0';
  dtostrf(temp, 4, 1, buf);
  return buf;
}

void myLongEvent(lv_event_t * e, uint32_t howLongInMs)
{
    lv_event_code_t code = lv_event_get_code(e);
    static uint32_t t;
    if(code == LV_EVENT_PRESSED) {
        t = lv_tick_get();
    } 
    else if(code == LV_EVENT_PRESSING) {
        if(lv_tick_elaps(t) > howLongInMs) {
            /*Do something*/
        }
    }
}

void* allocateAndInitializeNode(NodeType_t type) {
    void *node = NULL;

    // Initialize and allocate according to the node type
    switch (type) {
        case STEP_NODE:
            node = ps_malloc(sizeof(stepNode));
            if (node != NULL) {
                memset(node, 0, sizeof(stepNode));
                stepNode* step = (stepNode*)node;
                step->step.stepDetails = (sStepDetail *)ps_malloc(sizeof(sStepDetail));
                if (step->step.stepDetails == NULL) {
                    // Handle memory allocation failure
                    free(step);
                    return NULL;
                }
                memset(step->step.stepDetails, 0, sizeof(sStepDetail));
            } else {
                // Handle memory allocation failure
                return NULL;
            }
            break;

        case PROCESS_NODE:
            node = ps_malloc(sizeof(processNode));
            if (node != NULL) {
                memset(node, 0, sizeof(processNode));
                processNode* process = (processNode*)node;
                process->process.processDetails = (sProcessDetail *)ps_malloc(sizeof(sProcessDetail));
                if (process->process.processDetails == NULL) {
                    // Handle memory allocation failure
                    free(process);
                    return NULL;
                }
                memset(process->process.processDetails, 0, sizeof(sProcessDetail));

                process->process.processDetails->checkup = (sCheckup *)ps_malloc(sizeof(sCheckup));
                if (process->process.processDetails->checkup == NULL) {
                    // Handle memory allocation failure
                    free(process->process.processDetails);
                    free(process);
                    return NULL;
                }
                memset(process->process.processDetails->checkup, 0, sizeof(sCheckup));
            } else {
                // Handle memory allocation failure
                return NULL;
            }
            break;

        default:
            // Handle invalid node type
            return NULL;
    }

    return node;
}

void* isNodeInList(void* list, void* node, NodeType_t type) {
    if (list == NULL || node == NULL) {
        return NULL;
    }

    switch (type) {
        case STEP_NODE: {
            stepList* sList = (stepList*)list;
            stepNode* current = sList->start;

            while (current != NULL) {
                if (current == (stepNode*)node) {
                    return (void*)current;  // Nodo trovato
                }
                current = current->next;
            }
            break;
        }

        case PROCESS_NODE: {
            processList* pList = (processList*)list;
            processNode* current = pList->start;

            while (current != NULL) {
                if (current == (processNode*)node) {
                    return (void*)current;  // Nodo trovato
                }
                current = current->next;
            }
            break;
        }
    }

    // Nodo non trovato
    return NULL;
}



void initGlobals( void ) {
  // Initialise the main GUI structure to zero
  memset(&gui, 0, sizeof(gui));       
  
  //gui.page.processes.processElementsList.start = NULL;  // Not rquired memset above does this only need to set non-zero values ( NULL is also zero )
  //gui.page.processes.processElementsList.end   = NULL;
  //gui.page.processes.processElementsList.size  = 0;

  // We only need to initialise the non-zero values
  gui.element.cleanPopup.titleLinePoints[1].x = 200;
  gui.element.filterPopup.titleLinePoints[1].x = 200;
  gui.element.rollerPopup.titleLinePoints[1].x = 200;
  gui.element.messagePopup.titleLinePoints[1].x = 200;

  gui.page.processes.titleLinePoints[1].x = 310;
  gui.page.settings.titleLinePoints[1].x = 310;
  gui.page.tools.titleLinePoints[1].x = 310;
  
  gui.element.rollerPopup.tempCelsiusOptions = createRollerValues(0,40,"",false);
  gui.element.rollerPopup.tempFahrenheitOptions = createRollerValues(0,40,"",true);
  gui.element.rollerPopup.minutesOptions = createRollerValues(0,240,"",false);
  gui.element.rollerPopup.secondsOptions = createRollerValues(0,59,"",false); 
  gui.element.rollerPopup.tempToleranceOptions = createRollerValues(0,5,"0.",false);

  //gui.element.filterPopup.filterName = ""; // Not Required this will set this to some constant pointer which is not good...
  //gui.element.filterPopup.isColorFilter = FILM_TYPE_NA;   // This breaks filtering not needed
  //gui.element.filterPopup.isBnWFilter = FILM_TYPE_NA;
  //gui.element.filterPopup.isBnWFilter = 0;
  //gui.element.filterPopup.preferredOnly = 0;
  
  //gui.page.processes.isFiltered = 0; // Not Required memset takes care of this also

  gui.tempProcessNode = (processNode*) allocateAndInitializeNode(PROCESS_NODE);
  gui.tempStepNode = (stepNode*) allocateAndInitializeNode(STEP_NODE);  
  
  create_keyboard();
}

//without the commented part, the keyboard will be shown OVER the caller
void showKeyboard(lv_obj_t * whoCallMe, lv_obj_t * textArea){
    if(textArea != NULL){
    //lv_obj_add_flag(whoCallMe, LV_OBJ_FLAG_HIDDEN);
      if(strlen(lv_textarea_get_text(textArea)) > 0)
        lv_textarea_set_text(gui.element.keyboardPopup.keyboardTextArea, lv_textarea_get_text(textArea));
      else
        lv_textarea_set_text(gui.element.keyboardPopup.keyboardTextArea, "");

      lv_obj_remove_flag(gui.element.keyboardPopup.keyBoardParent, LV_OBJ_FLAG_HIDDEN);
      lv_obj_move_foreground(gui.element.keyboardPopup.keyBoardParent);
    }
      else 
        lv_textarea_set_text(gui.element.keyboardPopup.keyboardTextArea, "");
}

void hideKeyboard(lv_obj_t * whoCallMe){
    lv_obj_add_flag(gui.element.keyboardPopup.keyBoardParent, LV_OBJ_FLAG_HIDDEN);
    lv_obj_move_background(gui.element.keyboardPopup.keyBoardParent);
    lv_textarea_set_text(gui.element.keyboardPopup.keyboardTextArea, "");
    //lv_obj_remove_flag(whoCallMe, LV_OBJ_FLAG_HIDDEN);
}

void my_touchpad_read(lv_indev_t* dev, lv_indev_data_t* data) {
      uint16_t touchX, touchY;

      data->state = LV_INDEV_STATE_REL;

      if (lcd.getTouch(&touchX, &touchY))
      {
          data->state = LV_INDEV_STATE_PR;
          data->point.x = touchX;
          data->point.y = touchY;
      }
}


#if LV_USE_LOG != 0
void my_print( lv_log_level_t level, const char * buf )
{
    LV_UNUSED(level);
    Serial.println(buf);
    Serial.flush();
}
#endif

void my_disp_flush(lv_display_t* display, const lv_area_t* area, unsigned char* data) {

  uint16_t w = (area->x2 - area->x1 + 1);
  uint16_t h = (area->y2 - area->y1 + 1);
  uint32_t size =  w * h * 2;

  lv_draw_sw_rgb565_swap(data, size);
  lcd.pushImageDMA(area->x1, area->y1, w, h, (uint16_t*)data);
  lv_display_flush_ready(display);
}


uint8_t SD_init()
{
    if (!SD.begin(SD_CS))
    {
        LV_LOG_USER("Card Mount Failed");
        return 1;
    }
    uint8_t cardType = SD.cardType();

    if (cardType == CARD_NONE)
    {
        LV_LOG_USER("No SD card attached");
        return 1;
    }

    LV_LOG_USER("SD Card Type: ");
    if (cardType == CARD_MMC)
    {
        LV_LOG_USER("MMC");
    }
    else if (cardType == CARD_SD)
    {
        LV_LOG_USER("SDSC");
    }
    else if (cardType == CARD_SDHC)
    {
        LV_LOG_USER("SDHC");
    }
    else
    {
        LV_LOG_USER("UNKNOWN");
    }

    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    LV_LOG_USER("SD Card Size: %lluMB\n", cardSize);
    //listDir(SD, "/", 2);

    LV_LOG_USER("SD init over.");
    return 0;
}

void createFile(fs::FS &fs, const char *path)
{
    LV_LOG_USER("Checking for file: %s", path);

    // Controlla se il file esiste
    if (!fs.exists(path))
    {
        LV_LOG_USER("File does not exist, creating file: %s", path);

        // Crea il file se non esiste
        File file = fs.open(path, FILE_WRITE);
        if (!file)
        {
            LV_LOG_USER("Failed to create file");
            return;
        }
        file.close(); // Chiudi subito dopo la creazione per assicurarti che il file esista
    }
    else
    {
        LV_LOG_USER("File already exists: %s", path);
    }
}

void initPinouts() {
  if (SD_init()) {
    initErrors = INIT_ERROR_SD;
    LV_LOG_USER("ERROR:   SD initErrors %d", initErrors);
  } else{
    createFile(SD, FILENAME_SAVE);
    LV_LOG_USER("SD INIT OVER initErrors %d", initErrors);
    }

  //I2C1 init
  
  Wire.begin(I2C1_SDA, I2C1_SCL);
  Wire.beginTransmission(I2C1_ADR);

  if (Wire.endTransmission() == 0) {
    LV_LOG_USER("I2C1 device found at address 0x%x! TOUCH INIT OVER", I2C1_ADR);
  } else {
    initErrors = INIT_ERROR_WIRE;
    LV_LOG_USER("Unknown error at address 0x%x ERROR:   TOUCH", I2C1_ADR);
  }
  if (!mcp.begin_I2C()) {
    LV_LOG_USER("MCP23017_1 init ERROR!");
    initErrors = INIT_ERROR_I2C_MCP;
  } else {
      LV_LOG_USER("MCP23017_1 init OK!");
      initializeRelayPins();
      initializeMotorPins();
  }


#if 0 //Maybe this will never work, appears that on this board there is 1*I2C and 1*GPIO, the 1*I2C is the one that has io38 and io39 as SCL/SDA...
  Wire1.begin(I2C2_SDA, I2C2_SCL); 
  Wire1.beginTransmission(I2C1_ADR);

  if (Wire1.endTransmission() == 0) {
    LV_LOG_USER("I2C2 device found at address 0x%x! TOUCH INIT OVER", I2C1_ADR);
  } else {
    initErrors = INIT_ERROR_WIRE;
    LV_LOG_USER("Unknown error at address 0x%x ERROR:   TOUCH", I2C1_ADR);
  }
  if (!mcp.begin_I2C(I2C1_ADR,&Wire1)) {
    LV_LOG_USER("MCP23017_2 init ERROR!");
    initErrors = INIT_ERROR_I2C_MCP;
  } else {
      LV_LOG_USER("MCP23017_2 init OK!");
      initializeRelayPins();
      //initializeMotorPins();
  }
#endif

#if 0 

  if (!ads.begin(I2C2_ADR)) {
    LV_LOG_USER("ADS1115 init ERROR!");
    initErrors = INIT_ERROR_I2C_ADS;
  } else {
      LV_LOG_USER("ADS1115 init OK!");
      //initializeTemperatureSensor();
  }
#endif
  if (initErrors) {

    LV_LOG_USER("SOMETHING WRONG initErrors %d", initErrors);
  } else{
    LV_LOG_USER("ALL SUCCESS initErrors %d", initErrors);
  }
}

void readConfigFile(fs::FS &fs, const char *path, bool enableLog) {
  if(fs.exists(path)) {

    File file = fs.open(path, FILE_READ);

    // Load Machine Settings
    if( file.read((uint8_t*)&gui.page.settings.settingsParams, sizeof(gui.page.settings.settingsParams)) < 1){
      file.close();
      LV_LOG_USER("Configuration file is empty aborting load");
      return;
    }

    if(enableLog) {
        LV_LOG_USER("--- MACHINE PARAMS ---");
        LV_LOG_USER("tempUnit:%d",gui.page.settings.settingsParams.tempUnit);
//        LV_LOG_USER("size:%d",sizeof(gui.page.settings.settingsParams.tempUnit));
        LV_LOG_USER("waterInlet:%d",gui.page.settings.settingsParams.waterInlet);
//        LV_LOG_USER("size:%d",sizeof(gui.page.settings.settingsParams.waterInlet));
        LV_LOG_USER("calibratedTemp:%d",gui.page.settings.settingsParams.calibratedTemp);
//        LV_LOG_USER("size:%d",sizeof(gui.page.settings.settingsParams.calibratedTemp));
        LV_LOG_USER("filmRotationSpeedSetpoint:%d",gui.page.settings.settingsParams.filmRotationSpeedSetpoint);
//        LV_LOG_USER("size:%d",sizeof(gui.page.settings.settingsParams.filmRotationSpeedSetpoint));
        LV_LOG_USER("rotationIntervalSetpoint:%d",gui.page.settings.settingsParams.rotationIntervalSetpoint);
//        LV_LOG_USER("size:%d",sizeof(gui.page.settings.settingsParams.rotationIntervalSetpoint));
        LV_LOG_USER("randomSetpoint:%d",gui.page.settings.settingsParams.randomSetpoint);
//        LV_LOG_USER("size:%d",sizeof(gui.page.settings.settingsParams.randomSetpoint));
        LV_LOG_USER("isPersistentAlarm:%d",gui.page.settings.settingsParams.isPersistentAlarm);
//        LV_LOG_USER("size:%d",sizeof(gui.page.settings.settingsParams.isPersistentAlarm));
        LV_LOG_USER("isProcessAutostart:%d",gui.page.settings.settingsParams.isProcessAutostart);
//        LV_LOG_USER("size:%d",sizeof(gui.page.settings.settingsParams.isProcessAutostart));
        LV_LOG_USER("drainFillOverlapSetpoint:%d",gui.page.settings.settingsParams.drainFillOverlapSetpoint);
//        LV_LOG_USER("size:%d",sizeof(gui.page.settings.settingsParams.drainFillOverlapSetpoint));
        LV_LOG_USER("multiRinseTime:%d",gui.page.settings.settingsParams.multiRinseTime);
//        LV_LOG_USER("size:%d",sizeof(gui.page.settings.settingsParams.multiRinseTime));
    }   

    // Load Processes
    processList *processElementsList = &gui.page.processes.processElementsList;
    processElementsList->start = NULL;
    processElementsList->end = NULL;
    processElementsList->size = 0;    
    // Read process list size
    file.read((uint8_t*)&processElementsList->size, sizeof(processElementsList->size));
    if(enableLog) LV_LOG_USER("Number of Processes:%d", processElementsList->size);

    for(int32_t process = 0; process < processElementsList->size; process++){
      if(enableLog) LV_LOG_USER("Process:%d", process);

      processNode *nodeP = (processNode*) allocateAndInitializeNode(PROCESS_NODE);
      if (nodeP == NULL) {
          LV_LOG_USER("Failed to allocate memory for process node");
          continue;
      }
      file.read((uint8_t*)&nodeP->process.processDetails->processNameString, sizeof(nodeP->process.processDetails->processNameString));
      file.read((uint8_t*)&nodeP->process.processDetails->temp, sizeof(nodeP->process.processDetails->temp));
      file.read((uint8_t*)&nodeP->process.processDetails->tempTolerance, sizeof(nodeP->process.processDetails->tempTolerance));
      file.read((uint8_t*)&nodeP->process.processDetails->isTempControlled, sizeof(nodeP->process.processDetails->isTempControlled));
      file.read((uint8_t*)&nodeP->process.processDetails->isPreferred, sizeof(nodeP->process.processDetails->isPreferred));
      file.read((uint8_t*)&nodeP->process.processDetails->filmType, sizeof(nodeP->process.processDetails->filmType));
      file.read((uint8_t*)&nodeP->process.processDetails->timeMins, sizeof(nodeP->process.processDetails->timeMins));
      file.read((uint8_t*)&nodeP->process.processDetails->timeSecs, sizeof(nodeP->process.processDetails->timeSecs));

      if (processElementsList->start == NULL) {
        processElementsList->start = nodeP;
        nodeP->prev = NULL;
      } else {
        processElementsList->end->next = nodeP;
        nodeP->prev = processElementsList->end;
      }
      processElementsList->end = nodeP;
      processElementsList->end->next = NULL;

      if(enableLog){
        LV_LOG_USER("--- PROCESS PARAMS ---");
        LV_LOG_USER("processNameString:%s",nodeP->process.processDetails->processNameString);
//        LV_LOG_USER("size:%d",sizeof(nodeP->process.processDetails->processNameString));
        LV_LOG_USER("temp:%d",nodeP->process.processDetails->temp);
//        LV_LOG_USER("size:%d",sizeof(nodeP->process.processDetails->temp));
        LV_LOG_USER("tempTolerance:%f",nodeP->process.processDetails->tempTolerance);
//        LV_LOG_USER("size:%d",sizeof(nodeP->process.processDetails->tempTolerance));
        LV_LOG_USER("isTempControlled:%d",nodeP->process.processDetails->isTempControlled);
//        LV_LOG_USER("size:%d",sizeof(nodeP->process.processDetails->isTempControlled));
        LV_LOG_USER("isPreferred:%d",nodeP->process.processDetails->isPreferred);
//        LV_LOG_USER("size:%d",sizeof(nodeP->process.processDetails->isPreferred));
        LV_LOG_USER("filmType:%d",nodeP->process.processDetails->filmType);
//        LV_LOG_USER("size:%d",sizeof(nodeP->process.processDetails->filmType));
        LV_LOG_USER("timeMins:%d",nodeP->process.processDetails->timeMins);
//        LV_LOG_USER("size:%d",sizeof(nodeP->process.processDetails->timeMins));
        LV_LOG_USER("timeSecs:%d",nodeP->process.processDetails->timeSecs);
//        LV_LOG_USER("size:%d",sizeof(nodeP->process.processDetails->timeSecs));
      }

      stepList *stepElementsList = &nodeP->process.processDetails->stepElementsList;
      stepElementsList->start = NULL;
      stepElementsList->end = NULL;
      stepElementsList->size = 0;

      // Write step list size
      file.read((uint8_t*)&stepElementsList->size, sizeof(stepElementsList->size));
      if(enableLog) LV_LOG_USER("Process Steps:%d", stepElementsList->size);
      for(int32_t step = 0; step < stepElementsList->size; step++){                
        if(enableLog) LV_LOG_USER("Step:%d", step);
        stepNode *nodeS = (stepNode*) allocateAndInitializeNode(STEP_NODE);
        if (nodeS == NULL) {
          LV_LOG_USER("Failed to allocate memory for step node");
          continue;
        }

        file.read((uint8_t*)&nodeS->step.stepDetails->stepNameString, sizeof(nodeS->step.stepDetails->stepNameString));
        file.read((uint8_t*)&nodeS->step.stepDetails->timeMins, sizeof(nodeS->step.stepDetails->timeMins));
        file.read((uint8_t*)&nodeS->step.stepDetails->timeSecs, sizeof(nodeS->step.stepDetails->timeSecs));
        file.read((uint8_t*)&nodeS->step.stepDetails->type, sizeof(nodeS->step.stepDetails->type));
        file.read((uint8_t*)&nodeS->step.stepDetails->source, sizeof(nodeS->step.stepDetails->source));
        file.read((uint8_t*)&nodeS->step.stepDetails->discardAfterProc, sizeof(nodeS->step.stepDetails->discardAfterProc));
        
        if (stepElementsList->start == NULL) {
          stepElementsList->start = nodeS;
          nodeS->prev = NULL;
        } else {
          stepElementsList->end->next = nodeS;
          nodeS->prev = stepElementsList->end;
        }
        stepElementsList->end = nodeS;
        stepElementsList->end->next = NULL;

        if(enableLog){
          LV_LOG_USER("--- STEP PARAMS ---");
          LV_LOG_USER("stepNameString:%s",nodeS->step.stepDetails->stepNameString);
//          LV_LOG_USER("size:%d",sizeof(nodeS->step.stepDetails->stepNameString));
          LV_LOG_USER("timeMins:%d",nodeS->step.stepDetails->timeMins);
//          LV_LOG_USER("size:%d",sizeof(nodeS->step.stepDetails->timeMins));
          LV_LOG_USER("timeSecs:%d",nodeS->step.stepDetails->timeSecs);
//          LV_LOG_USER("size:%d",sizeof(nodeS->step.stepDetails->timeSecs));
          LV_LOG_USER("type:%d",nodeS->step.stepDetails->type);
//          LV_LOG_USER("size:%d",sizeof(nodeS->step.stepDetails->type));
          LV_LOG_USER("source:%d",nodeS->step.stepDetails->source);
//          LV_LOG_USER("size:%d",sizeof(nodeS->step.stepDetails->source));
          LV_LOG_USER("discardAfterProc:%d",nodeS->step.stepDetails->discardAfterProc);
//          LV_LOG_USER("size:%d",sizeof(nodeS->step.stepDetails->discardAfterProc));
        }

      }

    }
    file.close();
  } else {
    LV_LOG_USER("Failed to open configuration file for reading using default");
  }
}

void writeConfigFile(fs::FS &fs, const char *path, bool enableLog) {
    if (initErrors == 0) {
        LV_LOG_USER("Writing configuration file: %s", path);
        SD.remove(path);

        File file = fs.open(path, FILE_WRITE);
        if (!file) {
            LV_LOG_USER("Failed to open file for writing");
            return;
        }

        // Write Machine Parameters
        file.write((uint8_t *)&gui.page.settings.settingsParams, sizeof(gui.page.settings.settingsParams));

        if (enableLog) {
            LV_LOG_USER("--- MACHINE PARAMS ---");
            LV_LOG_USER("tempUnit:%d", gui.page.settings.settingsParams.tempUnit);
            LV_LOG_USER("waterInlet:%d", gui.page.settings.settingsParams.waterInlet);
            LV_LOG_USER("calibratedTemp:%d", gui.page.settings.settingsParams.calibratedTemp);
            LV_LOG_USER("filmRotationSpeedSetpoint:%d", gui.page.settings.settingsParams.filmRotationSpeedSetpoint);
            LV_LOG_USER("rotationIntervalSetpoint:%d", gui.page.settings.settingsParams.rotationIntervalSetpoint);
            LV_LOG_USER("randomSetpoint:%d", gui.page.settings.settingsParams.randomSetpoint);
            LV_LOG_USER("isPersistentAlarm:%d", gui.page.settings.settingsParams.isPersistentAlarm);
            LV_LOG_USER("isProcessAutostart:%d", gui.page.settings.settingsParams.isProcessAutostart);
            LV_LOG_USER("drainFillOverlapSetpoint:%d", gui.page.settings.settingsParams.drainFillOverlapSetpoint);
        }

        // Write Processes
        processNode *currentProcessNode = gui.page.processes.processElementsList.start;
        // Write process list size
        file.write((uint8_t *)&gui.page.processes.processElementsList.size, sizeof(gui.page.processes.processElementsList.size));

        while (currentProcessNode != NULL) {
            file.write((uint8_t *)currentProcessNode->process.processDetails->processNameString, sizeof(currentProcessNode->process.processDetails->processNameString));
            file.write((uint8_t *)&currentProcessNode->process.processDetails->temp, sizeof(currentProcessNode->process.processDetails->temp));
            file.write((uint8_t *)&currentProcessNode->process.processDetails->tempTolerance, sizeof(currentProcessNode->process.processDetails->tempTolerance));
            file.write((uint8_t *)&currentProcessNode->process.processDetails->isTempControlled, sizeof(currentProcessNode->process.processDetails->isTempControlled));
            file.write((uint8_t *)&currentProcessNode->process.processDetails->isPreferred, sizeof(currentProcessNode->process.processDetails->isPreferred));
            file.write((uint8_t *)&currentProcessNode->process.processDetails->filmType, sizeof(currentProcessNode->process.processDetails->filmType));
            file.write((uint8_t *)&currentProcessNode->process.processDetails->timeMins, sizeof(currentProcessNode->process.processDetails->timeMins));
            file.write((uint8_t *)&currentProcessNode->process.processDetails->timeSecs, sizeof(currentProcessNode->process.processDetails->timeSecs));

            if (enableLog) {
                LV_LOG_USER("--- PROCESS PARAMS ---");
                LV_LOG_USER("processNameString:%s", currentProcessNode->process.processDetails->processNameString);
                LV_LOG_USER("temp:%d", currentProcessNode->process.processDetails->temp);
                LV_LOG_USER("tempTolerance:%f", currentProcessNode->process.processDetails->tempTolerance);
                LV_LOG_USER("isTempControlled:%d", currentProcessNode->process.processDetails->isTempControlled);
                LV_LOG_USER("isPreferred:%d", currentProcessNode->process.processDetails->isPreferred);
                LV_LOG_USER("filmType:%d", currentProcessNode->process.processDetails->filmType);
                LV_LOG_USER("timeMins:%d", currentProcessNode->process.processDetails->timeMins);
                LV_LOG_USER("timeSecs:%d", currentProcessNode->process.processDetails->timeSecs);
            }

            stepNode *currentStepNode = currentProcessNode->process.processDetails->stepElementsList.start;
            // Write step list size
            file.write((uint8_t *)&currentProcessNode->process.processDetails->stepElementsList.size, sizeof(currentProcessNode->process.processDetails->stepElementsList.size));
            while (currentStepNode != NULL) {
                file.write((uint8_t *)currentStepNode->step.stepDetails->stepNameString, sizeof(currentStepNode->step.stepDetails->stepNameString));
                file.write((uint8_t *)&currentStepNode->step.stepDetails->timeMins, sizeof(currentStepNode->step.stepDetails->timeMins));
                file.write((uint8_t *)&currentStepNode->step.stepDetails->timeSecs, sizeof(currentStepNode->step.stepDetails->timeSecs));
                file.write((uint8_t *)&currentStepNode->step.stepDetails->type, sizeof(currentStepNode->step.stepDetails->type));
                file.write((uint8_t *)&currentStepNode->step.stepDetails->source, sizeof(currentStepNode->step.stepDetails->source));
                file.write((uint8_t *)&currentStepNode->step.stepDetails->discardAfterProc, sizeof(currentStepNode->step.stepDetails->discardAfterProc));

                if (enableLog) {
                    LV_LOG_USER("--- STEP PARAMS ---");
                    LV_LOG_USER("stepNameString:%s", currentStepNode->step.stepDetails->stepNameString);
                    LV_LOG_USER("timeMins:%d", currentStepNode->step.stepDetails->timeMins);
                    LV_LOG_USER("timeSecs:%d", currentStepNode->step.stepDetails->timeSecs);
                    LV_LOG_USER("type:%d", currentStepNode->step.stepDetails->type);
                    LV_LOG_USER("source:%d", currentStepNode->step.stepDetails->source);
                    LV_LOG_USER("discardAfterProc:%d", currentStepNode->step.stepDetails->discardAfterProc);
                }
                currentStepNode = currentStepNode->next;
            }
            currentProcessNode = currentProcessNode->next;
        }
        file.close();
    }
}

void calculateTotalTime(processNode *processNode){
    uint32_t mins = 0;
    uint8_t  secs = 0;

     stepList *stepElementsList;
     memset( &stepElementsList, 0, sizeof( stepElementsList ) );    
     stepElementsList = &(processNode->process.processDetails->stepElementsList);   

            stepNode *stepNode;
            memset( &stepNode, 0, sizeof( stepNode ) ); 
            stepNode = stepElementsList->start;

            while(stepNode != NULL){                
                mins += stepNode->step.stepDetails->timeMins;
                secs += stepNode->step.stepDetails->timeSecs;

                if (secs >= 60) {
                    mins += secs / 60;
                    secs = secs % 60;
                }
                stepNode = stepNode->next;
            }
    processNode->process.processDetails->timeMins = mins;
    processNode->process.processDetails->timeSecs = secs;

    lv_label_set_text_fmt(processNode->process.processDetails->processTotalTimeValue, "%dm%ds", processNode->process.processDetails->timeMins, 
      processNode->process.processDetails->timeSecs); 
    LV_LOG_USER("Process %p has a total tilme of %dmin:%dsec", processNode, mins, secs);
}



uint8_t calculatePercentage(uint32_t minutes, uint8_t seconds, uint32_t total_minutes, uint8_t total_seconds) {
    // Calcola il tempo totale in secondi
    uint32_t total_time_seconds = total_minutes * 60 + total_seconds;

    // Calcola il tempo trascorso in secondi
    uint32_t elapsed_time_seconds = minutes * 60 + seconds;

    // Calcola la percentuale di tempo trascorso rispetto al tempo totale
    uint8_t percentage = (uint8_t)((elapsed_time_seconds * 100) / total_time_seconds);

    // Assicurati che la percentuale sia compresa tra 0 e 100
    if (percentage < 0) {
        percentage = 0;
    } else if (percentage > 100) {
        percentage = 100;
    }

    return percentage;
}


void updateProcessElement(processNode *process){
  processNode* existingProcess = (processNode*)isNodeInList((void*)&(gui.page.processes.processElementsList), process, PROCESS_NODE);
  
  if(existingProcess != NULL) {
      LV_LOG_USER("Updating process element in list");
      //Update time
      lv_label_set_text_fmt(existingProcess->process.processTime, "%dm%ds", process->process.processDetails->timeMins, 
        process->process.processDetails->timeSecs); 
      
      //Update temp
      lv_obj_send_event(existingProcess->process.processElement, LV_EVENT_REFRESH, NULL);


      if(process->process.processDetails->isTempControlled == false)
        {
          lv_obj_add_flag(process->process.processTempIcon, LV_OBJ_FLAG_HIDDEN);
          lv_obj_add_flag(process->process.processTemp, LV_OBJ_FLAG_HIDDEN);
          lv_obj_align(process->process.processTimeIcon, LV_ALIGN_LEFT_MID, -10, 17);
          lv_obj_align(process->process.processTime, LV_ALIGN_LEFT_MID, 12, 17);
        }
        else{
            lv_obj_align(process->process.processTime, LV_ALIGN_LEFT_MID, 87, 17);
            lv_obj_align(process->process.processTimeIcon, LV_ALIGN_LEFT_MID, 65, 17);
            lv_obj_clear_flag(process->process.processTempIcon, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(process->process.processTemp, LV_OBJ_FLAG_HIDDEN);
        }

 
      //Update preferred
      if(process->process.processDetails->isPreferred == 1){
            lv_obj_set_style_text_color(existingProcess->process.preferredIcon, lv_color_hex(RED), LV_PART_MAIN);
      }
      else{
            lv_obj_set_style_text_color(existingProcess->process.preferredIcon, lv_color_hex(WHITE), LV_PART_MAIN);
      }
      
      //Update name
      lv_label_set_text(existingProcess->process.processName, process->process.processDetails->processNameString);

      //Update film type
      lv_label_set_text(existingProcess->process.processTypeIcon, process->process.processDetails->filmType == BLACK_AND_WHITE_FILM ? blackwhite_icon : colorpalette_icon);


  } 
}



void updateStepElement(processNode *referenceProcess, stepNode *step){
  stepNode* existingStep = (stepNode*)isNodeInList((void*)&(referenceProcess->process.processDetails->stepElementsList), step, STEP_NODE);

      if(existingStep != NULL) {
         LV_LOG_USER("Updating element element in list");
         
         //Update name
         lv_label_set_text(existingStep->step.stepName, step->step.stepDetails->stepNameString);

        //Update source
         lv_label_set_text_fmt(existingStep->step.sourceLabel, "From:%s", processSourceList[step->step.stepDetails->source]); 

        //Update discard after icon
         if(step->step.stepDetails->discardAfterProc){
             lv_obj_set_style_text_color(existingStep->step.discardAfterIcon, lv_color_hex(WHITE), LV_PART_MAIN);
           } else {
             lv_obj_set_style_text_color(existingStep->step.discardAfterIcon, lv_color_hex(GREY), LV_PART_MAIN);
           }

          //Update type icon
          if(step->step.stepDetails->type == CHEMISTRY)
              lv_label_set_text(existingStep->step.stepTypeIcon, chemical_icon);
          if(step->step.stepDetails->type == RINSE)
              lv_label_set_text(existingStep->step.stepTypeIcon, rinse_icon);           
          if(step->step.stepDetails->type == MULTI_RINSE)
              lv_label_set_text(existingStep->step.stepTypeIcon, multiRinse_icon); 

          //Update time
          lv_label_set_text_fmt(existingStep->step.stepTime, "%dm%ds", step->step.stepDetails->timeMins, step->step.stepDetails->timeSecs); 
      }
}


uint32_t loadSDCardProcesses() {

    int32_t tempSize = 1;

    if (gui.page.processes.processElementsList.size > 0) {
        processNode *process = gui.page.processes.processElementsList.start;

        while (process != NULL) {
            processElementCreate(process, tempSize);
            process = process->next;
            tempSize++;
        }
        return gui.page.processes.processElementsList.size;
    } else {
        return 0;
    }
}

char* generateRandomCharArray(uint8_t length) {
  char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
  char *randomArray = new char[length + 1]; // Allocazione dinamica dell'array più uno per il terminatore di stringa

  for (uint8_t i = 0; i < length; ++i) {
    randomArray[i] = charset[random(0, sizeof(charset) - 1)];
  }

  randomArray[length] = '\0'; // Aggiungi il terminatore di stringa

  return randomArray;
}

void initializeRelayPins(){
  for (uint8_t i = 0; i < RELAY_NUMBER ; i++) {
        mcp.pinMode(developingRelays[i], OUTPUT);
        mcp.digitalWrite(developingRelays[i], LOW);
        LV_LOG_USER("Relay Initialization %d : %d", developingRelays[i],mcp.digitalRead(developingRelays[i]));
        }
  }


void cleanRelayManager(uint8_t pumpFrom, uint8_t pumpTo,uint8_t pumpDir,bool activePump){
    if (activePump) { // SET TO ON SELECTED RELAYS
        mcp.digitalWrite(pumpFrom, HIGH);
        LV_LOG_USER("From %d on : %d", pumpFrom, mcp.digitalRead(pumpFrom));
        mcp.digitalWrite(pumpTo, HIGH);
        LV_LOG_USER("To %d on : %d", pumpTo, mcp.digitalRead(pumpTo));
        mcp.digitalWrite(pumpDir, HIGH);
        LV_LOG_USER("Direction %d on : %d", pumpDir, mcp.digitalRead(pumpDir));
    } else { // SET TO OFF ALL THE RELAY
        if (pumpFrom != INVALID_RELAY && pumpTo != INVALID_RELAY && pumpDir != INVALID_RELAY) {
            mcp.digitalWrite(pumpFrom, LOW);
            LV_LOG_USER("From %d off : %d", pumpFrom, mcp.digitalRead(pumpFrom));
            mcp.digitalWrite(pumpTo, LOW);
            LV_LOG_USER("To %d off : %d", pumpTo, mcp.digitalRead(pumpTo));
            mcp.digitalWrite(pumpDir, LOW);
            LV_LOG_USER("Direction %d off : %d", pumpDir, mcp.digitalRead(pumpDir));
        } else {
            for (uint8_t i = 0; i < RELAY_NUMBER; i++) {
                mcp.digitalWrite(developingRelays[i], LOW);
                LV_LOG_USER("Relay %d off : %d", developingRelays[i], mcp.digitalRead(developingRelays[i]));
            }
        }
    }

}


void sendValueToRelay(uint8_t pumpFrom, uint8_t pumpDir, bool activePump) {
    if (activePump) { // SET TO ON SELECTED RELAYS
        mcp.digitalWrite(pumpFrom, HIGH);
        LV_LOG_USER("From %d on : %d", pumpFrom, mcp.digitalRead(pumpFrom));
        mcp.digitalWrite(pumpDir, HIGH);
        LV_LOG_USER("Direction %d on : %d", pumpDir, mcp.digitalRead(pumpDir));
        gui.tempProcessNode->process.processDetails->checkup->isAlreadyPumping = true;
    } else { // SET TO OFF ALL THE RELAY
        if (pumpFrom != NULL && pumpDir != NULL) {
            mcp.digitalWrite(pumpFrom, LOW);
            LV_LOG_USER("From %d off : %d", pumpFrom, mcp.digitalRead(pumpFrom));
            mcp.digitalWrite(pumpDir, LOW);
            LV_LOG_USER("Direction %d off : %d", pumpDir, mcp.digitalRead(pumpDir));
        } else {
            for (uint8_t i = 0; i < RELAY_NUMBER; i++) {
                mcp.digitalWrite(developingRelays[i], LOW);
                LV_LOG_USER("Relay %d off : %d", developingRelays[i], mcp.digitalRead(developingRelays[i]));
            }
        }
    }
}


void initializeMotorPins(){
    mcp.pinMode(MOTOR_IN1_PIN , OUTPUT);
    mcp.digitalWrite(MOTOR_IN1_PIN , LOW);
    LV_LOG_USER("Motor Pin Initialization %d: %d",MOTOR_IN1_PIN,mcp.digitalRead(MOTOR_IN1_PIN));

    mcp.pinMode(MOTOR_IN2_PIN , OUTPUT);
    mcp.digitalWrite(MOTOR_IN2_PIN , LOW);
    LV_LOG_USER("Motor Pin Initialization %d: %d",MOTOR_IN2_PIN,mcp.digitalRead(MOTOR_IN2_PIN));

    //this to use the second "i2c" pins as analog
    ledcSetup(0, 5000, 8);
    ledcAttachPin(MOTOR_ENA_PIN, 0);

    LV_LOG_USER("Motor Pin Initialization %d: %d",MOTOR_ENA_PIN,analogRead(MOTOR_ENA_PIN));
}

void stopMotor(uint8_t pin1, uint8_t pin2){
  for (uint8_t dutyCycle = analogVal_rotationSpeedPercent; dutyCycle >= MOTOR_MIN_ANALOG_VAL; dutyCycle--) {
    ledcWrite(0, dutyCycle);
    delay(10); // small delay to see the change in brightness
  }
  mcp.digitalWrite(pin1, LOW);
  mcp.digitalWrite(pin2, LOW);
  ledcWrite(0, 0);
  LV_LOG_USER("Run stopMotor");
}

void runMotorFW(uint8_t pin1, uint8_t pin2){
  mcp.digitalWrite(pin1, HIGH);
  mcp.digitalWrite(pin2, LOW);
  
  for (uint8_t dutyCycle = MOTOR_MIN_ANALOG_VAL; dutyCycle <= analogVal_rotationSpeedPercent; dutyCycle++) {
    ledcWrite(0, dutyCycle);
    delay(10); // small delay to see the change in brightness
  }

  LV_LOG_USER("Run runMotorFW at speed %d",analogVal_rotationSpeedPercent);
}

void runMotorRV(uint8_t pin1, uint8_t pin2){
  mcp.digitalWrite(pin1, LOW);
  mcp.digitalWrite(pin2, HIGH);
  
  for (uint8_t dutyCycle = MOTOR_MIN_ANALOG_VAL; dutyCycle <= analogVal_rotationSpeedPercent; dutyCycle++) {
    ledcWrite(0, dutyCycle);
    delay(10);
  }  
  LV_LOG_USER("Run runMotorRV at speed %d",analogVal_rotationSpeedPercent);
}

void setMotorSpeed(uint8_t pin,uint8_t spd){//max 255
  ledcWrite(0, spd);
  LV_LOG_USER("Set motor speed: %d",spd);
}

void setMotorSpeedUp(uint8_t pin, uint8_t spd){//max 255
  for(uint8_t i = 0; i <= spd; i++){
    ledcWrite(0, i);
    delay(10);
  }
  LV_LOG_USER("Increase speed to: %d",spd);
}

void setMotorSpeedDown(uint8_t pin, uint8_t spd){
  for(uint8_t i = spd; i >= 0; --i){
    ledcWrite(0, i);
    delay(10);
  }
  LV_LOG_USER("Decrease speed to: %d",spd);
}




//++++++++++++++++ READ TEMPERATURE SENSOR METHODS ++++++++++++++++



void initializeTemperatureSensor()
{
  if (!sensorTempBath.begin(TEMPERATURE_BATH_PIN)) {   // Indirizzo I2C del SHT30
    LV_LOG_USER("Could not find a valid sensorTempBath sensor, check wiring!");
  }
  if (!sensorTempChemical.begin(TEMPERATURE_CHEMICAL_PIN)) {   // Indirizzo I2C del SHT30
    LV_LOG_USER("Could not find a valid sensorTempBath sensor, check wiring!");
  }
}

float getTemperature(Adafruit_SHT31 tempSensor){
  float tempC = tempSensor.readTemperature();
  float tempF = (tempC * 9.0 / 5.0) + 32.0;

  if (!isnan(tempC)) {  // Controlla se la lettura è valida
    LV_LOG_USER("Temp *C = %f | Temp *F = %f",tempC, tempF);
    return tempC;
  } else {
    Serial.println("Failed to read temperature");
    return -255; // A value to show it's broken!
  }
}


void testPin(uint8_t pin){
    mcp.digitalWrite(pin, HIGH);
    delay(500);
    mcp.digitalWrite(pin, LOW);
    delay(500);
}

void toLowerCase(char *str) {
    while (*str) {
        *str = tolower((unsigned char)*str);
        str++;
    }
}

uint8_t caseInsensitiveStrstr(const char *haystack, const char *needle) {

    size_t haystackLen = strlen(haystack);
    size_t needleLen = strlen(needle);
    
    char haystackLower[haystackLen + 1];
    char needleLower[needleLen + 1];
    
    strcpy(haystackLower, haystack);
    strcpy(needleLower, needle);
    
    toLowerCase(haystackLower);
    toLowerCase(needleLower);
    
    return strstr(haystackLower, needleLower) != NULL;
}


void filterAndDisplayProcesses() {
    processNode *currentNode = gui.page.processes.processElementsList.start;
    int32_t displayedCount = 1;

    if (gui.page.processes.isFiltered == 1)
        removeFiltersAndDisplayAllProcesses();

    // Debugging info
    LV_LOG_USER("Filter %s, %d, %d, %d", 
                gui.element.filterPopup.filterName ? gui.element.filterPopup.filterName : "", 
                gui.element.filterPopup.isColorFilter, 
                gui.element.filterPopup.isBnWFilter, 
                gui.element.filterPopup.preferredOnly);

    while (currentNode != NULL) {
        bool isFiltered = true;

        // Check name filter
        if (gui.element.filterPopup.filterName != NULL && strlen(gui.element.filterPopup.filterName) > 0) {
            if (!caseInsensitiveStrstr(currentNode->process.processDetails->processNameString, gui.element.filterPopup.filterName)) {
                isFiltered = false;
            }
        }

        // Check film type filter (Color or BnW)
        if (gui.element.filterPopup.isColorFilter) {
            if (currentNode->process.processDetails->filmType != COLOR_FILM) {
                isFiltered = false;
            }
        } else if (gui.element.filterPopup.isBnWFilter) {
            if (currentNode->process.processDetails->filmType != BLACK_AND_WHITE_FILM) {
                isFiltered = false;
            }
        }

        // Check preferred status filter
        if (gui.element.filterPopup.preferredOnly) {
            if (!currentNode->process.processDetails->isPreferred) {
                isFiltered = false;
            }
        }

        currentNode->process.isFiltered = isFiltered;
        currentNode = currentNode->next;
    }

    lv_obj_clean(gui.page.processes.processesListContainer);   
    lv_obj_update_layout(gui.page.processes.processesListContainer);

    currentNode = gui.page.processes.processElementsList.start;
    while (currentNode != NULL) { 
        if (currentNode->process.isFiltered == true) {
            processElementCreate(currentNode, displayedCount);
            displayedCount++;
        }
        currentNode = currentNode->next;
    }

    gui.page.processes.isFiltered = 1;
}


void removeFiltersAndDisplayAllProcesses() {
    processNode *currentNode = gui.page.processes.processElementsList.start;
    if (currentNode == NULL) {
        LV_LOG_USER("processElementsList.start is NULL");
        return;
    }

    int32_t displayedCount = 1;  // Initialize count

    // Pulizia del contenitore della lista dei processi per prepararlo alla visualizzazione di tutti i processi
    lv_obj_clean(gui.page.processes.processesListContainer);
    lv_obj_update_layout(gui.page.processes.processesListContainer);
    // Itera attraverso tutti i nodi dei processi e li visualizza
    while (currentNode != NULL) {
        LV_LOG_USER("Process %d created",displayedCount);
        processElementCreate(currentNode, displayedCount);
        currentNode = currentNode->next;
        displayedCount++;
    }

    // Aggiorna il layout del contenitore per riallineare gli elementi visibili
    lv_obj_update_layout(gui.page.processes.processesListContainer);
}




void emptyList(void *list, NodeType_t type) {
    if (list == NULL) {
        return;
    }

    // Determina il tipo di lista e imposta i puntatori generici di conseguenza
    if (type == PROCESS_NODE) {  // processList
        processList *plist = (processList *)list;
        processNode *currentNode = plist->start;

        while (currentNode != NULL) {
            processNode *nextNode = currentNode->next;
            //free(currentNode); //CAUSE CRASH!!
            currentNode = nextNode;
        }

        // Reimposta le proprietà della lista
        plist->start = NULL;
        plist->end = NULL;
        plist->size = 0;
    } else if (type == STEP_NODE) {  // stepList
        stepList *slist = (stepList *)list;
        stepNode *currentNode = slist->start;

        while (currentNode != NULL) {
            stepNode *nextNode = currentNode->next;
            //free(currentNode); //CAUSE CRASH!!
            currentNode = nextNode;
        }

        // Reimposta le proprietà della lista
        slist->start = NULL;
        slist->end = NULL;
        slist->size = 0;
    }
}


void readMachineStats(machineStatistics * machineStats) {  
  // Apri il namespace "stats" in modalità RO
  preferences.begin("stats", true);

  // Leggi i valori dalla memoria, usa valori di default se non esistono
  machineStats->completed = preferences.getULong("completed", 0);
  machineStats->totalMins = preferences.getULong64("totalMins", 0);
  machineStats->clean = preferences.getULong("clean", 0);
  machineStats->stopped = preferences.getULong("stopped", 0);

#if FILM_USE_LOG != 0
  LV_LOG_USER("Get values: \ncompletedProcesses: %d \ntotalMins: %llu \ncompletedCleanCycle: %d \nstoppedProcesses: %d\n", 
                machineStats->completed, 
                machineStats->totalMins, 
                machineStats->clean, 
                machineStats->stopped);
#endif
  // Chiudi il namespace
  preferences.end();
}

void writeMachineStats(machineStatistics * machineStats) {
  // Apri il namespace "stats" in modalità RW
  preferences.begin("stats", false);

  // Scrivi i valori nella memoria
  preferences.putUInt("completed", machineStats->completed);
  preferences.putULong64("totalMins", machineStats->totalMins);
  preferences.putUInt("clean",machineStats->clean);
  preferences.putUInt("stopped", machineStats->stopped);

  // Chiudi il namespace
  preferences.end();

  LV_LOG_USER("Set values: \ncompletedProcesses: %d \ntotalMins: %llu \ncompletedCleanCycle: %d \nstoppedProcesses: %d\n", 
                machineStats->completed, 
                machineStats->totalMins, 
                machineStats->clean, 
                machineStats->stopped);
  }


uint32_t findRolleStringIndex(const char *target, const char *array) {
    const char *start = array;
    uint32_t index = 0;

    while (*start != '\0') {
        // Troviamo la fine della stringa corrente
        const char *end = strchr(start, '\n');
        if (end == NULL) {
            end = start + strlen(start);  // Se non troviamo '\n', prendiamo fino alla fine della stringa
        }

        // Lunghezza della stringa corrente
        uint32_t length = end - start;

        // Confrontiamo la sottostringa
        if (strncmp(start, target, length) == 0 && target[length] == '\0') {
            return index;
        }

        // Passiamo alla prossima riga
        start = end;
        if (*start == '\n') {
            start++;  // Salta il '\n' se lo troviamo
        }
        index++;
    }

    return -1; // Stringa non trovata
}


char* getRollerStringIndex(uint32_t index, const char *list) {
    const char *start = list;
    const char *end;
    uint32_t currentIndex = 0;

    while ((end = strchr(start, '\n')) != NULL) {
        if (currentIndex == index) {
            uint32_t length = end - start;
            char *result = (char*) malloc((length + 1) * sizeof(char));
            if (result == NULL) {
                return NULL; // Allocation failed
            }
            strncpy(result, start, length);
            result[length] = '\0';
            return result;
        }
        start = end + 1;
        currentIndex++;
    }

    // Check for the last item in the list (without a trailing newline)
    if (currentIndex == index) {
        uint32_t length = strlen(start);
        char *result = (char*) malloc((length + 1) * sizeof(char));
        if (result == NULL) {
            return NULL; // Allocation failed
        }
        strcpy(result, start);
        return result;
    }

    // If index is out of bounds
    return NULL;
}


bool copyAndRenameFile(fs::FS &fs, const char* sourceFile, const char* destFile) {
    // Verifica se il file di destinazione esiste già e rimuovilo se necessario
    if (fs.exists(destFile)) {
        fs.remove(destFile);
    }

    // Apre il file sorgente in modalità lettura
    File srcFile = fs.open(sourceFile, FILE_READ);
    if (!srcFile) {
        LV_LOG_USER("Errore nell'apertura del file sorgente!");
        return false;
    }

    // Crea o sovrascrive il file destinazione
    File destFileObj = fs.open(destFile, FILE_WRITE);
    if (!destFileObj) {
        LV_LOG_USER("Errore nell'apertura del file destinazione!");
        srcFile.close();
        return false;
    }

    // Legge il contenuto del file sorgente e lo scrive nel file destinazione
    while (srcFile.available()) {
        destFileObj.write(srcFile.read());
    }

    // Chiude entrambi i file
    srcFile.close();
    destFileObj.close();

    // Verifica se il file destinazione è stato creato correttamente e se non è vuoto
    if (fs.exists(destFile)) {
        File checkFile = fs.open(destFile, FILE_READ);
        if (checkFile && checkFile.size() > 0) {
            LV_LOG_USER("File copiato e rinominato con successo!");
            checkFile.close();
            return true;
        } else {
            LV_LOG_USER("Errore: il file di destinazione è vuoto o non può essere letto.");
        }
        checkFile.close();
    } else {
        LV_LOG_USER("Errore: il file di destinazione non è stato creato correttamente.");
    }

    return false;
}

char* generateRandomSuffix(const char* baseName) {
    static char newProcessName[MAX_PROC_NAME_LEN + 1]; // Include spazio per il carattere nullo

    // Controlla se baseName ha già un suffisso numerico
    size_t len = strlen(baseName);
    uint8_t suffix = 1; // Suffisso iniziale

    // Calcola la lunghezza massima possibile per il nome base (senza suffisso)
    size_t maxBaseLen = MAX_PROC_NAME_LEN - 4; // 4 caratteri per "_000"

    if (len > maxBaseLen) {
        // Se baseName è più lungo del massimo consentito per il nome base, accorcialo
        strncpy(newProcessName, baseName, maxBaseLen); // Copia solo maxBaseLen caratteri
        newProcessName[maxBaseLen] = '\0'; // Assicura che ci sia il terminatore nullo
        len = maxBaseLen; // Aggiorna la lunghezza effettiva del nuovo nome base
    } else {
        // Altrimenti copialo direttamente
        strncpy(newProcessName, baseName, sizeof(newProcessName)); // Copia tutta la stringa
        newProcessName[sizeof(newProcessName) - 1] = '\0'; // Assicura che ci sia il terminatore nullo
    }

    // Verifica se baseName ha già un suffisso numerico valido
    if (len > 4 && baseName[len - 4] == '_' && isdigit(baseName[len - 3]) && isdigit(baseName[len - 2]) && isdigit(baseName[len - 1])) {
        // Se baseName ha già un suffisso numerico, estrai il suffisso corrente
        suffix = atoi(&baseName[len - 3]); // Ottieni il suffisso numerico
        newProcessName[len - 4] = '\0'; // Rimuovi il suffisso numerico per creare il nuovo nome base
    }

    bool uniqueNameFound = false;

    while (!uniqueNameFound) {
        // Crea il nuovo suffisso
        char suffixStr[5]; // 4 caratteri per "_000" + 1 per il terminatore nullo
        snprintf(suffixStr, sizeof(suffixStr), "_%03d", suffix); // Formatta il suffisso a tre cifre con zeri a sinistra se necessario

        // Concatena il suffisso al nuovo nome
        strncat(newProcessName, suffixStr, sizeof(newProcessName) - strlen(newProcessName) - 1);

        // Verifica se il nuovo nome con suffisso è unico nella lista
        uniqueNameFound = true;
        processNode *current = gui.page.processes.processElementsList.start;
        while (current != NULL) {
            if (strcmp(current->process.processDetails->processNameString, newProcessName) == 0) {
                uniqueNameFound = false;
                suffix++; // Incrementa il suffisso
                // Rimuovi il suffisso aggiunto per ricominciare il ciclo e provare con un nuovo suffisso
                size_t baseLen = strlen(newProcessName) - 4; // Lunghezza del nome base senza il suffisso
                newProcessName[baseLen] = '\0'; // Termina la stringa dopo il nome base
                break;
            }
            current = current->next;
        }
    }

    return newProcessName;
}


sStepDetail *deepCopyStepDetail(sStepDetail *original) {
    if (original == NULL) return NULL;
    sStepDetail *copy = (sStepDetail*)malloc(sizeof(sStepDetail));
    if (copy == NULL) return NULL;
    memcpy(copy, original, sizeof(sStepDetail));
    // Copia profonda di eventuali strutture interne se necessario
    return copy;
}

singleStep *deepCopySingleStep(singleStep *original) {
    if (original == NULL) return NULL;
    singleStep *copy = (singleStep*)malloc(sizeof(singleStep));
    if (copy == NULL) return NULL;
    memcpy(copy, original, sizeof(singleStep));
    copy->stepDetails = deepCopyStepDetail(original->stepDetails);
    // Copia profonda di eventuali strutture interne se necessario
    return copy;
}

stepNode *deepCopyStepNode(stepNode *original) {
    if (original == NULL) return NULL;
    stepNode *copy = (stepNode*)malloc(sizeof(stepNode));
    if (copy == NULL) return NULL;
    memcpy(copy, original, sizeof(stepNode));
    copy->step = *deepCopySingleStep(&original->step);
    return copy;
}

stepList deepCopyStepList(stepList original) {
    stepList copy;
    copy.start = NULL;
    copy.end = NULL;
    copy.size = original.size;
    stepNode *current = original.start;
    stepNode **copyCurrent = &copy.start;

    while (current != NULL) {
        *copyCurrent = deepCopyStepNode(current);
        if (*copyCurrent != NULL) {
            (*copyCurrent)->prev = (copyCurrent == &copy.start) ? NULL : (*(copyCurrent - 1));
            copyCurrent = &(*copyCurrent)->next;
        }
        current = current->next;
    }

    copy.end = (copyCurrent == &copy.start) ? NULL : (*(copyCurrent - 1));
    return copy;
}

sCheckup *deepCopyCheckup(sCheckup *original) {
    if (original == NULL) return NULL;
    sCheckup *copy = (sCheckup*)malloc(sizeof(sCheckup));
    if (copy == NULL) return NULL;
    memcpy(copy, original, sizeof(sCheckup));
    // Copia profonda di eventuali strutture interne se necessario
    return copy;
}

sProcessDetail *deepCopyProcessDetail(sProcessDetail *original) {
    if (original == NULL) return NULL;
    sProcessDetail *copy =  (sProcessDetail*)malloc(sizeof(sProcessDetail));
    if (copy == NULL) return NULL;
    memcpy(copy, original, sizeof(sProcessDetail));
    copy->stepElementsList = deepCopyStepList(original->stepElementsList);
    copy->checkup = deepCopyCheckup(original->checkup);
    return copy;
}

singleProcess *deepCopySingleProcess(singleProcess *original) {
    if (original == NULL) return NULL;
    singleProcess *copy =  (singleProcess*)malloc(sizeof(singleProcess));
    if (copy == NULL) return NULL;
    memcpy(copy, original, sizeof(singleProcess));
    copy->processDetails = deepCopyProcessDetail(original->processDetails);
    return copy;
}

struct processNode *deepCopyProcessNode(struct processNode *original) {
    if (original == NULL) return NULL;
    struct processNode *copy =  (processNode*)malloc(sizeof(struct processNode));
    if (copy == NULL) return NULL;
    memcpy(copy, original, sizeof(struct processNode));
    copy->process = *deepCopySingleProcess(&original->process);
    return copy;
}

char *ftoa(char *a, float f, uint8_t precisione) {
   if (precisione > 8) return NULL;
   //
   long p[]  = {0, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000};
   char *ret = a;
   long interi   = (long)f;
   long decimali = abs( (long)( (f - interi) * p[precisione] ) );
   //
   ltoa(interi, a, 10);
   while (*a != '\0') a++;
   *a = '.';
   a++;
   ltoa(decimali, a, 10);
   return ret;
}

uint8_t getValueForChemicalSource(uint8_t source) {
    switch (source) {
        case C1:
            return C1_RLY;
        case C2:
            return C2_RLY;
        case C3:
            return C3_RLY;
        case WASTE:
            return WASTE_RLY;
        case WB:
            return WB_RLY; // if is water, the drain is always on waste
        default:
            return 255;
    }
}




void getMinutesAndSeconds(uint8_t containerFillingTime, const bool containerToClean[3]) {
    // Moltiplica containerFillingTime per 2
    uint32_t totalTime = (containerFillingTime * 2) * gui.element.cleanPopup.cleanCycles;

    // Conta il numero di contenitori selezionati
    uint8_t selectedContainersCount = 0;
    for (uint8_t i = 0; i < 3; ++i) {
        if (containerToClean[i]) {
            selectedContainersCount++;
        }
    }

    // Moltiplica il tempo totale per il numero di contenitori selezionati
    totalTime *= selectedContainersCount;

    // Calcola minuti e secondi
    gui.element.cleanPopup.totalMins = totalTime / 60;
    gui.element.cleanPopup.totalSecs = totalTime % 60;
}


uint8_t getRandomRotationInterval() {
    uint8_t baseTime = gui.page.settings.settingsParams.rotationIntervalSetpoint;
    uint8_t randomPercentage = gui.page.settings.settingsParams.randomSetpoint;

    // Calculate percentage
    uint8_t variation = (baseTime * randomPercentage) / 100;
    
    // Calculate the intervals
    uint8_t minValue = baseTime - variation; // Minimo valore che può essere restituito
    uint8_t maxValue = baseTime;             // Massimo valore che può essere restituito

    // Calculate the offset between [-variation, variation]
    int8_t randomOffset = (rand() % (2 * variation + 1)) - variation; // [ -variation, variation ]
    
    uint8_t result = minValue + randomOffset;
    
    // Limit values between 5 and 60 seconds, the min e max value for rotationIntervalSetpoint.
    if (result > 60) {
        result = 60;
    }
    if (result < 5) {
        result = 5;
    }

    LV_LOG_USER("baseTime: %d, randomPercentage: %d, variation: %d, randomOffset: %d, result: %d",
                baseTime, randomPercentage, variation, randomOffset, result);

    return result;
}



void pwmLedTest(){
   LV_LOG_USER("pwmLedTest");
   for (uint8_t dutyCycle = MOTOR_MIN_ANALOG_VAL; dutyCycle <= MOTOR_MAX_ANALOG_VAL; dutyCycle++) {
    ledcWrite(0, dutyCycle);
    mcp.digitalWrite(MOTOR_IN1_PIN, LOW);
    mcp.digitalWrite(MOTOR_IN2_PIN, HIGH);
    LV_LOG_USER("dutyCycle %d",dutyCycle);
    delay(10); // small delay to see the change in brightness
  }

  // Example: Decrease LED brightness
  for (uint8_t dutyCycle = MOTOR_MAX_ANALOG_VAL; dutyCycle >= MOTOR_MIN_ANALOG_VAL; dutyCycle--) {
    ledcWrite(0, dutyCycle);
    mcp.digitalWrite(MOTOR_IN1_PIN, LOW);
    mcp.digitalWrite(MOTOR_IN2_PIN, HIGH);
    LV_LOG_USER("dutyCycle %d",dutyCycle);
    delay(10); // small delay to see the change in brightness
  }
}

uint8_t mapPercentageToValue(uint8_t percentage, uint8_t minPercent, uint8_t maxPercent) {
    uint8_t value = ((percentage - minPercent) * (MOTOR_MAX_ANALOG_VAL - MOTOR_MIN_ANALOG_VAL)) / (maxPercent - minPercent) + MOTOR_MIN_ANALOG_VAL;
    return value;
}
