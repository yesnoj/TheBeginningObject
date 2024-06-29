#include "misc/lv_palette.h"
#include "misc/lv_area.h"

/**
 * @file element_filter.c
 *
 */


//ESSENTIAL INCLUDES
#include <lvgl.h>
#include "../../include/definitions.h"


extern struct gui_components gui;

//ACCESSORY INCLUDES




void event_cleanPopup(lv_event_t * e){
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t * obj = (lv_obj_t *)lv_event_get_target(e);
  lv_obj_t * objCont = (lv_obj_t *)lv_obj_get_parent(obj);
  lv_obj_t * mboxCont = (lv_obj_t *)lv_obj_get_parent(objCont);
  lv_obj_t * mboxParent = (lv_obj_t *)lv_obj_get_parent(mboxCont);

  lv_obj_t * data = (lv_obj_t *)lv_event_get_user_data(e);
  if(code == LV_EVENT_SHORT_CLICKED){
    if(obj == gui.element.cleanPopup.cleanSpinBoxPlusButton){
      lv_spinbox_increment(gui.element.cleanPopup.cleanSpinBox);
    }
    if(obj == gui.element.cleanPopup.cleanSpinBoxMinusButton){
      lv_spinbox_decrement(gui.element.cleanPopup.cleanSpinBox);
    }
  }
  if(code == LV_EVENT_CLICKED){
    if(obj == gui.element.cleanPopup.cleanRunButton){
        lv_obj_add_flag(gui.element.cleanPopup.cleanSettingsContainer, LV_OBJ_FLAG_HIDDEN);
        lv_obj_remove_flag(gui.element.cleanPopup.cleanProcessContainer, LV_OBJ_FLAG_HIDDEN);
        gui.page.tools.machineStats.clean ++;
        qSysAction( SAVE_MACHINE_STATS );
    }
    if(obj == gui.element.cleanPopup.cleanCancelButton){
      lv_obj_add_flag(gui.element.cleanPopup.cleanPopupParent, LV_OBJ_FLAG_HIDDEN);
    }
  }
}


