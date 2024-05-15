/**
 * @file element_filter.c
 *
 */


//ESSENTIAL INCLUDES
#include <lvgl.h>
#include "../../include/definitions.h"


extern struct gui_components gui;

//ACCESSORY INCLUDES


uint32_t active_index_2 = 0;
lv_obj_t * mBoxContainer;
lv_obj_t * mBoxTitle;
lv_obj_t * mBoxNameContainer;
lv_obj_t * mBoxNameLabel;
lv_obj_t * selectColorContainerRadioButton;
lv_obj_t * mBoxColorLabel;
lv_obj_t * mBoxBnWLabel;
lv_obj_t * mBoxPreferredContainer;
lv_obj_t * mBoxPreferredLabel;
lv_obj_t * mBoxResetFilterLabel;
lv_obj_t * mBoxApplyFilterLabel;

void event_filterMBox(lv_event_t * e){
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t * obj = (lv_obj_t *)lv_event_get_target(e);
  lv_obj_t * objCont = (lv_obj_t *)lv_obj_get_parent(obj);
  lv_obj_t * mboxCont = (lv_obj_t *)lv_obj_get_parent(objCont);
  lv_obj_t * mboxParent = (lv_obj_t *)lv_obj_get_parent(mboxCont);

  lv_obj_t * data = (lv_obj_t *)lv_event_get_user_data(e);

  if(obj == mBoxApplyFilterButton || obj == mBoxResetFilterButton){
      if(code == LV_EVENT_CLICKED) {    
        if(obj == mBoxApplyFilterButton){
          LV_LOG_USER("Apply BUTTON");        
          lv_style_reset(&style_mBoxTitleLine);
          lv_obj_add_flag(mBoxFilterPopupParent, LV_OBJ_FLAG_HIDDEN);
          lv_obj_remove_state(mBoxSelectColorRadioButton, LV_STATE_CHECKED);
          lv_obj_remove_state(mBoxSelectBnWRadioButton, LV_STATE_CHECKED);
          lv_obj_remove_state(mBoxOnlyPreferredSwitch, LV_STATE_CHECKED);
        }
        if(obj == mBoxResetFilterButton){
          LV_LOG_USER("Reset BUTTON");
          lv_textarea_set_text(mBoxNameTextArea, "");
          lv_obj_remove_state(mBoxOnlyPreferredSwitch, LV_STATE_CHECKED);
          lv_obj_remove_state(mBoxSelectColorRadioButton, LV_STATE_CHECKED);
          lv_obj_remove_state(mBoxSelectBnWRadioButton, LV_STATE_CHECKED);
        }
      }
  }
/*
  if(obj == mBoxNameTextArea){
     if(code == LV_EVENT_FOCUSED || code == LV_EVENT_CLICKED) {
        LV_LOG_USER("LV_EVENT_FOCUSED on mBoxNameTextArea");
        //lv_obj_set_user_data(keyboard,mBoxFilterPopupParent);
        lv_obj_add_flag(mboxParent, LV_OBJ_FLAG_HIDDEN);
        lv_obj_remove_flag(keyBoardParent, LV_OBJ_FLAG_HIDDEN);
      }

      if(code == LV_EVENT_DEFOCUSED) {
        LV_LOG_USER("LV_EVENT_DEFOCUSED on mBoxNameTextArea");
        lv_obj_remove_flag(mboxParent, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(keyBoardParent, LV_OBJ_FLAG_HIDDEN);
      }
  }
  */

  if(obj == mBoxSelectColorRadioButton || obj == mBoxSelectBnWRadioButton){
    if(code == LV_EVENT_VALUE_CHANGED) {
        if(obj == mBoxSelectColorRadioButton)
          LV_LOG_USER("State color: %s", lv_obj_has_state(obj, LV_STATE_CHECKED) ? "On" : "Off");
        if(obj == mBoxSelectBnWRadioButton)
          LV_LOG_USER("State bnw: %s", lv_obj_has_state(obj, LV_STATE_CHECKED) ? "On" : "Off");
      }
    }
  if(obj == mBoxOnlyPreferredSwitch){
    if(code == LV_EVENT_VALUE_CHANGED) {
        LV_LOG_USER("State preferred: %s", lv_obj_has_state(obj, LV_STATE_CHECKED) ? "On" : "Off");
      }
    }
}






