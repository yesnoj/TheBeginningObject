#include "display/lv_display.h"
#include "core/lv_obj_style_gen.h"

/**
 * @file page_films.h
 *
 */

#include "core/lv_obj_style.h"
#include "misc/lv_area.h"

#ifndef PAGE_FILMS_H
#define PAGE_FILMS_H

#ifdef __cplusplus
extern "C" {
#endif

//ESSENTIAL INCLUDES

//ACCESSORY INCLUDES

static lv_style_t style;


static void event_btn_selectFilm(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);

    lv_obj_t * filmSelected = (lv_obj_t *)lv_event_get_target(e);
    lv_obj_t * cont = (lv_obj_t *)lv_event_get_current_target(e);
    if(filmSelected == cont) return;
    
   
     switch(code) {
        case LV_EVENT_CLICKED:
            previousState = currentState;
            currentState = Display_State_2_Film;
            LV_LOG_USER("FILM Button PRESSED, new current state: %d",currentState);
            LV_LOG_USER("FILM Button PRESSED ID: %d",lv_obj_get_index(filmSelected));
            //isEnteringInPage = 1;
            lv_obj_set_style_bg_color(filmSelected, lv_palette_main(LV_PALETTE_RED), 0);
            break;
        case LV_EVENT_LONG_PRESSED:
            LV_LOG_USER("FILM Button LONG PRESSED, new current state: %d",currentState);
            lv_obj_set_style_bg_color(filmSelected, lv_palette_main(LV_PALETTE_RED), 0);
            LV_LOG_USER("FILM Button LONG PRESSED ID: %d",lv_obj_get_index(filmSelected));
            break;
        default:
            break;
    }
    
}


static void event_films_style_delete(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_DELETE) {
        //delete styles to prevent memoryleak
        lv_style_reset(&style_btn_back_films);
        lv_style_reset(&labelStyle_btn_generic);
        lv_style_reset(&style);
        lv_style_reset(&style_btn_generic);
    }
}

//LV_SYMBOL_LEFT

