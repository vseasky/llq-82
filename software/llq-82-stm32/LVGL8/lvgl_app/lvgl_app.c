/*
 * @author                          : Seasky.Liu
 * @Date                            : 2021-11-22 14:56:40
 * @LastEditTime: 2022-02-04 17:40:28
 * @LastEditors: Please set LastEditors
 * @Description                     : https://github.com/SEASKY-Master
 * @FilePath                        : \llq-82\LVGL8\lvgl_app\lvgl_app.c
 * @symbol_custom_string_obkoro1    :
 * 版权所有:@Seasky.liu
 * 联系方式:liuwei_seasky@163.com
 * 开源协议:请遵从开源协议（项目仓库中有说明），未经作者允许、严禁用于商业用途
 * ************************************************************************
 *            If you want a thing done well, do it yourself.
 * ************************************************************************
 */
#include "lvgl_app.h"
#include "llq_image.h"
#include "stdio.h"
#include "flash_task.h"
#include "bsp_gui.h"

lvgl_app_info lvgl_task_app_t;

const llq_82_style_color llq_82_color_code[LLQ_MAX_COLOR_NUM] = {
    {"llq_red", "#FF0000"},
    {"llq_green", "#00FF00"},
    {"llq_blue", "#0000FF"},
    {"llq_yellow", "#FFFF00"},
    {"llq_magenta", "#FF00FF"},
    {"llq_cyan", "#00FFFF"},
    {"llq_white", "#FFFFFF"},
    {"llq_black", "#000000"},
    {"llq_lightGray", "#D3D3D3"},
    {"llq_dimgray", "#696969"},
};
static void roller_event_handler(lv_event_t *e);
static void lv_app_win_selected(lvgl_app_info *lvgl_app_t, uint8_t level_t);
static void lv_gui_level1_ctr(lvgl_app_info *lvgl_app_t);
static void lv_gui_passmenu(lvgl_app_info *lvgl_app_t);
static void lv_gui_password(lvgl_app_info *lvgl_app_t);
static void lv_password_save(lvgl_app_info *lvgl_app_t);
static void llq_lvgl_updata_style_code(void);

static lv_img_dsc_t my_img_dsc =
    {
        .header.always_zero = 0,
        .header.w = LLQ_IMAG_WIDTH,
        .header.h = LLQ_IMAGE_HIGH,
        .data_size = LLQ_IMAG_WIDTH * LLQ_IMAGE_HIGH * LV_COLOR_DEPTH / 8,
        .header.cf = LV_IMG_CF_TRUE_COLOR, /*Set the color format*/
        .data = gImage_llq,
};

static uint8_t str_to_uint(char c_char)
{
    uint8_t to_utf8;
    if ((c_char >= '0') && (c_char <= '9'))
    {
        to_utf8 = c_char - '0';
    }
    else if ((c_char >= 'a') && (c_char <= 'f'))
    {
        to_utf8 = c_char - 'a' + 10;
    }
    else if ((c_char >= 'A') && (c_char <= 'F'))
    {
        to_utf8 = c_char - 'A' + 10;
    }
    return to_utf8;
}

static lv_color_t llq_lv_color_make(const char *color)
{
    uint8_t r_t, g_t, b_t;
    if (color[0] == '#')
    {
        r_t = (str_to_uint(color[1]) << 4) | (str_to_uint(color[2]));
        g_t = (str_to_uint(color[3]) << 4) | (str_to_uint(color[4]));
        b_t = (str_to_uint(color[5]) << 4) | (str_to_uint(color[6]));
    }
    return lv_color_make(r_t, g_t, b_t);
}

static void llq_lvgl_style_set(char *background, char *font_select, char *font_setting, char *font_normal, char *font_input)
{
    memset(&lvgl_task_app_t.llq_style, 0, sizeof(lvgl_task_app_t.llq_style));
    snprintf(lvgl_task_app_t.llq_style.background_color, 10, background);
    snprintf(lvgl_task_app_t.llq_style.font_select_color, 10, font_select);
    snprintf(lvgl_task_app_t.llq_style.font_setting_color, 10, font_setting);
    snprintf(lvgl_task_app_t.llq_style.font_normal_color, 10, font_normal);
    snprintf(lvgl_task_app_t.llq_style.font_input_color, 10, font_input);
}

