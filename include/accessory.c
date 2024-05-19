/**
 * @file accessory.c
 *
 */


/*********************
 *      INCLUDES
 *********************/
#include <string.h>
#include "lvgl.h"
#include "definitions.h"

/**********************
 *      MACROS
 **********************/

#define LESS_THAN_10 1
#define GREATER_THAN_9 2
#define GREATER_THAN_99 3

struct gui_components	gui;


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

void kb_event_cb(lv_event_t* e)
 {
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t * obj = (lv_obj_t *)lv_event_get_target(e);
  lv_obj_t * objCont = (lv_obj_t *)lv_obj_get_parent(obj);
  lv_obj_t * mboxCont = (lv_obj_t *)lv_obj_get_parent(objCont);
  lv_obj_t * mboxParent = (lv_obj_t *)lv_obj_get_parent(mboxCont);
  lv_obj_t * godFatherCont = (lv_obj_t *)lv_obj_get_parent(mboxParent);
  lv_obj_t * data = (lv_obj_t *)lv_event_get_user_data(e);
   

    if(obj == gui.element.filterPopup.mBoxNameTextArea){
      if(code == LV_EVENT_FOCUSED || code == LV_EVENT_CLICKED) {
          LV_LOG_USER("LV_EVENT_FOCUSED on gui.element.filterPopup.mBoxNameTextArea");
          lv_obj_set_user_data(obj,data);
          lv_obj_add_flag(mboxParent, LV_OBJ_FLAG_HIDDEN);
          lv_obj_remove_flag(keyBoardParent, LV_OBJ_FLAG_HIDDEN);
        }

        if(code == LV_EVENT_DEFOCUSED) {
          LV_LOG_USER("LV_EVENT_DEFOCUSED on gui.element.filterPopup.mBoxNameTextArea");
          lv_obj_remove_flag(mboxParent, LV_OBJ_FLAG_HIDDEN);
          lv_obj_add_flag(keyBoardParent, LV_OBJ_FLAG_HIDDEN);
        }
    }
    if (code == LV_EVENT_CANCEL) {
            if(obj == gui.element.filterPopup.mBoxFilterPopupParent){
                LV_LOG_USER("Press cancel from gui.element.filterPopup.mBoxFilterPopupParent");
                lv_textarea_set_text(keyboard_textArea, "");
                lv_obj_add_flag(keyBoardParent, LV_OBJ_FLAG_HIDDEN);
                lv_obj_remove_flag(gui.element.filterPopup.mBoxFilterPopupParent, LV_OBJ_FLAG_HIDDEN);
              }

      /*        if(obj == gui.element.step.stepDetails->stepDetailParent){
                LV_LOG_USER("Press cancel from gui.element.step.stepDetails->stepDetailParent");
                lv_textarea_set_text(keyboard_textArea, "");
                lv_obj_add_flag(keyBoardParent, LV_OBJ_FLAG_HIDDEN);
                lv_obj_remove_flag(gui.element.step.stepDetails->stepDetailParent, LV_OBJ_FLAG_HIDDEN);
         }
         */
     }
    if (code == LV_EVENT_READY) {
            if(obj == gui.element.filterPopup.mBoxNameTextArea){
              LV_LOG_USER("Press ok from gui.element.filterPopup.mBoxFilterPopupParent");
              lv_obj_clear_state(gui.element.filterPopup.mBoxNameTextArea, LV_STATE_FOCUSED);
              lv_textarea_set_text(gui.element.filterPopup.mBoxNameTextArea, lv_textarea_get_text(keyboard_textArea));
              lv_textarea_set_text(keyboard_textArea, "");
              lv_obj_add_flag(keyBoardParent, LV_OBJ_FLAG_HIDDEN);
              lv_obj_remove_flag(gui.element.filterPopup.mBoxFilterPopupParent, LV_OBJ_FLAG_HIDDEN);
              LV_LOG_USER(lv_textarea_get_text(keyboard_textArea));
            }
            /*
            if(obj == gui.element.step.stepDetails->stepDetailParent){
              LV_LOG_USER("Press ok from gui.element.step.stepDetails->stepDetailParent");
              lv_textarea_set_text(stepDetailNamelTextArea, lv_textarea_get_text(keyboard_textArea));
              lv_textarea_set_text(keyboard_textArea, "");
              lv_obj_add_flag(keyBoardParent, LV_OBJ_FLAG_HIDDEN);
              lv_obj_remove_flag(gui.element.step.stepDetails->stepDetailParent, LV_OBJ_FLAG_HIDDEN);
              LV_LOG_USER(lv_textarea_get_text(keyboard_textArea));
            }
            */
      }
      
 }



