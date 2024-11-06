#include "../ui.h"
#include "simple_test.h"
void ui_game_screen_init(void)
{
    ui_game = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_game, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    
    ui_2048 = lv_obj_create(ui_game);
    lv_obj_set_width(ui_2048, 240);
    lv_obj_set_height(ui_2048, 240);
    lv_obj_set_align(ui_2048, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_2048, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_2048, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_2048, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_2048, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_2048, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_2048, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_2048, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_2048, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_2048, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    
    lv_obj_add_event_cb(ui_game, ui_event_game, LV_EVENT_ALL, NULL);
    lv_100ask_2048_simple_test(ui_2048);
}