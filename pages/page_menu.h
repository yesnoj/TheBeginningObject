#include "misc/lv_color.h"
#include "core/lv_obj_style_gen.h"
#include "misc/lv_types.h"
/**
 * @file page_menu.h
 *
 */
#include "widgets/menu/lv_menu.h"
#include "core/lv_obj_style.h"
#include "misc/lv_area.h"

#ifndef PAGE_MENU_H
#define PAGE_MENU_H

#ifdef __cplusplus
extern "C" {
#endif

//ESSENTIAL INCLUDES


//ACCESSORY INCLUDES

#define TAB_PROCESSES 1
#define TAB_SETTINGS  2
#define TAB_TOOLS     3

static u_int8_t oldSelection = 0;
static u_int8_t newSelection = 0;

static lv_obj_t * processesTab;
static lv_obj_t * settingsTab;
static lv_obj_t * toolsTab;

static lv_obj_t * iconLabel;
static lv_obj_t * label;

static void back_event_handler(lv_event_t * e);
static void switch_handler(lv_event_t * e);
lv_obj_t * root_page;

static void event_tab_switch(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);

  if(code == LV_EVENT_CLICKED) {
    if(newTabSelected == NULL && oldTabSelected == NULL){
      newTabSelected = oldTabSelected = (lv_obj_t *)lv_event_get_target(e);
      newSelection = oldSelection = (u_int8_t)lv_obj_get_index(newTabSelected);
      lv_obj_set_style_bg_color(newTabSelected, lv_color_hex(GREEN_DARK), 0);

      LV_LOG_USER("FIRST TIME");
    }
    else {
        oldTabSelected = newTabSelected;
        oldSelection = newSelection;
        newTabSelected = (lv_obj_t *)lv_event_get_target(e);
        newSelection = (u_int8_t)lv_obj_get_index(newTabSelected);
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


void menu(){
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


static void back_event_handler(lv_event_t * e)
{
    lv_obj_t * obj = (lv_obj_t*)lv_event_get_target(e);
    lv_obj_t * menu = (lv_obj_t*)lv_event_get_user_data(e);

    if(lv_menu_back_button_is_root(menu, obj)) {
        lv_obj_t * mbox1 = lv_msgbox_create(NULL);
        lv_msgbox_add_title(mbox1, "Hello");
        lv_msgbox_add_text(mbox1, "Root back btn click.");
        lv_msgbox_add_close_button(mbox1);
    }
}

static void switch_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * menu = (lv_obj_t*)lv_event_get_user_data(e);
    lv_obj_t * obj = (lv_obj_t*)lv_event_get_target(e);
    if(code == LV_EVENT_VALUE_CHANGED) {
        if(lv_obj_has_state(obj, LV_STATE_CHECKED)) {
            lv_menu_set_page(menu, NULL);
            lv_menu_set_sidebar_page(menu, root_page);
            lv_obj_send_event(lv_obj_get_child(lv_obj_get_child(lv_menu_get_cur_sidebar_page(menu), 0), 0), LV_EVENT_CLICKED,
                              NULL);
        }
        else {
            lv_menu_set_sidebar_page(menu, NULL);
            lv_menu_clear_history(menu); /* Clear history because we will be showing the root page later */
            lv_menu_set_page(menu, root_page);
        }
    }
}








#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*PAGE_MENU_H*/