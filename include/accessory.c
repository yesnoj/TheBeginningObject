#include "core/lv_obj.h"
#include "misc/lv_event.h"
#include "lv_api_map_v8.h"
/**
 * @file accessory.c
 *
 */


/*********************
 *      INCLUDES
 *********************/
#include "lvgl.h"
#include "definitions.h"
#include <ArduinoJson.h>

#include <FS.h>
#include <SPI.h>
#include <Wire.h>
/**********************
 *      MACROS
 **********************/

#define LESS_THAN_10 1
#define GREATER_THAN_9 2
#define GREATER_THAN_99 3

struct gui_components	gui;

extern LGFX lcd;
void (*rebootBoard)(void) = 0;

/* Put a system request in the queue returns true if succesful false if queue is full */
uint8_t qSysAction( uint16_t msg ) {
  
  return xQueueSend( gui.sysActionQ, &msg, 0 );
}

void event_cb(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t * obj = (lv_obj_t *)lv_event_get_target(e);
  lv_obj_t * objCont = (lv_obj_t *)lv_obj_get_parent(obj);
  lv_obj_t * mboxCont = (lv_obj_t *)lv_obj_get_parent(objCont);

    //const char *label = lv_msgbox_get_active_btn(mbox);
    //LV_UNUSED(label);
    lv_msgbox_close(mboxCont);
    lv_obj_delete(mboxCont);
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
  lv_obj_t * objCont = (lv_obj_t *)lv_obj_get_parent(obj);
  lv_obj_t * mboxCont = (lv_obj_t *)lv_obj_get_parent(objCont);
  lv_obj_t * mboxParent = (lv_obj_t *)lv_obj_get_parent(mboxCont);
  lv_obj_t * godFatherCont = (lv_obj_t *)lv_obj_get_parent(mboxParent);
  lv_obj_t * data = (lv_obj_t *)lv_event_get_user_data(e);

   if(code == LV_EVENT_CLICKED){ 
      if(obj == gui.element.filterPopup.mBoxNameTextArea){
          LV_LOG_USER("LV_EVENT_FOCUSED on filterPopup.mBoxNameTextArea");
          lv_obj_set_user_data(gui.element.keyboardPopup.keyboard,obj);

          showKeyboard(gui.element.filterPopup.mBoxFilterPopupParent);
      }
      if(obj == tempProcessNode->process.processDetails->processDetailNameTextArea){
          LV_LOG_USER("LV_EVENT_FOCUSED on processDetailNameTextArea");
          lv_obj_set_user_data(gui.element.keyboardPopup.keyboard,obj);

          showKeyboard(tempProcessNode->process.processDetails->processDetailParent);
      }
      if(obj == tempStepNode->step.stepDetails->stepDetailNamelTextArea){
          LV_LOG_USER("LV_EVENT_FOCUSED on stepDetailNamelTextArea");
          lv_obj_set_user_data(gui.element.keyboardPopup.keyboard,obj);

          showKeyboard(tempStepNode->step.stepDetails->stepDetailParent);
      }
   }
  
  if(code == LV_EVENT_DEFOCUSED){
      if(obj == gui.element.filterPopup.mBoxNameTextArea){
          LV_LOG_USER("LV_EVENT_DEFOCUSED on filterPopup.mBoxNameTextArea");
          hideKeyboard(gui.element.filterPopup.mBoxFilterPopupParent);
      }
      if(obj == tempProcessNode->process.processDetails->processDetailNameTextArea){
          LV_LOG_USER("LV_EVENT_DEFOCUSED on processDetailNameTextArea");
          hideKeyboard(tempProcessNode->process.processDetails->processDetailParent);
      }
      if(obj == tempStepNode->step.stepDetails->stepDetailNamelTextArea){
          LV_LOG_USER("LV_EVENT_DEFOCUSED on stepDetailNamelTextArea");
          hideKeyboard(tempStepNode->step.stepDetails->stepDetailParent);
      }
   }
    
  if (code == LV_EVENT_CANCEL) {
      //after the LV_EVENT_FOCUSED, the caller send it self to the keyboard as userData
      LV_LOG_USER("LV_EVENT_CANCEL PRESSED");
      if(lv_obj_get_user_data(gui.element.keyboardPopup.keyboard) == gui.element.filterPopup.mBoxNameTextArea){
          LV_LOG_USER("LV_EVENT_DEFOCUSED on filterPopup.mBoxNameTextArea");
          hideKeyboard(gui.element.filterPopup.mBoxFilterPopupParent);
      }
      if(lv_obj_get_user_data(gui.element.keyboardPopup.keyboard) == tempProcessNode->process.processDetails->processDetailNameTextArea){
          LV_LOG_USER("LV_EVENT_DEFOCUSED on processDetailNameTextArea");
          hideKeyboard(tempProcessNode->process.processDetails->processDetailParent);
      }
      if(lv_obj_get_user_data(gui.element.keyboardPopup.keyboard) == tempStepNode->step.stepDetails->stepDetailNamelTextArea){
          LV_LOG_USER("LV_EVENT_DEFOCUSED on stepDetailNamelTextArea");
          hideKeyboard(tempStepNode->step.stepDetails->stepDetailParent);
      }
    }
    if (code == LV_EVENT_READY) {
      LV_LOG_USER("LV_EVENT_READY PRESSED");

            if(lv_obj_get_user_data(gui.element.keyboardPopup.keyboard) == gui.element.filterPopup.mBoxNameTextArea){
              LV_LOG_USER("Press ok from filterPopup.mBoxFilterPopupParent");
              lv_textarea_set_text(gui.element.filterPopup.mBoxNameTextArea, lv_textarea_get_text(gui.element.keyboardPopup.keyboardTextArea));
              gui.element.filterPopup.filterName = lv_textarea_get_text(gui.element.keyboardPopup.keyboardTextArea);
              
              hideKeyboard(gui.element.filterPopup.mBoxFilterPopupParent);
            }
            if(lv_obj_get_user_data(gui.element.keyboardPopup.keyboard) == tempProcessNode->process.processDetails->processDetailNameTextArea){
              LV_LOG_USER("Press ok from processDetailNameTextArea");
              lv_textarea_set_text(tempProcessNode->process.processDetails->processDetailNameTextArea, lv_textarea_get_text(gui.element.keyboardPopup.keyboardTextArea));

              hideKeyboard(tempProcessNode->process.processDetails->processDetailParent);
            }
            if(lv_obj_get_user_data(gui.element.keyboardPopup.keyboard) == tempStepNode->step.stepDetails->stepDetailNamelTextArea){
              LV_LOG_USER("Press ok from stepDetailNamelTextArea");
              lv_textarea_set_text(tempStepNode->step.stepDetails->stepDetailNamelTextArea, lv_textarea_get_text(gui.element.keyboardPopup.keyboardTextArea));

              hideKeyboard(tempStepNode->step.stepDetails->stepDetailParent);
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
    lv_obj_add_event_cb(gui.element.keyboardPopup.keyboard, event_keyboard, LV_EVENT_ALL, NULL);
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

float convertCelsiusoToFahrenheit(uint32_t tempC){
  return ((tempC * 1.8) + 32); 
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

char *createRollerValues( uint32_t minVal ,uint32_t maxVal, const char* extra_str ) {
    uint32_t buf_len = calc_buf_len( maxVal, strlen(extra_str));
    uint32_t buf_ptr = 0;
    char *buf = (char *)malloc( buf_len );

    for( uint32_t i = minVal; i <= maxVal; i++ ) {
        buf_ptr += lv_snprintf( &buf[buf_ptr], (buf_len - buf_ptr), "%s%d\n", extra_str, i );
    }
    //LV_LOG_USER("Created :%s",buf);
    return buf;
}


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
    
    // Initialize and allocate according the node type
    switch (type) {
        case STEP_NODE:
            node = malloc(sizeof(stepNode));
            if (node != NULL) {
                memset(node, 0, sizeof(stepNode));
                stepNode* step = (stepNode*)node;
                step->step.stepDetails = (sStepDetail *) malloc(sizeof(sStepDetail));
                if (step->step.stepDetails == NULL) {
                //    // Handle memory allocation failure
                   free(step->step.stepDetails);  // Clean up previously allocated memory
                }
                memset(step->step.stepDetails, 0, sizeof(sStepDetail));
             }
        break;
        
        case PROCESS_NODE:
            node = malloc(sizeof(processNode));
            if (node != NULL) {
                memset(node, 0, sizeof(processNode));
                processNode* process = (processNode*)node;
                process->process.processDetails = (sProcessDetail *)malloc(sizeof(sProcessDetail));
                if (process->process.processDetails == NULL) {
                    // Handle memory allocation failure
                    free(process);
                    return NULL;
                }
                memset(process->process.processDetails, 0, sizeof(sProcessDetail));
                
                process->process.processDetails->checkup = (sCheckup *)malloc(sizeof(sCheckup));
                if (process->process.processDetails->checkup == NULL) {
                    // Handle memory allocation failure
                    free(process->process.processDetails);
                    free(process);
                    return NULL;
                }
                memset(process->process.processDetails->checkup, 0, sizeof(sCheckup));
            }
            break;
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



void init_globals( void ) {
  // Initialise the main GUI structure to zero
	memset(&gui, 0, sizeof(gui));		
  
  gui.page.processes.processElementsList.start = NULL;
  gui.page.processes.processElementsList.end   = NULL;
  gui.page.processes.processElementsList.size  = 0;

  // We only need to initialise the non-zero values
  gui.element.filterPopup.titleLinePoints[1].x = 200;
  gui.element.rollerPopup.titleLinePoints[1].x = 200;
  gui.element.messagePopup.titleLinePoints[1].x = 200;

  gui.page.processes.titleLinePoints[1].x = 310;
  gui.page.settings.titleLinePoints[1].x = 310;
  gui.page.tools.titleLinePoints[1].x = 310;
  
  gui.element.rollerPopup.tempCelsiusOptions = createRollerValues(1,40,"");
  gui.element.rollerPopup.minutesOptions = createRollerValues(0,240,"");
  gui.element.rollerPopup.secondsOptions = createRollerValues(0,60,""); 
  gui.element.rollerPopup.tempCelsiusToleranceOptions = createRollerValues(0,5,"0.");

  tempProcessNode = (processNode*) allocateAndInitializeNode(PROCESS_NODE);
  tempStepNode = (stepNode*) allocateAndInitializeNode(STEP_NODE);  
 
  create_keyboard();
}

//without the commented part, the keyboard will be shown OVER the caller
void showKeyboard(lv_obj_t * whoCallMe){
    //lv_obj_add_flag(whoCallMe, LV_OBJ_FLAG_HIDDEN);
    lv_textarea_set_text(gui.element.keyboardPopup.keyboardTextArea, "");
    lv_obj_remove_flag(gui.element.keyboardPopup.keyBoardParent, LV_OBJ_FLAG_HIDDEN);
    lv_obj_move_foreground(gui.element.keyboardPopup.keyBoardParent);
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


int SD_init()
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


void initSD_I2C(){
  if (SD_init())
    {
        initErrors += 1;
        LV_LOG_USER("ERROR:   SD initErrors %d",initErrors);
    }
    else
        LV_LOG_USER("SD INIT OVER initErrors %d",initErrors);

    //I2C init
    Wire.begin(I2C_SDA, I2C_SCL);
    Wire.beginTransmission(I2C_ADR);

    if (Wire.endTransmission() == 0)
    {
        LV_LOG_USER("I2C device found at address 0x%x! TOUCH INIT OVER",I2C_ADR);
    }
    else
    {
        initErrors += 1;
        LV_LOG_USER("Unknown error at address 0x%x ERROR:   TOUCH",I2C_ADR);
    }

    lcd.setCursor(0, 64);
    if (initErrors)
    {

        LV_LOG_USER("SOMETHING WRONG initErrors %d",initErrors);
    }
    else
        LV_LOG_USER("ALL SUCCESS initErrors %d",initErrors);
}


machineSettings readJSONFile(fs::FS &fs, const char *filename, machineSettings &settings) {
    File file = fs.open(filename);
    
    StaticJsonDocument<192> doc;

    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, file);
    if (error)
      LV_LOG_USER("Failed to read file, using default configuration");
    else{
    // Copy values from the JsonDocument to the Config
        settings.tempUnit = doc["tempUnit"];                 
        settings.waterInlet = doc["waterInlet"];                
        settings.calibratedTemp = doc["calibratedTemp"] ;
        settings.filmRotationSpeedSetpoint = doc["filmRotationSpeedSetpoint"];
        settings.rotationIntervalSetpoint = doc["rotationIntervalSetpoint"];
        settings.randomSetpoint = doc["randomSetpoint"];
        settings.isPersistentAlarm = doc["isPersistentAlarm"];
        settings.isProcessAutostart = doc["isProcessAutostart"];
        settings.drainFillOverlapSetpoint = doc["drainFillOverlapSetpoint"];

        // Close the file (Curiously, File's destructor doesn't close the file)
    }
   
    file.close();
    return settings;
}

void writeJSONFile(fs::FS &fs, const char *path,const machineSettings &settings){
  if(initErrors == 0){
    LV_LOG_USER("Writing file: %s", path);
    SD.remove(path);

    File file = fs.open(path, FILE_WRITE);
    if (!file) {
        LV_LOG_USER("Failed to open file for writing");
        rebootBoard();
        //return;
    }

    StaticJsonDocument<192> doc;

    doc["tempUnit"]                   = settings.tempUnit;
    doc["waterInlet"]                 = settings.waterInlet;
    doc["calibratedTemp"]             = settings.calibratedTemp;
    doc["filmRotationSpeedSetpoint"]  = settings.filmRotationSpeedSetpoint;
    doc["rotationIntervalSetpoint"]   = settings.rotationIntervalSetpoint;
    doc["randomSetpoint"]             = settings.randomSetpoint;
    doc["isPersistentAlarm"]          = settings.isPersistentAlarm;
    doc["isProcessAutostart"]         = settings.isProcessAutostart;
    doc["drainFillOverlapSetpoint"]   = settings.drainFillOverlapSetpoint;

    if (serializeJson(doc, file)) {
        file.close();
        LV_LOG_USER("File written successfully");
    } else {
        LV_LOG_USER("Write failed");
    }
    file.close();
  }
  else
    return;
}

gui_components readFULLJSONFile(fs::FS &fs, const char *filename, gui_components &gui, uint32_t enableLog) {
    if(initErrors == 0){
        File file = fs.open(filename);
        
        DynamicJsonDocument doc(49152);

        // Deserialize the JSON document
        DeserializationError error = deserializeJson(doc, file);
        if (error) {
            LV_LOG_USER("Failed to read file, using default configuration");
        } else {
            // Copy values from the JsonDocument to the Config
            JsonObject machineSettings = doc["machineSettings"];
            gui.page.settings.settingsParams.tempUnit                  = machineSettings["tempUnit"];                 
            gui.page.settings.settingsParams.waterInlet                = machineSettings["waterInlet"];                
            gui.page.settings.settingsParams.calibratedTemp            = machineSettings["calibratedTemp"];
            gui.page.settings.settingsParams.filmRotationSpeedSetpoint = machineSettings["filmRotationSpeedSetpoint"];
            gui.page.settings.settingsParams.rotationIntervalSetpoint  = machineSettings["rotationIntervalSetpoint"];
            gui.page.settings.settingsParams.randomSetpoint            = machineSettings["randomSetpoint"];
            gui.page.settings.settingsParams.isPersistentAlarm         = machineSettings["isPersistentAlarm"];
            gui.page.settings.settingsParams.isProcessAutostart        = machineSettings["isProcessAutostart"];
            gui.page.settings.settingsParams.drainFillOverlapSetpoint  = machineSettings["drainFillOverlapSetpoint"];

            if(enableLog){
                LV_LOG_USER("--- MACHINE PARAMS ---");
                LV_LOG_USER("tempUnit:%d",gui.page.settings.settingsParams.tempUnit);
                LV_LOG_USER("waterInlet:%d",gui.page.settings.settingsParams.waterInlet);
                LV_LOG_USER("calibratedTemp:%d",gui.page.settings.settingsParams.calibratedTemp);
                LV_LOG_USER("filmRotationSpeedSetpoint:%d",gui.page.settings.settingsParams.filmRotationSpeedSetpoint);
                LV_LOG_USER("rotationIntervalSetpoint:%d",gui.page.settings.settingsParams.rotationIntervalSetpoint);
                LV_LOG_USER("randomSetpoint:%d",gui.page.settings.settingsParams.randomSetpoint);
                LV_LOG_USER("isPersistentAlarm:%d",gui.page.settings.settingsParams.isPersistentAlarm);
                LV_LOG_USER("isProcessAutostart:%d",gui.page.settings.settingsParams.isProcessAutostart);
                LV_LOG_USER("drainFillOverlapSetpoint:%d",gui.page.settings.settingsParams.drainFillOverlapSetpoint);
            }

            JsonObject Filter = doc["Filter"];
            gui.element.filterPopup.filterName = Filter["filterName"];
            gui.element.filterPopup.isColorFilter = Filter["isColorFilter"];
            gui.element.filterPopup.isBnWFilter = Filter["isBnWFilter"];
            gui.element.filterPopup.preferredOnly = Filter["preferredOnly"];

            if(enableLog){
                LV_LOG_USER("--- FILTER PARAMS ---");
                LV_LOG_USER("filterName:%s",gui.element.filterPopup.filterName);
                LV_LOG_USER("isColorFilter:%d",gui.element.filterPopup.isColorFilter);
                LV_LOG_USER("isBnWFilter:%d",gui.element.filterPopup.isBnWFilter);
                LV_LOG_USER("preferredOnly:%d",gui.element.filterPopup.preferredOnly);
            }

            processList *processElementsList = &(gui.page.processes.processElementsList);
            processElementsList->start = NULL;
            processElementsList->end = NULL;
            processElementsList->size = 0;

            for (JsonPair Processe : doc["Processes"].as<JsonObject>()) {
                processNode *nodeP = (processNode*) allocateAndInitializeNode(PROCESS_NODE);

                // Assign process details
                nodeP->process.processDetails->processNameString = Processe.value()["processNameString"];
                nodeP->process.processDetails->temp = Processe.value()["temp"];
                nodeP->process.processDetails->tempTolerance = Processe.value()["tempTolerance"];
                nodeP->process.processDetails->isTempControlled = Processe.value()["isTempControlled"];
                nodeP->process.processDetails->isPreferred = Processe.value()["isPreferred"];
                nodeP->process.processDetails->filmType = Processe.value()["filmType"];
                nodeP->process.processDetails->timeMins = Processe.value()["timeMins"];
                nodeP->process.processDetails->timeSecs = Processe.value()["timeSecs"];

                if (processElementsList->start == NULL) {
                    processElementsList->start = nodeP;
                    nodeP->prev = NULL;
                } else {
                    processElementsList->end->next = nodeP;
                    nodeP->prev = processElementsList->end;
                }
                processElementsList->end = nodeP;
                processElementsList->end->next = NULL;
                processElementsList->size++;

                if(enableLog){
                    LV_LOG_USER("--- PROCESS PARAMS ---");
                    LV_LOG_USER("processNameString:%s",nodeP->process.processDetails->processNameString);
                    LV_LOG_USER("temp:%d",nodeP->process.processDetails->temp);
                    LV_LOG_USER("tempTolerance:%d",nodeP->process.processDetails->tempTolerance);
                    LV_LOG_USER("isTempControlled:%d",nodeP->process.processDetails->isTempControlled);
                    LV_LOG_USER("isPreferred:%d",nodeP->process.processDetails->isPreferred);
                    LV_LOG_USER("filmType:%d",nodeP->process.processDetails->filmType);
                    LV_LOG_USER("timeMins:%d",nodeP->process.processDetails->timeMins);
                    LV_LOG_USER("timeSecs:%d",nodeP->process.processDetails->timeSecs);
                }

                stepList *stepElementsList = &(nodeP->process.processDetails->stepElementsList);
                stepElementsList->start = NULL;
                stepElementsList->end = NULL;
                stepElementsList->size = 0;

                for (JsonPair Processe_value_Step : Processe.value()["Steps"].as<JsonObject>()) {
                    stepNode *nodeS = (stepNode*) allocateAndInitializeNode(STEP_NODE);

                    // Assign step details
                    nodeS->step.stepDetails->stepNameString = Processe_value_Step.value()["stepNameString"];
                    nodeS->step.stepDetails->timeMins = Processe_value_Step.value()["timeMins"];
                    nodeS->step.stepDetails->timeSecs = Processe_value_Step.value()["timeSecs"];
                    nodeS->step.stepDetails->type = Processe_value_Step.value()["type"];
                    nodeS->step.stepDetails->source = Processe_value_Step.value()["source"];
                    nodeS->step.stepDetails->discardAfterProc = Processe_value_Step.value()["discardAfterProc"];

                    if (stepElementsList->start == NULL) {
                        stepElementsList->start = nodeS;
                        nodeS->prev = NULL;
                    } else {
                        stepElementsList->end->next = nodeS;
                        nodeS->prev = stepElementsList->end;
                    }
                    stepElementsList->end = nodeS;
                    stepElementsList->end->next = NULL;
                    stepElementsList->size++;

                    if(enableLog){
                        LV_LOG_USER("--- STEP PARAMS ---");
                        LV_LOG_USER("stepNameString:%s",nodeS->step.stepDetails->stepNameString);
                        LV_LOG_USER("timeSecs:%d",nodeS->step.stepDetails->timeSecs);
                        LV_LOG_USER("timeMins:%d",nodeS->step.stepDetails->timeMins);
                        LV_LOG_USER("type:%d",nodeS->step.stepDetails->type);
                        LV_LOG_USER("source:%d",nodeS->step.stepDetails->source);
                        LV_LOG_USER("discardAfterProc:%d",nodeS->step.stepDetails->discardAfterProc);
                    }
                }
            }
        }
        file.close();
        return gui;
    }
}


void writeFullJSONFile(fs::FS &fs, const char *path,const gui_components gui) {
    if(initErrors == 0){
        LV_LOG_USER("Writing file: %s", path);
        SD.remove(path);
        uint8_t processCounter = 0;
        uint8_t stepCounter = 0;
        char processName[20];
        char stepName[20];

        File file = fs.open(path, FILE_WRITE);
        if (!file) {
            LV_LOG_USER("Failed to open file for writing");
            rebootBoard();
            //return;
        }

        StaticJsonDocument<65536> doc;
        JsonObject machineSettings = doc.createNestedObject("machineSettings");

        machineSettings["tempUnit"]                   = gui.page.settings.settingsParams.tempUnit;
        machineSettings["waterInlet"]                 = gui.page.settings.settingsParams.waterInlet;
        machineSettings["calibratedTemp"]             = gui.page.settings.settingsParams.calibratedTemp;
        machineSettings["filmRotationSpeedSetpoint"]  = gui.page.settings.settingsParams.filmRotationSpeedSetpoint;
        machineSettings["rotationIntervalSetpoint"]   = gui.page.settings.settingsParams.rotationIntervalSetpoint;
        machineSettings["randomSetpoint"]             = gui.page.settings.settingsParams.randomSetpoint;
        machineSettings["isPersistentAlarm"]          = gui.page.settings.settingsParams.isPersistentAlarm;
        machineSettings["isProcessAutostart"]         = gui.page.settings.settingsParams.isProcessAutostart;
        machineSettings["drainFillOverlapSetpoint"]   = gui.page.settings.settingsParams.drainFillOverlapSetpoint;

        LV_LOG_USER("--- MACHINE PARAMS ---");
        LV_LOG_USER("tempUnit:%d",gui.page.settings.settingsParams.tempUnit);
        LV_LOG_USER("waterInlet:%d",gui.page.settings.settingsParams.waterInlet);
        LV_LOG_USER("calibratedTemp:%d",gui.page.settings.settingsParams.calibratedTemp);
        LV_LOG_USER("filmRotationSpeedSetpoint:%d",gui.page.settings.settingsParams.filmRotationSpeedSetpoint);
        LV_LOG_USER("rotationIntervalSetpoint:%d",gui.page.settings.settingsParams.rotationIntervalSetpoint);
        LV_LOG_USER("randomSetpoint:%d",gui.page.settings.settingsParams.randomSetpoint);
        LV_LOG_USER("isPersistentAlarm:%d",gui.page.settings.settingsParams.isPersistentAlarm);
        LV_LOG_USER("isProcessAutostart:%d",gui.page.settings.settingsParams.isProcessAutostart);
        LV_LOG_USER("drainFillOverlapSetpoint:%d",gui.page.settings.settingsParams.drainFillOverlapSetpoint);


        JsonObject Filter = doc.createNestedObject("Filter");
        Filter["filterName"] = gui.element.filterPopup.filterName;
        Filter["isColorFilter"] = gui.element.filterPopup.isColorFilter;
        Filter["isBnWFilter"] = gui.element.filterPopup.isBnWFilter;
        Filter["preferredOnly"] = gui.element.filterPopup.preferredOnly;

        LV_LOG_USER("--- FILTER PARAMS ---");
        LV_LOG_USER("filterName:%s",gui.element.filterPopup.filterName);
        LV_LOG_USER("isColorFilter:%d",gui.element.filterPopup.isColorFilter);
        LV_LOG_USER("isBnWFilter:%d",gui.element.filterPopup.isBnWFilter);
        LV_LOG_USER("preferredOnly:%d",gui.element.filterPopup.preferredOnly);

        JsonObject Processes = doc.createNestedObject("Processes");
        
        const processList *processElementsList;
        memset( &processElementsList, 0, sizeof( processElementsList ) );	
        processElementsList = &(gui.page.processes.processElementsList);

        processNode *currentProcessNode;
        memset( &currentProcessNode, 0, sizeof( currentProcessNode ) );	
        currentProcessNode = processElementsList->start;

        while(currentProcessNode != NULL){
            snprintf(processName, sizeof(processName), "Process%d", processCounter);
            JsonObject currentProcess = Processes.createNestedObject(processName);
            currentProcess["processNameString"] = currentProcessNode->process.processDetails->processNameString;
            currentProcess["temp"] = currentProcessNode->process.processDetails->temp;
            currentProcess["tempTolerance"] = currentProcessNode->process.processDetails->tempTolerance;
            currentProcess["isTempControlled"] = currentProcessNode->process.processDetails->isTempControlled;
            currentProcess["isPreferred"] = currentProcessNode->process.processDetails->isPreferred;
            currentProcess["filmType"] = currentProcessNode->process.processDetails->filmType;
            currentProcess["timeMins"] = currentProcessNode->process.processDetails->timeMins;
            currentProcess["timeSecs"] = currentProcessNode->process.processDetails->timeSecs;

            LV_LOG_USER("--- PROCESS PARAMS ---");
            LV_LOG_USER("processNameString:%s",currentProcessNode->process.processDetails->processNameString);
            LV_LOG_USER("temp:%d",currentProcessNode->process.processDetails->temp);
            LV_LOG_USER("tempTolerance:%d",currentProcessNode->process.processDetails->tempTolerance);
            LV_LOG_USER("isTempControlled:%d",currentProcessNode->process.processDetails->isTempControlled);
            LV_LOG_USER("isPreferred:%d",currentProcessNode->process.processDetails->isPreferred);
            LV_LOG_USER("filmType:%d",currentProcessNode->process.processDetails->filmType);
            LV_LOG_USER("timeMins:%d",currentProcessNode->process.processDetails->timeMins);
            LV_LOG_USER("timeSecs:%d",currentProcessNode->process.processDetails->timeSecs);

            stepList *stepElementsList;
            memset( &stepElementsList, 0, sizeof( stepElementsList ) );	
            stepElementsList = &(currentProcessNode->process.processDetails->stepElementsList);   

            stepNode *currentStepNode;
            memset( &currentStepNode, 0, sizeof( currentStepNode ) );	
            currentStepNode = stepElementsList->start;

            processCounter++;
            stepCounter = 0;

            JsonObject currentProcessSteps = currentProcess.createNestedObject("Steps");
            while(currentStepNode != NULL){                
                snprintf(stepName, sizeof(stepName), "Step%d", stepCounter);
                JsonObject currentStep = currentProcessSteps.createNestedObject(stepName);
                currentStep["stepNameString"] = currentStepNode->step.stepDetails->stepNameString;
                currentStep["timeMins"] = currentStepNode->step.stepDetails->timeMins;
                currentStep["timeSecs"] = currentStepNode->step.stepDetails->timeSecs;
                currentStep["type"] = currentStepNode->step.stepDetails->type;
                currentStep["source"] = currentStepNode->step.stepDetails->source;
                currentStep["discardAfterProc"] = currentStepNode->step.stepDetails->discardAfterProc;

                LV_LOG_USER("--- STEP PARAMS ---");
                LV_LOG_USER("stepNameString:%s",currentStepNode->step.stepDetails->stepNameString);
                LV_LOG_USER("timeMins:%d",currentStepNode->step.stepDetails->timeMins);
                LV_LOG_USER("timeSecs:%d",currentStepNode->step.stepDetails->timeSecs);
                LV_LOG_USER("type:%d",currentStepNode->step.stepDetails->type);
                LV_LOG_USER("source:%d",currentStepNode->step.stepDetails->source);
                LV_LOG_USER("discardAfterProc:%d",currentStepNode->step.stepDetails->discardAfterProc);

                currentStepNode = currentStepNode->next;
                stepCounter++;
            }

          currentProcessNode = currentProcessNode->next;
        }
        
        processCounter = 0;
        stepCounter = 0;

        if (serializeJson(doc, file)) {
            file.close();
            LV_LOG_USER("File written successfully");
        } else {
            LV_LOG_USER("Write failed");
        }
        file.close();
    }
    else
        return;
}

// Funzione per scrivere su un file
void writeFile(fs::FS &fs, const char *path, const char *message) {
    LV_LOG_USER("Writing file: %s", path);
    File file = fs.open(path, FILE_WRITE);
    if (!file) {
        LV_LOG_USER("Failed to open file for writing");
        return;
    }
    if (file.print(message)) {
        LV_LOG_USER("File written successfully");
    } else {
        LV_LOG_USER("Write failed");
    }
    file.close();
}

// Funzione per leggere un file
void readFile(fs::FS &fs, const char *path) {
    LV_LOG_USER("Reading file: %s", path);

    char data[100]; // Supponiamo che tu legga al massimo 100 byte
    File file = fs.open(path);
    if (!file) {
        LV_LOG_USER("Failed to open file for reading");
        return;
    }
    LV_LOG_USER("File Content:");
    while (file.available()) {
        LV_LOG_USER("Data read from file: %.*s", file.readBytes(data, sizeof(data)), data);
    }
    file.close();
}

void calcolateTotalTime(processNode *processNode){
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

    sprintf(formatted_string, "%dm%ds", processNode->process.processDetails->timeMins, processNode->process.processDetails->timeSecs);
    lv_label_set_text(processNode->process.processDetails->processTotalTimeValue, formatted_string); 
    LV_LOG_USER("Process %p has a total tilme of %dmin:%dsec", processNode, mins, secs);
}



uint8_t calcolatePercentage(uint32_t minutes, uint8_t seconds, uint32_t total_minutes, uint8_t total_seconds) {
    // Calcola il tempo totale in secondi
    uint32_t total_time_seconds = total_minutes * 60 + total_seconds;

    // Calcola il tempo trascorso in secondi
    uint32_t elapsed_time_seconds = minutes * 60 + seconds;

    // Calcola la percentuale di tempo trascorso rispetto al tempo totale
    int percentage = (int)((elapsed_time_seconds * 100) / total_time_seconds);

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
      sprintf(formatted_string, "%dm%ds", process->process.processDetails->timeMins, process->process.processDetails->timeSecs);
      lv_label_set_text(existingProcess->process.processTime, formatted_string); 
      
      //Update temp
      lv_label_set_text_fmt(existingProcess->process.processTemp, "%d °C", process->process.processDetails->temp );
 
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
         sprintf(formatted_string, "From:%s", processSourceList[step->step.stepDetails->source]);        
         lv_label_set_text(existingStep->step.sourceLabel, formatted_string); 

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
          sprintf(formatted_string, "%dm%ds", step->step.stepDetails->timeMins, step->step.stepDetails->timeSecs);
          lv_label_set_text(existingStep->step.stepTime, formatted_string); 
      }
}


uint32_t loadSDCardProcesses() {
    if (gui.page.processes.processElementsList.size > 0) {
        processNode *process = gui.page.processes.processElementsList.start;

        while (process != NULL) {
            processElementCreate(process);
            process = process->next;
        }
        return gui.page.processes.processElementsList.size;
    } else {
        return 0;
    }
}

char* generateRandomCharArray(int length) {
  char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
  char *randomArray = new char[length + 1]; // Allocazione dinamica dell'array più uno per il terminatore di stringa

  for (int i = 0; i < length; ++i) {
    randomArray[i] = charset[random(0, sizeof(charset) - 1)];
  }

  randomArray[length] = '\0'; // Aggiungi il terminatore di stringa

  return randomArray;
}

/*

void listDir(fs::FS &fs, const char *dirname, uint8_t levels)
{
    LV_LOG_USER("Listing directory: %s", dirname);

    File root = fs.open(dirname);
    if (!root)
    {
        LV_LOG_USER("Failed to open directory");
        return;
    }
    if (!root.isDirectory())
    {
        LV_LOG_USER("Not a directory");
        return;
    }

    File file = root.openNextFile();
    while (file)
    {
        if (file.isDirectory())
        {
            LV_LOG_USER("  DIR : %s",file.name());
            if (levels)
            {
                listDir(fs, file.name(), levels - 1);
            }
        }
        else
        {
            LV_LOG_USER("  FILE: %s",file.name());
            LV_LOG_USER("  SIZE: %s",file.size());
        }
        file = root.openNextFile();
    }
}




void readFile(fs::FS &fs, const char *path)
{
   LV_LOG_USER("Reading file: %s", path);

    File file = fs.open(path);
    if (!file)
    {
        LV_LOG_USER("Failed to open file for reading");
        return;
    }

    LV_LOG_USER("Read from file: ");
    while (file.available())
    {
        LV_LOG_USER("%s",file.read());
    }
    file.close();
}

void writeFile(fs::FS &fs, const char *path, const char *message)
{
    LV_LOG_USER("Writing file: %s", path);

    File file = fs.open(path, FILE_WRITE);
    if (!file)
    {
        LV_LOG_USER("Failed to open file for writing");
        return;
    }
    if (file.print(message))
    {
        LV_LOG_USER("File written");
    }
    else
    {
        LV_LOG_USER("Write failed");
    }
    file.close();
}

void appendFile(fs::FS &fs, const char *path, const char *message)
{
    LV_LOG_USER("Appending to file: %s", path);

    File file = fs.open(path, FILE_APPEND);
    if (!file)
    {
        LV_LOG_USER("Failed to open file for appending");
        return;
    }
    if (file.print(message))
    {
        LV_LOG_USER("Message appended");
    }
    else
    {
        LV_LOG_USER("Append failed");
    }
    file.close();
}



//Display image from file
int print_img(fs::FS &fs, String filename, int x, int y)
{
    File f = fs.open(filename, "r");
    if (!f)
    {
        LV_LOG_USER("Failed to open file for reading");
        f.close();
        return 0;
    }

    f.seek(54);
    int X = x;
    int Y = y;
    uint8_t RGB[3 * X];
    for (int row = 0; row < Y; row++)
    {
        f.seek(54 + 3 * X * row);
        f.read(RGB, 3 * X);
        lcd.pushImage(0, row, X, 1, (lgfx::rgb888_t *)RGB);
    }

    f.close();
    return 0;
}

*/