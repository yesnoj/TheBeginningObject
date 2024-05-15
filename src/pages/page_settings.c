/**
 * @file page_settings.c
 *
 */


//ESSENTIAL INCLUDE
#include <lvgl.h>
#include "../../include/definitions.h"

extern struct gui_components gui;


//ACCESSORY INCLUDES

static lv_obj_t * settingsLabel;
static lv_obj_t * tempUnitLabel;
static lv_obj_t * waterInletLabel;
static lv_obj_t * tempSensorTuneButtonLabel;
static lv_obj_t * tempSensorTuningLabel;
static lv_obj_t * filmRotationSpeedLabel;
static lv_obj_t * filmRotationInverseIntervalLabel;
static lv_obj_t * filmRotationRandomLabel;
static lv_obj_t * persistentAlarmLabel;
static lv_obj_t * autostartLabel;
static lv_obj_t * drainFillTimeLabel;

static lv_obj_t * drainFillTimeValueLabel;
static lv_obj_t * filmRotationInverseIntervalValueLabel;
static lv_obj_t * filmRotationRandomValueLabel;
static lv_obj_t * filmRotationSpeedValueLabel;

static lv_obj_t * settingsContainer;
static lv_obj_t * tempUnitContainer;
static lv_obj_t * waterInletContainer;
static lv_obj_t * tempTuningContainer;
static lv_obj_t * filmRotationSpeedContainer;
static lv_obj_t * filmRotationInverseIntervallContainer;
static lv_obj_t * randomContainer;
static lv_obj_t * persistentAlarmContainer;
static lv_obj_t * autostartContainer;
static lv_obj_t * drainFillTimeContainer;

static lv_obj_t * autostartSwitch;
static lv_obj_t * persistentAlarmSwitch;
static lv_obj_t * waterInletSwitch;

static lv_obj_t * filmRotationSpeedSlider;
static lv_obj_t * filmRotationInversionIntervalSlider;
static lv_obj_t * filmRandomlSlider;
static lv_obj_t * drainFillTimeSlider;


static lv_obj_t * tempUnitCelsiusRadioButton;
static lv_obj_t * tempUnitFahrenheitRadioButton;

static uint32_t active_index = 0;

void event_settings_style_delete(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_DELETE) {
        //list of all styles to be reset, so clean the memory.
        lv_style_reset(&style_sectionTitleLine);

    }
}

