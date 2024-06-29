#include <sys/_stdint.h>

/**
 * @file page_settings.c
 *
 */


//ESSENTIAL INCLUDE
#include <lvgl.h>
#include "../../include/definitions.h"

extern struct gui_components gui;


//ACCESSORY INCLUDES


void event_settings_style_delete(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_DELETE) {
        //list of all styles to be reset, so clean the memory.
        lv_style_reset(&gui.page.settings.style_sectionTitleLine);

    }
}

void event_settingPopupMBox(lv_event_t * e){
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t * obj = (lv_obj_t *)lv_event_get_target(e);
  lv_obj_t * objCont = (lv_obj_t *)lv_obj_get_parent(obj);
  lv_obj_t * mboxCont = (lv_obj_t *)lv_obj_get_parent(objCont);
  lv_obj_t * data = (lv_obj_t *)lv_event_get_user_data(e);
  
  if(data == gui.page.settings.tempSensorTuningLabel){
    messagePopupCreate(messagePopupDetailTitle_text,tempAlertMBox_text,NULL,NULL,NULL);
  }
  if(data == gui.page.settings.filmRotationSpeedLabel){
    messagePopupCreate(messagePopupDetailTitle_text,filmRotationSpeedAlertMBox_text,NULL,NULL,NULL);
  }
  if(data == gui.page.settings.filmRotationInverseIntervalLabel){
    messagePopupCreate(messagePopupDetailTitle_text,rotationInverseIntervalAlertMBox_text,NULL,NULL,NULL);
  }
  if(data == gui.page.settings.filmRotationRandomLabel){
    messagePopupCreate(messagePopupDetailTitle_text,filmRotationRandomAlertMBox_text,NULL,NULL,NULL);
  }
  if(data == gui.page.settings.persistentAlarmLabel){
    messagePopupCreate(messagePopupDetailTitle_text,soundAlertMBox_text,NULL,NULL,NULL);
  }
  if(data == gui.page.settings.autostartLabel){
    messagePopupCreate(messagePopupDetailTitle_text,autostartAlertMBox_text,NULL,NULL,NULL);
  }
  if(data == gui.page.settings.drainFillTimeLabel){
    messagePopupCreate(messagePopupDetailTitle_text,drainFillTimeAlertMBox_text,NULL,NULL,NULL);
  }
}


