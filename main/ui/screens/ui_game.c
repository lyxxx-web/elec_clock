#include "../ui.h"
void ui_game_screen_init(void)
{
    ui_game = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_game, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    
    lv_obj_set_style_bg_color(ui_game, lv_color_hex(0xbaeb34), LV_PART_MAIN | LV_STATE_DEFAULT);
    
    lv_obj_add_event_cb(ui_game, ui_event_game, LV_EVENT_ALL, NULL);
}