void config_updata_to_flash(void)
{
    /*更新到flash*/
    flash_info_lvgl_style(&lvgl_task_app_t.llq_style_code);
}
void llq_lvgl_set_style_code(llq_style_enum pStyle, llq_82_color_enum pColor)
{
    lvgl_task_app_t.llq_style_code.code[pStyle] = (llq_82_color_enum)pColor;

    config_updata_to_flash();

    llq_lvgl_updata_style_code();
}
static void llq_lvgl_updata_style_code(void)
{
    llq_lvgl_style_set(llq_82_color_code[lvgl_task_app_t.llq_style_code.code[LLQ_BACK_COLOR]].color_type,
                       llq_82_color_code[lvgl_task_app_t.llq_style_code.code[LLQ_SELECT_COLOR]].color_type,
                       llq_82_color_code[lvgl_task_app_t.llq_style_code.code[LLQ_SETTING_COLOR]].color_type,
                       llq_82_color_code[lvgl_task_app_t.llq_style_code.code[LLQ_NORMAL_COLOR]].color_type,
                       llq_82_color_code[lvgl_task_app_t.llq_style_code.code[LLQ_INPUT_COLOR]].color_type);
}

void set_lvgl_def(llq_lvgl_style_code *plvgl_style)
{
    lvgl_task_app_t.llq_style_code.code[LLQ_BACK_COLOR] = LLQ_WHITE;
    lvgl_task_app_t.llq_style_code.code[LLQ_SELECT_COLOR] = LLQ_RED;
    lvgl_task_app_t.llq_style_code.code[LLQ_SETTING_COLOR] = LLQ_GREEN;
    lvgl_task_app_t.llq_style_code.code[LLQ_NORMAL_COLOR] = LLQ_BLUE;
    lvgl_task_app_t.llq_style_code.code[LLQ_INPUT_COLOR] = LLQ_MAGENTA;
    memset(plvgl_style, 0, sizeof(llq_lvgl_style_code));
    memcpy(plvgl_style, &lvgl_task_app_t.llq_style_code, sizeof(llq_lvgl_style_code));
    llq_lvgl_updata_style_code();
}
void set_lvgl_style(llq_lvgl_style_code *plvgl_style)
{
    memset(&lvgl_task_app_t.llq_style_code, 0, sizeof(llq_lvgl_style_code));
    memcpy(&lvgl_task_app_t.llq_style_code, plvgl_style, sizeof(llq_lvgl_style_code));
    llq_lvgl_updata_style_code();
}
void lvgl_app_init(lvgl_app_info *lvgl_app_t)
{
    uint8_t i = 0;
    //主界面
    lvgl_app_t->main_win.app_obj_main = lv_obj_create(NULL);
    lv_obj_set_size(lvgl_app_t->main_win.app_obj_main, LV_HOR_RES, LV_VER_RES);
    lv_obj_set_style_bg_color(lvgl_app_t->main_win.app_obj_main, lv_color_black(), 0);
    lv_obj_set_style_bg_opa(lvgl_app_t->main_win.app_obj_main, LV_STATE_DEFAULT, LV_OPA_100);
    lv_obj_set_pos(lvgl_app_t->main_win.app_obj_main, 0, 0);

    lvgl_app_t->main_win.label_info = lv_label_create(lvgl_app_t->main_win.app_obj_main);
    lv_label_set_recolor(lvgl_app_t->main_win.label_info, true);
    lv_label_set_text_fmt(lvgl_app_t->main_win.label_info, LLQ_82_BLUE_TYPE1 LV_SYMBOL_WIFI LV_SYMBOL_BLUETOOTH "%d%%" LV_SYMBOL_BATTERY_EMPTY, 99);
    lv_obj_align(lvgl_app_t->main_win.label_info, LV_ALIGN_TOP_RIGHT, -5, 0);

    lvgl_app_t->main_win.icon = lv_img_create(lvgl_app_t->main_win.app_obj_main);
    lv_img_set_src(lvgl_app_t->main_win.icon, &my_img_dsc);
    lv_obj_align(lvgl_app_t->main_win.icon, LV_ALIGN_CENTER, 0, 0);
    /*子界面*/
    lvgl_app_t->widget1.app_obj_widget = lv_obj_create(NULL);
    lv_obj_set_size(lvgl_app_t->widget1.app_obj_widget, LV_HOR_RES, LV_VER_RES);
    lvgl_task_app_t.widget1.lvgl_back_ground = llq_lv_color_make(lvgl_task_app_t.llq_style.background_color);

    lv_obj_set_style_bg_color(lvgl_app_t->widget1.app_obj_widget, lvgl_task_app_t.widget1.lvgl_back_ground, 0);
    lv_obj_set_style_bg_opa(lvgl_app_t->widget1.app_obj_widget, LV_STATE_DEFAULT, LV_OPA_100);
    lv_obj_set_pos(lvgl_app_t->widget1.app_obj_widget, 0, 0);
    for (i = 0; i < LVGL_MAX_LABLE; i++)
    {
        lvgl_app_t->widget1.lable[i] = lv_label_create(lvgl_app_t->widget1.app_obj_widget);
        lv_label_set_recolor(lvgl_app_t->widget1.lable[i], true);
        lv_label_set_long_mode(lvgl_app_t->widget1.lable[i], LV_LABEL_LONG_SCROLL_CIRCULAR); /*Circular scroll*/
        lv_obj_set_width(lvgl_app_t->widget1.lable[i], LV_HOR_RES - 20);
        lv_label_set_text_fmt(lvgl_app_t->widget1.lable[i], "current num: [%d]", i);
        lv_obj_align(lvgl_app_t->widget1.lable[i], LV_ALIGN_TOP_LEFT, 10, i * (LV_VER_RES) / 5);
    }
    lv_scr_load(lvgl_app_t->main_win.app_obj_main);
    get_oled_info_data();
}