void event_settings_handler(lv_event_t * e)
{
    uint32_t * active_id = (uint32_t *)lv_event_get_user_data(e);
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * cont = (lv_obj_t *)lv_event_get_current_target(e);
    lv_obj_t * act_cb = (lv_obj_t *)lv_event_get_target(e);
    lv_obj_t * old_cb = (lv_obj_t *)lv_obj_get_child(cont, *active_id);

    /*Do nothing if the container was clicked*/

    if(act_cb == cont && cont != gui.page.settings.waterInletSwitch && cont != gui.page.settings.tempSensorTuneButton && cont != gui.page.settings.filmRotationSpeedSlider && cont != gui.page.settings.filmRotationInversionIntervalSlider && cont != gui.page.settings.filmRandomlSlider && cont != gui.page.settings.persistentAlarmSwitch && cont != gui.page.settings.autostartSwitch && cont != gui.page.settings.drainFillTimeSlider && cont != gui.page.settings.multiRinseTimeSlider) 
    //if(act_cb == cont) 
      return;

    if(act_cb == gui.page.settings.tempUnitCelsiusRadioButton || act_cb == gui.page.settings.tempUnitFahrenheitRadioButton){
       if(code == LV_EVENT_CLICKED) {
            lv_obj_remove_state(old_cb, LV_STATE_CHECKED);
            lv_obj_add_state(act_cb, LV_STATE_CHECKED); 
            *active_id = lv_obj_get_index(act_cb);
            LV_LOG_USER("Selected °C or °F: %d", (int)gui.page.settings.active_index);
            gui.page.settings.settingsParams.tempUnit = (int)gui.page.settings.active_index;
            qSysAction( SAVE_PROCESS_CONFIG );
            (lv_obj_t *)lv_obj_get_child(cont, *active_id);
            uint8_t i = 0;
            for(i = 0; i < lv_obj_get_child_cnt(gui.page.processes.processesListContainer); i++) {
              lv_obj_send_event(lv_obj_get_child(gui.page.processes.processesListContainer, i), LV_EVENT_REFRESH, NULL);
            }
       }
    }
 
    if(act_cb == gui.page.settings.waterInletSwitch){
      if(code == LV_EVENT_VALUE_CHANGED) {
          LV_LOG_USER("State Inlet: %s", lv_obj_has_state(act_cb, LV_STATE_CHECKED) ? "On" : "Off");
          gui.page.settings.settingsParams.waterInlet = lv_obj_has_state(act_cb, LV_STATE_CHECKED);
          qSysAction( SAVE_PROCESS_CONFIG );
        }
    }


    if(act_cb == gui.page.settings.tempSensorTuneButton){
      if(code == LV_EVENT_SHORT_CLICKED) {
          LV_LOG_USER("TUNE short click");
          rollerPopupCreate(gui.element.rollerPopup.tempCelsiusOptions,tuneTempPopupTitle_text,gui.page.settings.tempSensorTuneButton, gui.page.settings.settingsParams.calibratedTemp);
        }
      if(code == LV_EVENT_LONG_PRESSED_REPEAT) {
          LV_LOG_USER("TUNE Long click");
        }
    }

    if(act_cb == gui.page.settings.filmRotationSpeedSlider){
      if(code == LV_EVENT_VALUE_CHANGED) {
          lv_label_set_text_fmt((lv_obj_t*)lv_event_get_user_data(e), "%drpm", lv_slider_get_value(act_cb));
          LV_LOG_USER("Film Speed Rotation : %d",lv_slider_get_value(act_cb));
          gui.page.settings.settingsParams.filmRotationSpeedSetpoint = lv_slider_get_value(act_cb);
        }
      if(code == LV_EVENT_RELEASED){
          qSysAction( SAVE_PROCESS_CONFIG );
        }
    }

    if(act_cb == gui.page.settings.filmRotationInversionIntervalSlider){
      if(code == LV_EVENT_VALUE_CHANGED) {
          lv_label_set_text_fmt((lv_obj_t*)lv_event_get_user_data(e), "%dsec%", lv_slider_get_value(act_cb));
          LV_LOG_USER("Film Rotation Inversion Interval : %d",lv_slider_get_value(act_cb));
          gui.page.settings.settingsParams.rotationIntervalSetpoint = lv_slider_get_value(act_cb);
        }
      if(code == LV_EVENT_RELEASED){
          qSysAction( SAVE_PROCESS_CONFIG );
        }    }

    if(act_cb == gui.page.settings.filmRandomlSlider){
      if(code == LV_EVENT_VALUE_CHANGED) {
        lv_label_set_text_fmt((lv_obj_t*)lv_event_get_user_data(e), "%~%d%%", lv_slider_get_value(act_cb));
        LV_LOG_USER("Film Randomness : %d",lv_slider_get_value(act_cb));
        gui.page.settings.settingsParams.randomSetpoint = lv_slider_get_value(act_cb);
        }
      if(code == LV_EVENT_RELEASED){
          qSysAction( SAVE_PROCESS_CONFIG );
        }    }

    if(act_cb == gui.page.settings.persistentAlarmSwitch){
      if(code == LV_EVENT_VALUE_CHANGED) {
          LV_LOG_USER("Persistent Alarm: %s", lv_obj_has_state(act_cb, LV_STATE_CHECKED) ? "On" : "Off");
          gui.page.settings.settingsParams.isPersistentAlarm = lv_obj_has_state(act_cb, LV_STATE_CHECKED);
          qSysAction( SAVE_PROCESS_CONFIG );
        }
    }

    if(act_cb == gui.page.settings.autostartSwitch){
      if(code == LV_EVENT_VALUE_CHANGED) {
          LV_LOG_USER("Autostart : %s", lv_obj_has_state(act_cb, LV_STATE_CHECKED) ? "On" : "Off");
          gui.page.settings.settingsParams.isProcessAutostart = lv_obj_has_state(act_cb, LV_STATE_CHECKED);
          qSysAction( SAVE_PROCESS_CONFIG );
        }
    }


    if(act_cb == gui.page.settings.drainFillTimeSlider) {
        if(code == LV_EVENT_RELEASED) {
            uint8_t current_value = gui.page.settings.settingsParams.drainFillOverlapSetpoint;
            uint8_t new_value = lv_slider_get_value(act_cb);

            if(new_value > current_value) {
                new_value = current_value + 50;
            } else if(new_value < current_value) {
                new_value = current_value - 50;
            }


            lv_slider_set_value(act_cb, new_value, LV_ANIM_OFF);  // Update the slider value to the nearest 30
            lv_label_set_text_fmt((lv_obj_t*)lv_event_get_user_data(e), "%d%%", lv_slider_get_value(act_cb));
            LV_LOG_USER("Drain/fill time overlap percent : %d",lv_slider_get_value(act_cb));
            gui.page.settings.settingsParams.drainFillOverlapSetpoint = lv_slider_get_value(act_cb);
            qSysAction(SAVE_PROCESS_CONFIG);
        }
    }


    if(act_cb == gui.page.settings.multiRinseTimeSlider) {
        if(code == LV_EVENT_RELEASED) {
            uint8_t current_value = gui.page.settings.settingsParams.multiRinseTime;
            uint8_t new_value = lv_slider_get_value(act_cb);

            if(new_value > current_value) {
                new_value = current_value + 30;
            } else if(new_value < current_value) {
                new_value = current_value - 30;
            }

            // Ensure new_value is within valid bounds (assuming 60 to 180 as mentioned)
            if(new_value < 60) new_value = 60;
            if(new_value > 180) new_value = 180;

            lv_slider_set_value(act_cb, new_value, LV_ANIM_OFF);  // Update the slider value to the nearest 30
            lv_label_set_text_fmt((lv_obj_t*)lv_event_get_user_data(e), "%ds", new_value);
            LV_LOG_USER("Multi rinse cycle time (s): %d", new_value);
            gui.page.settings.settingsParams.multiRinseTime = new_value;
            qSysAction(SAVE_PROCESS_CONFIG);
        }
    }
    
}

//TO TEST
//lv_obj_set_scroll_dir(mBoxPopupTextContainer, LV_DIR_VER);


