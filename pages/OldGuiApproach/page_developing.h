#include "core/lv_obj.h"
#include "misc/lv_palette.h"
/**
 * @file page_developing.h
 *
 */


#ifndef PAGE_DEVELOPING_H
#define PAGE_DEVELOPING_H

#ifdef __cplusplus
extern "C" {
#endif

//ESSENTIAL INCLUDE

//ACCESSORY INCLUDES



static void event_develop_style_delete(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_DELETE) {
        lv_style_reset(&style_btn_back_develop);
        lv_style_reset(&style_progress_bargraph);
        lv_style_reset(&style_progress_bargraph_bg);
        lv_style_reset(&style_cycle_bargraph);
        lv_style_reset(&style_cycle_bargraph_bg);
        lv_style_reset(&style_step_bargraph);
        lv_style_reset(&style_step_bargraph_bg);
    }
}



void developing(void)
{   lv_obj_del(lv_screen_active());
    screen_developing = lv_obj_create(NULL);
    lv_scr_load(screen_developing);

    lv_obj_set_style_bg_color(screen_developing, LV_COLOR_MAKE(255, 255, 255), LV_PART_MAIN);


/*********************
 *    PAGE HEADER
 *********************/

    lv_obj_t * title = lv_label_create(screen_developing);
    lv_label_set_text(title, dev_clean_developing_title_txt);
    lv_obj_set_style_text_color(screen_developing, lv_color_hex(BLACK), LV_PART_MAIN);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_22, 0);
    lv_obj_align(title, LV_ALIGN_TOP_LEFT, 10 , 10);  

    lv_obj_t * subTitle = lv_label_create(screen_developing);
    lv_label_set_text(subTitle, dev_clean_info_txt);
    lv_obj_set_style_text_color(screen_developing, lv_color_hex(BLACK), LV_PART_MAIN);
    lv_obj_set_style_text_font(subTitle, &lv_font_montserrat_18, 0);
    lv_obj_align(subTitle, LV_ALIGN_TOP_LEFT, 10 , 30);  

    remainingTime = lv_label_create(screen_developing);
    lv_label_set_text(remainingTime, "31:04 min");
    lv_obj_set_style_text_color(remainingTime, lv_color_hex(RED), LV_PART_MAIN);
    lv_obj_set_style_text_font(remainingTime, &lv_font_montserrat_18, 0);
    lv_obj_align(remainingTime, LV_ALIGN_TOP_LEFT, 180 , 32);     

    lv_obj_t * btn_back = lv_button_create(screen_developing);
    lv_obj_set_size(btn_back, 46, 46);
    lv_obj_align(btn_back, LV_ALIGN_TOP_RIGHT, -10 , 10);    
    lv_obj_add_event_cb(btn_back, event_btn_back, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(btn_back, event_develop_style_delete, LV_EVENT_DELETE, NULL);
    
    lv_obj_t * label_btn_back = lv_label_create(btn_back);
    lv_label_set_text(label_btn_back, LV_SYMBOL_LEFT);
    lv_obj_align(label_btn_back, LV_ALIGN_CENTER, 0, 0);

    lv_style_init(&style_btn_back_develop);
    lv_style_set_bg_color(&style_btn_back_develop, lv_color_hex(GREEN));
    lv_style_set_border_color(&style_btn_back_develop, lv_color_hex(GREEN_DARK));
    lv_style_set_border_width(&style_btn_back_develop, 3);
    lv_style_set_shadow_width(&style_btn_back_develop, 10);
    lv_style_set_shadow_offset_y(&style_btn_back_develop, 5);
    lv_style_set_shadow_opa(&style_btn_back_develop, LV_OPA_50);
    lv_obj_add_style(btn_back, &style_btn_back_develop, 0);

   

/*********************
 *    PAGE ELEMENTS
 *********************/


    totalProgress_bar = lv_bar_create(screen_developing);
    lv_obj_set_size(totalProgress_bar, 400, 20);
    lv_obj_set_pos(totalProgress_bar, 10, 130); 

    lv_style_init(&style_progress_bargraph);
    lv_style_set_bg_opa(&style_progress_bargraph, LV_OPA_COVER);
    //lv_style_set_bg_color(&style_bargraph, lv_palette_main(LV_PALETTE_BLUE));
    lv_obj_set_style_bg_color(totalProgress_bar, lv_palette_main(LV_PALETTE_GREEN), LV_PART_INDICATOR);
    lv_obj_add_style(totalProgress_bar, &style_progress_bargraph, LV_PART_INDICATOR);


    lv_style_init(&style_progress_bargraph_bg);
    lv_style_set_border_color(&style_progress_bargraph_bg, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_border_width(&style_progress_bargraph_bg, 2);
    lv_style_set_pad_all(&style_progress_bargraph_bg, 6);
    lv_style_set_radius(&style_progress_bargraph_bg, 6);
    lv_obj_add_style(totalProgress_bar, &style_progress_bargraph_bg, 0);
   
    lv_obj_t * totalProgress_label = lv_label_create(screen_developing);
    lv_label_set_text(totalProgress_label, dev_clean_total);
    lv_obj_set_style_text_font(totalProgress_label, &lv_font_montserrat_22, 0);
    lv_obj_set_pos(totalProgress_label, 10, 100);

    totalProgress_value = lv_label_create(screen_developing);
    lv_obj_set_style_text_font(totalProgress_value, &lv_font_montserrat_22, 0);
    lv_obj_set_pos(totalProgress_value, 420, 128);



    cycleProgress_bar = lv_bar_create(screen_developing);
    lv_obj_set_size(cycleProgress_bar, 400, 20);
    lv_obj_set_pos(cycleProgress_bar, 10, 130 + 70);

    lv_style_init(&style_cycle_bargraph);
    lv_style_set_bg_opa(&style_cycle_bargraph, LV_OPA_COVER);
    //lv_style_set_bg_color(&style_bargraph, lv_palette_main(LV_PALETTE_BLUE));
    lv_obj_set_style_bg_color(cycleProgress_bar, lv_palette_main(LV_PALETTE_BLUE), LV_PART_INDICATOR);
    lv_obj_add_style(cycleProgress_bar, &style_cycle_bargraph, LV_PART_INDICATOR);


    lv_style_init(&style_cycle_bargraph_bg);
    lv_style_set_border_color(&style_cycle_bargraph_bg, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_border_width(&style_cycle_bargraph_bg, 2);
    lv_style_set_pad_all(&style_cycle_bargraph_bg, 6);
    lv_style_set_radius(&style_cycle_bargraph_bg, 6);
    lv_obj_add_style(cycleProgress_bar, &style_cycle_bargraph_bg, 0);
   
    lv_obj_t * cycleProgress_label = lv_label_create(screen_developing);
    lv_label_set_text(cycleProgress_label, dev_clean_total);
    lv_obj_set_style_text_font(cycleProgress_label, &lv_font_montserrat_22, 0);
    lv_obj_set_pos(cycleProgress_label, 10, 100 + 70);

    cycleProgress_value = lv_label_create(screen_developing);
    lv_obj_set_style_text_font(cycleProgress_value, &lv_font_montserrat_22, 0);
    lv_obj_set_pos(cycleProgress_value, 420, 128 + 70);



    stepProgress_bar = lv_bar_create(screen_developing);
    lv_obj_set_size(stepProgress_bar, 400, 20);
    lv_obj_set_pos(stepProgress_bar, 10, 130 + 140);

    lv_style_init(&style_step_bargraph);
    lv_style_set_bg_opa(&style_step_bargraph, LV_OPA_COVER);
    //lv_style_set_bg_color(&style_bargraph, lv_palette_main(LV_PALETTE_BLUE));
    lv_obj_set_style_bg_color(stepProgress_bar, lv_palette_main(LV_PALETTE_YELLOW), LV_PART_INDICATOR);
    lv_obj_add_style(stepProgress_bar, &style_step_bargraph, LV_PART_INDICATOR);


    lv_style_init(&style_step_bargraph_bg);
    lv_style_set_border_color(&style_step_bargraph_bg, lv_palette_main(LV_PALETTE_BLUE));
    lv_style_set_border_width(&style_step_bargraph_bg, 2);
    lv_style_set_pad_all(&style_step_bargraph_bg, 6);
    lv_style_set_radius(&style_step_bargraph_bg, 6);
    lv_obj_add_style(stepProgress_bar, &style_step_bargraph_bg, 0);
   
    lv_obj_t * stepProgress_label = lv_label_create(screen_developing);
    lv_label_set_text(stepProgress_label, dev_clean_total);
    lv_obj_set_style_text_font(stepProgress_label, &lv_font_montserrat_22, 0);
    lv_obj_set_pos(stepProgress_label, 10, 100 + 140);

    stepProgress_value = lv_label_create(screen_developing);
    lv_obj_set_style_text_font(stepProgress_value, &lv_font_montserrat_22, 0);
    lv_obj_set_pos(stepProgress_value, 420, 128 + 140);




    createMessageBox(dev_clean_developing_title_txt, dev_clean_developing_Complete , buttonCancel_label, buttonOK_label);
}




#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*PAGE_DEVELOPING_H*/