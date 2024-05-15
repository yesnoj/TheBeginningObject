/**
 * @file page_processes.c
 *
 */


//ESSENTIAL INCLUDE
#include <lvgl.h>
#include "../../include/definitions.h"

extern struct gui_components gui;

//ACCESSORY INCLUDES


void event_processes_style_delete(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_DELETE) {
        //list of all styles to be reset, so clean the memory.
        lv_style_reset(&gui.page.processes.style_sectionTitleLine);
    }
}

void event_tabProcesses(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t * obj = (lv_obj_t *)lv_event_get_target(e);

  if(obj == gui.page.processes.processFilterButton){
    if(code == LV_EVENT_CLICKED) {
        LV_LOG_USER("New Filter Creation popup");
        if(gui.element.filterPopup.mBoxFilterPopupParent == NULL){
          filterPopupCreate();
        }
        else{
          lv_textarea_set_text(gui.element.filterPopup.mBoxNameTextArea, "");
          lv_obj_remove_state(gui.element.filterPopup.mBoxOnlyPreferredSwitch, LV_STATE_CHECKED);
          lv_obj_remove_state(gui.element.filterPopup.mBoxSelectColorRadioButton, LV_STATE_CHECKED);
          lv_obj_remove_state(gui.element.filterPopup.mBoxSelectBnWRadioButton, LV_STATE_CHECKED);
          lv_obj_remove_flag(gui.element.filterPopup.mBoxFilterPopupParent, LV_OBJ_FLAG_HIDDEN);
          }
    }
  }

  if(obj == gui.page.processes.newProcessButton){
    if(code == LV_EVENT_CLICKED) {
      processDetail(gui.page.processes.processesList);
      gui.element.step.stepCounter = 0;
      LV_LOG_USER("New Process Creation popup");
    }
  }

}
 

void initProcesses(void){
  /*********************
  *    PAGE HEADER
  *********************/
  LV_LOG_USER("Processes Creation");
  gui.page.processes.processesSection = lv_obj_create(gui.page.menu.screen_mainMenu);
  lv_obj_set_pos(gui.page.processes.processesSection, 140, 7);                           
  lv_obj_set_size(gui.page.processes.processesSection, 335, 303); 
  lv_obj_remove_flag(gui.page.processes.processesSection, LV_OBJ_FLAG_SCROLLABLE); 
  lv_obj_set_style_border_color(gui.page.processes.processesSection, lv_color_hex(GREEN_DARK), 0);
  

  /*********************
  *    PAGE ELEMENTS
  *********************/
  //ADD NEW PROCESS BUTTON
  gui.page.processes.newProcessButton = lv_obj_create(gui.page.processes.processesSection);
  lv_obj_align(gui.page.processes.newProcessButton, LV_ALIGN_TOP_LEFT, -20, -12);
  lv_obj_set_size(gui.page.processes.newProcessButton, 200, 45); 
  lv_obj_remove_flag(gui.page.processes.newProcessButton, LV_OBJ_FLAG_SCROLLABLE); 
  lv_obj_set_style_border_opa(gui.page.processes.newProcessButton, LV_OPA_TRANSP, 0);
  //lv_obj_set_style_border_color(gui.page.processes.newProcessButton, lv_color_hex(GREEN_DARK), 0);
  lv_obj_add_event_cb(gui.page.processes.newProcessButton, event_tabProcesses, LV_EVENT_CLICKED, gui.page.processes.newProcessButton);

      //PROCESSES LABEL
      gui.page.processes.processesLabel = lv_label_create(gui.page.processes.newProcessButton);         
      lv_label_set_text(gui.page.processes.processesLabel, Processes_text); 
      lv_obj_set_style_text_font(gui.page.processes.processesLabel, &lv_font_montserrat_28, 0);              
      lv_obj_align(gui.page.processes.processesLabel, LV_ALIGN_LEFT_MID, -5, 0);

      gui.page.processes.iconNewProcessLabel = lv_label_create(gui.page.processes.newProcessButton);          
      lv_label_set_text(gui.page.processes.iconNewProcessLabel, newProcess_icon);                  
      lv_obj_set_style_text_font(gui.page.processes.iconNewProcessLabel, &FilMachineFontIcons_30, 0);
      lv_obj_align(gui.page.processes.iconNewProcessLabel, LV_ALIGN_RIGHT_MID, 5, 0);


  //FILTER BUTTON CONTAINER
  gui.page.processes.processFilterButton = lv_obj_create(gui.page.processes.processesSection);
  lv_obj_align(gui.page.processes.processFilterButton, LV_ALIGN_TOP_RIGHT, 10, -12);
  lv_obj_set_size(gui.page.processes.processFilterButton, 45, 45); 
  lv_obj_remove_flag(gui.page.processes.processFilterButton, LV_OBJ_FLAG_SCROLLABLE); 
  lv_obj_set_style_border_opa(gui.page.processes.processFilterButton, LV_OPA_TRANSP, 0);
  //lv_obj_set_style_border_color(gui.page.processes.processFilterButton, lv_color_hex(GREEN_DARK), 0);
  lv_obj_add_event_cb(gui.page.processes.processFilterButton, event_tabProcesses, LV_EVENT_CLICKED, gui.page.processes.processFilterButton);
                
      //FILTER BUTTON ICON
      gui.page.processes.iconFilterLabel = lv_label_create(gui.page.processes.processFilterButton);          
      lv_label_set_text(gui.page.processes.iconFilterLabel, funnel_icon);                  
      lv_obj_set_style_text_font(gui.page.processes.iconFilterLabel, &FilMachineFontIcons_30, 0);
      lv_obj_align(gui.page.processes.iconFilterLabel, LV_ALIGN_CENTER, 0, 0);


  gui.page.processes.processesList = lv_obj_create(gui.page.processes.processesSection);
  lv_obj_set_pos(gui.page.processes.processesList, -18, 31);                           
  lv_obj_set_size(gui.page.processes.processesList, 335, 254);  
  lv_obj_set_style_border_opa(gui.page.processes.processesList, LV_OPA_TRANSP, 0);
  lv_obj_set_scroll_dir(gui.page.processes.processesList, LV_DIR_VER);
  //lv_obj_set_style_border_color(gui.page.processes.processesList, lv_color_hex(GREY), 0);

  //CREATE STYLE AND LINE FOR THE TITLE
  /*Create style*/
  lv_style_init(&gui.page.processes.titleLinePoints);
  lv_style_set_line_width(&gui.page.processes.titleLinePoints, 2);
  lv_style_set_line_color(&gui.page.processes.titleLinePoints, lv_palette_main(LV_PALETTE_GREEN));
  lv_style_set_line_rounded(&gui.page.processes.titleLinePoints, true);

  /*Create a line and apply the new style*/
  gui.page.processes.sectionTitleLine = lv_line_create(gui.page.processes.processesSection);
  lv_line_set_points(gui.page.processes.sectionTitleLine, gui.page.processes.titleLinePoints, 2);
  lv_obj_add_style(gui.page.processes.sectionTitleLine, &gui.page.processes.style_sectionTitleLine, 0);
  lv_obj_align(gui.page.processes.sectionTitleLine, LV_ALIGN_CENTER, 0, - 103);

  //lv_obj_update_layout(gui.page.processes.processesSection);


    

}


void processes(void)
{   
  if(gui.page.processes.processesSection == NULL){
    initProcesses();
  }

  lv_obj_clear_flag(gui.page.processes.processesSection, LV_OBJ_FLAG_HIDDEN);
  lv_style_set_line_color(&gui.page.processes.titleLinePoints, lv_palette_main(LV_PALETTE_GREEN));

}


