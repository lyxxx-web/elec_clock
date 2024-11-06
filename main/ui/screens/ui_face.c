#include "../ui.h"
void ui_face_screen_init(void)
{

    ui_face = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_face, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Panel_face = lv_obj_create(ui_face);
    lv_obj_set_width(ui_Panel_face, 240);
    lv_obj_set_height(ui_Panel_face, 240);
    lv_obj_set_align(ui_Panel_face, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Panel_face, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_Panel_face, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Panel_face, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Panel_face, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui_Panel_face, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_Panel_face, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_Panel_face, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_Panel_face, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_Panel_face, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_face_canvas = lv_canvas_create(ui_Panel_face);
    // lv_canvas_set_buffer(ui_dice_canvas, cbuf, CANVAS_WIDTH, CANVAS_HEIGHT, LV_IMG_CF_TRUE_COLOR);
    // lv_obj_center(ui_dice_canvas);
    lv_obj_align(ui_face_canvas, LV_ALIGN_BOTTOM_MID, 0, 0);

    lv_obj_add_event_cb(ui_face, ui_event_face, LV_EVENT_ALL, NULL);
}