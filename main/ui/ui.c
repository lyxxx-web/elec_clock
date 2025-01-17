// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.4.2
// LVGL version: 8.3.11
// Project name: SquareLine_Project

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_err.h"
#include "esp_check.h"
#include "ui.h"
#include "ui_helpers.h"
#include "bsp/esp-bsp.h"
#include "cube_dice.h"
#include "lottie_player.h"

#include "thorvg_capi.h"
#include "mmap_generate_assets.h"

#include "bsp/esp-bsp.h"

#define TAG "ui"

#define DICE_SIZE_HOR           100*2
#define DICE_SIZE_VER           100*2
#define swap16(x)               (((x) << 8) | ((x) >> 8))

#define LOTTIE_SIZE_HOR         (240)
#define LOTTIE_SIZE_VER         (240)

#define LV_EVENT_RENEW          (_LV_EVENT_LAST + 1)

extern mmap_assets_handle_t asset_handle;

static uint8_t *framebuf = NULL;
static lv_timer_t *timer_tinygl = NULL;
static lv_timer_t *timer_player = NULL;

tinyGL_modle_handle_t tinygl_handle;
lottie_palyer_handle_t player_handle;

///////////////////// VARIABLES ////////////////////
void muyushow_Animation(lv_obj_t * TargetObject, int delay);
void blink_Animation(lv_obj_t * TargetObject, int delay);
void top_Animation(lv_obj_t * TargetObject, int delay);
void weather_Animation(lv_obj_t * TargetObject, int delay);
void bottom_Animation(lv_obj_t * TargetObject, int delay);
void opaon_Animation(lv_obj_t * TargetObject, int delay);
void shaizishow_Animation(lv_obj_t * TargetObject, int delay);
void gonde_txt_Animation(lv_obj_t * TargetObject, int delay);

// SCREEN: ui_home
void ui_home_screen_init(void);
void ui_event_home(lv_event_t * e);
lv_obj_t * ui_home;
lv_obj_t * ui_Panel1;
lv_obj_t * ui_hour;
lv_obj_t * ui_min;
lv_obj_t * ui_date;
lv_obj_t * ui_weekday;
lv_obj_t * ui_temp;
lv_obj_t * ui_weather;
lv_obj_t * ui_colon;
lv_obj_t * ui_weathershow;

// SCREEN: ui_muyuplay
void ui_muyuplay_screen_init(void);
void ui_event_muyuplay(lv_event_t * e);
lv_obj_t * ui_muyuplay;
void ui_event_Panel3(lv_event_t * e);
lv_obj_t * ui_Panel3;
lv_obj_t * ui_muyu;
lv_obj_t * ui_gongdetxt;

// SCREEN: ui_dice
void ui_dice_screen_init(void);
void ui_event_dice(lv_event_t * e);
lv_obj_t * ui_dice;
lv_obj_t * ui_Panel4;
lv_obj_t * ui_shaizibut;
lv_obj_t * ui_shaizitxt;
lv_obj_t * ui_dice_canvas;

// SCREEN: ui_title
void ui_title_screen_init(void);
lv_obj_t * ui_title;
lv_obj_t * title_panel;
lv_obj_t * title_wifistate;
lv_obj_t * title_powerstate;
lv_obj_t * title_batterybor;
lv_obj_t * title_timestate;
lv_obj_t * title_batterytxt;

// SCREEN:ui_face
void ui_face_screen_init(void);
lv_obj_t * ui_face;
lv_obj_t * ui_Panel_face;
lv_obj_t * ui_face_canvas;
void ui_event_face(lv_event_t *e);

// SCREEN:ui_fish
void ui_fish_screen_init(void);
lv_obj_t * ui_fish;
lv_obj_t * ui_Panel_fish;
lv_obj_t * ui_fish_canvas;
void ui_event_fish(lv_event_t *e);

