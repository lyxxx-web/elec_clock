#include "../ui.h"
void ui_fish_screen_init(void)
{
    ui_fish = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_fish, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    // ui_Panel3 = lv_obj_create(ui_fish);
    // lv_obj_set_width(ui_Panel3, 240);
    // lv_obj_set_height(ui_Panel3, 240);
    // lv_obj_set_align(ui_Panel3, LV_ALIGN_CENTER);
    // lv_obj_clear_flag(ui_Panel3, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    // lv_obj_set_style_radius(ui_Panel3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_color(ui_Panel3, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_bg_opa(ui_Panel3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_border_width(ui_Panel3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_pad_left(ui_Panel3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_pad_right(ui_Panel3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_pad_top(ui_Panel3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    // lv_obj_set_style_pad_bottom(ui_Panel3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_img_fish = lv_img_create(ui_fish);
    lv_img_set_src(ui_img_fish, &ui_img_muyu_png);
    lv_obj_set_width(ui_img_fish, LV_SIZE_CONTENT);   /// 256
    lv_obj_set_height(ui_img_fish, LV_SIZE_CONTENT);    /// 256
    lv_obj_set_align(ui_img_fish, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_img_fish, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_img_fish, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_img_set_zoom(ui_img_fish, 200);


    lv_obj_add_event_cb(ui_Panel3, ui_event_Panel3, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_fish, ui_event_muyuplay, LV_EVENT_ALL, NULL);
}