void filterPopupCreate (void){
  /*********************
   *    PAGE ELEMENTS
   *********************/
  if (mBoxFilterPopupParent == NULL)
  {
      mBoxFilterPopupParent = lv_obj_class_create_obj(&lv_msgbox_backdrop_class, lv_layer_top());
      lv_obj_class_init_obj(mBoxFilterPopupParent);
      lv_obj_remove_flag(mBoxFilterPopupParent, LV_OBJ_FLAG_IGNORE_LAYOUT);
      lv_obj_set_size(mBoxFilterPopupParent, LV_PCT(100), LV_PCT(100));

      mBoxContainer = lv_obj_create(mBoxFilterPopupParent);
      lv_obj_align(mBoxContainer, LV_ALIGN_CENTER, 0, 0);
      lv_obj_set_size(mBoxContainer, 320, 240); 
      lv_obj_remove_flag(mBoxContainer, LV_OBJ_FLAG_SCROLLABLE); 

      mBoxTitle = lv_label_create(mBoxContainer);         
      lv_label_set_text(mBoxTitle, filterPopupTitle_text); 
      lv_obj_set_style_text_font(mBoxTitle, &lv_font_montserrat_22, 0);              
      lv_obj_align(mBoxTitle, LV_ALIGN_TOP_MID, 0, - 10);

      /*Create style*/
      lv_style_init(&style_mBoxTitleLine);
      lv_style_set_line_width(&style_mBoxTitleLine, 2);
      lv_style_set_line_color(&style_mBoxTitleLine, lv_palette_main(LV_PALETTE_GREEN));
      lv_style_set_line_rounded(&style_mBoxTitleLine, true);

      /*Create a line and apply the new style*/
      mBoxTitleLine = lv_line_create(mBoxContainer);
      lv_line_set_points(mBoxTitleLine, mBoxTitleLine_points, 2);
      lv_obj_add_style(mBoxTitleLine, &style_mBoxTitleLine, 0);
      lv_obj_align(mBoxTitleLine, LV_ALIGN_TOP_MID, 0, 23);

      //NAME TO FILTER
      mBoxNameContainer = lv_obj_create(mBoxContainer);
      lv_obj_remove_flag(mBoxNameContainer, LV_OBJ_FLAG_SCROLLABLE); 
      lv_obj_align(mBoxNameContainer, LV_ALIGN_TOP_LEFT, -7, 30);
      lv_obj_set_size(mBoxNameContainer, 300, 40); 
      lv_obj_set_style_border_opa(mBoxNameContainer, LV_OPA_TRANSP, 0);
      //lv_obj_set_style_border_color(mBoxNameContainer, lv_color_hex(GREEN_DARK), 0);

          mBoxNameLabel = lv_label_create(mBoxNameContainer);         
          lv_label_set_text(mBoxNameLabel, filterPopupName_text); 
          lv_obj_set_style_text_font(mBoxNameLabel, &lv_font_montserrat_22, 0);              
          lv_obj_align(mBoxNameLabel, LV_ALIGN_LEFT_MID, -10, 0);
      
          mBoxNameTextArea = lv_textarea_create(mBoxNameContainer);
          lv_textarea_set_one_line(mBoxNameTextArea, true);
          lv_textarea_set_placeholder_text(mBoxNameTextArea, filterPopupNamePlaceHolder_text);
          lv_obj_align(mBoxNameTextArea, LV_ALIGN_LEFT_MID, 65, 0);
          lv_obj_set_width(mBoxNameTextArea, 215);
          //lv_obj_add_event_cb(mBoxNameTextArea, event_filterMBox, LV_EVENT_ALL, mBoxNameTextArea);
          lv_obj_add_event_cb(mBoxNameTextArea, kb_event_cb, LV_EVENT_ALL, mBoxNameTextArea);
          lv_obj_add_state(mBoxNameTextArea, LV_STATE_FOCUSED); /*To be sure the cursor is visible*/
          lv_obj_set_style_bg_color(mBoxNameTextArea, lv_palette_darken(LV_PALETTE_GREY, 3), 0);
          lv_obj_set_style_border_color(mBoxNameTextArea, lv_color_hex(WHITE), 0);


      //CHOOSE COLOR/B&W/Both
      selectColorContainerRadioButton = lv_obj_create(mBoxContainer);
      lv_obj_remove_flag(selectColorContainerRadioButton, LV_OBJ_FLAG_SCROLLABLE); 
      lv_obj_align(selectColorContainerRadioButton, LV_ALIGN_LEFT_MID, -7, -5);
      lv_obj_set_size(selectColorContainerRadioButton, 300, 40); 
      //lv_obj_set_style_border_color(selectColorContainerRadioButton, lv_color_hex(GREEN_DARK), 0);
      lv_obj_set_style_border_opa(selectColorContainerRadioButton, LV_OPA_TRANSP, 0);

          //COLOR RADIO BUTTON WITH LABEL
          mBoxSelectColorRadioButton = lv_obj_create(selectColorContainerRadioButton);
          lv_obj_remove_flag(mBoxSelectColorRadioButton, LV_OBJ_FLAG_SCROLLABLE); 
          lv_obj_align(mBoxSelectColorRadioButton, LV_ALIGN_LEFT_MID, -20, 0);
          lv_obj_set_size(mBoxSelectColorRadioButton, 100, 40); 
          //lv_obj_set_style_border_color(mBoxSelectColorRadioButton, lv_color_hex(GREY), 0);
          lv_obj_set_style_border_opa(mBoxSelectColorRadioButton, LV_OPA_TRANSP, 0);

          mBoxColorLabel = lv_label_create(mBoxSelectColorRadioButton);         
          lv_label_set_text(mBoxColorLabel, filterPopupColor_text); 
          lv_obj_set_style_text_font(mBoxColorLabel, &lv_font_montserrat_22, 0);              
          lv_obj_align(mBoxColorLabel, LV_ALIGN_LEFT_MID, - 10, 0);

          mBoxSelectColorRadioButton = create_radiobutton(mBoxSelectColorRadioButton, "", 25, 0, 27, &lv_font_montserrat_18, lv_color_hex(GREEN_DARK), lv_palette_main(LV_PALETTE_GREEN));
          lv_obj_add_event_cb(mBoxSelectColorRadioButton, event_filterMBox, LV_EVENT_VALUE_CHANGED, mBoxSelectColorRadioButton);

          //B&W RADIO BUTTON WITH LABEL
          mBoxSelectBnWRadioButton = lv_obj_create(selectColorContainerRadioButton);
          lv_obj_remove_flag(mBoxSelectBnWRadioButton, LV_OBJ_FLAG_SCROLLABLE); 
          lv_obj_align(mBoxSelectBnWRadioButton, LV_ALIGN_LEFT_MID, 80, 0);
          lv_obj_set_size(mBoxSelectBnWRadioButton, 100, 40); 
          //lv_obj_set_style_border_color(mBoxSelectBnWRadioButton, lv_color_hex(GREY), 0);
          lv_obj_set_style_border_opa(mBoxSelectBnWRadioButton, LV_OPA_TRANSP, 0);

          mBoxBnWLabel = lv_label_create(mBoxSelectBnWRadioButton);         
          lv_label_set_text(mBoxBnWLabel, filterPopupBnW_text); 
          lv_obj_set_style_text_font(mBoxBnWLabel, &lv_font_montserrat_22, 0);              
          lv_obj_align(mBoxBnWLabel, LV_ALIGN_LEFT_MID, - 10, 0);

          mBoxSelectBnWRadioButton = create_radiobutton(mBoxSelectBnWRadioButton, "", 25, 0, 27, &lv_font_montserrat_18, lv_color_hex(GREEN_DARK), lv_palette_main(LV_PALETTE_GREEN));
          lv_obj_add_event_cb(mBoxSelectBnWRadioButton, event_filterMBox, LV_EVENT_VALUE_CHANGED, mBoxSelectBnWRadioButton);
        

      //ONLY PREFERRED FILTER
      mBoxPreferredContainer = lv_obj_create(mBoxContainer);
      lv_obj_remove_flag(mBoxPreferredContainer, LV_OBJ_FLAG_SCROLLABLE); 
      lv_obj_align(mBoxPreferredContainer, LV_ALIGN_BOTTOM_LEFT, -7, -45);
      lv_obj_set_size(mBoxPreferredContainer, 300, 40); 
      //lv_obj_set_style_border_color(mBoxPreferredContainer, lv_color_hex(GREEN_DARK), 0);
      lv_obj_set_style_border_opa(mBoxPreferredContainer, LV_OPA_TRANSP, 0);

          mBoxPreferredLabel = lv_label_create(mBoxPreferredContainer);         
          lv_label_set_text(mBoxPreferredLabel, filterPopupPreferred_text); 
          lv_obj_set_style_text_font(mBoxPreferredLabel, &lv_font_montserrat_22, 0);              
          lv_obj_align(mBoxPreferredLabel, LV_ALIGN_LEFT_MID, -10, 0);

          mBoxOnlyPreferredSwitch = lv_switch_create(mBoxPreferredContainer);
          lv_obj_add_event_cb(mBoxOnlyPreferredSwitch, event_filterMBox, LV_EVENT_VALUE_CHANGED, mBoxOnlyPreferredSwitch);
          lv_obj_align(mBoxOnlyPreferredSwitch, LV_ALIGN_LEFT_MID, 170, 0);
          lv_obj_set_style_bg_color(mBoxOnlyPreferredSwitch, lv_palette_darken(LV_PALETTE_GREY, 3), LV_STATE_DEFAULT);
          lv_obj_set_style_bg_color(mBoxOnlyPreferredSwitch,  lv_palette_main(LV_PALETTE_GREEN), LV_PART_KNOB | LV_STATE_DEFAULT);
          lv_obj_set_style_bg_color(mBoxOnlyPreferredSwitch, lv_color_hex(GREEN_DARK) , LV_PART_INDICATOR | LV_STATE_CHECKED);



      //RESET FILTER
      mBoxResetFilterButton = lv_button_create(mBoxContainer);
      lv_obj_set_size(mBoxResetFilterButton, BUTTON_MBOX_WIDTH, BUTTON_MBOX_HEIGHT);
      lv_obj_align(mBoxResetFilterButton, LV_ALIGN_BOTTOM_LEFT, 10 , 10);
      lv_obj_add_event_cb(mBoxResetFilterButton, event_filterMBox, LV_EVENT_CLICKED, mBoxResetFilterButton);
      lv_obj_set_style_bg_color(mBoxResetFilterButton, lv_color_hex(GREEN_DARK), LV_PART_MAIN);

          mBoxResetFilterLabel = lv_label_create(mBoxResetFilterButton);
          lv_label_set_text(mBoxResetFilterLabel, filterPopupResetButton_text);
          lv_obj_set_style_text_font(mBoxResetFilterLabel, &lv_font_montserrat_22, 0);
          lv_obj_align(mBoxResetFilterLabel, LV_ALIGN_CENTER, 0, 0);



      //APPLY FILTER
      mBoxApplyFilterButton = lv_button_create(mBoxContainer);
      lv_obj_set_size(mBoxApplyFilterButton, BUTTON_MBOX_WIDTH, BUTTON_MBOX_HEIGHT);
      lv_obj_align(mBoxApplyFilterButton, LV_ALIGN_BOTTOM_RIGHT, - 10 , 10);
      lv_obj_add_event_cb(mBoxApplyFilterButton, event_filterMBox, LV_EVENT_CLICKED, mBoxApplyFilterButton);
      lv_obj_set_style_bg_color(mBoxApplyFilterButton, lv_color_hex(GREEN_DARK), LV_PART_MAIN);

            mBoxApplyFilterLabel = lv_label_create(mBoxApplyFilterButton);
            lv_label_set_text(mBoxApplyFilterLabel, filterPopupApplyButton_text);
            lv_obj_set_style_text_font(mBoxApplyFilterLabel, &lv_font_montserrat_22, 0);
            lv_obj_align(mBoxApplyFilterLabel, LV_ALIGN_CENTER, 0, 0);
  }
  //create_keyboard(keyboardFilter, mBoxFilterPopupParent);
}