void ui_face_screen_init(void);
lv_obj_t * ui_face;
lv_obj_t * ui_Panel_face;
lv_obj_t * ui_face_canvas;
void ui_event_face(lv_event_t *e);
lv_obj_t * ui_img_face;

// SCREEN:ui_game_2048
void ui_game_screen_init(void);
lv_obj_t * ui_game;
void ui_event_game(lv_event_t *e);

void ui_event____initial_actions0(lv_event_t * e);
lv_obj_t * ui____initial_actions0;

///////////////////// TEST LVGL SETTINGS ////////////////////
#if LV_COLOR_DEPTH != 16
#error "LV_COLOR_DEPTH should be 16bit to match SquareLine Studio's settings"
#endif
#if LV_COLOR_16_SWAP !=1
#error "LV_COLOR_16_SWAP should be 1 to match SquareLine Studio's settings"
#endif

///////////////////// ANIMATIONS ////////////////////
void muyushow_Animation(lv_obj_t * TargetObject, int delay)
{
    ui_anim_user_data_t * PropertyAnimation_0_user_data = lv_mem_alloc(sizeof(ui_anim_user_data_t));
    PropertyAnimation_0_user_data->target = TargetObject;
    PropertyAnimation_0_user_data->val = -1;
    lv_anim_t PropertyAnimation_0;
    lv_anim_init(&PropertyAnimation_0);
    lv_anim_set_time(&PropertyAnimation_0, 150);
    lv_anim_set_user_data(&PropertyAnimation_0, PropertyAnimation_0_user_data);
    lv_anim_set_custom_exec_cb(&PropertyAnimation_0, _ui_anim_callback_set_image_zoom);
    lv_anim_set_values(&PropertyAnimation_0, -50, 0);
    lv_anim_set_path_cb(&PropertyAnimation_0, lv_anim_path_overshoot);
    lv_anim_set_delay(&PropertyAnimation_0, delay + 0);
    lv_anim_set_deleted_cb(&PropertyAnimation_0, _ui_anim_callback_free_user_data);
    lv_anim_set_playback_time(&PropertyAnimation_0, 0);
    lv_anim_set_playback_delay(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_count(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_delay(&PropertyAnimation_0, 0);
    lv_anim_set_early_apply(&PropertyAnimation_0, false);
    lv_anim_set_get_value_cb(&PropertyAnimation_0, &_ui_anim_callback_get_image_zoom);
    lv_anim_start(&PropertyAnimation_0);

}
void gonde_txt_Animation(lv_obj_t * TargetObject, int delay)
{
    ui_anim_user_data_t * opacity_anim_user_data = lv_mem_alloc(sizeof(ui_anim_user_data_t));
    opacity_anim_user_data->target = TargetObject;

    lv_anim_t opacity_anim;
    lv_anim_init(&opacity_anim);
    lv_anim_set_time(&opacity_anim, 600);
    lv_anim_set_user_data(&opacity_anim, opacity_anim_user_data);
    lv_anim_set_custom_exec_cb(&opacity_anim, _ui_anim_callback_set_text_opacity);
    lv_anim_set_values(&opacity_anim, 255, 0);
    lv_anim_set_path_cb(&opacity_anim, lv_anim_path_linear);
    lv_anim_set_delay(&opacity_anim, delay);
    lv_anim_set_deleted_cb(&opacity_anim, _ui_anim_callback_free_user_data);
    lv_anim_start(&opacity_anim);

    ui_anim_user_data_t * position_anim_user_data = lv_mem_alloc(sizeof(ui_anim_user_data_t));
    position_anim_user_data->target = TargetObject;

    lv_anim_t position_anim;
    lv_anim_init(&position_anim);
    lv_anim_set_time(&position_anim, 600);
    lv_anim_set_user_data(&position_anim, position_anim_user_data);
    lv_anim_set_custom_exec_cb(&position_anim, _ui_anim_callback_set_y);
    lv_anim_set_values(&position_anim, -50, -95);
    lv_anim_set_path_cb(&position_anim, lv_anim_path_linear);
    lv_anim_set_delay(&position_anim, delay);
    lv_anim_set_deleted_cb(&position_anim, _ui_anim_callback_free_user_data);
    lv_anim_start(&position_anim);
}

void blink_Animation(lv_obj_t * TargetObject, int delay)
{
    ui_anim_user_data_t * PropertyAnimation_0_user_data = lv_mem_alloc(sizeof(ui_anim_user_data_t));
    PropertyAnimation_0_user_data->target = TargetObject;
    PropertyAnimation_0_user_data->val = -1;
    lv_anim_t PropertyAnimation_0;
    lv_anim_init(&PropertyAnimation_0);
    lv_anim_set_time(&PropertyAnimation_0, 1000);
    lv_anim_set_user_data(&PropertyAnimation_0, PropertyAnimation_0_user_data);
    lv_anim_set_custom_exec_cb(&PropertyAnimation_0, _ui_anim_callback_set_opacity);
    lv_anim_set_values(&PropertyAnimation_0, 255, 100);
    lv_anim_set_path_cb(&PropertyAnimation_0, lv_anim_path_ease_in);
    lv_anim_set_delay(&PropertyAnimation_0, delay + 0);
    lv_anim_set_deleted_cb(&PropertyAnimation_0, _ui_anim_callback_free_user_data);
    lv_anim_set_playback_time(&PropertyAnimation_0, 0);
    lv_anim_set_playback_delay(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_count(&PropertyAnimation_0, LV_ANIM_REPEAT_INFINITE);
    lv_anim_set_repeat_delay(&PropertyAnimation_0, 0);
    lv_anim_set_early_apply(&PropertyAnimation_0, false);
    lv_anim_set_get_value_cb(&PropertyAnimation_0, &_ui_anim_callback_get_opacity);
    lv_anim_start(&PropertyAnimation_0);

}
void top_Animation(lv_obj_t * TargetObject, int delay)
{
    ui_anim_user_data_t * PropertyAnimation_0_user_data = lv_mem_alloc(sizeof(ui_anim_user_data_t));
    PropertyAnimation_0_user_data->target = TargetObject;
    PropertyAnimation_0_user_data->val = -1;
    lv_anim_t PropertyAnimation_0;
    lv_anim_init(&PropertyAnimation_0);
    lv_anim_set_time(&PropertyAnimation_0, 500);
    lv_anim_set_user_data(&PropertyAnimation_0, PropertyAnimation_0_user_data);
    lv_anim_set_custom_exec_cb(&PropertyAnimation_0, _ui_anim_callback_set_y);
    lv_anim_set_values(&PropertyAnimation_0, -50, 0);
    lv_anim_set_path_cb(&PropertyAnimation_0, lv_anim_path_overshoot);
    lv_anim_set_delay(&PropertyAnimation_0, delay + 0);
    lv_anim_set_deleted_cb(&PropertyAnimation_0, _ui_anim_callback_free_user_data);
    lv_anim_set_playback_time(&PropertyAnimation_0, 0);
    lv_anim_set_playback_delay(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_count(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_delay(&PropertyAnimation_0, 0);
    lv_anim_set_early_apply(&PropertyAnimation_0, false);
    lv_anim_set_get_value_cb(&PropertyAnimation_0, &_ui_anim_callback_get_y);
    lv_anim_start(&PropertyAnimation_0);

}
void weather_Animation(lv_obj_t * TargetObject, int delay)
{
    ui_anim_user_data_t * PropertyAnimation_0_user_data = lv_mem_alloc(sizeof(ui_anim_user_data_t));
    PropertyAnimation_0_user_data->target = TargetObject;
    PropertyAnimation_0_user_data->val = -1;
    lv_anim_t PropertyAnimation_0;
    lv_anim_init(&PropertyAnimation_0);
    lv_anim_set_time(&PropertyAnimation_0, 500);
    lv_anim_set_user_data(&PropertyAnimation_0, PropertyAnimation_0_user_data);
    lv_anim_set_custom_exec_cb(&PropertyAnimation_0, _ui_anim_callback_set_x);
    lv_anim_set_values(&PropertyAnimation_0, -150, 0);
    lv_anim_set_path_cb(&PropertyAnimation_0, lv_anim_path_overshoot);
    lv_anim_set_delay(&PropertyAnimation_0, delay + 0);
    lv_anim_set_deleted_cb(&PropertyAnimation_0, _ui_anim_callback_free_user_data);
    lv_anim_set_playback_time(&PropertyAnimation_0, 0);
    lv_anim_set_playback_delay(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_count(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_delay(&PropertyAnimation_0, 0);
    lv_anim_set_early_apply(&PropertyAnimation_0, false);
    lv_anim_set_get_value_cb(&PropertyAnimation_0, &_ui_anim_callback_get_x);
    lv_anim_start(&PropertyAnimation_0);

}
void bottom_Animation(lv_obj_t * TargetObject, int delay)
{
    ui_anim_user_data_t * PropertyAnimation_0_user_data = lv_mem_alloc(sizeof(ui_anim_user_data_t));
    PropertyAnimation_0_user_data->target = TargetObject;
    PropertyAnimation_0_user_data->val = -1;
    lv_anim_t PropertyAnimation_0;
    lv_anim_init(&PropertyAnimation_0);
    lv_anim_set_time(&PropertyAnimation_0, 500);
    lv_anim_set_user_data(&PropertyAnimation_0, PropertyAnimation_0_user_data);
    lv_anim_set_custom_exec_cb(&PropertyAnimation_0, _ui_anim_callback_set_y);
    lv_anim_set_values(&PropertyAnimation_0, 100, 0);
    lv_anim_set_path_cb(&PropertyAnimation_0, lv_anim_path_overshoot);
    lv_anim_set_delay(&PropertyAnimation_0, delay + 0);
    lv_anim_set_deleted_cb(&PropertyAnimation_0, _ui_anim_callback_free_user_data);
    lv_anim_set_playback_time(&PropertyAnimation_0, 0);
    lv_anim_set_playback_delay(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_count(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_delay(&PropertyAnimation_0, 0);
    lv_anim_set_early_apply(&PropertyAnimation_0, false);
    lv_anim_set_get_value_cb(&PropertyAnimation_0, &_ui_anim_callback_get_y);
    lv_anim_start(&PropertyAnimation_0);

}
void opaon_Animation(lv_obj_t * TargetObject, int delay)
{
    ui_anim_user_data_t * PropertyAnimation_0_user_data = lv_mem_alloc(sizeof(ui_anim_user_data_t));
    PropertyAnimation_0_user_data->target = TargetObject;
    PropertyAnimation_0_user_data->val = -1;
    lv_anim_t PropertyAnimation_0;
    lv_anim_init(&PropertyAnimation_0);
    lv_anim_set_time(&PropertyAnimation_0, 500);
    lv_anim_set_user_data(&PropertyAnimation_0, PropertyAnimation_0_user_data);
    lv_anim_set_custom_exec_cb(&PropertyAnimation_0, _ui_anim_callback_set_text_opacity);
    lv_anim_set_values(&PropertyAnimation_0, 0, 255);
    lv_anim_set_path_cb(&PropertyAnimation_0, lv_anim_path_linear);
    lv_anim_set_delay(&PropertyAnimation_0, delay + 0);
    lv_anim_set_deleted_cb(&PropertyAnimation_0, _ui_anim_callback_free_user_data);
    lv_anim_set_playback_time(&PropertyAnimation_0, 0);
    lv_anim_set_playback_delay(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_count(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_delay(&PropertyAnimation_0, 0);
    lv_anim_set_early_apply(&PropertyAnimation_0, false);
    lv_anim_set_get_value_cb(&PropertyAnimation_0, &_ui_anim_callback_get_opacity);
    lv_anim_start(&PropertyAnimation_0);

}

void shaizishow_Animation(lv_obj_t * TargetObject, int delay)
{
    ui_anim_user_data_t * PropertyAnimation_0_user_data = lv_mem_alloc(sizeof(ui_anim_user_data_t));
    PropertyAnimation_0_user_data->target = TargetObject;
    PropertyAnimation_0_user_data->val = -1;
    lv_anim_t PropertyAnimation_0;
    lv_anim_init(&PropertyAnimation_0);
    lv_anim_set_time(&PropertyAnimation_0, 150);
    lv_anim_set_user_data(&PropertyAnimation_0, PropertyAnimation_0_user_data);
    lv_anim_set_custom_exec_cb(&PropertyAnimation_0, _ui_anim_callback_set_opacity);
    lv_anim_set_values(&PropertyAnimation_0, 255, 0);
    lv_anim_set_path_cb(&PropertyAnimation_0, lv_anim_path_linear);
    lv_anim_set_delay(&PropertyAnimation_0, delay + 0);
    lv_anim_set_deleted_cb(&PropertyAnimation_0, _ui_anim_callback_free_user_data);
    lv_anim_set_playback_time(&PropertyAnimation_0, 0);
    lv_anim_set_playback_delay(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_count(&PropertyAnimation_0, 0);
    lv_anim_set_repeat_delay(&PropertyAnimation_0, 0);
    lv_anim_set_early_apply(&PropertyAnimation_0, false);
    lv_anim_set_get_value_cb(&PropertyAnimation_0, &_ui_anim_callback_get_opacity);
    lv_anim_start(&PropertyAnimation_0);

}

///////////////////// FUNCTIONS ////////////////////
void ui_event_home(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if (event_code == LV_EVENT_SCREEN_LOAD_START) {
        bottom_Animation(ui_hour, 0);
        bottom_Animation(ui_min, 0);
        weather_Animation(ui_weathershow, 0);
        opaon_Animation(ui_weather, 0);
        opaon_Animation(ui_temp, 0);
        opaon_Animation(ui_weekday, 0);
        opaon_Animation(ui_date, 0);
        top_Animation(title_panel, 0);
        lv_obj_set_parent(title_panel, ui_Panel1);
    }
    if (event_code == LV_EVENT_GESTURE &&  lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_LEFT) {
        lv_indev_wait_release(lv_indev_get_act());
        _ui_screen_change(&ui_muyuplay, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_muyuplay_screen_init);
    }
}
void ui_event_muyuplay(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if (event_code == LV_EVENT_SCREEN_LOAD_START) {
        lv_obj_set_parent(title_panel, ui_Panel3);
    }
    if (event_code == LV_EVENT_GESTURE &&  lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT) {
        lv_indev_wait_release(lv_indev_get_act());
        _ui_screen_change(&ui_home, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_home_screen_init);
    }
    if (event_code == LV_EVENT_GESTURE &&  lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_LEFT) {
        lv_indev_wait_release(lv_indev_get_act());
        _ui_screen_change(&ui_dice, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_dice_screen_init);
    }
}
void ui_event_Panel3(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if (event_code == LV_EVENT_PRESSED) {
        muyushow_Animation(ui_muyu, 0);
        gonde_txt_Animation(ui_gongdetxt, 0);
    }
}

static void frame_rgb888_to_rgb565(uint8_t *in, uint16_t *out, int width, int height)
{
    int numPixels = width * height;
    for (int i = 0; i < numPixels; i++) {
        uint8_t r = in[i * 3 + 2];
        uint8_t g = in[i * 3 + 1];
        uint8_t b = in[i * 3 + 0];

        uint16_t r565 = (r >> 3) & 0x1F;
        uint16_t g565 = (g >> 2) & 0x3F;
        uint16_t b565 = (b >> 3) & 0x1F;

        uint16_t rgb565Value = (r565 << 11) | (g565 << 5) | b565;
        out[i] = swap16(rgb565Value);
    }
}

static void frame_argb888_to_rgb565(uint8_t *in, uint16_t *out, int width, int height)
{
    int numPixels = width * height;
    for (int i = 0; i < numPixels; i++) {
        uint8_t r = in[i * 4 + 2];
        uint8_t g = in[i * 4 + 1];
        uint8_t b = in[i * 4 + 0];

        uint16_t r565 = (r >> 3) & 0x1F;
        uint16_t g565 = (g >> 2) & 0x3F;
        uint16_t b565 = (b >> 3) & 0x1F;

        uint16_t rgb565Value = (r565 << 11) | (g565 << 5) | b565;
        out[i] = swap16(rgb565Value);
    }
}

static void win_tinyGL_timer_cb(lv_timer_t *tmr)
{
    cube_dice_update(tinygl_handle);
    frame_rgb888_to_rgb565(framebuf, (uint16_t *)framebuf, DICE_SIZE_HOR, DICE_SIZE_VER);
    lv_canvas_set_buffer(ui_dice_canvas, framebuf, DICE_SIZE_HOR, DICE_SIZE_VER, LV_IMG_CF_TRUE_COLOR);
}

static void win_lottie_timer_player_cb(lv_timer_t *tmr)
{
    assert(tmr->user_data);
    lottie_player_cfg_t *data = tmr->user_data;

    if (ESP_OK == lottie_player_update(player_handle)) {
        frame_argb888_to_rgb565(framebuf, (uint16_t *)framebuf, data->width, data->height);
        lv_canvas_set_buffer(data->target_obj, framebuf, data->width, data->height, LV_IMG_CF_TRUE_COLOR);
    }
}

bool fish_player_end(void *user_data)
{
    lottie_player_cfg_t *cfg = (lottie_player_cfg_t *)user_data;
    ESP_LOGI("UI", "fish end:%p, %p", cfg->target_obj, ui_fish_canvas);
    lv_event_send(ui_fish, LV_EVENT_RENEW, NULL);
    return false;
}

bool face_player_end(void *user_data)
{
    lottie_player_cfg_t *cfg = (lottie_player_cfg_t *)user_data;
    ESP_LOGI("UI", "face end:%p, %p", cfg->target_obj, ui_face_canvas);
    lv_event_send(ui_face, LV_EVENT_RENEW, NULL);
    return true;
}

void delete_lottie_player()
{
    lv_timer_del(timer_player);
    timer_player = NULL;

    lottie_player_deinit(player_handle);
    player_handle = NULL;

    heap_caps_free(framebuf);
    framebuf = NULL;
}

void create_lottie_palyer(uint8_t index, lottie_player_cfg_t *cfg)
{
    if (player_handle) {
        ESP_LOGW("UI", "delete player before create");
        delete_lottie_player();
    }

    framebuf = heap_caps_calloc(cfg->width * cfg->height * 4, 1, MALLOC_CAP_SPIRAM);
    assert(framebuf);

    cfg->framebuf = framebuf;
    cfg->file_name = mmap_assets_get_name(asset_handle, index);
    cfg->file_size = mmap_assets_get_size(asset_handle, index);
    cfg->file_data = mmap_assets_get_mem(asset_handle, index);

    lottie_player_init(cfg, &player_handle);
    timer_player = lv_timer_create(win_lottie_timer_player_cb, 10, cfg);
}

void delete_tinygl_dice()
{
    lv_timer_del(timer_tinygl);
    timer_tinygl = NULL;

    cube_dice_deinit(tinygl_handle);
    tinygl_handle = NULL;

    heap_caps_free(framebuf);
    framebuf = NULL;
}

void create_tinygl_dice()
{
    tinyGL_config_t tiny_config = {
        .width = DICE_SIZE_HOR,
        .height = DICE_SIZE_VER,
    };

    framebuf = heap_caps_calloc(DICE_SIZE_HOR * DICE_SIZE_VER * 3, 1, MALLOC_CAP_SPIRAM);
    assert(framebuf);

    tiny_config.framebuf = framebuf;
    for (int i = 0; i < 6; i++) {
        tiny_config.texture[i].image = mmap_assets_get_mem(asset_handle, MMAP_ASSETS_DICE1_BMP + i);
        tiny_config.texture[i].image += 54;
        tiny_config.texture[i].width = mmap_assets_get_width(asset_handle, MMAP_ASSETS_DICE1_BMP + i);
        tiny_config.texture[i].height = mmap_assets_get_height(asset_handle, MMAP_ASSETS_DICE1_BMP + i);
    }

    cube_dice_init(&tiny_config, &tinygl_handle);
    timer_tinygl = lv_timer_create(win_tinyGL_timer_cb, 10, NULL);
}

void ui_event_dice(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if (event_code == LV_EVENT_SCREEN_LOAD_START) {
        lv_obj_set_parent(title_panel, ui_Panel4);
        ESP_LOGI(TAG, "### load tinyGL Dice ###");

        create_tinygl_dice();
        lv_obj_clear_flag(ui_dice_canvas, LV_OBJ_FLAG_HIDDEN);
    }

    if (event_code == LV_EVENT_GESTURE &&  lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_LEFT) {
        delete_tinygl_dice();
        lv_obj_add_flag(ui_dice_canvas, LV_OBJ_FLAG_HIDDEN);

        lv_indev_wait_release(lv_indev_get_act());
        _ui_screen_change(&ui_face, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_face_screen_init);
    }

    if (event_code == LV_EVENT_GESTURE &&  lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT) {
        delete_tinygl_dice();
        lv_obj_add_flag(ui_dice_canvas, LV_OBJ_FLAG_HIDDEN);

        lv_indev_wait_release(lv_indev_get_act());
        _ui_screen_change(&ui_muyuplay, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_muyuplay_screen_init);
    }
}

void ui_event_face(lv_event_t *e)
{
    static lottie_player_cfg_t player_cfg;
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);

    if (event_code == LV_EVENT_SCREEN_LOAD_START) {
        // lv_obj_set_parent(title_panel, ui_face);
        ESP_LOGI(TAG, "### load Lottie player, Face:%p ###", ui_face_canvas);

        player_cfg.width = LOTTIE_SIZE_HOR;
        player_cfg.height = LOTTIE_SIZE_VER;
        player_cfg.target_obj = ui_face_canvas;
        player_cfg.on_end = face_player_end;

        create_lottie_palyer(MMAP_ASSETS_LOOK_JSON, &player_cfg);
        lv_obj_clear_flag(ui_face_canvas, LV_OBJ_FLAG_HIDDEN);
    }
    if (event_code == LV_EVENT_RENEW) {
        ESP_LOGI(TAG, "### face renew ###");
    }

    if (event_code == LV_EVENT_GESTURE &&  lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_LEFT) {

        delete_lottie_player();
        lv_obj_add_flag(ui_face_canvas, LV_OBJ_FLAG_HIDDEN);

        lv_indev_wait_release(lv_indev_get_act());
        _ui_screen_change(&ui_fish, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_fish_screen_init);
    }
    if (event_code == LV_EVENT_GESTURE &&  lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT) {

        delete_lottie_player();
        lv_obj_add_flag(ui_face_canvas, LV_OBJ_FLAG_HIDDEN);

        lv_indev_wait_release(lv_indev_get_act());
        _ui_screen_change(&ui_dice, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_dice_screen_init);
    }
}

void ui_event_fish(lv_event_t *e)
{
    static lottie_player_cfg_t player_cfg;

    static uint16_t default_width = LOTTIE_SIZE_HOR;
    static uint16_t default_height = LOTTIE_SIZE_VER;

    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    lv_obj_t *user_data = lv_event_get_user_data(e);

    if (event_code == LV_EVENT_SCREEN_LOAD_START) {
        lv_obj_set_parent(title_panel, ui_fish);

        player_cfg.width = default_width;
        player_cfg.height = default_height;
        player_cfg.target_obj = ui_fish_canvas;
        player_cfg.on_end = fish_player_end;

        ESP_LOGI(TAG, "### load Lottie player, Fish:[%d, %d], %p###", default_width, default_height, ui_fish_canvas);

        create_lottie_palyer(MMAP_ASSETS_FISH_JSON, &player_cfg);
        lv_obj_clear_flag(ui_fish_canvas, LV_OBJ_FLAG_HIDDEN);
    }

    if (event_code == LV_EVENT_RENEW) {

        default_width *= 0.9;
        default_height *= 0.9;
        if (default_width < 100 || default_height < 100) {
            default_width = LOTTIE_SIZE_HOR;
            default_height = LOTTIE_SIZE_VER;
        }
        player_cfg.width = default_width;
        player_cfg.height = default_height;
        player_cfg.target_obj = ui_fish_canvas;
        player_cfg.on_end = fish_player_end;

        ESP_LOGI(TAG, "### load Lottie player, Fish:[%d, %d], %p ###", default_width, default_height, ui_fish_canvas);

        create_lottie_palyer(MMAP_ASSETS_FISH_JSON, &player_cfg);
        lv_obj_clear_flag(ui_fish_canvas, LV_OBJ_FLAG_HIDDEN);
    }

    if (event_code == LV_EVENT_GESTURE &&  lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_LEFT) {

        delete_lottie_player();
        lv_obj_add_flag(ui_fish_canvas, LV_OBJ_FLAG_HIDDEN);

        lv_indev_wait_release(lv_indev_get_act());
        _ui_screen_change(&ui_game, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_game_screen_init);
    }
    if (event_code == LV_EVENT_GESTURE &&  lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT) {

        delete_lottie_player();
        lv_obj_add_flag(ui_fish_canvas, LV_OBJ_FLAG_HIDDEN);

        lv_indev_wait_release(lv_indev_get_act());
        _ui_screen_change(&ui_face, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_dice_screen_init);
    }
}

void ui_event_game(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if (event_code == LV_EVENT_SCREEN_LOAD_START) {
        lv_obj_set_parent(title_panel, ui_game);
    }
    if (event_code == LV_EVENT_GESTURE &&  lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_LEFT) {
        lv_indev_wait_release(lv_indev_get_act());
        _ui_screen_change(&ui_home, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_home_screen_init);
    }
    if (event_code == LV_EVENT_GESTURE &&  lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT) {
        lv_indev_wait_release(lv_indev_get_act());
        _ui_screen_change(&ui_fish, LV_SCR_LOAD_ANIM_FADE_ON, 500, 0, &ui_fish_screen_init);
    }
}
void ui_event____initial_actions0(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t * target = lv_event_get_target(e);
    if (event_code == LV_EVENT_SCREEN_LOAD_START) {
        blink_Animation(ui_colon, 0);
    }
}

///////////////////// SCREENS ////////////////////

void ui_init(void)
{

    lv_disp_t * dispp = lv_disp_get_default();
    lv_theme_t * theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
                                               false, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    ui_title_screen_init();
    ui_home_screen_init();
    ui_muyuplay_screen_init();
    ui_dice_screen_init();
    ui_face_screen_init();
    ui_fish_screen_init();
    ui_game_screen_init();
    ui____initial_actions0 = lv_obj_create(NULL);
    lv_obj_add_event_cb(ui____initial_actions0, ui_event____initial_actions0, LV_EVENT_ALL, NULL);

    lv_disp_load_scr(ui____initial_actions0);
    lv_disp_load_scr(ui_home);
}