void lv_set_gui_lable(uint8_t num, char *str, uint8_t id, uint8_t enable)
{
    if (num < LVGL_MAX_LABLE)
    {
        if (enable != 0)
        {
            lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[num], "%s %4s %d %s", lvgl_task_app_t.llq_style.font_select_color, LV_SYMBOL_RIGHT, id, str);
        }
        else
        {
            lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[num], "%s %4s %d %s", lvgl_task_app_t.llq_style.font_normal_color, "   ", id, str);
        }
    }
}

void lv_set_gui_setting(uint8_t num, char *str, uint8_t id, uint8_t enable)
{
    if (num == 0)
    {
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[num], "%s %4s [%02d] %s", lvgl_task_app_t.llq_style.font_setting_color, LV_SYMBOL_SETTINGS, id, str);
    }
    if ((num < LVGL_MAX_LABLE) && (num > 0))
    {
        if (enable != 0)
        {
            lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[num], "%s %4s  %s", lvgl_task_app_t.llq_style.font_select_color, LV_SYMBOL_RIGHT, str);
        }
        else
        {
            lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[num], "%s %4s  %s", lvgl_task_app_t.llq_style.font_normal_color, " ", str);
        }
    }
}
void lv_set_gui_input(uint8_t num, char *str, uint8_t id, uint8_t enable)
{
    if (num == 0)
    {
        lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[num], "%s %4s [%02d] %s", lvgl_task_app_t.llq_style.font_setting_color, LV_SYMBOL_SETTINGS, id, str);
    }
    if ((num < LVGL_MAX_LABLE) && (num > 0))
    {
        if (enable != 0)
        {
            lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[num], "%s %4s  %s", lvgl_task_app_t.llq_style.font_input_color, LV_SYMBOL_EDIT, str);
        }
        else
        {
            lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[num], "%s %4s  %s", lvgl_task_app_t.llq_style.font_normal_color, " ", str);
        }
    }
}
void lv_app_select(GUI_MENU level_t)
{
    static GUI_MENU last_lvgl;
    if (last_lvgl != level_t)
    {
        lvgl_task_app_t.gui_key_ctr.level1 = level_t;
        switch (lvgl_task_app_t.gui_key_ctr.level1)
        {
        case LVGL_GUI_MAIN:
        {
            lv_scr_load(lvgl_task_app_t.main_win.app_obj_main);
        }
        break;
        case LVGL_GUI_MENU:
        {
            lv_scr_load(lvgl_task_app_t.widget1.app_obj_widget);
        }
        break;
        }
    }
    last_lvgl = level_t;
}

