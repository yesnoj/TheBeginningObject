#include "misc/lv_event.h"
/**
 * @file page_menu.c
 *
 */


//ESSENTIAL INCLUDES
#include <lvgl.h>
#include "../../include/definitions.h"


extern struct gui_components gui;


//ACCESSORY INCLUDES

void event_tab_switch(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);
  
  if(gui.page.menu.newSelection == TAB_PROCESSES && code == LV_EVENT_LONG_PRESSED){
      LV_LOG_USER("POPUP DELETE ALL PROCESSES");
      messagePopupCreate(deleteAllProcessPopupTitle_text,deleteAllProcessPopupBody_text, deleteButton_text, stepDetailCancel_text, &gui);
  }


  if(code == LV_EVENT_CLICKED) {
    if(gui.page.menu.newTabSelected == NULL && gui.page.menu.oldTabSelected == NULL){
      gui.page.menu.newTabSelected = gui.page.menu.oldTabSelected = (lv_obj_t *)lv_event_get_target(e);
      gui.page.menu.newSelection = gui.page.menu.oldSelection = (uint8_t)lv_obj_get_index(gui.page.menu.newTabSelected);
      lv_obj_set_style_bg_color(gui.page.menu.newTabSelected, lv_color_hex(GREEN_DARK), 0);

      LV_LOG_USER("FIRST TIME");
    }
    else {
        gui.page.menu.oldTabSelected = gui.page.menu.newTabSelected;
        gui.page.menu.oldSelection = gui.page.menu.newSelection;
        gui.page.menu.newTabSelected = (lv_obj_t *)lv_event_get_target(e);
        gui.page.menu.newSelection = (uint8_t)lv_obj_get_index(gui.page.menu.newTabSelected);
        LV_LOG_USER("NEXT TIME");
    }
    
  
    if(gui.page.menu.newSelection != gui.page.menu.oldSelection) 
      { 
        //FIRST HIDE THE CORRECT SECTION
        if(gui.page.menu.oldSelection == TAB_PROCESSES){
            lv_obj_add_flag(gui.page.processes.processesSection, LV_OBJ_FLAG_HIDDEN);
        }
        if(gui.page.menu.oldSelection == TAB_SETTINGS){
            lv_obj_add_flag(gui.page.settings.settingsSection, LV_OBJ_FLAG_HIDDEN);
        }
        if(gui.page.menu.oldSelection == TAB_TOOLS){
            lv_obj_add_flag(gui.page.tools.toolsSection, LV_OBJ_FLAG_HIDDEN);
        }

        //THEN SHOW THE NEW SECTION
        if(gui.page.menu.newSelection == TAB_PROCESSES){
            lv_obj_set_style_bg_color(gui.page.menu.newTabSelected, lv_color_hex(GREEN_DARK), 0);
            lv_obj_remove_local_style_prop(gui.page.menu.oldTabSelected, LV_STYLE_BG_COLOR, 0);
            
            lv_obj_clear_flag(gui.page.processes.processesSection, LV_OBJ_FLAG_HIDDEN);
        }
        if(gui.page.menu.newSelection == TAB_SETTINGS){
            lv_obj_set_style_bg_color(gui.page.menu.newTabSelected, lv_color_hex(ORANGE_DARK), 0);
            lv_obj_remove_local_style_prop(gui.page.menu.oldTabSelected, LV_STYLE_BG_COLOR, 0); 

            lv_obj_clear_flag(gui.page.settings.settingsSection, LV_OBJ_FLAG_HIDDEN);
        }
        if(gui.page.menu.newSelection == TAB_TOOLS){
            lv_obj_set_style_bg_color(gui.page.menu.newTabSelected, lv_color_hex(BLUE_DARK), 0);
            lv_obj_remove_local_style_prop(gui.page.menu.oldTabSelected, LV_STYLE_BG_COLOR, 0); 

            lv_obj_clear_flag(gui.page.tools.toolsSection, LV_OBJ_FLAG_HIDDEN);
        }
      }
    
    LV_LOG_USER("OLD PRESSED %d",(int*)lv_obj_get_index(gui.page.menu.oldTabSelected));
    LV_LOG_USER("NEW PRESSED %d",(int*)lv_obj_get_index(gui.page.menu.newTabSelected));
     
  }   
}    