void cleanPopup (void){
  /*********************
   *    PAGE ELEMENTS
   *********************/
  if (gui.element.cleanPopup.cleanPopupParent == NULL)
  {
      gui.element.cleanPopup.cleanPopupParent = lv_obj_class_create_obj(&lv_msgbox_backdrop_class, lv_layer_top());
      lv_obj_class_init_obj(gui.element.cleanPopup.cleanPopupParent);
      lv_obj_remove_flag(gui.element.cleanPopup.cleanPopupParent, LV_OBJ_FLAG_IGNORE_LAYOUT);
      lv_obj_set_size(gui.element.cleanPopup.cleanPopupParent, LV_PCT(100), LV_PCT(100));

      gui.element.cleanPopup.cleanContainer = lv_obj_create(gui.element.cleanPopup.cleanPopupParent);
      lv_obj_align(gui.element.cleanPopup.cleanContainer, LV_ALIGN_CENTER, 0, 0);
      lv_obj_set_size(gui.element.cleanPopup.cleanContainer, 320, 280); 
      lv_obj_remove_flag(gui.element.cleanPopup.cleanContainer, LV_OBJ_FLAG_SCROLLABLE); 

              gui.element.cleanPopup.cleanTitle = lv_label_create(gui.element.cleanPopup.cleanContainer);         
              lv_label_set_text(gui.element.cleanPopup.cleanTitle, cleanPopupTitle_text); 
              lv_obj_set_style_text_font(gui.element.cleanPopup.cleanTitle, &lv_font_montserrat_22, 0);              
              lv_obj_align(gui.element.cleanPopup.cleanTitle, LV_ALIGN_TOP_MID, 0, - 10);

              /*Create style*/
              lv_style_init(&gui.element.cleanPopup.style_cleanTitleLine);
              lv_style_set_line_width(&gui.element.cleanPopup.style_cleanTitleLine, 2);
              lv_style_set_line_color(&gui.element.cleanPopup.style_cleanTitleLine, lv_palette_main(LV_PALETTE_BLUE));
              lv_style_set_line_rounded(&gui.element.cleanPopup.style_cleanTitleLine, true);

              /*Create a line and apply the new style*/
              gui.element.cleanPopup.cleanPopupTitleLine = lv_line_create(gui.element.cleanPopup.cleanContainer);
              lv_line_set_points(gui.element.cleanPopup.cleanPopupTitleLine, gui.element.cleanPopup.titleLinePoints, 2);
              lv_obj_add_style(gui.element.cleanPopup.cleanPopupTitleLine, &gui.element.cleanPopup.style_cleanTitleLine, 0);
              lv_obj_align(gui.element.cleanPopup.cleanPopupTitleLine, LV_ALIGN_TOP_MID, 0, 23);

              gui.element.cleanPopup.cleanSettingsContainer = lv_obj_create(gui.element.cleanPopup.cleanPopupParent);
              lv_obj_align(gui.element.cleanPopup.cleanSettingsContainer, LV_ALIGN_CENTER, 0, -5);
              lv_obj_set_size(gui.element.cleanPopup.cleanSettingsContainer, 320, 180); 
              lv_obj_remove_flag(gui.element.cleanPopup.cleanSettingsContainer, LV_OBJ_FLAG_SCROLLABLE); 
              //lv_obj_set_style_border_color(gui.element.cleanPopup.cleanSettingsContainer , lv_color_hex(GREEN_DARK), 0);
              lv_obj_set_style_border_opa(gui.element.cleanPopup.cleanSettingsContainer , LV_OPA_TRANSP, 0);
            
                          gui.element.cleanPopup.cleanSubTitleLabel = lv_label_create(gui.element.cleanPopup.cleanSettingsContainer);         
                          lv_label_set_text(gui.element.cleanPopup.cleanSubTitleLabel, cleanPopupSubTitle_text); 
                          lv_obj_set_style_text_font(gui.element.cleanPopup.cleanSubTitleLabel, &lv_font_montserrat_18, 0);              
                          lv_obj_align(gui.element.cleanPopup.cleanSubTitleLabel, LV_ALIGN_TOP_MID, 0, -12);
                        
                          
                          gui.element.cleanPopup.cleanChemicalTanksContainer = lv_obj_create(gui.element.cleanPopup.cleanSettingsContainer);
                          lv_obj_remove_flag(gui.element.cleanPopup.cleanChemicalTanksContainer , LV_OBJ_FLAG_SCROLLABLE); 
                          lv_obj_align(gui.element.cleanPopup.cleanChemicalTanksContainer , LV_ALIGN_LEFT_MID, -7, -40);
                          lv_obj_set_size(gui.element.cleanPopup.cleanChemicalTanksContainer , 300, 50); 
                          //lv_obj_set_style_border_color(gui.element.cleanPopup.cleanChemicalTanksContainer , lv_color_hex(GREEN_DARK), 0);
                          lv_obj_set_style_border_opa(gui.element.cleanPopup.cleanChemicalTanksContainer , LV_OPA_TRANSP, 0);


                          //Container checkboxes
                          gui.element.cleanPopup.cleanSelectC1CheckBox = lv_obj_create(gui.element.cleanPopup.cleanChemicalTanksContainer);
                          lv_obj_remove_flag(gui.element.cleanPopup.cleanSelectC1CheckBox, LV_OBJ_FLAG_SCROLLABLE); 
                          lv_obj_align(gui.element.cleanPopup.cleanSelectC1CheckBox, LV_ALIGN_LEFT_MID, -15, 0);
                          lv_obj_set_size(gui.element.cleanPopup.cleanSelectC1CheckBox, 100, 40); 
                          //lv_obj_set_style_border_color(gui.element.cleanPopup.cleanSelectC1CheckBox, lv_color_hex(GREY), 0);
                          lv_obj_set_style_border_opa(gui.element.cleanPopup.cleanSelectC1CheckBox, LV_OPA_TRANSP, 0);

                                gui.element.cleanPopup.cleanC1CheckBoxLabel = lv_label_create(gui.element.cleanPopup.cleanSelectC1CheckBox);         
                                lv_label_set_text(gui.element.cleanPopup.cleanC1CheckBoxLabel, processSourceList[0]); 
                                lv_obj_set_style_text_font(gui.element.cleanPopup.cleanC1CheckBoxLabel, &lv_font_montserrat_22, 0);              
                                lv_obj_align(gui.element.cleanPopup.cleanC1CheckBoxLabel, LV_ALIGN_LEFT_MID, - 10, 0);

                                gui.element.cleanPopup.cleanSelectC1CheckBox = create_radiobutton(gui.element.cleanPopup.cleanSelectC1CheckBox, "", 0, 0, 27, &lv_font_montserrat_18, lv_color_hex(BLUE_DARK), lv_palette_main(LV_PALETTE_BLUE));
                                lv_obj_add_event_cb(gui.element.cleanPopup.cleanSelectC1CheckBox, event_cleanPopup, LV_EVENT_VALUE_CHANGED, gui.element.cleanPopup.cleanSelectC1CheckBox);


                          gui.element.cleanPopup.cleanSelectC2CheckBox = lv_obj_create(gui.element.cleanPopup.cleanChemicalTanksContainer);
                          lv_obj_remove_flag(gui.element.cleanPopup.cleanSelectC2CheckBox, LV_OBJ_FLAG_SCROLLABLE); 
                          lv_obj_align(gui.element.cleanPopup.cleanSelectC2CheckBox, LV_ALIGN_LEFT_MID, 85, 0);
                          lv_obj_set_size(gui.element.cleanPopup.cleanSelectC2CheckBox, 100, 40); 
                          //lv_obj_set_style_border_color(gui.element.cleanPopup.cleanSelectC2CheckBox, lv_color_hex(GREY), 0);
                          lv_obj_set_style_border_opa(gui.element.cleanPopup.cleanSelectC2CheckBox, LV_OPA_TRANSP, 0);

                                gui.element.cleanPopup.cleanC2CheckBoxLabel = lv_label_create(gui.element.cleanPopup.cleanSelectC2CheckBox);         
                                lv_label_set_text(gui.element.cleanPopup.cleanC2CheckBoxLabel, processSourceList[1]); 
                                lv_obj_set_style_text_font(gui.element.cleanPopup.cleanC2CheckBoxLabel, &lv_font_montserrat_22, 0);              
                                lv_obj_align(gui.element.cleanPopup.cleanC2CheckBoxLabel, LV_ALIGN_LEFT_MID, - 10, 0);

                                gui.element.cleanPopup.cleanSelectC2CheckBox = create_radiobutton(gui.element.cleanPopup.cleanSelectC2CheckBox, "", 0, 0, 27, &lv_font_montserrat_18, lv_color_hex(BLUE_DARK), lv_palette_main(LV_PALETTE_BLUE));
                                lv_obj_add_event_cb(gui.element.cleanPopup.cleanSelectC2CheckBox, event_cleanPopup, LV_EVENT_VALUE_CHANGED, gui.element.cleanPopup.cleanSelectC2CheckBox);


                          gui.element.cleanPopup.cleanSelectC3CheckBox = lv_obj_create(gui.element.cleanPopup.cleanChemicalTanksContainer);
                          lv_obj_remove_flag(gui.element.cleanPopup.cleanSelectC3CheckBox, LV_OBJ_FLAG_SCROLLABLE); 
                          lv_obj_align(gui.element.cleanPopup.cleanSelectC3CheckBox, LV_ALIGN_LEFT_MID, 185, 0);
                          lv_obj_set_size(gui.element.cleanPopup.cleanSelectC3CheckBox, 100, 40); 
                          //lv_obj_set_style_border_color(gui.element.cleanPopup.cleanSelectC3CheckBox, lv_color_hex(GREY), 0);
                          lv_obj_set_style_border_opa(gui.element.cleanPopup.cleanSelectC3CheckBox, LV_OPA_TRANSP, 0);

                                gui.element.cleanPopup.cleanC3CheckBoxLabel = lv_label_create(gui.element.cleanPopup.cleanSelectC3CheckBox);         
                                lv_label_set_text(gui.element.cleanPopup.cleanC3CheckBoxLabel, processSourceList[2]); 
                                lv_obj_set_style_text_font(gui.element.cleanPopup.cleanC3CheckBoxLabel, &lv_font_montserrat_22, 0);              
                                lv_obj_align(gui.element.cleanPopup.cleanC3CheckBoxLabel, LV_ALIGN_LEFT_MID, - 10, 0);

                                gui.element.cleanPopup.cleanSelectC3CheckBox = create_radiobutton(gui.element.cleanPopup.cleanSelectC3CheckBox, "", 0, 0, 27, &lv_font_montserrat_18, lv_color_hex(BLUE_DARK), lv_palette_main(LV_PALETTE_BLUE));
                                lv_obj_add_event_cb(gui.element.cleanPopup.cleanSelectC3CheckBox, event_cleanPopup, LV_EVENT_VALUE_CHANGED, gui.element.cleanPopup.cleanSelectC3CheckBox);  


                          
                          gui.element.cleanPopup.cleanSpinBoxContainer = lv_obj_create(gui.element.cleanPopup.cleanSettingsContainer);
                          lv_obj_remove_flag(gui.element.cleanPopup.cleanSpinBoxContainer, LV_OBJ_FLAG_SCROLLABLE); 
                          lv_obj_align(gui.element.cleanPopup.cleanSpinBoxContainer, LV_ALIGN_CENTER, 0, 5);
                          lv_obj_set_size(gui.element.cleanPopup.cleanSpinBoxContainer, 300, 50); 
                          //lv_obj_set_style_border_color(gui.element.cleanPopup.cleanSpinBoxContainer , lv_color_hex(GREEN_DARK), 0);
                          lv_obj_set_style_border_opa(gui.element.cleanPopup.cleanSpinBoxContainer, LV_OPA_TRANSP, 0);


                                gui.element.cleanPopup.cleanSpinBox = lv_spinbox_create(gui.element.cleanPopup.cleanSpinBoxContainer);
                                lv_spinbox_set_range(gui.element.cleanPopup.cleanSpinBox, 1, 5);
                                lv_spinbox_set_digit_format(gui.element.cleanPopup.cleanSpinBox, 1, 0);
                                lv_obj_set_width(gui.element.cleanPopup.cleanSpinBox, 30);
                                lv_obj_align(gui.element.cleanPopup.cleanSpinBox, LV_ALIGN_LEFT_MID, 185, 0);
                                lv_obj_set_style_bg_opa(gui.element.cleanPopup.cleanSpinBox, 0, LV_PART_CURSOR);


                                      gui.element.cleanPopup.cleanRepeatTimesLabel = lv_label_create(gui.element.cleanPopup.cleanSpinBoxContainer);         
                                      lv_label_set_text(gui.element.cleanPopup.cleanRepeatTimesLabel, cleanRoller_text); 
                                      lv_obj_set_style_text_font(gui.element.cleanPopup.cleanRepeatTimesLabel, &lv_font_montserrat_22, 0);              
                                      lv_obj_align(gui.element.cleanPopup.cleanRepeatTimesLabel, LV_ALIGN_LEFT_MID, - 10, 0);

                                      gui.element.cleanPopup.cleanSpinBoxPlusButton = lv_button_create(gui.element.cleanPopup.cleanSpinBoxContainer);
                                      lv_obj_set_size(gui.element.cleanPopup.cleanSpinBoxPlusButton, lv_obj_get_height(gui.element.cleanPopup.cleanSpinBox), lv_obj_get_height(gui.element.cleanPopup.cleanSpinBox));
                                      lv_obj_align_to(gui.element.cleanPopup.cleanSpinBoxPlusButton, gui.element.cleanPopup.cleanSpinBox, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
                                      lv_obj_set_style_bg_image_src(gui.element.cleanPopup.cleanSpinBoxPlusButton, LV_SYMBOL_PLUS, 0);
                                      lv_obj_add_event_cb(gui.element.cleanPopup.cleanSpinBoxPlusButton, event_cleanPopup, LV_EVENT_ALL,  NULL);

                                      gui.element.cleanPopup.cleanSpinBoxMinusButton = lv_button_create(gui.element.cleanPopup.cleanSpinBoxContainer);
                                      lv_obj_set_size(gui.element.cleanPopup.cleanSpinBoxMinusButton, lv_obj_get_height(gui.element.cleanPopup.cleanSpinBox), lv_obj_get_height(gui.element.cleanPopup.cleanSpinBox));
                                      lv_obj_align_to(gui.element.cleanPopup.cleanSpinBoxMinusButton, gui.element.cleanPopup.cleanSpinBox, LV_ALIGN_OUT_LEFT_MID, -5, 0);
                                      lv_obj_set_style_bg_image_src(gui.element.cleanPopup.cleanSpinBoxMinusButton, LV_SYMBOL_MINUS, 0);
                                      lv_obj_add_event_cb(gui.element.cleanPopup.cleanSpinBoxMinusButton, event_cleanPopup, LV_EVENT_ALL, NULL);




                      gui.element.cleanPopup.cleanDrainWaterLabelContainer = lv_obj_create(gui.element.cleanPopup.cleanSettingsContainer);
                      lv_obj_remove_flag(gui.element.cleanPopup.cleanDrainWaterLabelContainer , LV_OBJ_FLAG_SCROLLABLE); 
                      lv_obj_align(gui.element.cleanPopup.cleanDrainWaterLabelContainer, LV_ALIGN_CENTER, 0, 55);
                      lv_obj_set_size(gui.element.cleanPopup.cleanDrainWaterLabelContainer, 300, 40); 
                      //lv_obj_set_style_border_color(gui.element.cleanPopup.cleanDrainWaterLabelContainer , lv_color_hex(GREEN_DARK), 0);
                      lv_obj_set_style_border_opa(gui.element.cleanPopup.cleanDrainWaterLabelContainer , LV_OPA_TRANSP, 0);


                          gui.element.cleanPopup.cleanDrainWaterLabel = lv_label_create(gui.element.cleanPopup.cleanDrainWaterLabelContainer);         
                          lv_label_set_text(gui.element.cleanPopup.cleanDrainWaterLabel, cleanDrainWater_text); 
                          lv_obj_set_style_text_font(gui.element.cleanPopup.cleanDrainWaterLabel, &lv_font_montserrat_18, 0);              
                          lv_obj_align(gui.element.cleanPopup.cleanDrainWaterLabel, LV_ALIGN_LEFT_MID, -10, 0);

                          gui.element.cleanPopup.cleanDrainWaterSwitch = lv_switch_create(gui.element.cleanPopup.cleanDrainWaterLabelContainer);
                          lv_obj_add_event_cb(gui.element.cleanPopup.cleanDrainWaterSwitch , event_filterMBox, LV_EVENT_VALUE_CHANGED, gui.element.cleanPopup.cleanDrainWaterSwitch);
                          lv_obj_align(gui.element.cleanPopup.cleanDrainWaterSwitch , LV_ALIGN_LEFT_MID, 220, 0);
                          lv_obj_set_style_bg_color(gui.element.cleanPopup.cleanDrainWaterSwitch, lv_palette_darken(LV_PALETTE_GREY, 3), LV_STATE_DEFAULT);
                          lv_obj_set_style_bg_color(gui.element.cleanPopup.cleanDrainWaterSwitch,  lv_palette_main(LV_PALETTE_BLUE), LV_PART_KNOB | LV_STATE_DEFAULT);
                          lv_obj_set_style_bg_color(gui.element.cleanPopup.cleanDrainWaterSwitch, lv_color_hex(BLUE_DARK) , LV_PART_INDICATOR | LV_STATE_CHECKED);



              
              gui.element.cleanPopup.cleanRunButton = lv_button_create(gui.element.cleanPopup.cleanContainer);
              lv_obj_set_size(gui.element.cleanPopup.cleanRunButton, BUTTON_MBOX_WIDTH, BUTTON_MBOX_HEIGHT);
              lv_obj_align(gui.element.cleanPopup.cleanRunButton, LV_ALIGN_BOTTOM_LEFT, 10 , 10);
              lv_obj_add_event_cb(gui.element.cleanPopup.cleanRunButton, event_cleanPopup, LV_EVENT_CLICKED, NULL);
              lv_obj_set_style_bg_color(gui.element.cleanPopup.cleanRunButton, lv_color_hex(GREEN_DARK), LV_PART_MAIN);

                  gui.element.cleanPopup.cleanCancelButtonLabel = lv_label_create(gui.element.cleanPopup.cleanRunButton);
                  lv_label_set_text(gui.element.cleanPopup.cleanCancelButtonLabel, cleanRunButton_text);
                  lv_obj_set_style_text_font(gui.element.cleanPopup.cleanCancelButtonLabel, &lv_font_montserrat_22, 0);
                  lv_obj_align(gui.element.cleanPopup.cleanCancelButtonLabel, LV_ALIGN_CENTER, 0, 0);


              gui.element.cleanPopup.cleanCancelButton = lv_button_create(gui.element.cleanPopup.cleanContainer);
              lv_obj_set_size(gui.element.cleanPopup.cleanCancelButton, BUTTON_MBOX_WIDTH, BUTTON_MBOX_HEIGHT);
              lv_obj_align(gui.element.cleanPopup.cleanCancelButton, LV_ALIGN_BOTTOM_RIGHT, - 10 , 10);
              lv_obj_add_event_cb(gui.element.cleanPopup.cleanCancelButton, event_cleanPopup, LV_EVENT_CLICKED, NULL);
              lv_obj_set_style_bg_color(gui.element.cleanPopup.cleanCancelButton, lv_color_hex(RED_DARK), LV_PART_MAIN);

                  gui.element.cleanPopup.cleanCancelButtonLabel = lv_label_create(gui.element.cleanPopup.cleanCancelButton);
                  lv_label_set_text(gui.element.cleanPopup.cleanCancelButtonLabel, cleanCancelButton_text);
                  lv_obj_set_style_text_font(gui.element.cleanPopup.cleanCancelButtonLabel, &lv_font_montserrat_22, 0);
                  lv_obj_align(gui.element.cleanPopup.cleanCancelButtonLabel, LV_ALIGN_CENTER, 0, 0);
        
        

        gui.element.cleanPopup.cleanProcessContainer = lv_obj_create(gui.element.cleanPopup.cleanPopupParent);
        lv_obj_align(gui.element.cleanPopup.cleanProcessContainer, LV_ALIGN_CENTER, 0, -5);
        lv_obj_set_size(gui.element.cleanPopup.cleanProcessContainer, 320, 180); 
        lv_obj_remove_flag(gui.element.cleanPopup.cleanProcessContainer, LV_OBJ_FLAG_SCROLLABLE); 
        //lv_obj_set_style_border_color(gui.element.cleanPopup.cleanProcessContainer , lv_color_hex(GREEN_DARK), 0);
        lv_obj_set_style_border_opa(gui.element.cleanPopup.cleanProcessContainer , LV_OPA_TRANSP, 0);
        lv_obj_add_flag(gui.element.cleanPopup.cleanProcessContainer, LV_OBJ_FLAG_HIDDEN);
  }


}

