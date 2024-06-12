/**
 * @file page_tools.c
 *
 */


//ESSENTIAL INCLUDE
#include <lvgl.h>
#include "../../include/definitions.h"

extern struct gui_components gui;


//ACCESSORY INCLUDES



void event_toolsElement(lv_event_t * e){
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t * obj = (lv_obj_t *)lv_event_get_target(e);
  lv_obj_t * objCont = (lv_obj_t *)lv_obj_get_parent(obj);
  lv_obj_t * mboxCont = (lv_obj_t *)lv_obj_get_parent(objCont);
  lv_obj_t * mboxParent = (lv_obj_t *)lv_obj_get_parent(mboxCont);

  lv_obj_t * data = (lv_obj_t *)lv_event_get_user_data(e);
 
  if(obj == gui.page.tools.toolsCleaningIcon){
      if(code == LV_EVENT_CLICKED) {    
          LV_LOG_USER("PRESSED gui.page.tools.toolsCleaningButton");
          gui.page.tools.machineStats.completedCleanCycle ++;
          lv_label_set_text_fmt(gui.page.tools.toolStatCompleteCycleValue, "%d", gui.page.tools.machineStats.completedCleanCycle);
          qSysAction( SAVE_PROCESS_CONFIG );
      }
  }
  if(obj == gui.page.tools.toolsDrainingIcon){
      if(code == LV_EVENT_CLICKED) {    
           LV_LOG_USER("PRESSED gui.page.tools.toolsDrainingIcon");
      }
  }
  if(obj == gui.page.tools.toolsImportIcon){
      if(code == LV_EVENT_CLICKED) {    
           LV_LOG_USER("PRESSED gui.page.tools.toolsImportIcon");
      }
  }
  if(obj == gui.page.tools.toolsExportIcon){
      if(code == LV_EVENT_CLICKED) {    
           LV_LOG_USER("PRESSED gui.page.tools.toolsExportIcon");
      }
  }
  if(data == gui.page.tools.toolsImportLabel){
    if(code == LV_EVENT_CLICKED) {  
        messagePopupCreate(messagePopupDetailTitle_text,importConfigAndProcessesMBox_text,NULL, NULL, NULL);
      }
  }
  if(data == gui.page.tools.toolsExportLabel){
    if(code == LV_EVENT_CLICKED) {  
    messagePopupCreate(messagePopupDetailTitle_text,exportConfigAndProcessesMBox_text, NULL, NULL, NULL);
    }
  }
  if(data == gui.page.tools.toolCreditButton){
    if(code == LV_EVENT_CLICKED) {  
        LV_LOG_USER("PRESSED gui.page.tools.toolCreditButton");
        messagePopupCreate(softwareCredits_text,softwareCreditsValue_text, NULL, NULL, NULL);
    }
  }
}