void create_keyboard(lv_obj_t * keyB)
{   
    keyBoardParent = lv_obj_class_create_obj(&lv_msgbox_backdrop_class, lv_layer_top());
    lv_obj_class_init_obj(keyBoardParent);
    lv_obj_remove_flag(keyBoardParent, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_obj_set_size(keyBoardParent, LV_PCT(100), LV_PCT(100));

    keyB = lv_keyboard_create(keyBoardParent);
    lv_obj_add_event_cb(keyB, kb_event_cb, LV_EVENT_ALL, NULL);
    lv_obj_add_flag(keyB, LV_OBJ_FLAG_EVENT_BUBBLE);


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

    lv_keyboard_set_map(keyB, LV_KEYBOARD_MODE_USER_1, kb_map, kb_ctrl);
    lv_keyboard_set_mode(keyB, LV_KEYBOARD_MODE_USER_1);
    
    keyboard_textArea = lv_textarea_create(keyBoardParent);
    lv_obj_align(keyboard_textArea, LV_ALIGN_TOP_MID, 0, 10);
    //lv_obj_add_event_cb(keyboard_textArea, ta_event_cb, LV_EVENT_ALL, keyboard);
    lv_textarea_set_placeholder_text(keyboard_textArea, keyboard_placeholder_text);
    lv_obj_set_size(keyboard_textArea, lv_pct(90), 80);
    lv_obj_add_state(keyboard_textArea, LV_STATE_FOCUSED);
    lv_keyboard_set_textarea(keyB, keyboard_textArea);

    lv_obj_set_style_text_font(keyB, &lv_font_montserrat_26, 0);
    lv_obj_set_style_text_font(keyboard_textArea, &lv_font_montserrat_30, 0);
    
    lv_obj_add_flag(keyBoardParent, LV_OBJ_FLAG_HIDDEN);
}


/*

void create_keyboard(lv_obj_t * keyB, lv_obj_t * whoCallMe)
{   
    keyBoardParent = lv_obj_class_create_obj(&lv_msgbox_backdrop_class, lv_layer_top());
    lv_obj_class_init_obj(keyBoardParent);
    lv_obj_remove_flag(keyBoardParent, LV_OBJ_FLAG_IGNORE_LAYOUT);
    lv_obj_set_size(keyBoardParent, LV_PCT(100), LV_PCT(100));

    keyB = lv_keyboard_create(keyBoardParent);
    
    

    lv_obj_add_event_cb(keyB, kb_event_cb, LV_EVENT_ALL, whoCallMe);

    static const char * kb_map[] = {"Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", " ", "\n",
                                    "A", "S", "D", "F", "G", "H", "J", "K", "L",  " ", "\n",
                                    " "," ", "Z", "X", "C", "V", "B", "N", "M"," "," ","\n",
                                    LV_SYMBOL_CLOSE, LV_SYMBOL_BACKSPACE,  " ", " ", LV_SYMBOL_OK, NULL
                                   };


    static const lv_buttonmatrix_ctrl_t kb_ctrl[] = {4, 4, 4, 4, 4, 4, 4, 4, 4, 4, LV_BUTTONMATRIX_CTRL_HIDDEN,
                                                     4, 4, 4, 4, 4, 4, 4, 4, 4, LV_BUTTONMATRIX_CTRL_HIDDEN,
                                                     LV_BUTTONMATRIX_CTRL_HIDDEN, LV_BUTTONMATRIX_CTRL_HIDDEN, 4, 4, 4, 4, 4, 4, 4, LV_BUTTONMATRIX_CTRL_HIDDEN, LV_BUTTONMATRIX_CTRL_HIDDEN,
                                                     2, 2 | 2, 6, LV_BUTTONMATRIX_CTRL_HIDDEN | 2, 2
                                                    };



    lv_keyboard_set_map(keyB, LV_KEYBOARD_MODE_USER_1, kb_map, kb_ctrl);
    lv_keyboard_set_mode(keyB, LV_KEYBOARD_MODE_USER_1);

    keyboard_textArea = lv_textarea_create(keyBoardParent);
    lv_obj_align(keyboard_textArea, LV_ALIGN_TOP_MID, 0, 10);
    //lv_obj_add_event_cb(keyboard_textArea, ta_event_cb, LV_EVENT_ALL, keyboard);
    lv_textarea_set_placeholder_text(keyboard_textArea, keyboard_placeholder_text);
    lv_obj_set_size(keyboard_textArea, lv_pct(90), 80);
    lv_obj_add_state(keyboard_textArea, LV_STATE_FOCUSED);
    lv_keyboard_set_textarea(keyB, keyboard_textArea);

    lv_obj_set_style_text_font(keyB, &lv_font_montserrat_26, 0);
    lv_obj_set_style_text_font(keyboard_textArea, &lv_font_montserrat_30, 0);
    
    //lv_obj_add_flag(keyboard_textArea, LV_OBJ_FLAG_HIDDEN);
   // lv_obj_add_flag(keyboard, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(keyBoardParent, LV_OBJ_FLAG_HIDDEN);
}

*/

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






void sd_init()
{
  /*
    SD_SPI.begin(SD_CS, SD_MISO, SD_MOSI, SD_CS);
    if (!SD.begin(SD_CS, SD_SPI))
    {
        LV_LOG_USER("SD Card Failed");
        while (1)
            delay(1000);
    }
    else
    {
        LV_LOG_USER("Card Mount Successed");
    }
    LV_LOG_USER("SD init over.");
    */
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

char *createRollerValues( uint32_t maxVal, const char* extra_str ) {
    uint32_t buf_len = calc_buf_len( maxVal, strlen(extra_str));
    uint32_t buf_ptr = 0;
    char *buf = (char *)malloc( buf_len );

    for( uint32_t i = 1; i <= maxVal; i++ ) {
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


void init_globals( void ) {
  // Initialise the main GUI structure to zero
	memset( &gui, 0, sizeof( gui ) );		


  //gui.element.step.stepDetails = (sStepDetail *)malloc(sizeof(sStepDetail));

  
  //gui.page.processes.processElementsList.start->process.processDetails = (sProcessDetail *)malloc(sizeof(sProcessDetail));
  
  //gui.page.processes.processElementsList.start->process.processDetails

  // We only need to initialise the non-zero values
  gui.element.filterPopup.titleLinePoints[1].x = 200;
  gui.element.rollerPopup.titleLinePoints[1].x = 200;
  gui.element.messagePopup.titleLinePoints[1].x = 200;

  gui.page.processes.titleLinePoints[1].x = 310;
  gui.page.settings.titleLinePoints[1].x = 310;
  gui.page.tools.titleLinePoints[1].x = 310;
  
  gui.element.rollerPopup.tempCelsiusOptions = createRollerValues(40,"");
  gui.element.rollerPopup.minutesOptions = createRollerValues(240,"");
  gui.element.rollerPopup.secondsOptions = createRollerValues(60,""); 
  gui.element.rollerPopup.tempCelsiusToleranceOptions = createRollerValues(5,"0.");
  
  
  /*
  tempProcessNode->process.processDetails = (sProcessDetail *) malloc(sizeof(sProcessDetail));
    if (tempProcessNode->process.processDetails == NULL) {
        // Handle memory allocation failure
        free(tempProcessNode);  // Clean up previously allocated memory
        return;
    }
  memset( &tempProcessNode, 0, sizeof( tempProcessNode ) );

  tempStepNode->step.stepDetails = (sStepDetail *) malloc(sizeof(sStepDetail));
    if (tempStepNode->step.stepDetails == NULL) {
        // Handle memory allocation failure
        free(tempStepNode);  // Clean up previously allocated memory
        return;
    }
  memset( &tempStepNode, 0, sizeof( tempStepNode ) );
  */
}


