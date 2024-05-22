#include "misc/lv_area.h"
/**
 * @file element_messagePopup.c
 *
 */


//ESSENTIAL INCLUDES
#include <lvgl.h>
#include "../../include/definitions.h"

extern struct gui_components gui;

//ACCESSORY INCLUDES


void event_messagePopup(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = (lv_obj_t *)lv_event_get_target(e);
    lv_obj_t *objCont = (lv_obj_t *)lv_obj_get_parent(obj);
    lv_obj_t *mboxCont = (lv_obj_t *)lv_obj_get_parent(objCont);
    lv_obj_t *mboxParent = (lv_obj_t *)lv_obj_get_parent(mboxCont);

    lv_obj_t *btn = (lv_obj_t *)lv_event_get_current_target(e);
    lv_obj_t *mbox = (lv_obj_t *)lv_obj_get_parent(lv_obj_get_parent(btn));

    if (code == LV_EVENT_CLICKED)
    {
        if (obj == gui.element.messagePopup.mBoxPopupButtonClose)
        {
            LV_LOG_USER("Pressed gui.element.messagePopup.mBoxPopupButtonClose");
            lv_style_reset(&gui.element.messagePopup.style_mBoxPopupTitleLine);
            lv_msgbox_close(mboxCont);
            lv_obj_delete(mboxCont);
        }
        if (obj == gui.element.messagePopup.mBoxPopupButton1)
        {
            LV_LOG_USER("Pressed gui.element.messagePopup.mBoxPopupButton1");
            if (gui.element.messagePopup.whoCallMe == tempProcessNode)
            {
                LV_LOG_USER("Delete process from long press!");

                if( !deleteProcessElement( gui.element.messagePopup.whoCallMe ) ) {
                  LV_LOG_USER("Delete process element instance at address 0x%p Failed!", gui.element.messagePopup.whoCallMe);
                } else {
                  LV_LOG_USER("Delete process element instance at address 0x%p", gui.element.messagePopup.whoCallMe);
                }

                lv_style_reset(&gui.element.messagePopup.style_mBoxPopupTitleLine);
                lv_msgbox_close(mboxCont);
                lv_obj_delete(mboxCont);
                gui.element.messagePopup.mBoxPopupParent = NULL;
            }

            if (gui.element.messagePopup.whoCallMe == tempStepNode)
            {
                LV_LOG_USER("Delete step from long Step press!");
                
                if( !deleteStepElement( gui.element.messagePopup.whoCallMe , tempProcessNode) ) {
                  LV_LOG_USER("Delete step element instance at address 0x%p Failed!", gui.element.messagePopup.whoCallMe);
                } else {
                  LV_LOG_USER("Delete step element instance at address 0x%p", gui.element.messagePopup.whoCallMe);
                }
                
                lv_style_reset(&gui.element.messagePopup.style_mBoxPopupTitleLine);
                lv_msgbox_close(mboxCont);
                lv_obj_delete(mboxCont);
                gui.element.messagePopup.mBoxPopupParent = NULL;
            }

            if (gui.element.messagePopup.whoCallMe == processDetailParent)
            {
                LV_LOG_USER("Delete process from inside!");
                lv_style_reset(&gui.element.messagePopup.style_mBoxPopupTitleLine);
                lv_msgbox_close(mboxCont);
                lv_obj_delete(mboxCont);
                gui.element.messagePopup.mBoxPopupParent = NULL;

                lv_msgbox_close(processDetailParent);
                lv_obj_delete(processDetailParent);
                processDetailParent = NULL;
            }
        }
        if (obj == gui.element.messagePopup.mBoxPopupButton2)
        {
            LV_LOG_USER("Pressed gui.element.messagePopup.mBoxPopupButton2");
            if (gui.element.messagePopup.whoCallMe == processElement || gui.element.messagePopup.whoCallMe == processDetailParent || gui.element.messagePopup.whoCallMe == stepElement)
            {
                LV_LOG_USER("Cancel delete element!");
                lv_style_reset(&gui.element.messagePopup.style_mBoxPopupTitleLine);
                lv_msgbox_close(mboxCont);
                lv_obj_delete(mboxCont);
                gui.element.messagePopup.mBoxPopupParent = NULL;
            }
        }
    }

}       

