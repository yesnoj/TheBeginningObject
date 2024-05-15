/**
 * @file page_menu.c
 *
 */


//ESSENTIAL INCLUDES
#include <lvgl.h>
#include "../../include/definitions.h"


extern struct gui_components gui;


//ACCESSORY INCLUDES

#define TAB_PROCESSES 1
#define TAB_SETTINGS  2
#define TAB_TOOLS     3

static uint8_t oldSelection = 0;
static uint8_t newSelection = 0;

static lv_obj_t * processesTab;
static lv_obj_t * settingsTab;
static lv_obj_t * toolsTab;

static lv_obj_t * iconLabel;
static lv_obj_t * label;

lv_obj_t * root_page;

void event_tab_switch(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);

  if(code == LV_EVENT_CLICKED) {
    if(newTabSelected == NULL && oldTabSelected == NULL){
      newTabSelected = oldTabSelected = (lv_obj_t *)lv_event_get_target(e);
      newSelection = oldSelection = (uint8_t)lv_obj_get_index(newTabSelected);
      lv_obj_set_style_bg_color(newTabSelected, lv_color_hex(GREEN_DARK), 0);

      LV_LOG_USER("FIRST TIME");
    }
    else {
        oldTabSelected = newTabSelected;
        oldSelection = newSelection;
        newTabSelected = (lv_obj_t *)lv_event_get_target(e);
        newSelection = (uint8_t)lv_obj_get_index(newTabSelected);
        LV_LOG_USER("NEXT TIME");
    }
    
  
    if(newSelection != oldSelection) 
      { 
        //FIRST HIDE THE CORRECT SECTION
        if(oldSelection == TAB_PROCESSES){
            lv_obj_add_flag(processesSection, LV_OBJ_FLAG_HIDDEN);
        }
        if(oldSelection == TAB_SETTINGS){
            lv_obj_add_flag(settingsSection, LV_OBJ_FLAG_HIDDEN);
        }
        if(oldSelection == TAB_TOOLS){
            lv_obj_add_flag(toolsSection, LV_OBJ_FLAG_HIDDEN);
        }

        //THEN SHOW THE NEW SECTION
        if(newSelection == TAB_PROCESSES){
            lv_obj_set_style_bg_color(newTabSelected, lv_color_hex(GREEN_DARK), 0);
            lv_obj_remove_local_style_prop(oldTabSelected, LV_STYLE_BG_COLOR, 0);
            
            processes();
        }
        if(newSelection == TAB_SETTINGS){
            lv_obj_set_style_bg_color(newTabSelected, lv_color_hex(ORANGE_DARK), 0);
            lv_obj_remove_local_style_prop(oldTabSelected, LV_STYLE_BG_COLOR, 0); 

            settings();
        }
        if(newSelection == TAB_TOOLS){
            lv_obj_set_style_bg_color(newTabSelected, lv_color_hex(BLUE_DARK), 0);
            lv_obj_remove_local_style_prop(oldTabSelected, LV_STYLE_BG_COLOR, 0); 

            tools();
        }
      }
    
    LV_LOG_USER("OLD PRESSED %d",(int*)lv_obj_get_index(oldTabSelected));
    LV_LOG_USER("NEW PRESSED %d",(int*)lv_obj_get_index(newTabSelected));
     
  }   
}    


void menu(void){
    lv_obj_del(lv_screen_active());
    screen_mainMenu = lv_obj_create(NULL);
    lv_scr_load(screen_mainMenu);

    tempCelsiusOptions = createRollerValues(40,"");
    minutesOptions = createRollerValues(240,"");
    secondsOptions = createRollerValues(60,""); 
    tempCelsiusToleranceOptions = createRollerValues(5,"0.");

    processes();


    processesTab = lv_obj_create(screen_mainMenu);
    lv_obj_set_pos(processesTab, 5, 7);                            
    lv_obj_set_size(processesTab, 130, 97);   
    lv_obj_add_event_cb(processesTab, event_tab_switch, LV_EVENT_CLICKED, processesTab);
    lv_obj_set_style_border_color(processesTab, lv_color_hex(GREY), 0);
    lv_obj_set_style_border_opa(processesTab, LV_OPA_50, 0);
    lv_obj_remove_flag(processesTab, LV_OBJ_FLAG_SCROLLABLE);      

          iconLabel = lv_label_create(processesTab);          
          lv_label_set_text(iconLabel, tabProcess_icon);                  
          lv_obj_set_style_text_font(iconLabel, &FilMachineFontIcons_40, 0);
          lv_obj_align(iconLabel, LV_ALIGN_CENTER, 0, -10);


          label = lv_label_create(processesTab);         
          lv_label_set_text(label, tabProcess_label); 
          lv_obj_set_style_text_font(label, &lv_font_montserrat_18, 0);              
          lv_obj_align(label, LV_ALIGN_CENTER, 0, 29);
          lv_obj_send_event(processesTab, LV_EVENT_CLICKED, processesTab);



    settingsTab = lv_obj_create(screen_mainMenu);
    lv_obj_set_pos(settingsTab, 5, 7 + 6 + 97);                            
    lv_obj_set_size(settingsTab, 130, 97);   
    lv_obj_add_event_cb(settingsTab, event_tab_switch, LV_EVENT_CLICKED, settingsTab);
    lv_obj_set_style_border_color(settingsTab, lv_color_hex(GREY), 0);
    lv_obj_set_style_border_opa(settingsTab, LV_OPA_50, 0);
    lv_obj_remove_flag(settingsTab, LV_OBJ_FLAG_SCROLLABLE);      

          iconLabel = lv_label_create(settingsTab);          
          lv_label_set_text(iconLabel, tabSetting_icon);                  
          lv_obj_set_style_text_font(iconLabel, &FilMachineFontIcons_40, 0);
          lv_obj_align(iconLabel, LV_ALIGN_CENTER, 0, -10);

          label = lv_label_create(settingsTab);         
          lv_label_set_text(label, tabSetting_label); 
          lv_obj_set_style_text_font(label, &lv_font_montserrat_18, 0);              
          lv_obj_align(label, LV_ALIGN_CENTER, 0, 29);



    toolsTab = lv_obj_create(screen_mainMenu);
    lv_obj_set_pos(toolsTab, 5, 7 + 12 + (2 * 97));                            
    lv_obj_set_size(toolsTab, 130, 97);   
    lv_obj_add_event_cb(toolsTab, event_tab_switch, LV_EVENT_CLICKED, toolsTab);
    lv_obj_set_style_border_color(toolsTab, lv_color_hex(GREY), 0);
    lv_obj_set_style_border_opa(toolsTab, LV_OPA_50, 0);
    lv_obj_remove_flag(toolsTab, LV_OBJ_FLAG_SCROLLABLE);      

          iconLabel = lv_label_create(toolsTab);          
          lv_label_set_text(iconLabel, tabTools_icon);                  
          lv_obj_set_style_text_font(iconLabel, &FilMachineFontIcons_40, 0);
          lv_obj_align(iconLabel, LV_ALIGN_CENTER, 0, -10);


          label = lv_label_create(toolsTab);         
          lv_label_set_text(label, tabTools_label); 
          lv_obj_set_style_text_font(label, &lv_font_montserrat_18, 0);              
          lv_obj_align(label, LV_ALIGN_CENTER, 0, 29);

  create_keyboard(keyboard);
}