static void initTools(void){
/*********************
 *    PAGE HEADER
 *********************/
  LV_LOG_USER("Tools Creation");
  gui.page.tools.toolsSection = lv_obj_create(lv_screen_active());
  lv_obj_set_pos(gui.page.tools.toolsSection, 140, 7);                         
  lv_obj_set_size(gui.page.tools.toolsSection, 335, 303); 
  //lv_obj_remove_flag(gui.page.tools.toolsSection, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_scroll_dir(gui.page.tools.toolsSection, LV_DIR_VER);
  lv_obj_set_style_border_color(gui.page.tools.toolsSection, lv_color_hex(BLUE_DARK), 0);  

 

  //lv_obj_update_layout(gui.page.tools.toolsSection);

  /*********************
 *    PAGE ELEMENTS
 *********************/

  gui.page.tools.toolsMaintenanceLabel = lv_label_create(gui.page.tools.toolsSection);         
  lv_label_set_text(gui.page.tools.toolsMaintenanceLabel, Maintenance_text); 
  lv_obj_set_style_text_font(gui.page.tools.toolsMaintenanceLabel, &lv_font_montserrat_28, 0);              
  lv_obj_align(gui.page.tools.toolsMaintenanceLabel, LV_ALIGN_TOP_LEFT, -7, -5);

  /*Create style*/
  lv_style_init(&gui.page.tools.style_sectionTitleLine);
  lv_style_set_line_width(&gui.page.tools.style_sectionTitleLine, 2);
  lv_style_set_line_color(&gui.page.tools.style_sectionTitleLine, lv_color_hex(LIGHT_BLUE));
  lv_style_set_line_rounded(&gui.page.tools.style_sectionTitleLine, true);

  /*Create a line and apply the new style*/
  gui.page.tools.sectionTitleLine = lv_line_create(gui.page.tools.toolsSection);
  lv_line_set_points(gui.page.tools.sectionTitleLine, gui.page.tools.titleLinePoints, 2);
  lv_obj_add_style(gui.page.tools.sectionTitleLine, &gui.page.tools.style_sectionTitleLine, 0);
  lv_obj_align(gui.page.tools.sectionTitleLine, LV_ALIGN_TOP_MID, 0, 30);


  gui.page.tools.toolsCleaningContainer = lv_obj_create(gui.page.tools.toolsSection);
  lv_obj_align(gui.page.tools.toolsCleaningContainer, LV_ALIGN_TOP_LEFT, -15, 35);                         
  lv_obj_set_size(gui.page.tools.toolsCleaningContainer, 330, 50); 
  lv_obj_remove_flag(gui.page.tools.toolsCleaningContainer, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_scroll_dir(gui.page.tools.toolsCleaningContainer, LV_DIR_VER);    
  //lv_obj_set_style_border_color(gui.page.tools.toolsCleaningContainer, lv_color_hex(LV_PALETTE_GREEN), 0);
  lv_obj_set_style_border_opa(gui.page.tools.toolsCleaningContainer, LV_OPA_TRANSP, 0);

      gui.page.tools.toolsCleaningLabel = lv_label_create(gui.page.tools.toolsCleaningContainer);         
      lv_label_set_text(gui.page.tools.toolsCleaningLabel, cleanMachine_text); 
      lv_obj_set_style_text_font(gui.page.tools.toolsCleaningLabel, &lv_font_montserrat_20, 0);              
      lv_obj_align(gui.page.tools.toolsCleaningLabel, LV_ALIGN_LEFT_MID, -5, 0);

      gui.page.tools.toolsCleaningIcon = lv_label_create(gui.page.tools.toolsCleaningContainer);
      lv_label_set_text(gui.page.tools.toolsCleaningIcon, play_icon);
      lv_obj_set_style_text_font(gui.page.tools.toolsCleaningIcon, &FilMachineFontIcons_30, 0);
      lv_obj_align(gui.page.tools.toolsCleaningIcon, LV_ALIGN_RIGHT_MID, 0, 0);
      lv_obj_add_flag(gui.page.tools.toolsCleaningIcon, LV_OBJ_FLAG_CLICKABLE);
      lv_obj_add_event_cb(gui.page.tools.toolsCleaningIcon, event_toolsElement, LV_EVENT_CLICKED, gui.page.tools.toolsCleaningIcon);



  gui.page.tools.toolsDrainingContainer = lv_obj_create(gui.page.tools.toolsSection);
  lv_obj_align(gui.page.tools.toolsDrainingContainer, LV_ALIGN_TOP_LEFT, -15, 85);                         
  lv_obj_set_size(gui.page.tools.toolsDrainingContainer, 330, 50); 
  lv_obj_remove_flag(gui.page.tools.toolsDrainingContainer, LV_OBJ_FLAG_SCROLLABLE); 
  lv_obj_set_scroll_dir(gui.page.tools.toolsCleaningContainer, LV_DIR_VER);     
  //lv_obj_set_style_border_color(gui.page.tools.toolsDrainingContainer, lv_color_hex(LV_PALETTE_GREEN), 0);
  lv_obj_set_style_border_opa(gui.page.tools.toolsDrainingContainer, LV_OPA_TRANSP, 0);

      gui.page.tools.toolsDrainingLabel = lv_label_create(gui.page.tools.toolsDrainingContainer);         
      lv_label_set_text(gui.page.tools.toolsDrainingLabel, drainMachine_text); 
      lv_obj_set_style_text_font(gui.page.tools.toolsDrainingLabel, &lv_font_montserrat_20, 0);              
      lv_obj_align(gui.page.tools.toolsDrainingLabel, LV_ALIGN_LEFT_MID, -5, 0);

      gui.page.tools.toolsDrainingIcon = lv_label_create(gui.page.tools.toolsDrainingContainer);
      lv_label_set_text(gui.page.tools.toolsDrainingIcon, play_icon);
      lv_obj_set_style_text_font(gui.page.tools.toolsDrainingIcon, &FilMachineFontIcons_30, 0);
      lv_obj_align(gui.page.tools.toolsDrainingIcon, LV_ALIGN_RIGHT_MID, 0, 0);
      lv_obj_add_flag(gui.page.tools.toolsDrainingIcon, LV_OBJ_FLAG_CLICKABLE);
      lv_obj_add_event_cb(gui.page.tools.toolsDrainingIcon, event_toolsElement, LV_EVENT_CLICKED, gui.page.tools.toolsDrainingIcon);


  gui.page.tools.toolsUtilitiesLabel = lv_label_create(gui.page.tools.toolsSection);         
  lv_label_set_text(gui.page.tools.toolsUtilitiesLabel, Utilities_text); 
  lv_obj_set_style_text_font(gui.page.tools.toolsUtilitiesLabel, &lv_font_montserrat_28, 0);              
  lv_obj_align(gui.page.tools.toolsUtilitiesLabel, LV_ALIGN_TOP_LEFT, -7, 135);

  lv_style_init(&gui.page.tools.style_sectionTitleLine);
  lv_style_set_line_width(&gui.page.tools.style_sectionTitleLine, 2);
  lv_style_set_line_color(&gui.page.tools.style_sectionTitleLine, lv_color_hex(LIGHT_BLUE));
  lv_style_set_line_rounded(&gui.page.tools.style_sectionTitleLine, true);

  gui.page.tools.sectionTitleLine = lv_line_create(gui.page.tools.toolsSection);
  lv_line_set_points(gui.page.tools.sectionTitleLine, gui.page.tools.titleLinePoints, 2);
  lv_obj_add_style(gui.page.tools.sectionTitleLine, &gui.page.tools.style_sectionTitleLine, 0);
  lv_obj_align(gui.page.tools.sectionTitleLine, LV_ALIGN_TOP_MID, 0, 170);


  
  gui.page.tools.toolsImportContainer = lv_obj_create(gui.page.tools.toolsSection);
  lv_obj_align(gui.page.tools.toolsImportContainer, LV_ALIGN_TOP_LEFT, -15, 175);                         
  lv_obj_set_size(gui.page.tools.toolsImportContainer, 330, 50); 
  lv_obj_remove_flag(gui.page.tools.toolsImportContainer, LV_OBJ_FLAG_SCROLLABLE); 
  lv_obj_set_scroll_dir(gui.page.tools.toolsImportContainer, LV_DIR_VER);     
  //lv_obj_set_style_border_color(gui.page.tools.toolsImportContainer, lv_color_hex(LV_PALETTE_GREEN), 0);
  lv_obj_set_style_border_opa(gui.page.tools.toolsImportContainer, LV_OPA_TRANSP, 0);

      gui.page.tools.toolsImportLabel = lv_label_create(gui.page.tools.toolsImportContainer);         
      lv_label_set_text(gui.page.tools.toolsImportLabel, importConfigAndProcesses_text); 
      lv_obj_set_style_text_font(gui.page.tools.toolsImportLabel, &lv_font_montserrat_20, 0);              
      lv_obj_align(gui.page.tools.toolsImportLabel, LV_ALIGN_LEFT_MID, -5, 0);

      createQuestionMark(gui.page.tools.toolsImportContainer,gui.page.tools.toolsImportLabel,event_toolsElement, 2, -3);

      gui.page.tools.toolsImportIcon = lv_label_create(gui.page.tools.toolsImportContainer);
      lv_label_set_text(gui.page.tools.toolsImportIcon, play_icon);
      lv_obj_set_style_text_font(gui.page.tools.toolsImportIcon, &FilMachineFontIcons_30, 0);
      lv_obj_align(gui.page.tools.toolsImportIcon, LV_ALIGN_RIGHT_MID, 0, 0);
      lv_obj_add_flag(gui.page.tools.toolsImportIcon, LV_OBJ_FLAG_CLICKABLE);
      lv_obj_add_event_cb(gui.page.tools.toolsImportIcon, event_toolsElement, LV_EVENT_CLICKED, gui.page.tools.toolsImportIcon);



  gui.page.tools.toolsExportContainer = lv_obj_create(gui.page.tools.toolsSection);
  lv_obj_align(gui.page.tools.toolsExportContainer, LV_ALIGN_TOP_LEFT, -15, 225);                         
  lv_obj_set_size(gui.page.tools.toolsExportContainer, 330, 50); 
  lv_obj_remove_flag(gui.page.tools.toolsExportContainer, LV_OBJ_FLAG_SCROLLABLE);  
  lv_obj_set_scroll_dir(gui.page.tools.toolsExportContainer, LV_DIR_VER);    
  //lv_obj_set_style_border_color(gui.page.tools.toolsExportContainer, lv_color_hex(LV_PALETTE_GREEN), 0);
  lv_obj_set_style_border_opa(gui.page.tools.toolsExportContainer, LV_OPA_TRANSP, 0);

      gui.page.tools.toolsExportLabel = lv_label_create(gui.page.tools.toolsExportContainer);         
      lv_label_set_text(gui.page.tools.toolsExportLabel, exportConfigAndProcesses_text); 
      lv_obj_set_style_text_font(gui.page.tools.toolsExportLabel, &lv_font_montserrat_20, 0);              
      lv_obj_align(gui.page.tools.toolsExportLabel, LV_ALIGN_LEFT_MID, -5, 0);

      createQuestionMark(gui.page.tools.toolsExportContainer,gui.page.tools.toolsExportLabel,event_toolsElement, 2, -3);

      gui.page.tools.toolsExportIcon = lv_label_create(gui.page.tools.toolsExportContainer);
      lv_label_set_text(gui.page.tools.toolsExportIcon, play_icon);
      lv_obj_set_style_text_font(gui.page.tools.toolsExportIcon, &FilMachineFontIcons_30, 0);
      lv_obj_align(gui.page.tools.toolsExportIcon, LV_ALIGN_RIGHT_MID, 0, 0);
      lv_obj_add_flag(gui.page.tools.toolsExportIcon, LV_OBJ_FLAG_CLICKABLE);
      lv_obj_add_event_cb(gui.page.tools.toolsExportIcon, event_toolsElement, LV_EVENT_CLICKED, gui.page.tools.toolsExportIcon);




  gui.page.tools.toolsStatisticsLabel = lv_label_create(gui.page.tools.toolsSection);         
  lv_label_set_text(gui.page.tools.toolsStatisticsLabel, Statistics_text); 
  lv_obj_set_style_text_font(gui.page.tools.toolsStatisticsLabel, &lv_font_montserrat_28, 0);              
  lv_obj_align(gui.page.tools.toolsStatisticsLabel, LV_ALIGN_TOP_LEFT, -7, 275);

  lv_style_init(&gui.page.tools.style_sectionTitleLine);
  lv_style_set_line_width(&gui.page.tools.style_sectionTitleLine, 2);
  lv_style_set_line_color(&gui.page.tools.style_sectionTitleLine, lv_color_hex(LIGHT_BLUE));
  lv_style_set_line_rounded(&gui.page.tools.style_sectionTitleLine, true);

  gui.page.tools.sectionTitleLine = lv_line_create(gui.page.tools.toolsSection);
  lv_line_set_points(gui.page.tools.sectionTitleLine, gui.page.tools.titleLinePoints, 2);
  lv_obj_add_style(gui.page.tools.sectionTitleLine, &gui.page.tools.style_sectionTitleLine, 0);
  lv_obj_align(gui.page.tools.sectionTitleLine, LV_ALIGN_TOP_MID, 0, 310);


  gui.page.tools.toolsStatCompleteProcessesContainer = lv_obj_create(gui.page.tools.toolsSection);
  lv_obj_align(gui.page.tools.toolsStatCompleteProcessesContainer, LV_ALIGN_TOP_LEFT, -15, 315);                         
  lv_obj_set_size(gui.page.tools.toolsStatCompleteProcessesContainer, 330, 40); 
  lv_obj_remove_flag(gui.page.tools.toolsStatCompleteProcessesContainer, LV_OBJ_FLAG_SCROLLABLE);  
  lv_obj_set_scroll_dir(gui.page.tools.toolsStatCompleteProcessesContainer, LV_DIR_VER);    
  //lv_obj_set_style_border_color(gui.page.tools.toolsStatCompleteProcessesContainer, lv_color_hex(LV_PALETTE_GREEN), 0);
  lv_obj_set_style_border_opa(gui.page.tools.toolsStatCompleteProcessesContainer, LV_OPA_TRANSP, 0);

      gui.page.tools.toolStatCompletedProcessesLabel = lv_label_create(gui.page.tools.toolsStatCompleteProcessesContainer);         
      lv_label_set_text(gui.page.tools.toolStatCompletedProcessesLabel, statCompleteProcesses_text); 
      lv_obj_set_style_text_font(gui.page.tools.toolStatCompletedProcessesLabel, &lv_font_montserrat_20, 0);              
      lv_obj_align(gui.page.tools.toolStatCompletedProcessesLabel, LV_ALIGN_LEFT_MID, -5, 0);

      gui.page.tools.toolStatCompletedProcessesValue = lv_label_create(gui.page.tools.toolsStatCompleteProcessesContainer);     
      lv_label_set_text_fmt(gui.page.tools.toolStatCompletedProcessesValue, "%d", gui.page.tools.machineStats.completedProcesses);
      lv_obj_set_style_text_font(gui.page.tools.toolStatCompletedProcessesValue, &lv_font_montserrat_20, 0);              
      lv_obj_align(gui.page.tools.toolStatCompletedProcessesValue, LV_ALIGN_RIGHT_MID, -5, 0);
     

  gui.page.tools.toolsStatTotalTimeContainer = lv_obj_create(gui.page.tools.toolsSection);
  lv_obj_align(gui.page.tools.toolsStatTotalTimeContainer, LV_ALIGN_TOP_LEFT, -15, 355);                         
  lv_obj_set_size(gui.page.tools.toolsStatTotalTimeContainer, 330, 40); 
  lv_obj_remove_flag(gui.page.tools.toolsStatTotalTimeContainer, LV_OBJ_FLAG_SCROLLABLE);  
  lv_obj_set_scroll_dir(gui.page.tools.toolsStatTotalTimeContainer, LV_DIR_VER);    
  //lv_obj_set_style_border_color(gui.page.tools.toolsStatTotalTimeContainer, lv_color_hex(LV_PALETTE_GREEN), 0);
  lv_obj_set_style_border_opa(gui.page.tools.toolsStatTotalTimeContainer, LV_OPA_TRANSP, 0);

      gui.page.tools.toolStatTotalTimeLabel = lv_label_create(gui.page.tools.toolsStatTotalTimeContainer);         
      lv_label_set_text(gui.page.tools.toolStatTotalTimeLabel, statTotalProcessTime_text); 
      lv_obj_set_style_text_font(gui.page.tools.toolStatTotalTimeLabel, &lv_font_montserrat_20, 0);              
      lv_obj_align(gui.page.tools.toolStatTotalTimeLabel, LV_ALIGN_LEFT_MID, -5, 0);

      gui.page.tools.toolStatTotalTimeValue = lv_label_create(gui.page.tools.toolsStatTotalTimeContainer);
      lv_label_set_text_fmt(gui.page.tools.toolStatTotalTimeValue, "%d", gui.page.tools.machineStats.totalMins);
      lv_obj_set_style_text_font(gui.page.tools.toolStatTotalTimeValue, &lv_font_montserrat_20, 0);              
      lv_obj_align(gui.page.tools.toolStatTotalTimeValue, LV_ALIGN_RIGHT_MID, -5, 0);


  gui.page.tools.toolsStatCompleteCycleContainer = lv_obj_create(gui.page.tools.toolsSection);
  lv_obj_align(gui.page.tools.toolsStatCompleteCycleContainer, LV_ALIGN_TOP_LEFT, -15, 395);                         
  lv_obj_set_size(gui.page.tools.toolsStatCompleteCycleContainer, 330, 40); 
  lv_obj_remove_flag(gui.page.tools.toolsStatCompleteCycleContainer, LV_OBJ_FLAG_SCROLLABLE);  
  lv_obj_set_scroll_dir(gui.page.tools.toolsStatCompleteCycleContainer, LV_DIR_VER);    
  //lv_obj_set_style_border_color(gui.page.tools.toolsStatCompleteCycleContainer, lv_color_hex(LV_PALETTE_GREEN), 0);
  lv_obj_set_style_border_opa(gui.page.tools.toolsStatCompleteCycleContainer, LV_OPA_TRANSP, 0);

      gui.page.tools.toolStatCompleteCycleLabel = lv_label_create(gui.page.tools.toolsStatCompleteCycleContainer);         
      lv_label_set_text(gui.page.tools.toolStatCompleteCycleLabel, statCompleteCleanProcess_text); 
      lv_obj_set_style_text_font(gui.page.tools.toolStatCompleteCycleLabel, &lv_font_montserrat_20, 0);              
      lv_obj_align(gui.page.tools.toolStatCompleteCycleLabel, LV_ALIGN_LEFT_MID, -5, 0);

      gui.page.tools.toolStatCompleteCycleValue = lv_label_create(gui.page.tools.toolsStatCompleteCycleContainer);    
      lv_label_set_text_fmt(gui.page.tools.toolStatCompleteCycleValue, "%d", gui.page.tools.machineStats.completedCleanCycle);
      lv_obj_set_style_text_font(gui.page.tools.toolStatCompleteCycleValue, &lv_font_montserrat_20, 0);              
      lv_obj_align(gui.page.tools.toolStatCompleteCycleValue, LV_ALIGN_RIGHT_MID, -5, 0);


  gui.page.tools.toolsStatStoppedProcessesContainer = lv_obj_create(gui.page.tools.toolsSection);
  lv_obj_align(gui.page.tools.toolsStatStoppedProcessesContainer, LV_ALIGN_TOP_LEFT, -15, 435);                         
  lv_obj_set_size(gui.page.tools.toolsStatStoppedProcessesContainer, 330, 40); 
  lv_obj_remove_flag(gui.page.tools.toolsStatStoppedProcessesContainer, LV_OBJ_FLAG_SCROLLABLE);  
  lv_obj_set_scroll_dir(gui.page.tools.toolsStatStoppedProcessesContainer, LV_DIR_VER);    
  //lv_obj_set_style_border_color(gui.page.tools.toolsStatStoppedProcessesContainer, lv_color_hex(LV_PALETTE_GREEN), 0);
  lv_obj_set_style_border_opa(gui.page.tools.toolsStatStoppedProcessesContainer, LV_OPA_TRANSP, 0);

      gui.page.tools.toolStatStoppedProcessesLabel = lv_label_create(gui.page.tools.toolsStatStoppedProcessesContainer);         
      lv_label_set_text(gui.page.tools.toolStatStoppedProcessesLabel, statStoppedProcess_text); 
      lv_obj_set_style_text_font(gui.page.tools.toolStatStoppedProcessesLabel, &lv_font_montserrat_20, 0);              
      lv_obj_align(gui.page.tools.toolStatStoppedProcessesLabel, LV_ALIGN_LEFT_MID, -5, 0);

      gui.page.tools.toolStatStoppedProcessesValue = lv_label_create(gui.page.tools.toolsStatStoppedProcessesContainer);    
      lv_label_set_text_fmt(gui.page.tools.toolStatStoppedProcessesValue, "%d", gui.page.tools.machineStats.stoppedProcesses);
      lv_obj_set_style_text_font(gui.page.tools.toolStatStoppedProcessesValue, &lv_font_montserrat_20, 0);              
      lv_obj_align(gui.page.tools.toolStatStoppedProcessesValue, LV_ALIGN_RIGHT_MID, -5, 0);



  gui.page.tools.toolsSoftwareLabel = lv_label_create(gui.page.tools.toolsSection);         
  lv_label_set_text(gui.page.tools.toolsSoftwareLabel, Software_text); 
  lv_obj_set_style_text_font(gui.page.tools.toolsSoftwareLabel, &lv_font_montserrat_28, 0);              
  lv_obj_align(gui.page.tools.toolsSoftwareLabel, LV_ALIGN_TOP_LEFT, -7, 475);

  lv_style_init(&gui.page.tools.style_sectionTitleLine);
  lv_style_set_line_width(&gui.page.tools.style_sectionTitleLine, 2);
  lv_style_set_line_color(&gui.page.tools.style_sectionTitleLine, lv_color_hex(LIGHT_BLUE));
  lv_style_set_line_rounded(&gui.page.tools.style_sectionTitleLine, true);

  gui.page.tools.sectionTitleLine = lv_line_create(gui.page.tools.toolsSection);
  lv_line_set_points(gui.page.tools.sectionTitleLine, gui.page.tools.titleLinePoints, 2);
  lv_obj_add_style(gui.page.tools.sectionTitleLine, &gui.page.tools.style_sectionTitleLine, 0);
  lv_obj_align(gui.page.tools.sectionTitleLine, LV_ALIGN_TOP_MID, 0, 510);


  gui.page.tools.toolsSoftwareVersionContainer = lv_obj_create(gui.page.tools.toolsSection);
  lv_obj_align(gui.page.tools.toolsSoftwareVersionContainer, LV_ALIGN_TOP_LEFT, -15, 515);                         
  lv_obj_set_size(gui.page.tools.toolsSoftwareVersionContainer, 330, 40); 
  lv_obj_remove_flag(gui.page.tools.toolsSoftwareVersionContainer, LV_OBJ_FLAG_SCROLLABLE);  
  lv_obj_set_scroll_dir(gui.page.tools.toolsSoftwareVersionContainer, LV_DIR_VER);    
  //lv_obj_set_style_border_color(gui.page.tools.toolsSoftwareVersionContainer, lv_color_hex(LV_PALETTE_GREEN), 0);
  lv_obj_set_style_border_opa(gui.page.tools.toolsSoftwareVersionContainer, LV_OPA_TRANSP, 0);

      gui.page.tools.toolSoftWareVersionLabel = lv_label_create(gui.page.tools.toolsSoftwareVersionContainer);         
      lv_label_set_text(gui.page.tools.toolSoftWareVersionLabel, softwareVersion_text); 
      lv_obj_set_style_text_font(gui.page.tools.toolSoftWareVersionLabel, &lv_font_montserrat_20, 0);              
      lv_obj_align(gui.page.tools.toolSoftWareVersionLabel, LV_ALIGN_LEFT_MID, -5, 0);

      gui.page.tools.toolSoftwareVersionValue = lv_label_create(gui.page.tools.toolsSoftwareVersionContainer);         
      lv_label_set_text(gui.page.tools.toolSoftwareVersionValue, softwareVersionValue_text); 
      lv_obj_set_style_text_font(gui.page.tools.toolSoftwareVersionValue, &lv_font_montserrat_20, 0);              
      lv_obj_align(gui.page.tools.toolSoftwareVersionValue, LV_ALIGN_RIGHT_MID, -5, 0);


  gui.page.tools.toolsSoftwareSerialContainer = lv_obj_create(gui.page.tools.toolsSection);
  lv_obj_align(gui.page.tools.toolsSoftwareSerialContainer, LV_ALIGN_TOP_LEFT, -15, 555);                         
  lv_obj_set_size(gui.page.tools.toolsSoftwareSerialContainer, 330, 40); 
  lv_obj_remove_flag(gui.page.tools.toolsSoftwareSerialContainer, LV_OBJ_FLAG_SCROLLABLE);  
  lv_obj_set_scroll_dir(gui.page.tools.toolsSoftwareSerialContainer, LV_DIR_VER);    
  //lv_obj_set_style_border_color(gui.page.tools.toolsSoftwareSerialContainer, lv_color_hex(LV_PALETTE_GREEN), 0);
  lv_obj_set_style_border_opa(gui.page.tools.toolsSoftwareSerialContainer, LV_OPA_TRANSP, 0);

      gui.page.tools.toolSoftwareSerialLabel = lv_label_create(gui.page.tools.toolsSoftwareSerialContainer);         
      lv_label_set_text(gui.page.tools.toolSoftwareSerialLabel, softwareSerialNum_text); 
      lv_obj_set_style_text_font(gui.page.tools.toolSoftwareSerialLabel, &lv_font_montserrat_20, 0);              
      lv_obj_align(gui.page.tools.toolSoftwareSerialLabel, LV_ALIGN_LEFT_MID, -5, 0);

      gui.page.tools.toolSoftwareSerialValue = lv_label_create(gui.page.tools.toolsSoftwareSerialContainer);         
      lv_label_set_text(gui.page.tools.toolSoftwareSerialValue, softwareSerialNumValue_text); 
      lv_obj_set_style_text_font(gui.page.tools.toolSoftwareSerialValue, &lv_font_montserrat_20, 0);              
      lv_obj_align(gui.page.tools.toolSoftwareSerialValue, LV_ALIGN_RIGHT_MID, -5, 0);

  gui.page.tools.toolCreditButton = lv_button_create(gui.page.tools.toolsSection);
  lv_obj_set_size(gui.page.tools.toolCreditButton, BUTTON_TUNE_WIDTH, BUTTON_TUNE_HEIGHT);
  lv_obj_align(gui.page.tools.toolCreditButton, LV_ALIGN_TOP_MID, 5 , 595);
  lv_obj_add_event_cb(gui.page.tools.toolCreditButton, event_toolsElement, LV_EVENT_CLICKED, gui.page.tools.toolCreditButton);
  lv_obj_set_style_bg_color(gui.page.tools.toolCreditButton, lv_color_hex(LIGHT_BLUE), LV_PART_MAIN);

  gui.page.tools.toolCreditButtonLabel = lv_label_create(gui.page.tools.toolCreditButton);
  lv_label_set_text(gui.page.tools.toolCreditButtonLabel, softwareCredits_text);
  lv_obj_set_style_text_font(gui.page.tools.toolCreditButtonLabel, &lv_font_montserrat_18, 0);
  lv_obj_align(gui.page.tools.toolCreditButtonLabel, LV_ALIGN_CENTER, 0, 0);
}

void tools(void)
{   
  if(gui.page.tools.toolsSection == NULL){
    initTools();
  }

  lv_obj_clear_flag(gui.page.tools.toolsSection, LV_OBJ_FLAG_HIDDEN);
  lv_style_set_line_color(&gui.page.tools.style_sectionTitleLine, lv_palette_main(LV_PALETTE_BLUE));
}