void initSettings(void){
/*********************
 *    PAGE HEADER
 *********************/
  LV_LOG_USER("Settings Creation");
  gui.page.settings.settingsSection = lv_obj_create(lv_screen_active());
  lv_obj_set_pos(gui.page.settings.settingsSection, 140, 7);                            
  lv_obj_set_size(gui.page.settings.settingsSection, 335, 303); 
  lv_obj_remove_flag(gui.page.settings.settingsSection, LV_OBJ_FLAG_SCROLLABLE);    
  lv_obj_set_style_border_color(gui.page.settings.settingsSection, lv_color_hex(ORANGE_LIGHT), 0);


	lv_coord_t pad = 2;
	lv_obj_set_style_pad_left(gui.page.settings.settingsSection, pad, LV_PART_INDICATOR);
	lv_obj_set_style_pad_right(gui.page.settings.settingsSection, pad, LV_PART_INDICATOR);
	lv_obj_set_style_pad_top(gui.page.settings.settingsSection, pad, LV_PART_INDICATOR);
	lv_obj_set_style_pad_bottom(gui.page.settings.settingsSection, pad, LV_PART_INDICATOR);


 
  gui.page.settings.settingsLabel = lv_label_create(gui.page.settings.settingsSection);         
  lv_label_set_text(gui.page.settings.settingsLabel, Settings_text); 
  lv_obj_set_style_text_font(gui.page.settings.settingsLabel, &lv_font_montserrat_28, 0);              
  lv_obj_align(gui.page.settings.settingsLabel, LV_ALIGN_TOP_LEFT, -7, -5);

  /*Create style*/
  lv_style_init(&gui.page.settings.style_sectionTitleLine);
  lv_style_set_line_width(&gui.page.settings.style_sectionTitleLine, 2);
  lv_style_set_line_color(&gui.page.settings.style_sectionTitleLine, lv_palette_main(LV_PALETTE_ORANGE));
  lv_style_set_line_rounded(&gui.page.settings.style_sectionTitleLine, true);

  /*Create a line and apply the new style*/
  gui.page.settings.sectionTitleLine = lv_line_create(gui.page.settings.settingsSection);
  lv_line_set_points(gui.page.settings.sectionTitleLine, gui.page.settings.titleLinePoints, 2);
  lv_obj_add_style(gui.page.settings.sectionTitleLine, &gui.page.settings.style_sectionTitleLine, 0);
  lv_obj_align(gui.page.settings.sectionTitleLine, LV_ALIGN_CENTER, 0, - 103);

  lv_obj_update_layout(gui.page.settings.settingsSection);

  /*********************
 *    PAGE ELEMENTS
 *********************/

  gui.page.settings.settingsContainer = lv_obj_create(gui.page.settings.settingsSection);
  lv_obj_set_pos(gui.page.settings.settingsContainer, -22, 31);                           
  lv_obj_set_size(gui.page.settings.settingsContainer, 335, 254);  
  //lv_obj_set_style_border_color(gui.page.settings.settingsContainer, lv_color_hex(GREEN_DARK), 0);
  lv_obj_set_style_border_opa(gui.page.settings.settingsContainer, LV_OPA_TRANSP, 0);
  lv_obj_set_scroll_dir(gui.page.settings.settingsContainer, LV_DIR_VER);

  gui.page.settings.tempUnitContainer = lv_obj_create(gui.page.settings.settingsContainer);
  lv_obj_align(gui.page.settings.tempUnitContainer, LV_ALIGN_TOP_LEFT, -15, -17);                         
  lv_obj_set_size(gui.page.settings.tempUnitContainer, 330, 60); 
  lv_obj_remove_flag(gui.page.settings.tempUnitContainer, LV_OBJ_FLAG_SCROLLABLE);    
  //lv_obj_set_style_border_color(gui.page.settings.tempUnitContainer, lv_color_hex(LV_PALETTE_GREEN), 0);
  lv_obj_set_style_border_opa(gui.page.settings.tempUnitContainer, LV_OPA_TRANSP, 0);
  lv_obj_add_event_cb(gui.page.settings.tempUnitContainer, event_settings_handler, LV_EVENT_CLICKED, &gui.page.settings.active_index);

        gui.page.settings.tempUnitCelsiusRadioButton = create_radiobutton(gui.page.settings.tempUnitContainer, celsius_text, -55, 0, 27, &lv_font_montserrat_18, lv_color_hex(ORANGE_DARK), lv_color_hex(ORANGE));
        gui.page.settings.tempUnitFahrenheitRadioButton = create_radiobutton(gui.page.settings.tempUnitContainer, fahrenheit_text, 5, 0, 27, &lv_font_montserrat_18, lv_color_hex(ORANGE_DARK), lv_color_hex(ORANGE));
        
        //Make the checkbox checked according the saved param
        gui.page.settings.active_index = gui.page.settings.settingsParams.tempUnit;
        lv_obj_add_state(lv_obj_get_child(gui.page.settings.tempUnitContainer, gui.page.settings.settingsParams.tempUnit), LV_STATE_CHECKED);
           
        gui.page.settings.tempUnitLabel = lv_label_create(gui.page.settings.tempUnitContainer);         
        lv_label_set_text(gui.page.settings.tempUnitLabel, tempUnit_text); 
        lv_obj_set_style_text_font(gui.page.settings.tempUnitLabel, &lv_font_montserrat_20, 0);              
        lv_obj_align(gui.page.settings.tempUnitLabel, LV_ALIGN_LEFT_MID, -5, 0);


  gui.page.settings.waterInletContainer = lv_obj_create(gui.page.settings.settingsContainer);
  lv_obj_align(gui.page.settings.waterInletContainer, LV_ALIGN_TOP_LEFT, -15, 45);                         
  lv_obj_set_size(gui.page.settings.waterInletContainer, 330, 60); 
  lv_obj_remove_flag(gui.page.settings.waterInletContainer, LV_OBJ_FLAG_SCROLLABLE);    
  //lv_obj_set_style_border_color(gui.page.settings.waterInletContainer, lv_color_hex(LV_PALETTE_GREEN), 0);
  lv_obj_set_style_border_opa(gui.page.settings.waterInletContainer, LV_OPA_TRANSP, 0);

        gui.page.settings.waterInletLabel = lv_label_create(gui.page.settings.waterInletContainer);         
        lv_label_set_text(gui.page.settings.waterInletLabel, waterInlet_text); 
        lv_obj_set_style_text_font(gui.page.settings.waterInletLabel, &lv_font_montserrat_20, 0);              
        lv_obj_align(gui.page.settings.waterInletLabel, LV_ALIGN_LEFT_MID, -5, 0);

        gui.page.settings.waterInletSwitch = lv_switch_create(gui.page.settings.waterInletContainer);
        lv_obj_add_event_cb(gui.page.settings.waterInletSwitch, event_settings_handler, LV_EVENT_VALUE_CHANGED, gui.page.settings.waterInletSwitch);
        lv_obj_align(gui.page.settings.waterInletSwitch, LV_ALIGN_RIGHT_MID, 0, 0);
        lv_obj_set_style_bg_color(gui.page.settings.waterInletSwitch,  lv_palette_darken(LV_PALETTE_GREY, 3), LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(gui.page.settings.waterInletSwitch,  lv_color_hex(ORANGE), LV_PART_KNOB | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(gui.page.settings.waterInletSwitch,  lv_color_hex(ORANGE_DARK), LV_PART_INDICATOR | LV_STATE_CHECKED);
        lv_obj_add_state(gui.page.settings.waterInletSwitch, gui.page.settings.settingsParams.waterInlet);


  gui.page.settings.tempTuningContainer = lv_obj_create(gui.page.settings.settingsContainer);
  lv_obj_align(gui.page.settings.tempTuningContainer, LV_ALIGN_TOP_LEFT, -15, 107);                         
  lv_obj_set_size(gui.page.settings.tempTuningContainer, 330, 60); 
  lv_obj_remove_flag(gui.page.settings.tempTuningContainer, LV_OBJ_FLAG_SCROLLABLE);    
  //lv_obj_set_style_border_color(gui.page.settings.tempTuningContainer, lv_color_hex(LV_PALETTE_GREEN), 0);
  lv_obj_set_style_border_opa(gui.page.settings.tempTuningContainer, LV_OPA_TRANSP, 0);

        gui.page.settings.tempSensorTuningLabel = lv_label_create(gui.page.settings.tempTuningContainer);         
        lv_label_set_text(gui.page.settings.tempSensorTuningLabel, tempSensorTuning_text); 
        lv_obj_set_style_text_font(gui.page.settings.tempSensorTuningLabel, &lv_font_montserrat_20, 0);              
        lv_obj_align(gui.page.settings.tempSensorTuningLabel, LV_ALIGN_LEFT_MID, -5, 0);

        createQuestionMark(gui.page.settings.tempTuningContainer,gui.page.settings.tempSensorTuningLabel,event_settingPopupMBox, 2, -3);

        gui.page.settings.tempSensorTuneButton = lv_button_create(gui.page.settings.tempTuningContainer);
        lv_obj_set_size(gui.page.settings.tempSensorTuneButton, BUTTON_TUNE_WIDTH, BUTTON_TUNE_HEIGHT);
        lv_obj_align(gui.page.settings.tempSensorTuneButton, LV_ALIGN_RIGHT_MID, 5 , 0);
        lv_obj_add_event_cb(gui.page.settings.tempSensorTuneButton, event_settings_handler, LV_EVENT_CLICKED, gui.page.settings.tempSensorTuneButton);
        lv_obj_add_event_cb(gui.page.settings.tempSensorTuneButton, event_settings_handler, LV_EVENT_VALUE_CHANGED, gui.page.settings.tempSensorTuneButton);
        lv_obj_add_event_cb(gui.page.settings.tempSensorTuneButton, event_settings_handler, LV_EVENT_SHORT_CLICKED, gui.page.settings.tempSensorTuneButton);
        lv_obj_add_event_cb(gui.page.settings.tempSensorTuneButton, event_settings_handler, LV_EVENT_LONG_PRESSED_REPEAT, gui.page.settings.tempSensorTuneButton);
        lv_obj_add_event_cb(gui.page.settings.tempSensorTuneButton, event_settings_handler, LV_EVENT_RELEASED, gui.page.settings.tempSensorTuneButton);
        lv_obj_set_style_bg_color(gui.page.settings.tempSensorTuneButton, lv_color_hex(ORANGE), LV_PART_MAIN);

        gui.page.settings.tempSensorTuneButtonLabel = lv_label_create(gui.page.settings.tempSensorTuneButton);
        lv_label_set_text(gui.page.settings.tempSensorTuneButtonLabel, tuneButton_text);
        lv_obj_set_style_text_font(gui.page.settings.tempSensorTuneButtonLabel, &lv_font_montserrat_18, 0);
        lv_obj_align(gui.page.settings.tempSensorTuneButtonLabel, LV_ALIGN_CENTER, 0, 0);

  gui.page.settings.filmRotationSpeedContainer = lv_obj_create(gui.page.settings.settingsContainer);
  lv_obj_align(gui.page.settings.filmRotationSpeedContainer, LV_ALIGN_TOP_LEFT, -15, 179);                         
  lv_obj_set_size(gui.page.settings.filmRotationSpeedContainer, 330, 70); 
  lv_obj_remove_flag(gui.page.settings.filmRotationSpeedContainer, LV_OBJ_FLAG_SCROLLABLE);    
  //lv_obj_set_style_border_color(gui.page.settings.filmRotationSpeedContainer, lv_color_hex(LV_PALETTE_GREEN), 0);
  lv_obj_set_style_border_opa(gui.page.settings.filmRotationSpeedContainer, LV_OPA_TRANSP, 0);
   
        gui.page.settings.filmRotationSpeedLabel = lv_label_create(gui.page.settings.filmRotationSpeedContainer);         
        lv_label_set_text(gui.page.settings.filmRotationSpeedLabel, rotationSpeed_text); 
        lv_obj_set_style_text_font(gui.page.settings.filmRotationSpeedLabel, &lv_font_montserrat_20, 0);              
        lv_obj_align(gui.page.settings.filmRotationSpeedLabel, LV_ALIGN_TOP_LEFT, -5, -10);

        createQuestionMark(gui.page.settings.filmRotationSpeedContainer,gui.page.settings.filmRotationSpeedLabel,event_settingPopupMBox, 2, -3);
   
        gui.page.settings.filmRotationSpeedSlider = lv_slider_create(gui.page.settings.filmRotationSpeedContainer);
        lv_obj_align(gui.page.settings.filmRotationSpeedSlider, LV_ALIGN_TOP_LEFT, 0, 23);
        lv_obj_set_style_anim_duration(gui.page.settings.filmRotationSpeedSlider, 2000, 0);
        lv_obj_set_style_bg_color(gui.page.settings.filmRotationSpeedSlider,lv_color_hex(ORANGE) , LV_PART_KNOB);
        lv_obj_set_style_bg_color(gui.page.settings.filmRotationSpeedSlider,lv_color_hex(ORANGE_LIGHT) , LV_PART_INDICATOR);
        lv_obj_set_style_bg_color(gui.page.settings.filmRotationSpeedSlider, lv_palette_lighten(LV_PALETTE_GREY, 3), LV_PART_MAIN);
        lv_slider_set_value(gui.page.settings.filmRotationSpeedSlider, gui.page.settings.settingsParams.filmRotationSpeedSetpoint, LV_ANIM_OFF);
        lv_slider_set_range(gui.page.settings.filmRotationSpeedSlider, 20, 90);

  
        gui.page.settings.filmRotationSpeedValueLabel = lv_label_create(gui.page.settings.filmRotationSpeedContainer);
        lv_obj_set_style_text_font(gui.page.settings.filmRotationSpeedValueLabel, &lv_font_montserrat_22, 0);              
        lv_obj_align(gui.page.settings.filmRotationSpeedValueLabel, LV_ALIGN_TOP_RIGHT, 5, -10);
        lv_obj_add_event_cb(gui.page.settings.filmRotationSpeedSlider, event_settings_handler, LV_EVENT_CLICKED, gui.page.settings.filmRotationSpeedValueLabel);
        lv_obj_add_event_cb(gui.page.settings.filmRotationSpeedSlider, event_settings_handler, LV_EVENT_VALUE_CHANGED, gui.page.settings.filmRotationSpeedValueLabel);
        lv_obj_add_event_cb(gui.page.settings.filmRotationSpeedSlider, event_settings_handler, LV_EVENT_SHORT_CLICKED, gui.page.settings.filmRotationSpeedValueLabel);
        lv_obj_add_event_cb(gui.page.settings.filmRotationSpeedSlider, event_settings_handler, LV_EVENT_LONG_PRESSED_REPEAT, gui.page.settings.filmRotationSpeedValueLabel);
        lv_obj_add_event_cb(gui.page.settings.filmRotationSpeedSlider, event_settings_handler, LV_EVENT_RELEASED, gui.page.settings.filmRotationSpeedValueLabel);
        lv_label_set_text_fmt(gui.page.settings.filmRotationSpeedValueLabel, "%drpm", gui.page.settings.settingsParams.filmRotationSpeedSetpoint);



  gui.page.settings.filmRotationInverseIntervallContainer = lv_obj_create(gui.page.settings.settingsContainer);
  lv_obj_align(gui.page.settings.filmRotationInverseIntervallContainer, LV_ALIGN_TOP_LEFT, -15, 241);                         
  lv_obj_set_size(gui.page.settings.filmRotationInverseIntervallContainer, 330, 70); 
  lv_obj_remove_flag(gui.page.settings.filmRotationInverseIntervallContainer, LV_OBJ_FLAG_SCROLLABLE);    
  //lv_obj_set_style_border_color(gui.page.settings.filmRotationInverseIntervallContainer, lv_color_hex(LV_PALETTE_GREEN), 0);
  lv_obj_set_style_border_opa(gui.page.settings.filmRotationInverseIntervallContainer, LV_OPA_TRANSP, 0);
   
        gui.page.settings.filmRotationInverseIntervalLabel = lv_label_create(gui.page.settings.filmRotationInverseIntervallContainer);         
        lv_label_set_text(gui.page.settings.filmRotationInverseIntervalLabel, rotationInversionInterval_text); 
        lv_obj_set_style_text_font(gui.page.settings.filmRotationInverseIntervalLabel, &lv_font_montserrat_20, 0);              
        lv_obj_align(gui.page.settings.filmRotationInverseIntervalLabel, LV_ALIGN_TOP_LEFT, -5, -10);

        createQuestionMark(gui.page.settings.filmRotationInverseIntervallContainer,gui.page.settings.filmRotationInverseIntervalLabel,event_settingPopupMBox, 2, -3);
   
        gui.page.settings.filmRotationInversionIntervalSlider = lv_slider_create(gui.page.settings.filmRotationInverseIntervallContainer);
        lv_obj_align(gui.page.settings.filmRotationInversionIntervalSlider, LV_ALIGN_TOP_LEFT, 0, 23);
        lv_obj_set_style_anim_duration(gui.page.settings.filmRotationInversionIntervalSlider, 2000, 0);
        lv_obj_set_style_bg_color(gui.page.settings.filmRotationInversionIntervalSlider,lv_color_hex(ORANGE) , LV_PART_KNOB);
        lv_obj_set_style_bg_color(gui.page.settings.filmRotationInversionIntervalSlider,lv_color_hex(ORANGE_LIGHT) , LV_PART_INDICATOR);
        lv_obj_set_style_bg_color(gui.page.settings.filmRotationInversionIntervalSlider, lv_palette_lighten(LV_PALETTE_GREY, 3), LV_PART_MAIN);
        lv_slider_set_value(gui.page.settings.filmRotationInversionIntervalSlider, gui.page.settings.settingsParams.rotationIntervalSetpoint, LV_ANIM_OFF);
        lv_slider_set_range(gui.page.settings.filmRotationInversionIntervalSlider, 5, 60);

        gui.page.settings.filmRotationInverseIntervalValueLabel = lv_label_create(gui.page.settings.filmRotationInverseIntervallContainer);
        lv_obj_set_style_text_font(gui.page.settings.filmRotationInverseIntervalValueLabel, &lv_font_montserrat_22, 0);              
        lv_obj_align(gui.page.settings.filmRotationInverseIntervalValueLabel, LV_ALIGN_TOP_RIGHT, 5, -10);
        lv_obj_add_event_cb(gui.page.settings.filmRotationInversionIntervalSlider, event_settings_handler, LV_EVENT_CLICKED, gui.page.settings.filmRotationInverseIntervalValueLabel);
        lv_obj_add_event_cb(gui.page.settings.filmRotationInversionIntervalSlider, event_settings_handler, LV_EVENT_VALUE_CHANGED, gui.page.settings.filmRotationInverseIntervalValueLabel);
        lv_obj_add_event_cb(gui.page.settings.filmRotationInversionIntervalSlider, event_settings_handler, LV_EVENT_SHORT_CLICKED, gui.page.settings.filmRotationInverseIntervalValueLabel);
        lv_obj_add_event_cb(gui.page.settings.filmRotationInversionIntervalSlider, event_settings_handler, LV_EVENT_LONG_PRESSED, gui.page.settings.filmRotationInverseIntervalValueLabel);
        lv_obj_add_event_cb(gui.page.settings.filmRotationInversionIntervalSlider, event_settings_handler, LV_EVENT_RELEASED, gui.page.settings.filmRotationInverseIntervalValueLabel);
        lv_label_set_text_fmt(gui.page.settings.filmRotationInverseIntervalValueLabel, "%dsec%", gui.page.settings.settingsParams.rotationIntervalSetpoint);
        

  
  gui.page.settings.randomContainer = lv_obj_create(gui.page.settings.settingsContainer);
  lv_obj_align(gui.page.settings.randomContainer, LV_ALIGN_TOP_LEFT, -15, 303);                         
  lv_obj_set_size(gui.page.settings.randomContainer, 330, 70); 
  lv_obj_remove_flag(gui.page.settings.randomContainer, LV_OBJ_FLAG_SCROLLABLE);    
  //lv_obj_set_style_border_color(gui.page.settings.randomContainer, lv_color_hex(LV_PALETTE_GREEN), 0);
  lv_obj_set_style_border_opa(gui.page.settings.randomContainer, LV_OPA_TRANSP, 0);
   
        gui.page.settings.filmRotationRandomLabel = lv_label_create(gui.page.settings.randomContainer);         
        lv_label_set_text(gui.page.settings.filmRotationRandomLabel, rotationRandom_text); 
        lv_obj_set_style_text_font(gui.page.settings.filmRotationRandomLabel, &lv_font_montserrat_20, 0);              
        lv_obj_align(gui.page.settings.filmRotationRandomLabel, LV_ALIGN_TOP_LEFT, -5, -10);
   
        createQuestionMark(gui.page.settings.randomContainer,gui.page.settings.filmRotationRandomLabel,event_settingPopupMBox, 2, -3);

        gui.page.settings.filmRandomlSlider = lv_slider_create(gui.page.settings.randomContainer);
        lv_obj_align(gui.page.settings.filmRandomlSlider, LV_ALIGN_TOP_LEFT, 0, 23);
        lv_obj_set_style_anim_duration(gui.page.settings.filmRandomlSlider, 2000, 0);
        lv_obj_set_style_bg_color(gui.page.settings.filmRandomlSlider,lv_color_hex(ORANGE) , LV_PART_KNOB);
        lv_obj_set_style_bg_color(gui.page.settings.filmRandomlSlider,lv_color_hex(ORANGE_LIGHT) , LV_PART_INDICATOR);
        lv_obj_set_style_bg_color(gui.page.settings.filmRandomlSlider, lv_palette_lighten(LV_PALETTE_GREY, 3), LV_PART_MAIN);
        lv_slider_set_value(gui.page.settings.filmRandomlSlider, gui.page.settings.settingsParams.randomSetpoint, LV_ANIM_OFF);
        
        gui.page.settings.filmRotationRandomValueLabel = lv_label_create(gui.page.settings.randomContainer);
        lv_obj_set_style_text_font(gui.page.settings.filmRotationRandomValueLabel, &lv_font_montserrat_22, 0);              
        lv_obj_align(gui.page.settings.filmRotationRandomValueLabel, LV_ALIGN_TOP_RIGHT, 5, -10);
        lv_obj_add_event_cb(gui.page.settings.filmRandomlSlider, event_settings_handler, LV_EVENT_CLICKED, gui.page.settings.filmRotationRandomValueLabel);
        lv_obj_add_event_cb(gui.page.settings.filmRandomlSlider, event_settings_handler, LV_EVENT_VALUE_CHANGED, gui.page.settings.filmRotationRandomValueLabel);
        lv_obj_add_event_cb(gui.page.settings.filmRandomlSlider, event_settings_handler, LV_EVENT_SHORT_CLICKED, gui.page.settings.filmRotationRandomValueLabel);
        lv_obj_add_event_cb(gui.page.settings.filmRandomlSlider, event_settings_handler, LV_EVENT_LONG_PRESSED_REPEAT, gui.page.settings.filmRotationRandomValueLabel);
        lv_obj_add_event_cb(gui.page.settings.filmRandomlSlider, event_settings_handler, LV_EVENT_RELEASED, gui.page.settings.filmRotationRandomValueLabel);
        lv_label_set_text_fmt(gui.page.settings.filmRotationRandomValueLabel, "~%d%", gui.page.settings.settingsParams.randomSetpoint);


  gui.page.settings.persistentAlarmContainer = lv_obj_create(gui.page.settings.settingsContainer);
  lv_obj_align(gui.page.settings.persistentAlarmContainer, LV_ALIGN_TOP_LEFT, -15, 362);                         
  lv_obj_set_size(gui.page.settings.persistentAlarmContainer, 330, 60); 
  lv_obj_remove_flag(gui.page.settings.persistentAlarmContainer, LV_OBJ_FLAG_SCROLLABLE);    
  //lv_obj_set_style_border_color(gui.page.settings.persistentAlarmContainer, lv_color_hex(LV_PALETTE_GREEN), 0);
  lv_obj_set_style_border_opa(gui.page.settings.persistentAlarmContainer, LV_OPA_TRANSP, 0);

        gui.page.settings.persistentAlarmLabel = lv_label_create(gui.page.settings.persistentAlarmContainer);         
        lv_label_set_text(gui.page.settings.persistentAlarmLabel, persistentAlarm_text); 
        lv_obj_set_style_text_font(gui.page.settings.persistentAlarmLabel, &lv_font_montserrat_20, 0);              
        lv_obj_align(gui.page.settings.persistentAlarmLabel, LV_ALIGN_LEFT_MID, -5, 0);

        createQuestionMark(gui.page.settings.persistentAlarmContainer,gui.page.settings.persistentAlarmLabel,event_settingPopupMBox, 2, -3);

        gui.page.settings.persistentAlarmSwitch = lv_switch_create(gui.page.settings.persistentAlarmContainer);
        lv_obj_add_event_cb(gui.page.settings.persistentAlarmSwitch, event_settings_handler, LV_EVENT_VALUE_CHANGED, gui.page.settings.persistentAlarmSwitch);
        lv_obj_align(gui.page.settings.persistentAlarmSwitch, LV_ALIGN_RIGHT_MID, 0, 0);
        lv_obj_set_style_bg_color(gui.page.settings.persistentAlarmSwitch,  lv_palette_darken(LV_PALETTE_GREY, 3), LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(gui.page.settings.persistentAlarmSwitch,  lv_color_hex(ORANGE), LV_PART_KNOB | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(gui.page.settings.persistentAlarmSwitch,  lv_color_hex(ORANGE_DARK), LV_PART_INDICATOR | LV_STATE_CHECKED);
        lv_obj_add_state(gui.page.settings.persistentAlarmSwitch, gui.page.settings.settingsParams.isPersistentAlarm);


  gui.page.settings.autostartContainer = lv_obj_create(gui.page.settings.settingsContainer);
  lv_obj_align(gui.page.settings.autostartContainer, LV_ALIGN_TOP_LEFT, -15, 427);                         
  lv_obj_set_size(gui.page.settings.autostartContainer, 330, 60); 
  lv_obj_remove_flag(gui.page.settings.autostartContainer, LV_OBJ_FLAG_SCROLLABLE);    
  //lv_obj_set_style_border_color(gui.page.settings.autostartContainer, lv_color_hex(LV_PALETTE_GREEN), 0);
  lv_obj_set_style_border_opa(gui.page.settings.autostartContainer, LV_OPA_TRANSP, 0);

        gui.page.settings.autostartLabel = lv_label_create(gui.page.settings.autostartContainer);         
        lv_label_set_text(gui.page.settings.autostartLabel, autostart_text); 
        lv_obj_set_style_text_font(gui.page.settings.autostartLabel, &lv_font_montserrat_20, 0);              
        lv_obj_align(gui.page.settings.autostartLabel, LV_ALIGN_LEFT_MID, -5, 0);

        createQuestionMark(gui.page.settings.autostartContainer,gui.page.settings.autostartLabel,event_settingPopupMBox, 2, -3);

        gui.page.settings.autostartSwitch = lv_switch_create(gui.page.settings.autostartContainer);
        lv_obj_add_event_cb(gui.page.settings.autostartSwitch, event_settings_handler, LV_EVENT_VALUE_CHANGED, gui.page.settings.autostartSwitch);
        lv_obj_align(gui.page.settings.autostartSwitch, LV_ALIGN_RIGHT_MID, 0, 0);
        lv_obj_set_style_bg_color(gui.page.settings.autostartSwitch,  lv_palette_darken(LV_PALETTE_GREY, 3), LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(gui.page.settings.autostartSwitch,  lv_color_hex(ORANGE), LV_PART_KNOB | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(gui.page.settings.autostartSwitch,  lv_color_hex(ORANGE_DARK), LV_PART_INDICATOR | LV_STATE_CHECKED);
        lv_obj_add_state(gui.page.settings.autostartSwitch, gui.page.settings.settingsParams.isProcessAutostart);


  gui.page.settings.drainFillTimeContainer = lv_obj_create(gui.page.settings.settingsContainer);
  lv_obj_align(gui.page.settings.drainFillTimeContainer, LV_ALIGN_TOP_LEFT, -15, 499);                         
  lv_obj_set_size(gui.page.settings.drainFillTimeContainer, 330, 70); 
  lv_obj_remove_flag(gui.page.settings.drainFillTimeContainer, LV_OBJ_FLAG_SCROLLABLE);    
  //lv_obj_set_style_border_color(gui.page.settings.drainFillTimeContainer, lv_color_hex(LV_PALETTE_GREEN), 0);
  lv_obj_set_style_border_opa(gui.page.settings.drainFillTimeContainer, LV_OPA_TRANSP, 0);
   
        gui.page.settings.drainFillTimeLabel = lv_label_create(gui.page.settings.drainFillTimeContainer);         
        lv_label_set_text(gui.page.settings.drainFillTimeLabel, drainFillTime_text); 
        lv_obj_set_style_text_font(gui.page.settings.drainFillTimeLabel, &lv_font_montserrat_20, 0);              
        lv_obj_align(gui.page.settings.drainFillTimeLabel, LV_ALIGN_TOP_LEFT, -5, -10);

        createQuestionMark(gui.page.settings.drainFillTimeContainer,gui.page.settings.drainFillTimeLabel,event_settingPopupMBox, 2, -3);
   
        gui.page.settings.drainFillTimeSlider = lv_slider_create(gui.page.settings.drainFillTimeContainer);
        lv_obj_align(gui.page.settings.drainFillTimeSlider, LV_ALIGN_TOP_LEFT, 0, 23);
        lv_obj_set_style_anim_duration(gui.page.settings.drainFillTimeSlider, 2000, 0);
        lv_obj_set_style_bg_color(gui.page.settings.drainFillTimeSlider,lv_color_hex(ORANGE) , LV_PART_KNOB);
        lv_obj_set_style_bg_color(gui.page.settings.drainFillTimeSlider,lv_color_hex(ORANGE_LIGHT) , LV_PART_INDICATOR);
        lv_obj_set_style_bg_color(gui.page.settings.drainFillTimeSlider, lv_palette_lighten(LV_PALETTE_GREY, 3), LV_PART_MAIN);
        lv_slider_set_value(gui.page.settings.drainFillTimeSlider, gui.page.settings.settingsParams.drainFillOverlapSetpoint, LV_ANIM_OFF);


        gui.page.settings.drainFillTimeValueLabel = lv_label_create(gui.page.settings.drainFillTimeContainer);
        lv_obj_set_style_text_font(gui.page.settings.drainFillTimeValueLabel, &lv_font_montserrat_22, 0);              
        lv_obj_align(gui.page.settings.drainFillTimeValueLabel, LV_ALIGN_TOP_RIGHT, 5, -10);
        lv_obj_add_event_cb(gui.page.settings.drainFillTimeSlider, event_settings_handler, LV_EVENT_CLICKED, gui.page.settings.drainFillTimeValueLabel);
        lv_obj_add_event_cb(gui.page.settings.drainFillTimeSlider, event_settings_handler, LV_EVENT_VALUE_CHANGED, gui.page.settings.drainFillTimeValueLabel);
        lv_obj_add_event_cb(gui.page.settings.drainFillTimeSlider, event_settings_handler, LV_EVENT_SHORT_CLICKED, gui.page.settings.drainFillTimeValueLabel);
        lv_obj_add_event_cb(gui.page.settings.drainFillTimeSlider, event_settings_handler, LV_EVENT_LONG_PRESSED_REPEAT, gui.page.settings.drainFillTimeValueLabel);
        lv_obj_add_event_cb(gui.page.settings.drainFillTimeSlider, event_settings_handler, LV_EVENT_RELEASED, gui.page.settings.drainFillTimeValueLabel);
        lv_label_set_text_fmt(gui.page.settings.drainFillTimeValueLabel, "%d%%", gui.page.settings.settingsParams.drainFillOverlapSetpoint);


gui.page.settings.multiRinseTimeContainer = lv_obj_create(gui.page.settings.settingsContainer);
  lv_obj_align(gui.page.settings.multiRinseTimeContainer, LV_ALIGN_TOP_LEFT, -15, 561);                         
  lv_obj_set_size(gui.page.settings.multiRinseTimeContainer, 330, 70); 
  lv_obj_remove_flag(gui.page.settings.multiRinseTimeContainer, LV_OBJ_FLAG_SCROLLABLE);    
  //lv_obj_set_style_border_color(gui.page.settings.drainFillTimeContainer, lv_color_hex(LV_PALETTE_GREEN), 0);
  lv_obj_set_style_border_opa(gui.page.settings.multiRinseTimeContainer, LV_OPA_TRANSP, 0);
 // lv_obj_add_flag(gui.page.settings.multiRinseTimeContainer, LV_OBJ_FLAG_PRESS_LOCK);
 // lv_obj_add_flag(gui.page.settings.multiRinseTimeContainer, LV_OBJ_FLAG_CLICKABLE);

        gui.page.settings.multiRinseTimeLabel = lv_label_create(gui.page.settings.multiRinseTimeContainer);         
        lv_label_set_text(gui.page.settings.multiRinseTimeLabel, multiRinseTime_text); 
        lv_obj_set_style_text_font(gui.page.settings.multiRinseTimeLabel, &lv_font_montserrat_20, 0);              
        lv_obj_align(gui.page.settings.multiRinseTimeLabel, LV_ALIGN_TOP_LEFT, -5, -10);
   
        gui.page.settings.multiRinseTimeSlider = lv_slider_create(gui.page.settings.multiRinseTimeContainer);
        lv_obj_align(gui.page.settings.multiRinseTimeSlider, LV_ALIGN_TOP_LEFT, 0, 23);
        lv_obj_set_style_anim_duration(gui.page.settings.multiRinseTimeSlider, 2000, 0);
        lv_obj_set_style_bg_color(gui.page.settings.multiRinseTimeSlider,lv_color_hex(ORANGE) , LV_PART_KNOB);
        lv_obj_set_style_bg_color(gui.page.settings.multiRinseTimeSlider,lv_color_hex(ORANGE_LIGHT) , LV_PART_INDICATOR);
        lv_obj_set_style_bg_color(gui.page.settings.multiRinseTimeSlider, lv_palette_lighten(LV_PALETTE_GREY, 3), LV_PART_MAIN);
        lv_slider_set_value(gui.page.settings.multiRinseTimeSlider, gui.page.settings.settingsParams.multiRinseTime, LV_ANIM_OFF);
        lv_slider_set_range(gui.page.settings.multiRinseTimeSlider, 60, 180);
        //lv_obj_add_flag(gui.page.settings.multiRinseTimeSlider, LV_OBJ_FLAG_PRESS_LOCK);

        gui.page.settings.multiRinseTimeValueLabel = lv_label_create(gui.page.settings.multiRinseTimeContainer);
        lv_obj_set_style_text_font(gui.page.settings.multiRinseTimeValueLabel, &lv_font_montserrat_22, 0);              
        lv_obj_align(gui.page.settings.multiRinseTimeValueLabel, LV_ALIGN_TOP_RIGHT, 5, -10);
        lv_obj_add_event_cb(gui.page.settings.multiRinseTimeSlider, event_settings_handler, LV_EVENT_RELEASED, gui.page.settings.multiRinseTimeValueLabel);
        lv_label_set_text_fmt(gui.page.settings.multiRinseTimeValueLabel, "%ds", gui.page.settings.settingsParams.multiRinseTime);
      
}

void settings(void)
{   
  if(gui.page.settings.settingsSection == NULL){
    initSettings();
    lv_obj_add_flag(gui.page.settings.settingsSection, LV_OBJ_FLAG_HIDDEN);
  }

  
  lv_style_set_line_color(&gui.page.settings.style_sectionTitleLine, lv_palette_main(LV_PALETTE_ORANGE));
}