void event_settingPopupMBox(lv_event_t * e){
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t * obj = (lv_obj_t *)lv_event_get_target(e);
  lv_obj_t * objCont = (lv_obj_t *)lv_obj_get_parent(obj);
  lv_obj_t * mboxCont = (lv_obj_t *)lv_obj_get_parent(objCont);
  lv_obj_t * data = (lv_obj_t *)lv_event_get_user_data(e);
  
  if(data == tempSensorTuningLabel){
    messagePopupCreate(messagePopupDetailTitle_text,tempAlertMBox_text,NULL,NULL,NULL);
  }
  if(data == filmRotationSpeedLabel){
    messagePopupCreate(messagePopupDetailTitle_text,filmRotationSpeedAlertMBox_text,NULL,NULL,NULL);
  }
  if(data == filmRotationInverseIntervalLabel){
    messagePopupCreate(messagePopupDetailTitle_text,rotationInverseIntervalAlertMBox_text,NULL,NULL,NULL);
  }
  if(data == filmRotationRandomLabel){
    messagePopupCreate(messagePopupDetailTitle_text,filmRotationRandomAlertMBox_text,NULL,NULL,NULL);
  }
  if(data == persistentAlarmLabel){
    messagePopupCreate(messagePopupDetailTitle_text,soundAlertMBox_text,NULL,NULL,NULL);
  }
  if(data == autostartLabel){
    messagePopupCreate(messagePopupDetailTitle_text,autostartAlertMBox_text,NULL,NULL,NULL);
  }
  if(data == drainFillTimeLabel){
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

    if(act_cb == cont && cont != waterInletSwitch && cont != tempSensorTuneButton && cont != filmRotationSpeedSlider && cont != filmRotationInversionIntervalSlider && cont != filmRandomlSlider && cont != persistentAlarmSwitch && cont != autostartSwitch && cont != drainFillTimeSlider) 
    //if(act_cb == cont) 
      return;

    if(act_cb == tempUnitCelsiusRadioButton || act_cb == tempUnitFahrenheitRadioButton){
       if(code == LV_EVENT_CLICKED) {
            lv_obj_remove_state(old_cb, LV_STATE_CHECKED);
            lv_obj_add_state(act_cb, LV_STATE_CHECKED); 

            *active_id = lv_obj_get_index(act_cb);

            LV_LOG_USER("Selected °C or °F: %d", (int)active_index);
       }
    }

    if(act_cb == waterInletSwitch){
      if(code == LV_EVENT_VALUE_CHANGED) {
          LV_LOG_USER("State Inlet: %s", lv_obj_has_state(act_cb, LV_STATE_CHECKED) ? "On" : "Off");
        }
    }

    if(act_cb == tempSensorTuneButton){
      if(code == LV_EVENT_SHORT_CLICKED) {
          LV_LOG_USER("TUNE short click");
          rollerPopupCreate(tempCelsiusOptions,tuneTempPopupTitle_text,tempSensorTuneButton);
        }
      if(code == LV_EVENT_LONG_PRESSED_REPEAT) {
          LV_LOG_USER("TUNE Long click");
        }
    }

    if(act_cb == filmRotationSpeedSlider){
      if(code == LV_EVENT_VALUE_CHANGED) {
          lv_label_set_text_fmt((lv_obj_t*)lv_event_get_user_data(e), "%d%%", lv_slider_get_value(act_cb));
          LV_LOG_USER("Film Speed Rotation : %d",lv_slider_get_value(act_cb));
        }
    }

    if(act_cb == filmRotationInversionIntervalSlider){
      if(code == LV_EVENT_VALUE_CHANGED) {
          lv_label_set_text_fmt((lv_obj_t*)lv_event_get_user_data(e), "%dsec%", lv_slider_get_value(act_cb));
          LV_LOG_USER("Film Rotation Inversion Interval : %d",lv_slider_get_value(act_cb));
        }
    }

    if(act_cb == filmRandomlSlider){
      if(code == LV_EVENT_VALUE_CHANGED) {
        lv_label_set_text_fmt((lv_obj_t*)lv_event_get_user_data(e), "%~%d%%", lv_slider_get_value(act_cb));
        LV_LOG_USER("Film Randomness : %d",lv_slider_get_value(act_cb));
        }
    }

    if(act_cb == persistentAlarmSwitch){
      if(code == LV_EVENT_VALUE_CHANGED) {
          LV_LOG_USER("Persistent Alarm: %s", lv_obj_has_state(act_cb, LV_STATE_CHECKED) ? "On" : "Off");
        }
    }

    if(act_cb == autostartSwitch){
      if(code == LV_EVENT_VALUE_CHANGED) {
          LV_LOG_USER("Autostart : %s", lv_obj_has_state(act_cb, LV_STATE_CHECKED) ? "On" : "Off");
        }
    }
    
    if(act_cb == drainFillTimeSlider){
      if(code == LV_EVENT_VALUE_CHANGED) {
          lv_label_set_text_fmt((lv_obj_t*)lv_event_get_user_data(e), "%d%%", lv_slider_get_value(act_cb));
          LV_LOG_USER("Drain/fill time overlap percent : %d",lv_slider_get_value(act_cb));
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
  settingsSection = lv_obj_create(lv_screen_active());
  lv_obj_set_pos(settingsSection, 140, 7);                            
  lv_obj_set_size(settingsSection, 335, 303); 
  lv_obj_remove_flag(settingsSection, LV_OBJ_FLAG_SCROLLABLE);    
  lv_obj_set_style_border_color(settingsSection, lv_color_hex(ORANGE_DARK), 0);


	lv_coord_t pad = 2;
	lv_obj_set_style_pad_left(settingsSection, pad, LV_PART_INDICATOR);
	lv_obj_set_style_pad_right(settingsSection, pad, LV_PART_INDICATOR);
	lv_obj_set_style_pad_top(settingsSection, pad, LV_PART_INDICATOR);
	lv_obj_set_style_pad_bottom(settingsSection, pad, LV_PART_INDICATOR);


 
  settingsLabel = lv_label_create(settingsSection);         
  lv_label_set_text(settingsLabel, Settings_text); 
  lv_obj_set_style_text_font(settingsLabel, &lv_font_montserrat_28, 0);              
  lv_obj_align(settingsLabel, LV_ALIGN_TOP_LEFT, -7, -5);

  /*Create style*/
  lv_style_init(&style_sectionTitleLine);
  lv_style_set_line_width(&style_sectionTitleLine, 2);
  lv_style_set_line_color(&style_sectionTitleLine, lv_palette_main(LV_PALETTE_ORANGE));
  lv_style_set_line_rounded(&style_sectionTitleLine, true);

  /*Create a line and apply the new style*/
  sectionTitleLine = lv_line_create(settingsSection);
  lv_line_set_points(sectionTitleLine, sectionTitleLine_points, 2);
  lv_obj_add_style(sectionTitleLine, &style_sectionTitleLine, 0);
  lv_obj_align(sectionTitleLine, LV_ALIGN_CENTER, 0, - 103);

  lv_obj_update_layout(settingsSection);

  /*********************
 *    PAGE ELEMENTS
 *********************/

  settingsContainer = lv_obj_create(settingsSection);
  lv_obj_set_pos(settingsContainer, -22, 31);                           
  lv_obj_set_size(settingsContainer, 335, 254);  
  //lv_obj_set_style_border_color(settingsContainer, lv_color_hex(GREEN_DARK), 0);
  lv_obj_set_style_border_opa(settingsContainer, LV_OPA_TRANSP, 0);
  lv_obj_set_scroll_dir(settingsContainer, LV_DIR_VER);

  tempUnitContainer = lv_obj_create(settingsContainer);
  lv_obj_align(tempUnitContainer, LV_ALIGN_TOP_LEFT, -15, -17);                         
  lv_obj_set_size(tempUnitContainer, 330, 60); 
  lv_obj_remove_flag(tempUnitContainer, LV_OBJ_FLAG_SCROLLABLE);    
  //lv_obj_set_style_border_color(tempUnitContainer, lv_color_hex(LV_PALETTE_GREEN), 0);
  lv_obj_set_style_border_opa(tempUnitContainer, LV_OPA_TRANSP, 0);
  lv_obj_add_event_cb(tempUnitContainer, event_settings_handler, LV_EVENT_CLICKED, &active_index);

        tempUnitCelsiusRadioButton = create_radiobutton(tempUnitContainer, celsius_text, -55, 0, 27, &lv_font_montserrat_18, lv_color_hex(ORANGE_DARK), lv_color_hex(ORANGE));
        tempUnitFahrenheitRadioButton = create_radiobutton(tempUnitContainer, fahrenheit_text, 5, 0, 27, &lv_font_montserrat_18, lv_color_hex(ORANGE_DARK), lv_color_hex(ORANGE));

        //Make the first checkbox checked
        lv_obj_add_state(lv_obj_get_child(tempUnitContainer, 0), LV_STATE_CHECKED);
           
        tempUnitLabel = lv_label_create(tempUnitContainer);         
        lv_label_set_text(tempUnitLabel, tempUnit_text); 
        lv_obj_set_style_text_font(tempUnitLabel, &lv_font_montserrat_20, 0);              
        lv_obj_align(tempUnitLabel, LV_ALIGN_LEFT_MID, -5, 0);


  waterInletContainer = lv_obj_create(settingsContainer);
  lv_obj_align(waterInletContainer, LV_ALIGN_TOP_LEFT, -15, 45);                         
  lv_obj_set_size(waterInletContainer, 330, 60); 
  lv_obj_remove_flag(waterInletContainer, LV_OBJ_FLAG_SCROLLABLE);    
  //lv_obj_set_style_border_color(waterInletContainer, lv_color_hex(LV_PALETTE_GREEN), 0);
  lv_obj_set_style_border_opa(waterInletContainer, LV_OPA_TRANSP, 0);

        waterInletLabel = lv_label_create(waterInletContainer);         
        lv_label_set_text(waterInletLabel, waterInlet_text); 
        lv_obj_set_style_text_font(waterInletLabel, &lv_font_montserrat_20, 0);              
        lv_obj_align(waterInletLabel, LV_ALIGN_LEFT_MID, -5, 0);

        waterInletSwitch = lv_switch_create(waterInletContainer);
        lv_obj_add_event_cb(waterInletSwitch, event_settings_handler, LV_EVENT_VALUE_CHANGED, waterInletSwitch);
        lv_obj_align(waterInletSwitch, LV_ALIGN_RIGHT_MID, 0, 0);
        lv_obj_set_style_bg_color(waterInletSwitch,  lv_palette_darken(LV_PALETTE_GREY, 3), LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(waterInletSwitch,  lv_color_hex(ORANGE), LV_PART_KNOB | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(waterInletSwitch,  lv_color_hex(ORANGE_DARK), LV_PART_INDICATOR | LV_STATE_CHECKED);



  tempTuningContainer = lv_obj_create(settingsContainer);
  lv_obj_align(tempTuningContainer, LV_ALIGN_TOP_LEFT, -15, 107);                         
  lv_obj_set_size(tempTuningContainer, 330, 60); 
  lv_obj_remove_flag(tempTuningContainer, LV_OBJ_FLAG_SCROLLABLE);    
  //lv_obj_set_style_border_color(tempTuningContainer, lv_color_hex(LV_PALETTE_GREEN), 0);
  lv_obj_set_style_border_opa(tempTuningContainer, LV_OPA_TRANSP, 0);

        tempSensorTuningLabel = lv_label_create(tempTuningContainer);         
        lv_label_set_text(tempSensorTuningLabel, tempSensorTuning_text); 
        lv_obj_set_style_text_font(tempSensorTuningLabel, &lv_font_montserrat_20, 0);              
        lv_obj_align(tempSensorTuningLabel, LV_ALIGN_LEFT_MID, -5, 0);

        createQuestionMark(tempTuningContainer,tempSensorTuningLabel,event_settingPopupMBox, 2, -3);

        tempSensorTuneButton = lv_button_create(tempTuningContainer);
        lv_obj_set_size(tempSensorTuneButton, BUTTON_TUNE_WIDTH, BUTTON_TUNE_HEIGHT);
        lv_obj_align(tempSensorTuneButton, LV_ALIGN_RIGHT_MID, 5 , 0);
        lv_obj_add_event_cb(tempSensorTuneButton, event_settings_handler, LV_EVENT_ALL, tempSensorTuneButton);
        lv_obj_set_style_bg_color(tempSensorTuneButton, lv_color_hex(ORANGE), LV_PART_MAIN);

        tempSensorTuneButtonLabel = lv_label_create(tempSensorTuneButton);
        lv_label_set_text(tempSensorTuneButtonLabel, tuneButton_text);
        lv_obj_set_style_text_font(tempSensorTuneButtonLabel, &lv_font_montserrat_18, 0);
        lv_obj_align(tempSensorTuneButtonLabel, LV_ALIGN_CENTER, 0, 0);

  filmRotationSpeedContainer = lv_obj_create(settingsContainer);
  lv_obj_align(filmRotationSpeedContainer, LV_ALIGN_TOP_LEFT, -15, 179);                         
  lv_obj_set_size(filmRotationSpeedContainer, 330, 70); 
  lv_obj_remove_flag(filmRotationSpeedContainer, LV_OBJ_FLAG_SCROLLABLE);    
  //lv_obj_set_style_border_color(filmRotationSpeedContainer, lv_color_hex(LV_PALETTE_GREEN), 0);
  lv_obj_set_style_border_opa(filmRotationSpeedContainer, LV_OPA_TRANSP, 0);
   
        filmRotationSpeedLabel = lv_label_create(filmRotationSpeedContainer);         
        lv_label_set_text(filmRotationSpeedLabel, rotationSpeed_text); 
        lv_obj_set_style_text_font(filmRotationSpeedLabel, &lv_font_montserrat_20, 0);              
        lv_obj_align(filmRotationSpeedLabel, LV_ALIGN_TOP_LEFT, -5, -10);

        createQuestionMark(filmRotationSpeedContainer,filmRotationSpeedLabel,event_settingPopupMBox, 2, -3);
   
        filmRotationSpeedSlider = lv_slider_create(filmRotationSpeedContainer);
        lv_obj_align(filmRotationSpeedSlider, LV_ALIGN_TOP_LEFT, 0, 23);
        lv_obj_set_style_anim_duration(filmRotationSpeedSlider, 2000, 0);
        lv_obj_set_style_bg_color(filmRotationSpeedSlider,lv_color_hex(ORANGE) , LV_PART_KNOB);
        lv_obj_set_style_bg_color(filmRotationSpeedSlider,lv_color_hex(ORANGE_LIGHT) , LV_PART_INDICATOR);
        lv_obj_set_style_bg_color(filmRotationSpeedSlider, lv_palette_lighten(LV_PALETTE_GREY, 3), LV_PART_MAIN);
        
        filmRotationSpeedValueLabel = lv_label_create(filmRotationSpeedContainer);
        lv_obj_set_style_text_font(filmRotationSpeedValueLabel, &lv_font_montserrat_22, 0);              
        lv_label_set_text(filmRotationSpeedValueLabel, "0%");
        lv_obj_align(filmRotationSpeedValueLabel, LV_ALIGN_TOP_RIGHT, 5, -10);
        lv_obj_add_event_cb(filmRotationSpeedSlider, event_settings_handler, LV_EVENT_VALUE_CHANGED, filmRotationSpeedValueLabel);



  filmRotationInverseIntervallContainer = lv_obj_create(settingsContainer);
  lv_obj_align(filmRotationInverseIntervallContainer, LV_ALIGN_TOP_LEFT, -15, 241);                         
  lv_obj_set_size(filmRotationInverseIntervallContainer, 330, 70); 
  lv_obj_remove_flag(filmRotationInverseIntervallContainer, LV_OBJ_FLAG_SCROLLABLE);    
  //lv_obj_set_style_border_color(filmRotationInverseIntervallContainer, lv_color_hex(LV_PALETTE_GREEN), 0);
  lv_obj_set_style_border_opa(filmRotationInverseIntervallContainer, LV_OPA_TRANSP, 0);
   
        filmRotationInverseIntervalLabel = lv_label_create(filmRotationInverseIntervallContainer);         
        lv_label_set_text(filmRotationInverseIntervalLabel, rotationInversionInterval_text); 
        lv_obj_set_style_text_font(filmRotationInverseIntervalLabel, &lv_font_montserrat_20, 0);              
        lv_obj_align(filmRotationInverseIntervalLabel, LV_ALIGN_TOP_LEFT, -5, -10);

        createQuestionMark(filmRotationInverseIntervallContainer,filmRotationInverseIntervalLabel,event_settingPopupMBox, 2, -3);
   
        filmRotationInversionIntervalSlider = lv_slider_create(filmRotationInverseIntervallContainer);
        lv_obj_align(filmRotationInversionIntervalSlider, LV_ALIGN_TOP_LEFT, 0, 23);
        lv_obj_set_style_anim_duration(filmRotationInversionIntervalSlider, 2000, 0);
        lv_obj_set_style_bg_color(filmRotationInversionIntervalSlider,lv_color_hex(ORANGE) , LV_PART_KNOB);
        lv_obj_set_style_bg_color(filmRotationInversionIntervalSlider,lv_color_hex(ORANGE_LIGHT) , LV_PART_INDICATOR);
        lv_obj_set_style_bg_color(filmRotationInversionIntervalSlider, lv_palette_lighten(LV_PALETTE_GREY, 3), LV_PART_MAIN);
        
        filmRotationInverseIntervalValueLabel = lv_label_create(filmRotationInverseIntervallContainer);
        lv_obj_set_style_text_font(filmRotationInverseIntervalValueLabel, &lv_font_montserrat_22, 0);              
        lv_label_set_text(filmRotationInverseIntervalValueLabel, "0sec");
        lv_obj_align(filmRotationInverseIntervalValueLabel, LV_ALIGN_TOP_RIGHT, 5, -10);
        lv_obj_add_event_cb(filmRotationInversionIntervalSlider, event_settings_handler, LV_EVENT_VALUE_CHANGED, filmRotationInverseIntervalValueLabel);

  
  randomContainer = lv_obj_create(settingsContainer);
  lv_obj_align(randomContainer, LV_ALIGN_TOP_LEFT, -15, 303);                         
  lv_obj_set_size(randomContainer, 330, 70); 
  lv_obj_remove_flag(randomContainer, LV_OBJ_FLAG_SCROLLABLE);    
  //lv_obj_set_style_border_color(randomContainer, lv_color_hex(LV_PALETTE_GREEN), 0);
  lv_obj_set_style_border_opa(randomContainer, LV_OPA_TRANSP, 0);
   
        filmRotationRandomLabel = lv_label_create(randomContainer);         
        lv_label_set_text(filmRotationRandomLabel, rotationRandom_text); 
        lv_obj_set_style_text_font(filmRotationRandomLabel, &lv_font_montserrat_20, 0);              
        lv_obj_align(filmRotationRandomLabel, LV_ALIGN_TOP_LEFT, -5, -10);
   
        createQuestionMark(randomContainer,filmRotationRandomLabel,event_settingPopupMBox, 2, -3);

        filmRandomlSlider = lv_slider_create(randomContainer);
        lv_obj_align(filmRandomlSlider, LV_ALIGN_TOP_LEFT, 0, 23);
        lv_obj_set_style_anim_duration(filmRandomlSlider, 2000, 0);
        lv_obj_set_style_bg_color(filmRandomlSlider,lv_color_hex(ORANGE) , LV_PART_KNOB);
        lv_obj_set_style_bg_color(filmRandomlSlider,lv_color_hex(ORANGE_LIGHT) , LV_PART_INDICATOR);
        lv_obj_set_style_bg_color(filmRandomlSlider, lv_palette_lighten(LV_PALETTE_GREY, 3), LV_PART_MAIN);
        
        filmRotationRandomValueLabel = lv_label_create(randomContainer);
        lv_obj_set_style_text_font(filmRotationRandomValueLabel, &lv_font_montserrat_22, 0);              
        lv_label_set_text(filmRotationRandomValueLabel, "~0%");
        lv_obj_align(filmRotationRandomValueLabel, LV_ALIGN_TOP_RIGHT, 5, -10);
        lv_obj_add_event_cb(filmRandomlSlider, event_settings_handler, LV_EVENT_VALUE_CHANGED, filmRotationRandomValueLabel);

  persistentAlarmContainer = lv_obj_create(settingsContainer);
  lv_obj_align(persistentAlarmContainer, LV_ALIGN_TOP_LEFT, -15, 362);                         
  lv_obj_set_size(persistentAlarmContainer, 330, 60); 
  lv_obj_remove_flag(persistentAlarmContainer, LV_OBJ_FLAG_SCROLLABLE);    
  //lv_obj_set_style_border_color(persistentAlarmContainer, lv_color_hex(LV_PALETTE_GREEN), 0);
  lv_obj_set_style_border_opa(persistentAlarmContainer, LV_OPA_TRANSP, 0);

        persistentAlarmLabel = lv_label_create(persistentAlarmContainer);         
        lv_label_set_text(persistentAlarmLabel, persistentAlarm_text); 
        lv_obj_set_style_text_font(persistentAlarmLabel, &lv_font_montserrat_20, 0);              
        lv_obj_align(persistentAlarmLabel, LV_ALIGN_LEFT_MID, -5, 0);

        createQuestionMark(persistentAlarmContainer,persistentAlarmLabel,event_settingPopupMBox, 2, -3);

        persistentAlarmSwitch = lv_switch_create(persistentAlarmContainer);
        lv_obj_add_event_cb(persistentAlarmSwitch, event_settings_handler, LV_EVENT_VALUE_CHANGED, persistentAlarmSwitch);
        lv_obj_align(persistentAlarmSwitch, LV_ALIGN_RIGHT_MID, 0, 0);
        lv_obj_set_style_bg_color(persistentAlarmSwitch,  lv_palette_darken(LV_PALETTE_GREY, 3), LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(persistentAlarmSwitch,  lv_color_hex(ORANGE), LV_PART_KNOB | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(persistentAlarmSwitch,  lv_color_hex(ORANGE_DARK), LV_PART_INDICATOR | LV_STATE_CHECKED);


  autostartContainer = lv_obj_create(settingsContainer);
  lv_obj_align(autostartContainer, LV_ALIGN_TOP_LEFT, -15, 427);                         
  lv_obj_set_size(autostartContainer, 330, 60); 
  lv_obj_remove_flag(autostartContainer, LV_OBJ_FLAG_SCROLLABLE);    
  //lv_obj_set_style_border_color(autostartContainer, lv_color_hex(LV_PALETTE_GREEN), 0);
  lv_obj_set_style_border_opa(autostartContainer, LV_OPA_TRANSP, 0);

        autostartLabel = lv_label_create(autostartContainer);         
        lv_label_set_text(autostartLabel, autostart_text); 
        lv_obj_set_style_text_font(autostartLabel, &lv_font_montserrat_20, 0);              
        lv_obj_align(autostartLabel, LV_ALIGN_LEFT_MID, -5, 0);

        createQuestionMark(autostartContainer,autostartLabel,event_settingPopupMBox, 2, -3);

        autostartSwitch = lv_switch_create(autostartContainer);
        lv_obj_add_event_cb(autostartSwitch, event_settings_handler, LV_EVENT_VALUE_CHANGED, autostartSwitch);
        lv_obj_align(autostartSwitch, LV_ALIGN_RIGHT_MID, 0, 0);
        lv_obj_set_style_bg_color(autostartSwitch,  lv_palette_darken(LV_PALETTE_GREY, 3), LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(autostartSwitch,  lv_color_hex(ORANGE), LV_PART_KNOB | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_color(autostartSwitch,  lv_color_hex(ORANGE_DARK), LV_PART_INDICATOR | LV_STATE_CHECKED);


  drainFillTimeContainer = lv_obj_create(settingsContainer);
  lv_obj_align(drainFillTimeContainer, LV_ALIGN_TOP_LEFT, -15, 499);                         
  lv_obj_set_size(drainFillTimeContainer, 330, 70); 
  lv_obj_remove_flag(drainFillTimeContainer, LV_OBJ_FLAG_SCROLLABLE);    
  //lv_obj_set_style_border_color(drainFillTimeContainer, lv_color_hex(LV_PALETTE_GREEN), 0);
  lv_obj_set_style_border_opa(drainFillTimeContainer, LV_OPA_TRANSP, 0);
   
        drainFillTimeLabel = lv_label_create(drainFillTimeContainer);         
        lv_label_set_text(drainFillTimeLabel, drainFillTime_text); 
        lv_obj_set_style_text_font(drainFillTimeLabel, &lv_font_montserrat_20, 0);              
        lv_obj_align(drainFillTimeLabel, LV_ALIGN_TOP_LEFT, -5, -10);

        createQuestionMark(drainFillTimeContainer,drainFillTimeLabel,event_settingPopupMBox, 2, -3);
   
        drainFillTimeSlider = lv_slider_create(drainFillTimeContainer);
        lv_obj_align(drainFillTimeSlider, LV_ALIGN_TOP_LEFT, 0, 23);
        lv_obj_set_style_anim_duration(drainFillTimeSlider, 2000, 0);
        lv_obj_set_style_bg_color(drainFillTimeSlider,lv_color_hex(ORANGE) , LV_PART_KNOB);
        lv_obj_set_style_bg_color(drainFillTimeSlider,lv_color_hex(ORANGE_LIGHT) , LV_PART_INDICATOR);
        lv_obj_set_style_bg_color(drainFillTimeSlider, lv_palette_lighten(LV_PALETTE_GREY, 3), LV_PART_MAIN);
        
        drainFillTimeValueLabel = lv_label_create(drainFillTimeContainer);
        lv_obj_set_style_text_font(drainFillTimeValueLabel, &lv_font_montserrat_22, 0);              
        lv_label_set_text(drainFillTimeValueLabel, "0%");
        lv_obj_align(drainFillTimeValueLabel, LV_ALIGN_TOP_RIGHT, 5, -10);
        lv_obj_add_event_cb(drainFillTimeSlider, event_settings_handler, LV_EVENT_VALUE_CHANGED, drainFillTimeValueLabel);
}

void settings(void)
{   
  if(settingsSection == NULL){
    initSettings();
  }

  lv_obj_clear_flag(settingsSection, LV_OBJ_FLAG_HIDDEN);
  lv_style_set_line_color(&style_sectionTitleLine, lv_palette_main(LV_PALETTE_ORANGE));

}

