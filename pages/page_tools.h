#include "misc/lv_event.h"
#include "font/lv_font.h"
#include "misc/lv_area.h"
#include "misc/lv_types.h"
#include "core/lv_obj.h"
#include "misc/lv_palette.h"
/**
 * @file page_tools.h
 *
 */


#ifndef PAGE_TOOLS_H
#define PAGE_TOOLS_H

#ifdef __cplusplus
extern "C" {
#endif

//ESSENTIAL INCLUDE

//ACCESSORY INCLUDES
static lv_obj_t * toolsCleaningContainer;
static lv_obj_t * toolsDrainingContainer;
static lv_obj_t * toolsImportContainer;
static lv_obj_t * toolsExportContainer;



static lv_obj_t * toolsCleaningLabel;
static lv_obj_t * toolsDrainingLabel;
static lv_obj_t * toolsImportLabel;
static lv_obj_t * toolsExportLabel;

static lv_obj_t * toolsCleaningIcon;
static lv_obj_t * toolsDrainingIcon;
static lv_obj_t * toolsImportIcon;
static lv_obj_t * toolsExportIcon;

static lv_obj_t * toolsMaintenanceLabel;
static lv_obj_t * toolsUtilitiesLabel;
static lv_obj_t * toolsStatisticsLabel;
static lv_obj_t * toolsSoftwareLabel;

static lv_obj_t * toolsStatCompleteProcessesContainer;
static lv_obj_t * toolsStatTotalTimeContainer;
static lv_obj_t * toolsStatCompleteCycleContainer;
static lv_obj_t * toolsStatStoppedProcessesContainer;

static lv_obj_t * toolStatCompletedProcessesLabel;
static lv_obj_t * toolStatCompletedProcessesValue;
static lv_obj_t * toolStatTotalTimeLabel;
static lv_obj_t * toolStatTotalTimeValue;
static lv_obj_t * toolStatCompleteCycleLabel;
static lv_obj_t * toolStatCompleteCycleValue;
static lv_obj_t * toolStatStoppedProcessesLabel;
static lv_obj_t * toolStatStoppedProcessesValue;


static lv_obj_t * toolsSoftwareVersionContainer;
static lv_obj_t * toolsSoftwareSerialContainer;

static lv_obj_t * toolSoftWareVersionLabel;
static lv_obj_t * toolSoftwareVersionValue;
static lv_obj_t * toolSoftwareSerialLabel;
static lv_obj_t * toolSoftwareSerialValue;

static lv_obj_t * toolCreditButton;
static lv_obj_t * toolCreditButtonLabel;

static void event_toolsElement(lv_event_t * e){
  lv_event_code_t code = lv_event_get_code(e);
  lv_obj_t * obj = (lv_obj_t *)lv_event_get_target(e);
  lv_obj_t * objCont = (lv_obj_t *)lv_obj_get_parent(obj);
  lv_obj_t * mboxCont = (lv_obj_t *)lv_obj_get_parent(objCont);
  lv_obj_t * mboxParent = (lv_obj_t *)lv_obj_get_parent(mboxCont);

  lv_obj_t * data = (lv_obj_t *)lv_event_get_user_data(e);
 
  if(obj == toolsCleaningIcon){
      if(code == LV_EVENT_CLICKED) {    
           LV_LOG_USER("toolsCleaningButton");
      }
  }
  if(obj == toolsDrainingIcon){
      if(code == LV_EVENT_CLICKED) {    
           LV_LOG_USER("toolsDrainingIcon");
      }
  }
  if(obj == toolsImportIcon){
      if(code == LV_EVENT_CLICKED) {    
           LV_LOG_USER("toolsImportIcon");
      }
  }
  if(obj == toolsExportIcon){
      if(code == LV_EVENT_CLICKED) {    
           LV_LOG_USER("toolsExportIcon");
      }
  }
  if(data == toolsImportLabel){
    if(code == LV_EVENT_CLICKED) {  
        messagePopupCreate(messagePopupDetailTitle_text,importConfigAndProcessesMBox_text,NULL, NULL, NULL);
      }
  }
  if(data == toolsExportLabel){
    if(code == LV_EVENT_CLICKED) {  
    messagePopupCreate(messagePopupDetailTitle_text,exportConfigAndProcessesMBox_text, NULL, NULL, NULL);
    }
  }
  if(data == toolCreditButton){
    if(code == LV_EVENT_CLICKED) {  
    messagePopupCreate(softwareCredits_text,softwareCreditsValue_text, NULL, NULL, NULL);
    }
  }
}


static void initTools(void){
/*********************
 *    PAGE HEADER
 *********************/
  LV_LOG_USER("Tools Creation");
  toolsSection = lv_obj_create(lv_screen_active());
  lv_obj_set_pos(toolsSection, 140, 7);                         
  lv_obj_set_size(toolsSection, 335, 303); 
  //lv_obj_remove_flag(toolsSection, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_scroll_dir(toolsSection, LV_DIR_VER);
  lv_obj_set_style_border_color(toolsSection, lv_color_hex(BLUE_DARK), 0);  

 

  //lv_obj_update_layout(toolsSection);

  /*********************
 *    PAGE ELEMENTS
 *********************/

  toolsMaintenanceLabel = lv_label_create(toolsSection);         
  lv_label_set_text(toolsMaintenanceLabel, Maintenance_text); 
  lv_obj_set_style_text_font(toolsMaintenanceLabel, &lv_font_montserrat_28, 0);              
  lv_obj_align(toolsMaintenanceLabel, LV_ALIGN_TOP_LEFT, -7, -5);

  /*Create style*/
  lv_style_init(&style_sectionTitleLine);
  lv_style_set_line_width(&style_sectionTitleLine, 2);
  lv_style_set_line_color(&style_sectionTitleLine, lv_color_hex(LIGHT_BLUE));
  lv_style_set_line_rounded(&style_sectionTitleLine, true);

  /*Create a line and apply the new style*/
  sectionTitleLine = lv_line_create(toolsSection);
  lv_line_set_points(sectionTitleLine, sectionTitleLine_points, 2);
  lv_obj_add_style(sectionTitleLine, &style_sectionTitleLine, 0);
  lv_obj_align(sectionTitleLine, LV_ALIGN_TOP_MID, 0, 30);


  toolsCleaningContainer = lv_obj_create(toolsSection);
  lv_obj_align(toolsCleaningContainer, LV_ALIGN_TOP_LEFT, -15, 35);                         
  lv_obj_set_size(toolsCleaningContainer, 330, 50); 
  lv_obj_remove_flag(toolsCleaningContainer, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_scroll_dir(toolsCleaningContainer, LV_DIR_VER);    
  //lv_obj_set_style_border_color(toolsCleaningContainer, lv_color_hex(LV_PALETTE_GREEN), 0);
  lv_obj_set_style_border_opa(toolsCleaningContainer, LV_OPA_TRANSP, 0);

      toolsCleaningLabel = lv_label_create(toolsCleaningContainer);         
      lv_label_set_text(toolsCleaningLabel, cleanMachine_text); 
      lv_obj_set_style_text_font(toolsCleaningLabel, &lv_font_montserrat_20, 0);              
      lv_obj_align(toolsCleaningLabel, LV_ALIGN_LEFT_MID, -5, 0);

      toolsCleaningIcon = lv_label_create(toolsCleaningContainer);
      lv_label_set_text(toolsCleaningIcon, play_Icon);
      lv_obj_set_style_text_font(toolsCleaningIcon, &FilMachineFontIcons_30, 0);
      lv_obj_align(toolsCleaningIcon, LV_ALIGN_RIGHT_MID, 0, 0);
      lv_obj_add_flag(toolsCleaningIcon, LV_OBJ_FLAG_CLICKABLE);
      lv_obj_add_event_cb(toolsCleaningIcon, event_toolsElement, LV_EVENT_CLICKED, toolsCleaningIcon);



  toolsDrainingContainer = lv_obj_create(toolsSection);
  lv_obj_align(toolsDrainingContainer, LV_ALIGN_TOP_LEFT, -15, 85);                         
  lv_obj_set_size(toolsDrainingContainer, 330, 50); 
  lv_obj_remove_flag(toolsDrainingContainer, LV_OBJ_FLAG_SCROLLABLE); 
  lv_obj_set_scroll_dir(toolsCleaningContainer, LV_DIR_VER);     
  //lv_obj_set_style_border_color(toolsDrainingContainer, lv_color_hex(LV_PALETTE_GREEN), 0);
  lv_obj_set_style_border_opa(toolsDrainingContainer, LV_OPA_TRANSP, 0);

      toolsDrainingLabel = lv_label_create(toolsDrainingContainer);         
      lv_label_set_text(toolsDrainingLabel, drainMachine_text); 
      lv_obj_set_style_text_font(toolsDrainingLabel, &lv_font_montserrat_20, 0);              
      lv_obj_align(toolsDrainingLabel, LV_ALIGN_LEFT_MID, -5, 0);

      toolsDrainingIcon = lv_label_create(toolsDrainingContainer);
      lv_label_set_text(toolsDrainingIcon, play_Icon);
      lv_obj_set_style_text_font(toolsDrainingIcon, &FilMachineFontIcons_30, 0);
      lv_obj_align(toolsDrainingIcon, LV_ALIGN_RIGHT_MID, 0, 0);
      lv_obj_add_flag(toolsDrainingIcon, LV_OBJ_FLAG_CLICKABLE);
      lv_obj_add_event_cb(toolsDrainingIcon, event_toolsElement, LV_EVENT_CLICKED, toolsDrainingIcon);


  toolsUtilitiesLabel = lv_label_create(toolsSection);         
  lv_label_set_text(toolsUtilitiesLabel, Utilities_text); 
  lv_obj_set_style_text_font(toolsUtilitiesLabel, &lv_font_montserrat_28, 0);              
  lv_obj_align(toolsUtilitiesLabel, LV_ALIGN_TOP_LEFT, -7, 135);

  lv_style_init(&style_sectionTitleLine);
  lv_style_set_line_width(&style_sectionTitleLine, 2);
  lv_style_set_line_color(&style_sectionTitleLine, lv_color_hex(LIGHT_BLUE));
  lv_style_set_line_rounded(&style_sectionTitleLine, true);

  sectionTitleLine = lv_line_create(toolsSection);
  lv_line_set_points(sectionTitleLine, sectionTitleLine_points, 2);
  lv_obj_add_style(sectionTitleLine, &style_sectionTitleLine, 0);
  lv_obj_align(sectionTitleLine, LV_ALIGN_TOP_MID, 0, 170);


  
  toolsImportContainer = lv_obj_create(toolsSection);
  lv_obj_align(toolsImportContainer, LV_ALIGN_TOP_LEFT, -15, 175);                         
  lv_obj_set_size(toolsImportContainer, 330, 50); 
  lv_obj_remove_flag(toolsImportContainer, LV_OBJ_FLAG_SCROLLABLE); 
  lv_obj_set_scroll_dir(toolsImportContainer, LV_DIR_VER);     
  //lv_obj_set_style_border_color(toolsImportContainer, lv_color_hex(LV_PALETTE_GREEN), 0);
  lv_obj_set_style_border_opa(toolsImportContainer, LV_OPA_TRANSP, 0);

      toolsImportLabel = lv_label_create(toolsImportContainer);         
      lv_label_set_text(toolsImportLabel, importConfigAndProcesses_text); 
      lv_obj_set_style_text_font(toolsImportLabel, &lv_font_montserrat_20, 0);              
      lv_obj_align(toolsImportLabel, LV_ALIGN_LEFT_MID, -5, 0);

      createQuestionMark(toolsImportContainer,toolsImportLabel,event_toolsElement, 2, -3);

      toolsImportIcon = lv_label_create(toolsImportContainer);
      lv_label_set_text(toolsImportIcon, play_Icon);
      lv_obj_set_style_text_font(toolsImportIcon, &FilMachineFontIcons_30, 0);
      lv_obj_align(toolsImportIcon, LV_ALIGN_RIGHT_MID, 0, 0);
      lv_obj_add_flag(toolsImportIcon, LV_OBJ_FLAG_CLICKABLE);
      lv_obj_add_event_cb(toolsImportIcon, event_toolsElement, LV_EVENT_CLICKED, toolsImportIcon);



  toolsExportContainer = lv_obj_create(toolsSection);
  lv_obj_align(toolsExportContainer, LV_ALIGN_TOP_LEFT, -15, 225);                         
  lv_obj_set_size(toolsExportContainer, 330, 50); 
  lv_obj_remove_flag(toolsExportContainer, LV_OBJ_FLAG_SCROLLABLE);  
  lv_obj_set_scroll_dir(toolsExportContainer, LV_DIR_VER);    
  //lv_obj_set_style_border_color(toolsExportContainer, lv_color_hex(LV_PALETTE_GREEN), 0);
  lv_obj_set_style_border_opa(toolsExportContainer, LV_OPA_TRANSP, 0);

      toolsExportLabel = lv_label_create(toolsExportContainer);         
      lv_label_set_text(toolsExportLabel, exportConfigAndProcesses_text); 
      lv_obj_set_style_text_font(toolsExportLabel, &lv_font_montserrat_20, 0);              
      lv_obj_align(toolsExportLabel, LV_ALIGN_LEFT_MID, -5, 0);

      createQuestionMark(toolsExportContainer,toolsExportLabel,event_toolsElement, 2, -3);

      toolsExportIcon = lv_label_create(toolsExportContainer);
      lv_label_set_text(toolsExportIcon, play_Icon);
      lv_obj_set_style_text_font(toolsExportIcon, &FilMachineFontIcons_30, 0);
      lv_obj_align(toolsExportIcon, LV_ALIGN_RIGHT_MID, 0, 0);
      lv_obj_add_flag(toolsExportIcon, LV_OBJ_FLAG_CLICKABLE);
      lv_obj_add_event_cb(toolsExportIcon, event_toolsElement, LV_EVENT_CLICKED, toolsExportIcon);




  toolsStatisticsLabel = lv_label_create(toolsSection);         
  lv_label_set_text(toolsStatisticsLabel, Statistics_text); 
  lv_obj_set_style_text_font(toolsStatisticsLabel, &lv_font_montserrat_28, 0);              
  lv_obj_align(toolsStatisticsLabel, LV_ALIGN_TOP_LEFT, -7, 275);

  lv_style_init(&style_sectionTitleLine);
  lv_style_set_line_width(&style_sectionTitleLine, 2);
  lv_style_set_line_color(&style_sectionTitleLine, lv_color_hex(LIGHT_BLUE));
  lv_style_set_line_rounded(&style_sectionTitleLine, true);

  sectionTitleLine = lv_line_create(toolsSection);
  lv_line_set_points(sectionTitleLine, sectionTitleLine_points, 2);
  lv_obj_add_style(sectionTitleLine, &style_sectionTitleLine, 0);
  lv_obj_align(sectionTitleLine, LV_ALIGN_TOP_MID, 0, 310);


  toolsStatCompleteProcessesContainer = lv_obj_create(toolsSection);
  lv_obj_align(toolsStatCompleteProcessesContainer, LV_ALIGN_TOP_LEFT, -15, 315);                         
  lv_obj_set_size(toolsStatCompleteProcessesContainer, 330, 40); 
  lv_obj_remove_flag(toolsStatCompleteProcessesContainer, LV_OBJ_FLAG_SCROLLABLE);  
  lv_obj_set_scroll_dir(toolsStatCompleteProcessesContainer, LV_DIR_VER);    
  //lv_obj_set_style_border_color(toolsStatCompleteProcessesContainer, lv_color_hex(LV_PALETTE_GREEN), 0);
  lv_obj_set_style_border_opa(toolsStatCompleteProcessesContainer, LV_OPA_TRANSP, 0);

      toolStatCompletedProcessesLabel = lv_label_create(toolsStatCompleteProcessesContainer);         
      lv_label_set_text(toolStatCompletedProcessesLabel, statCompleteProcesses_text); 
      lv_obj_set_style_text_font(toolStatCompletedProcessesLabel, &lv_font_montserrat_20, 0);              
      lv_obj_align(toolStatCompletedProcessesLabel, LV_ALIGN_LEFT_MID, -5, 0);

      toolStatCompletedProcessesValue = lv_label_create(toolsStatCompleteProcessesContainer);         
      lv_label_set_text(toolStatCompletedProcessesValue, "2"); 
      lv_obj_set_style_text_font(toolStatCompletedProcessesValue, &lv_font_montserrat_20, 0);              
      lv_obj_align(toolStatCompletedProcessesValue, LV_ALIGN_RIGHT_MID, -5, 0);
     

  toolsStatTotalTimeContainer = lv_obj_create(toolsSection);
  lv_obj_align(toolsStatTotalTimeContainer, LV_ALIGN_TOP_LEFT, -15, 355);                         
  lv_obj_set_size(toolsStatTotalTimeContainer, 330, 40); 
  lv_obj_remove_flag(toolsStatTotalTimeContainer, LV_OBJ_FLAG_SCROLLABLE);  
  lv_obj_set_scroll_dir(toolsStatTotalTimeContainer, LV_DIR_VER);    
  //lv_obj_set_style_border_color(toolsStatTotalTimeContainer, lv_color_hex(LV_PALETTE_GREEN), 0);
  lv_obj_set_style_border_opa(toolsStatTotalTimeContainer, LV_OPA_TRANSP, 0);

      toolStatTotalTimeLabel = lv_label_create(toolsStatTotalTimeContainer);         
      lv_label_set_text(toolStatTotalTimeLabel, statTotalProcessTime_text); 
      lv_obj_set_style_text_font(toolStatTotalTimeLabel, &lv_font_montserrat_20, 0);              
      lv_obj_align(toolStatTotalTimeLabel, LV_ALIGN_LEFT_MID, -5, 0);

      toolStatTotalTimeValue = lv_label_create(toolsStatTotalTimeContainer);         
      lv_label_set_text(toolStatTotalTimeValue, "2"); 
      lv_obj_set_style_text_font(toolStatTotalTimeValue, &lv_font_montserrat_20, 0);              
      lv_obj_align(toolStatTotalTimeValue, LV_ALIGN_RIGHT_MID, -5, 0);


  toolsStatCompleteCycleContainer = lv_obj_create(toolsSection);
  lv_obj_align(toolsStatCompleteCycleContainer, LV_ALIGN_TOP_LEFT, -15, 395);                         
  lv_obj_set_size(toolsStatCompleteCycleContainer, 330, 40); 
  lv_obj_remove_flag(toolsStatCompleteCycleContainer, LV_OBJ_FLAG_SCROLLABLE);  
  lv_obj_set_scroll_dir(toolsStatCompleteCycleContainer, LV_DIR_VER);    
  //lv_obj_set_style_border_color(toolsStatCompleteCycleContainer, lv_color_hex(LV_PALETTE_GREEN), 0);
  lv_obj_set_style_border_opa(toolsStatCompleteCycleContainer, LV_OPA_TRANSP, 0);

      toolStatCompleteCycleLabel = lv_label_create(toolsStatCompleteCycleContainer);         
      lv_label_set_text(toolStatCompleteCycleLabel, statCompleteCleanProcess_text); 
      lv_obj_set_style_text_font(toolStatCompleteCycleLabel, &lv_font_montserrat_20, 0);              
      lv_obj_align(toolStatCompleteCycleLabel, LV_ALIGN_LEFT_MID, -5, 0);

      toolStatCompleteCycleValue = lv_label_create(toolsStatCompleteCycleContainer);         
      lv_label_set_text(toolStatCompleteCycleValue, "2"); 
      lv_obj_set_style_text_font(toolStatCompleteCycleValue, &lv_font_montserrat_20, 0);              
      lv_obj_align(toolStatCompleteCycleValue, LV_ALIGN_RIGHT_MID, -5, 0);


  toolsStatStoppedProcessesContainer = lv_obj_create(toolsSection);
  lv_obj_align(toolsStatStoppedProcessesContainer, LV_ALIGN_TOP_LEFT, -15, 435);                         
  lv_obj_set_size(toolsStatStoppedProcessesContainer, 330, 40); 
  lv_obj_remove_flag(toolsStatStoppedProcessesContainer, LV_OBJ_FLAG_SCROLLABLE);  
  lv_obj_set_scroll_dir(toolsStatStoppedProcessesContainer, LV_DIR_VER);    
  //lv_obj_set_style_border_color(toolsStatStoppedProcessesContainer, lv_color_hex(LV_PALETTE_GREEN), 0);
  lv_obj_set_style_border_opa(toolsStatStoppedProcessesContainer, LV_OPA_TRANSP, 0);

      toolStatStoppedProcessesLabel = lv_label_create(toolsStatStoppedProcessesContainer);         
      lv_label_set_text(toolStatStoppedProcessesLabel, statStoppedProcess_text); 
      lv_obj_set_style_text_font(toolStatStoppedProcessesLabel, &lv_font_montserrat_20, 0);              
      lv_obj_align(toolStatStoppedProcessesLabel, LV_ALIGN_LEFT_MID, -5, 0);

      toolStatStoppedProcessesValue = lv_label_create(toolsStatStoppedProcessesContainer);         
      lv_label_set_text(toolStatStoppedProcessesValue, "2"); 
      lv_obj_set_style_text_font(toolStatStoppedProcessesValue, &lv_font_montserrat_20, 0);              
      lv_obj_align(toolStatStoppedProcessesValue, LV_ALIGN_RIGHT_MID, -5, 0);



  toolsSoftwareLabel = lv_label_create(toolsSection);         
  lv_label_set_text(toolsSoftwareLabel, Software_text); 
  lv_obj_set_style_text_font(toolsSoftwareLabel, &lv_font_montserrat_28, 0);              
  lv_obj_align(toolsSoftwareLabel, LV_ALIGN_TOP_LEFT, -7, 475);

  lv_style_init(&style_sectionTitleLine);
  lv_style_set_line_width(&style_sectionTitleLine, 2);
  lv_style_set_line_color(&style_sectionTitleLine, lv_color_hex(LIGHT_BLUE));
  lv_style_set_line_rounded(&style_sectionTitleLine, true);

  sectionTitleLine = lv_line_create(toolsSection);
  lv_line_set_points(sectionTitleLine, sectionTitleLine_points, 2);
  lv_obj_add_style(sectionTitleLine, &style_sectionTitleLine, 0);
  lv_obj_align(sectionTitleLine, LV_ALIGN_TOP_MID, 0, 510);


  toolsSoftwareVersionContainer = lv_obj_create(toolsSection);
  lv_obj_align(toolsSoftwareVersionContainer, LV_ALIGN_TOP_LEFT, -15, 515);                         
  lv_obj_set_size(toolsSoftwareVersionContainer, 330, 40); 
  lv_obj_remove_flag(toolsSoftwareVersionContainer, LV_OBJ_FLAG_SCROLLABLE);  
  lv_obj_set_scroll_dir(toolsSoftwareVersionContainer, LV_DIR_VER);    
  //lv_obj_set_style_border_color(toolsSoftwareVersionContainer, lv_color_hex(LV_PALETTE_GREEN), 0);
  lv_obj_set_style_border_opa(toolsSoftwareVersionContainer, LV_OPA_TRANSP, 0);

      toolSoftWareVersionLabel = lv_label_create(toolsSoftwareVersionContainer);         
      lv_label_set_text(toolSoftWareVersionLabel, softwareVersion_text); 
      lv_obj_set_style_text_font(toolSoftWareVersionLabel, &lv_font_montserrat_20, 0);              
      lv_obj_align(toolSoftWareVersionLabel, LV_ALIGN_LEFT_MID, -5, 0);

      toolSoftwareVersionValue = lv_label_create(toolsSoftwareVersionContainer);         
      lv_label_set_text(toolSoftwareVersionValue, softwareVersionValue_text); 
      lv_obj_set_style_text_font(toolSoftwareVersionValue, &lv_font_montserrat_20, 0);              
      lv_obj_align(toolSoftwareVersionValue, LV_ALIGN_RIGHT_MID, -5, 0);


  toolsSoftwareSerialContainer = lv_obj_create(toolsSection);
  lv_obj_align(toolsSoftwareSerialContainer, LV_ALIGN_TOP_LEFT, -15, 555);                         
  lv_obj_set_size(toolsSoftwareSerialContainer, 330, 40); 
  lv_obj_remove_flag(toolsSoftwareSerialContainer, LV_OBJ_FLAG_SCROLLABLE);  
  lv_obj_set_scroll_dir(toolsSoftwareSerialContainer, LV_DIR_VER);    
  //lv_obj_set_style_border_color(toolsSoftwareSerialContainer, lv_color_hex(LV_PALETTE_GREEN), 0);
  lv_obj_set_style_border_opa(toolsSoftwareSerialContainer, LV_OPA_TRANSP, 0);

      toolSoftwareSerialLabel = lv_label_create(toolsSoftwareSerialContainer);         
      lv_label_set_text(toolSoftwareSerialLabel, softwareSerialNum_text); 
      lv_obj_set_style_text_font(toolSoftwareSerialLabel, &lv_font_montserrat_20, 0);              
      lv_obj_align(toolSoftwareSerialLabel, LV_ALIGN_LEFT_MID, -5, 0);

      toolSoftwareSerialValue = lv_label_create(toolsSoftwareSerialContainer);         
      lv_label_set_text(toolSoftwareSerialValue, softwareSerialNumValue_text); 
      lv_obj_set_style_text_font(toolSoftwareSerialValue, &lv_font_montserrat_20, 0);              
      lv_obj_align(toolSoftwareSerialValue, LV_ALIGN_RIGHT_MID, -5, 0);

  toolCreditButton = lv_button_create(toolsSection);
  lv_obj_set_size(toolCreditButton, BUTTON_TUNE_WIDTH, BUTTON_TUNE_HEIGHT);
  lv_obj_align(toolCreditButton, LV_ALIGN_TOP_MID, 5 , 595);
  lv_obj_add_event_cb(toolCreditButton, event_toolsElement, LV_EVENT_CLICKED, toolCreditButton);
  lv_obj_set_style_bg_color(toolCreditButton, lv_color_hex(LIGHT_BLUE), LV_PART_MAIN);

  toolCreditButtonLabel = lv_label_create(toolCreditButton);
  lv_label_set_text(toolCreditButtonLabel, softwareCredits_text);
  lv_obj_set_style_text_font(toolCreditButtonLabel, &lv_font_montserrat_18, 0);
  lv_obj_align(toolCreditButtonLabel, LV_ALIGN_CENTER, 0, 0);
}

void tools(void)
{   
  if(toolsSection == NULL){
    initTools();
  }

  lv_obj_clear_flag(toolsSection, LV_OBJ_FLAG_HIDDEN);
  lv_style_set_line_color(&style_sectionTitleLine, lv_palette_main(LV_PALETTE_BLUE));
}



#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*PAGE_TOOLS_H*/