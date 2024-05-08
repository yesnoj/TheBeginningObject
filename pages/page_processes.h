#include "xtensa/hal.h"
#include "misc/lv_area.h"
#include "misc/lv_event.h"
#include "misc/lv_types.h"
#include "core/lv_obj_style_gen.h"
#include "misc/lv_color.h"
#include "core/lv_obj.h"
#include "misc/lv_palette.h"
/**
 * @file page_processes.h
 *
 */



#ifndef PAGE_PROCESSES_H
#define PAGE_PROCESSES_H

#ifdef __cplusplus
extern "C" {
#endif


//ESSENTIAL INCLUDE

//ACCESSORY INCLUDES

static lv_obj_t * processesLabel;
static lv_obj_t * iconFilterLabel;
static lv_obj_t * iconNewProcessLabel;
static lv_obj_t * processesList;

static void event_processes_style_delete(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_DELETE) {
        //list of all styles to be reset, so clean the memory.
        lv_style_reset(&style_sectionTitleLine);
    }
}

static void event_tabProcesses(lv_event_t * e)
{
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t * obj = (lv_obj_t *)lv_event_get_target(e);

  if(obj == processFilterButton){
    if(code == LV_EVENT_CLICKED) {
        LV_LOG_USER("New Filter Creation popup");
        if(mBoxFilterPopupParent == NULL){
          filterPopupCreate();
        }
        else{
          lv_textarea_set_text(mBoxNameTextArea, "");
          lv_obj_remove_state(mBoxOnlyPreferredSwitch, LV_STATE_CHECKED);
          lv_obj_remove_state(mBoxSelectColorRadioButton, LV_STATE_CHECKED);
          lv_obj_remove_state(mBoxSelectBnWRadioButton, LV_STATE_CHECKED);
          lv_obj_remove_flag(mBoxFilterPopupParent, LV_OBJ_FLAG_HIDDEN);
          }
    }
  }

  if(obj == newProcessButton){
    if(code == LV_EVENT_CLICKED) {
      processDetail(processesList);
      stepCounter = 0;
      LV_LOG_USER("New Process Creation popup");
    }
  }

}
 

static void initProcesses(void){
  /*********************
  *    PAGE HEADER
  *********************/
  LV_LOG_USER("Processes Creation");
  processesSection = lv_obj_create(screen_mainMenu);
  lv_obj_set_pos(processesSection, 140, 7);                           
  lv_obj_set_size(processesSection, 335, 303); 
  lv_obj_remove_flag(processesSection, LV_OBJ_FLAG_SCROLLABLE); 
  lv_obj_set_style_border_color(processesSection, lv_color_hex(GREEN_DARK), 0);
  

  /*********************
  *    PAGE ELEMENTS
  *********************/
  //ADD NEW PROCESS BUTTON
  newProcessButton = lv_obj_create(processesSection);
  lv_obj_align(newProcessButton, LV_ALIGN_TOP_LEFT, -20, -12);
  lv_obj_set_size(newProcessButton, 200, 45); 
  lv_obj_remove_flag(newProcessButton, LV_OBJ_FLAG_SCROLLABLE); 
  lv_obj_set_style_border_opa(newProcessButton, LV_OPA_TRANSP, 0);
  //lv_obj_set_style_border_color(newProcessButton, lv_color_hex(GREEN_DARK), 0);
  lv_obj_add_event_cb(newProcessButton, event_tabProcesses, LV_EVENT_CLICKED, newProcessButton);

      //PROCESSES LABEL
      processesLabel = lv_label_create(newProcessButton);         
      lv_label_set_text(processesLabel, Processes_text); 
      lv_obj_set_style_text_font(processesLabel, &lv_font_montserrat_28, 0);              
      lv_obj_align(processesLabel, LV_ALIGN_LEFT_MID, -5, 0);

      iconNewProcessLabel = lv_label_create(newProcessButton);          
      lv_label_set_text(iconNewProcessLabel, newProcess_icon);                  
      lv_obj_set_style_text_font(iconNewProcessLabel, &FilMachineFontIcons_30, 0);
      lv_obj_align(iconNewProcessLabel, LV_ALIGN_RIGHT_MID, 5, 0);


  //FILTER BUTTON CONTAINER
  processFilterButton = lv_obj_create(processesSection);
  lv_obj_align(processFilterButton, LV_ALIGN_TOP_RIGHT, 10, -12);
  lv_obj_set_size(processFilterButton, 45, 45); 
  lv_obj_remove_flag(processFilterButton, LV_OBJ_FLAG_SCROLLABLE); 
  lv_obj_set_style_border_opa(processFilterButton, LV_OPA_TRANSP, 0);
  //lv_obj_set_style_border_color(processFilterButton, lv_color_hex(GREEN_DARK), 0);
  lv_obj_add_event_cb(processFilterButton, event_tabProcesses, LV_EVENT_CLICKED, processFilterButton);
                
      //FILTER BUTTON ICON
      iconFilterLabel = lv_label_create(processFilterButton);          
      lv_label_set_text(iconFilterLabel, funnel_icon);                  
      lv_obj_set_style_text_font(iconFilterLabel, &FilMachineFontIcons_30, 0);
      lv_obj_align(iconFilterLabel, LV_ALIGN_CENTER, 0, 0);


  processesList = lv_obj_create(processesSection);
  lv_obj_set_pos(processesList, -18, 31);                           
  lv_obj_set_size(processesList, 335, 254);  
  lv_obj_set_style_border_opa(processesList, LV_OPA_TRANSP, 0);
  lv_obj_set_scroll_dir(processesList, LV_DIR_VER);
  //lv_obj_set_style_border_color(processesList, lv_color_hex(GREY), 0);

  //CREATE STYLE AND LINE FOR THE TITLE
  /*Create style*/
  lv_style_init(&style_sectionTitleLine);
  lv_style_set_line_width(&style_sectionTitleLine, 2);
  lv_style_set_line_color(&style_sectionTitleLine, lv_palette_main(LV_PALETTE_GREEN));
  lv_style_set_line_rounded(&style_sectionTitleLine, true);

  /*Create a line and apply the new style*/
  sectionTitleLine = lv_line_create(processesSection);
  lv_line_set_points(sectionTitleLine, sectionTitleLine_points, 2);
  lv_obj_add_style(sectionTitleLine, &style_sectionTitleLine, 0);
  lv_obj_align(sectionTitleLine, LV_ALIGN_CENTER, 0, - 103);

  //lv_obj_update_layout(processesSection);


    

}


void processes(void)
{   
  if(processesSection == NULL){
    initProcesses();
  }

  lv_obj_clear_flag(processesSection, LV_OBJ_FLAG_HIDDEN);
  lv_style_set_line_color(&style_sectionTitleLine, lv_palette_main(LV_PALETTE_GREEN));

}




#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*PAGE_PROCESSES_H*/