void filmsPage()
{  
    lv_obj_del(lv_screen_active());
    screen_filmsPage = lv_obj_create(NULL);
    lv_scr_load(screen_filmsPage);

    lv_obj_set_style_bg_color(screen_filmsPage, LV_COLOR_MAKE(255, 255, 255), LV_PART_MAIN);

/*********************
 *    PAGE HEADER
 *********************/



    lv_obj_t * title = lv_label_create(screen_filmsPage);
    lv_label_set_text(title, dev_clean_film_title_text);
    lv_obj_set_style_text_color(lv_screen_active(), lv_color_hex(BLACK), LV_PART_MAIN);
    lv_obj_set_style_text_font(title, &lv_font_montserrat_22, 0);
    lv_obj_align(title, LV_ALIGN_TOP_LEFT, 10 , 10);  


    lv_obj_t * btn_back = lv_button_create(screen_filmsPage);
    lv_obj_set_size(btn_back, 46, 46);
    //lv_obj_set_pos(btn_back, 320, 210); 
    lv_obj_align(btn_back, LV_ALIGN_TOP_RIGHT, -10 , 10);    
    lv_obj_add_event_cb(btn_back, event_btn_back, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(btn_back, event_films_style_delete, LV_EVENT_DELETE, NULL);



    lv_obj_t * label_btn_back = lv_label_create(btn_back);
    lv_label_set_text(label_btn_back, LV_SYMBOL_LEFT);
    lv_obj_align(label_btn_back, LV_ALIGN_CENTER, 0, 0);


    lv_style_init(&style_btn_back_films);
    lv_style_set_bg_color(&style_btn_back_films, lv_color_hex(GREEN));
    lv_style_set_border_color(&style_btn_back_films, lv_color_hex(GREEN_DARK));
    lv_style_set_border_width(&style_btn_back_films, 3);
    lv_style_set_shadow_width(&style_btn_back_films, 10);
    lv_style_set_shadow_offset_y(&style_btn_back_films, 5);
    lv_style_set_shadow_opa(&style_btn_back_films, LV_OPA_50);
    lv_obj_add_style(btn_back, &style_btn_back_films, 0);



/*********************
 *    PAGE ELEMENTS
 *********************/
    
    
    lv_style_init(&style);
    lv_style_set_flex_flow(&style, LV_FLEX_FLOW_ROW_WRAP);
    lv_style_set_flex_main_place(&style, LV_FLEX_ALIGN_SPACE_EVENLY);
    lv_style_set_layout(&style, LV_LAYOUT_FLEX);

    lv_obj_t * cont = lv_obj_create(lv_screen_active());
    lv_obj_set_size(cont, 480, 230);
    lv_obj_align(cont, LV_ALIGN_TOP_LEFT, 0 , 80);   
    lv_obj_add_style(cont, &style, 0);
    lv_obj_set_style_bg_color(cont, LV_COLOR_MAKE(255, 255, 255), LV_PART_MAIN);
    lv_obj_set_style_border_color(cont, LV_COLOR_MAKE(255, 255, 255), LV_PART_MAIN);


    for(uint32_t i = 0; i < 6; i++) {
      lv_obj_t * buttonCustom = lv_button_create(cont);
    //lv_obj_assign_id(buttonCustom, 1);
      lv_obj_set_size(buttonCustom, BUTTON_START_WIDTH  , BUTTON_START_HEIGHT);
      lv_obj_add_flag(buttonCustom, LV_OBJ_FLAG_EVENT_BUBBLE);

/*
      lv_obj_t * label = lv_label_create(buttonCustom);
      lv_label_set_text_fmt(label, "%"LV_PRIu32, i);
      lv_obj_center(label);
*/

      lv_style_init(&style_btn_generic);
      lv_style_set_bg_color(&style_btn_generic, lv_color_hex(LIGHT_BLUE));
      lv_style_set_border_color(&style_btn_generic, lv_color_hex(LIGHT_BLUE_DARK));
      lv_style_set_border_width(&style_btn_generic, 3);
      lv_style_set_shadow_width(&style_btn_generic, 10);
      lv_style_set_shadow_offset_y(&style_btn_generic, 5);
      lv_style_set_shadow_opa(&style_btn_generic, LV_OPA_50);
      lv_obj_add_style(buttonCustom, &style_btn_generic, 0);

      lv_style_init(&labelStyle_btn_generic);
      lv_obj_t * label_btn_generic = lv_label_create(buttonCustom);
      lv_label_set_text(label_btn_generic, buttonCustom_label);
      lv_obj_set_style_text_font(label_btn_generic, &lv_font_montserrat_22, 0);
      lv_style_set_text_color(&labelStyle_btn_generic, lv_color_hex(WHITE));
      lv_obj_add_style(label_btn_generic, &labelStyle_btn_generic, 0);
      lv_obj_align(label_btn_generic, LV_ALIGN_CENTER, 0, 0);    
    }
lv_obj_add_event_cb(cont, event_btn_selectFilm, LV_EVENT_ALL, NULL);



/*
    lv_obj_t * btn_tankSize_600 = lv_button_create(screen_filmsPage);
    lv_obj_set_size(btn_tankSize_600, BUTTON_START_WIDTH, BUTTON_START_HEIGHT);
    //lv_obj_set_pos(btn_back, 320, 210); 
    lv_obj_align(btn_tankSize_600, LV_ALIGN_LEFT_MID, 10 , 0);    
    lv_obj_add_event_cb(btn_tankSize_600, event_btn_selectFilm, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(btn_tankSize_600, event_tankSize_style_delete, LV_EVENT_DELETE, NULL);

    lv_style_init(&style_btn_clean_600);
    lv_style_set_bg_color(&style_btn_clean_600, lv_color_hex(LIGHT_BLUE));
    lv_style_set_border_color(&style_btn_clean_600, lv_color_hex(LIGHT_BLUE_DARK));
    lv_style_set_border_width(&style_btn_clean_600, 3);
    lv_style_set_shadow_width(&style_btn_clean_600, 10);
    lv_style_set_shadow_offset_y(&style_btn_clean_600, 5);
    lv_style_set_shadow_opa(&style_btn_clean_600, LV_OPA_50);
    lv_obj_add_style(btn_tankSize_600, &style_btn_clean_600, 0);

    lv_style_init(&labelStyle_clean_600);
    lv_obj_t * label_btn_clean_600 = lv_label_create(btn_tankSize_600);
    lv_label_set_text(label_btn_clean_600, buttonCustom);
    lv_obj_set_style_text_font(label_btn_clean_600, &lv_font_montserrat_22, 0);
    lv_style_set_text_color(&labelStyle_clean_600, lv_color_hex(WHITE));
    lv_obj_add_style(label_btn_clean_600, &labelStyle_clean_600, 0);
    lv_obj_align(label_btn_clean_600, LV_ALIGN_CENTER, 0, 0);

    

    lv_obj_t * btn_tankSize_1000 = lv_button_create(screen_filmsPage);
    lv_obj_set_size(btn_tankSize_1000, BUTTON_START_WIDTH, BUTTON_START_HEIGHT);
    //lv_obj_set_pos(btn_back, 320, 210); 
    lv_obj_align(btn_tankSize_1000, LV_ALIGN_CENTER, 0 , 0);
    lv_obj_add_event_cb(btn_tankSize_1000, event_btn_selectFilm, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(btn_tankSize_1000, event_tankSize_style_delete, LV_EVENT_DELETE, NULL);

    lv_style_init(&style_btn_clean_1000);
    lv_style_set_bg_color(&style_btn_clean_1000, lv_color_hex(LIGHT_BLUE));
    lv_style_set_border_color(&style_btn_clean_1000, lv_color_hex(LIGHT_BLUE_DARK));
    lv_style_set_border_width(&style_btn_clean_1000, 3);
    lv_style_set_shadow_width(&style_btn_clean_1000, 10);
    lv_style_set_shadow_offset_y(&style_btn_clean_1000, 5);
    lv_style_set_shadow_opa(&style_btn_clean_1000, LV_OPA_50);
    lv_obj_add_style(btn_tankSize_1000, &style_btn_clean_1000, 0);


    lv_style_init(&labelStyle_clean_1000);
    lv_obj_t * label_btn_clean_1000 = lv_label_create(btn_tankSize_1000);
    lv_label_set_text(label_btn_clean_1000, buttonCustom);
    lv_obj_set_style_text_font(label_btn_clean_1000, &lv_font_montserrat_22, 0);
    lv_style_set_text_color(&labelStyle_clean_1000, lv_color_hex(WHITE));
    lv_obj_add_style(label_btn_clean_1000, &labelStyle_clean_1000, 0);
    lv_obj_align(label_btn_clean_1000, LV_ALIGN_CENTER, 0, 0);

    
    lv_obj_t * btn_clean_custom = lv_button_create(screen_filmsPage);
    lv_obj_set_size(btn_clean_custom, BUTTON_START_WIDTH, BUTTON_START_HEIGHT);
    //lv_obj_set_pos(btn_back, 320, 210); 
    lv_obj_align(btn_clean_custom, LV_ALIGN_RIGHT_MID, -10 , 0);
    lv_obj_add_event_cb(btn_clean_custom, event_btn_selectFilm, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(btn_clean_custom, event_tankSize_style_delete, LV_EVENT_DELETE, NULL);

    lv_style_init(&style_btn_clean_custom);
    lv_style_set_bg_color(&style_btn_clean_custom, lv_color_hex(LIGHT_BLUE));
    lv_style_set_border_color(&style_btn_clean_custom, lv_color_hex(LIGHT_BLUE_DARK));
    lv_style_set_border_width(&style_btn_clean_custom, 3);
    lv_style_set_shadow_width(&style_btn_clean_custom, 10);
    lv_style_set_shadow_offset_y(&style_btn_clean_custom, 5);
    lv_style_set_shadow_opa(&style_btn_clean_custom, LV_OPA_50);
    lv_obj_add_style(btn_clean_custom, &style_btn_clean_custom, 0);

    lv_style_init(&labelStyle_clean_custom);
    lv_obj_t * label_btn_clean_custom= lv_label_create(btn_clean_custom);
    lv_label_set_text(label_btn_clean_custom, buttonCustom);
    lv_obj_set_style_text_font(label_btn_clean_custom, &lv_font_montserrat_22, 0);
    lv_style_set_text_color(&labelStyle_clean_custom, lv_color_hex(WHITE));
    lv_obj_add_style(label_btn_clean_custom, &labelStyle_clean_custom, 0);
    lv_obj_align(label_btn_clean_custom, LV_ALIGN_CENTER, 0, 0);

*/
    
}






#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*PAGE_FILMS_H*/