void lv_main_lable_updata(void)
{
    static uint16_t timer_count = 0;
    static uint8_t battery_select;
    char lable_str[128] = {0};
    char battery_elec_p[8] = {0};
    // 1.
    if (lvgl_task_app_t.main_win.lable_status.wifi_status)
    {
        strcat(lable_str, LV_SYMBOL_WIFI);
    }
    // 2.
    if (lvgl_task_app_t.main_win.lable_status.ble_status)
    {
        strcat(lable_str, LV_SYMBOL_BLUETOOTH);
    }
    else if(lvgl_task_app_t.main_win.lable_status.usb_status)
    {
        strcat(lable_str, LV_SYMBOL_USB);
    }
    // 3.
    snprintf(battery_elec_p, 8, "%d%%", lvgl_task_app_t.main_win.lable_status.battery_elec);
    strcat(lable_str, battery_elec_p);
    if (lvgl_task_app_t.main_win.lable_status.charger_status)
    {
        timer_count++;
        //周期是50ms
        if (timer_count >= 20)
        {
            battery_select++;
            timer_count = 0;
        }
        if (battery_select >= 5)
        {
            battery_select = 0;
        }
    }
    else
    {
        if (lvgl_task_app_t.main_win.lable_status.battery_elec <= 100)
        {
            battery_select = lvgl_task_app_t.main_win.lable_status.battery_elec / 20;
            if (battery_select >= 5)
            {
                battery_select = 4;
            }
        }
    }
    switch (battery_select)
    {
    case 0:
        strcat(lable_str, LV_SYMBOL_BATTERY_EMPTY);
        break;
    case 1:
        strcat(lable_str, LV_SYMBOL_BATTERY_1);
        break;
    case 2:
        strcat(lable_str, LV_SYMBOL_BATTERY_2);
        break;
    case 3:
        strcat(lable_str, LV_SYMBOL_BATTERY_3);
        break;
    case 4:
        strcat(lable_str, LV_SYMBOL_BATTERY_FULL);
        break;
    default:
        strcat(lable_str, LV_SYMBOL_BATTERY_FULL);
        break;
    }
    // 4.
    if (lvgl_task_app_t.main_win.lable_status.charger_status)
    {
        strcat(lable_str, LV_SYMBOL_CHARGE);
    }
    lv_label_set_text_fmt(lvgl_task_app_t.main_win.label_info, "%s %s", LLQ_82_BLUE_TYPE1, lable_str);
}
void lv_set_bel_wifi(uint8_t ble_t, uint8_t wifi_t, uint8_t usb_t)
{
    lvgl_task_app_t.main_win.lable_status.ble_status = ble_t;
    lvgl_task_app_t.main_win.lable_status.wifi_status = wifi_t;
    lvgl_task_app_t.main_win.lable_status.usb_status = usb_t;
}

void lv_bat_set_value(uint8_t value, uint8_t ischarger)
{
    lvgl_task_app_t.main_win.lable_status.battery_elec = value;
    lvgl_task_app_t.main_win.lable_status.charger_status = ischarger;
}

void lv_color_code_config(uint8_t color_id, uint8_t get_finger, uint8_t enable)
{
    char curr_style[16] = {0};
    uint8_t k;
    static const uint8_t slider_width = MAX_GUI_SHOW_NUM - 1;
    static uint8_t min = 0, max = MAX_GUI_SHOW_NUM - 1;
    //如果移动到最大选项
    if (get_finger > max)
    {
        min = get_finger - slider_width;
        max = get_finger;
    }
    else if (get_finger < min)
    {
        min = get_finger;
        max = get_finger + slider_width;
    }

    if (enable)
    {
        llq_lvgl_set_style_code(color_id, get_finger);
    }
    for (k = min; k <= max; k++)
    {
        if (k < LLQ_MAX_COLOR_NUM)
        {
            memset(curr_style, 0, sizeof(curr_style));
            if (k == lvgl_task_app_t.llq_style_code.code[color_id])
            {
                snprintf(curr_style, 16, "# #FF0000 %s", LV_SYMBOL_OK);
            }
            if (k == get_finger)
            {
                lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[k - min], "%s %4s # %s %s %s", lvgl_task_app_t.llq_style.font_select_color, LV_SYMBOL_RIGHT, llq_82_color_code[k].color_type, llq_82_color_code[k].color_name, curr_style);
            }
            else
            {
                lv_label_set_text_fmt(lvgl_task_app_t.widget1.lable[k - min], "%s %4s %s %s", llq_82_color_code[k].color_type, " ", llq_82_color_code[k].color_name, curr_style);
            }
        }
    }
}