void menu(void){
    lv_obj_del(lv_screen_active());
    gui.page.menu.screen_mainMenu = lv_obj_create(NULL);
    lv_scr_load(gui.page.menu.screen_mainMenu);

    processes();
    settings();
    tools();
    
    gui.page.menu.processesTab = lv_obj_create(gui.page.menu.screen_mainMenu);
    lv_obj_set_pos(gui.page.menu.processesTab, 5, 7);                            
    lv_obj_set_size(gui.page.menu.processesTab, 130, 97);   
    lv_obj_add_event_cb(gui.page.menu.processesTab, event_tab_switch, LV_EVENT_CLICKED, gui.page.menu.processesTab);
    lv_obj_add_event_cb(gui.page.menu.processesTab, event_tab_switch, LV_EVENT_LONG_PRESSED, gui.page.menu.processesTab);
    lv_obj_set_style_border_color(gui.page.menu.processesTab, lv_color_hex(GREY), 0);
    lv_obj_set_style_border_opa(gui.page.menu.processesTab, LV_OPA_50, 0);
    lv_obj_remove_flag(gui.page.menu.processesTab, LV_OBJ_FLAG_SCROLLABLE);      

          gui.page.menu.iconLabel = lv_label_create(gui.page.menu.processesTab);          
          lv_label_set_text(gui.page.menu.iconLabel, tabProcess_icon);                  
          lv_obj_set_style_text_font(gui.page.menu.iconLabel, &FilMachineFontIcons_40, 0);
          lv_obj_align(gui.page.menu.iconLabel, LV_ALIGN_CENTER, 0, -10);


          gui.page.menu.label = lv_label_create(gui.page.menu.processesTab);         
          lv_label_set_text(gui.page.menu.label, tabProcess_label); 
          lv_obj_set_style_text_font(gui.page.menu.label, &lv_font_montserrat_18, 0);              
          lv_obj_align(gui.page.menu.label, LV_ALIGN_CENTER, 0, 29);
          lv_obj_send_event(gui.page.menu.processesTab, LV_EVENT_CLICKED, gui.page.menu.processesTab);



    gui.page.menu.settingsTab = lv_obj_create(gui.page.menu.screen_mainMenu);
    lv_obj_set_pos(gui.page.menu.settingsTab, 5, 7 + 6 + 97);                            
    lv_obj_set_size(gui.page.menu.settingsTab, 130, 97);   
    lv_obj_add_event_cb(gui.page.menu.settingsTab, event_tab_switch, LV_EVENT_CLICKED, gui.page.menu.settingsTab);
    lv_obj_set_style_border_color(gui.page.menu.settingsTab, lv_color_hex(GREY), 0);
    lv_obj_set_style_border_opa(gui.page.menu.settingsTab, LV_OPA_50, 0);
    lv_obj_remove_flag(gui.page.menu.settingsTab, LV_OBJ_FLAG_SCROLLABLE);      

          gui.page.menu.iconLabel = lv_label_create(gui.page.menu.settingsTab);          
          lv_label_set_text(gui.page.menu.iconLabel, tabSetting_icon);                  
          lv_obj_set_style_text_font(gui.page.menu.iconLabel, &FilMachineFontIcons_40, 0);
          lv_obj_align(gui.page.menu.iconLabel, LV_ALIGN_CENTER, 0, -10);

          gui.page.menu.label = lv_label_create(gui.page.menu.settingsTab);         
          lv_label_set_text(gui.page.menu.label, tabSetting_label); 
          lv_obj_set_style_text_font(gui.page.menu.label, &lv_font_montserrat_18, 0);              
          lv_obj_align(gui.page.menu.label, LV_ALIGN_CENTER, 0, 29);



    gui.page.menu.toolsTab = lv_obj_create(gui.page.menu.screen_mainMenu);
    lv_obj_set_pos(gui.page.menu.toolsTab, 5, 7 + 12 + (2 * 97));                            
    lv_obj_set_size(gui.page.menu.toolsTab, 130, 97);   
    lv_obj_add_event_cb(gui.page.menu.toolsTab, event_tab_switch, LV_EVENT_CLICKED, gui.page.menu.toolsTab);
    lv_obj_set_style_border_color(gui.page.menu.toolsTab, lv_color_hex(GREY), 0);
    lv_obj_set_style_border_opa(gui.page.menu.toolsTab, LV_OPA_50, 0);
    lv_obj_remove_flag(gui.page.menu.toolsTab, LV_OBJ_FLAG_SCROLLABLE);      

          gui.page.menu.iconLabel = lv_label_create(gui.page.menu.toolsTab);          
          lv_label_set_text(gui.page.menu.iconLabel, tabTools_icon);                  
          lv_obj_set_style_text_font(gui.page.menu.iconLabel, &FilMachineFontIcons_40, 0);
          lv_obj_align(gui.page.menu.iconLabel, LV_ALIGN_CENTER, 0, -10);


          gui.page.menu.label = lv_label_create(gui.page.menu.toolsTab);         
          lv_label_set_text(gui.page.menu.label, tabTools_label); 
          lv_obj_set_style_text_font(gui.page.menu.label, &lv_font_montserrat_18, 0);              
          lv_obj_align(gui.page.menu.label, LV_ALIGN_CENTER, 0, 29);

  
}