void messagePopupCreate(const char * popupTitleText,const char * popupText,const char * textButton1, const char * textButton2, lv_obj_t * whoCallMe){
  /*********************
  *    PAGE HEADER
  *********************/
   gui.element.messagePopup.whoCallMe = whoCallMe;

   LV_LOG_USER("Message popup create");  
   gui.element.messagePopup.mBoxPopupParent = lv_obj_class_create_obj(&lv_msgbox_backdrop_class, lv_layer_top());
   lv_obj_class_init_obj(gui.element.messagePopup.mBoxPopupParent);
   lv_obj_remove_flag(gui.element.messagePopup.mBoxPopupParent, LV_OBJ_FLAG_IGNORE_LAYOUT);
   lv_obj_set_size(gui.element.messagePopup.mBoxPopupParent, LV_PCT(100), LV_PCT(100));

         gui.element.messagePopup.mBoxPopupContainer = lv_obj_create(gui.element.messagePopup.mBoxPopupParent);
         lv_obj_align(gui.element.messagePopup.mBoxPopupContainer, LV_ALIGN_CENTER, 0, 0);
         lv_obj_set_size(gui.element.messagePopup.mBoxPopupContainer, 320, 240); 
         lv_obj_remove_flag(gui.element.messagePopup.mBoxPopupContainer, LV_OBJ_FLAG_SCROLLABLE); 

         gui.element.messagePopup.mBoxPopupTitle = lv_label_create(gui.element.messagePopup.mBoxPopupContainer);         
         lv_label_set_text(gui.element.messagePopup.mBoxPopupTitle, popupTitleText); 
         lv_obj_set_style_text_font(gui.element.messagePopup.mBoxPopupTitle, &lv_font_montserrat_22, 0);              
         lv_obj_align(gui.element.messagePopup.mBoxPopupTitle, LV_ALIGN_TOP_MID, 0, - 10);


   /*Create style*/
   lv_style_init(&gui.element.messagePopup.style_mBoxPopupTitleLine);
   lv_style_set_line_width(&gui.element.messagePopup.style_mBoxPopupTitleLine, 2);
   lv_style_set_line_rounded(&gui.element.messagePopup.style_mBoxPopupTitleLine, true);

   /*Create a line and apply the new style*/
   gui.element.messagePopup.mBoxPopupTitleLine = lv_line_create(gui.element.messagePopup.mBoxPopupContainer);
   lv_line_set_points(gui.element.messagePopup.mBoxPopupTitleLine, gui.element.messagePopup.titleLinePoints, 2);
   lv_obj_add_style(gui.element.messagePopup.mBoxPopupTitleLine, &gui.element.messagePopup.style_mBoxPopupTitleLine, 0);
   lv_obj_align(gui.element.messagePopup.mBoxPopupTitleLine, LV_ALIGN_TOP_MID, 0, 23);


  /*********************
  *    PAGE ELEMENTS
  *********************/

   gui.element.messagePopup.mBoxPopupTextContainer = lv_obj_create(gui.element.messagePopup.mBoxPopupContainer);
   lv_obj_align(gui.element.messagePopup.mBoxPopupTextContainer, LV_ALIGN_TOP_MID, 0, 30);
   lv_obj_set_size(gui.element.messagePopup.mBoxPopupTextContainer, 305, 180); 
   lv_obj_set_style_border_opa(gui.element.messagePopup.mBoxPopupTextContainer, LV_OPA_TRANSP, 0);
   //lv_obj_set_style_border_color(gui.element.messagePopup.mBoxPopupTextContainer, lv_color_hex(GREEN_DARK), 0);
   lv_obj_set_scroll_dir(gui.element.messagePopup.mBoxPopupTextContainer, LV_DIR_VER);

         gui.element.messagePopup.mBoxPopupText = lv_label_create(gui.element.messagePopup.mBoxPopupTextContainer);         
         lv_label_set_text(gui.element.messagePopup.mBoxPopupText, popupText); 
         lv_obj_set_style_text_font(gui.element.messagePopup.mBoxPopupText, &lv_font_montserrat_20, 0);              
         lv_obj_align(gui.element.messagePopup.mBoxPopupText, LV_ALIGN_CENTER, 0, -18);
         lv_obj_set_size(gui.element.messagePopup.mBoxPopupText, 295, LV_SIZE_CONTENT);
         lv_label_set_long_mode(gui.element.messagePopup.mBoxPopupText, LV_LABEL_LONG_WRAP);
         lv_obj_set_style_text_align(gui.element.messagePopup.mBoxPopupText , LV_TEXT_ALIGN_CENTER, 0);

  if(gui.element.messagePopup.whoCallMe == NULL){
      gui.element.messagePopup.mBoxPopupButtonClose = lv_button_create(gui.element.messagePopup.mBoxPopupContainer);
      lv_obj_set_size(gui.element.messagePopup.mBoxPopupButtonClose, BUTTON_POPUP_CLOSE_WIDTH, BUTTON_POPUP_CLOSE_HEIGHT);
      lv_obj_align(gui.element.messagePopup.mBoxPopupButtonClose, LV_ALIGN_TOP_RIGHT, 7 , -10);
      lv_obj_add_event_cb(gui.element.messagePopup.mBoxPopupButtonClose, event_messagePopup, LV_EVENT_CLICKED, gui.element.filterPopup.mBoxApplyFilterButton);

            gui.element.messagePopup.mBoxPopupButtonLabel = lv_label_create(gui.element.messagePopup.mBoxPopupButtonClose);         
            lv_label_set_text(gui.element.messagePopup.mBoxPopupButtonLabel, closePopup_icon); 
            lv_obj_set_style_text_font(gui.element.messagePopup.mBoxPopupButtonLabel, &FilMachineFontIcons_20, 0);              
            lv_obj_align(gui.element.messagePopup.mBoxPopupButtonLabel, LV_ALIGN_CENTER, 0, 0);
  }
  else{
      gui.element.messagePopup.mBoxPopupButton1 = lv_button_create(gui.element.messagePopup.mBoxPopupContainer);
      lv_obj_set_size(gui.element.messagePopup.mBoxPopupButton1, BUTTON_MBOX_WIDTH, BUTTON_MBOX_HEIGHT);
      lv_obj_align(gui.element.messagePopup.mBoxPopupButton1, LV_ALIGN_BOTTOM_LEFT, 10 , 10);
      lv_obj_add_event_cb(gui.element.messagePopup.mBoxPopupButton1, event_messagePopup, LV_EVENT_CLICKED, gui.element.messagePopup.mBoxPopupButton1);
      lv_obj_set_style_bg_color(gui.element.messagePopup.mBoxPopupButton1, lv_color_hex(RED_DARK), LV_PART_MAIN);

          gui.element.messagePopup.mBoxPopupButton1Label = lv_label_create(gui.element.messagePopup.mBoxPopupButton1);
          lv_label_set_text(gui.element.messagePopup.mBoxPopupButton1Label, textButton1);
          lv_obj_set_style_text_font(gui.element.messagePopup.mBoxPopupButton1Label, &lv_font_montserrat_24, 0);
          lv_obj_align(gui.element.messagePopup.mBoxPopupButton1Label, LV_ALIGN_CENTER, 0, 0);


      gui.element.messagePopup.mBoxPopupButton2 = lv_button_create(gui.element.messagePopup.mBoxPopupContainer);
      lv_obj_set_size(gui.element.messagePopup.mBoxPopupButton2, BUTTON_MBOX_WIDTH, BUTTON_MBOX_HEIGHT);
      lv_obj_align(gui.element.messagePopup.mBoxPopupButton2, LV_ALIGN_BOTTOM_RIGHT, - 10 , 10);
      lv_obj_add_event_cb(gui.element.messagePopup.mBoxPopupButton2, event_messagePopup, LV_EVENT_CLICKED, gui.element.messagePopup.mBoxPopupButton2);
      lv_obj_set_style_bg_color(gui.element.messagePopup.mBoxPopupButton2, lv_color_hex(GREEN_DARK), LV_PART_MAIN);

            gui.element.messagePopup.mBoxPopupButton2Label = lv_label_create(gui.element.messagePopup.mBoxPopupButton2);
            lv_label_set_text(gui.element.messagePopup.mBoxPopupButton2Label, textButton2);
            lv_obj_set_style_text_font(gui.element.messagePopup.mBoxPopupButton2Label, &lv_font_montserrat_24, 0);
            lv_obj_align(gui.element.messagePopup.mBoxPopupButton2Label, LV_ALIGN_CENTER, 0, 0);
  }
}

