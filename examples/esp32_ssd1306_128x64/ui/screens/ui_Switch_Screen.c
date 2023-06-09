// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.3.0
// LVGL version: 8.3.6
// Project name: esp32_ssd1306_128x64_test

#include "../ui.h"

void ui_Switch_Screen_screen_init(void)
{
    ui_Switch_Screen = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Switch_Screen, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_Panel2 = lv_obj_create(ui_Switch_Screen);
    lv_obj_set_width(ui_Panel2, 128);
    lv_obj_set_height(ui_Panel2, 64);
    lv_obj_set_align(ui_Panel2, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_Panel2, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(ui_Panel2, LV_FLEX_ALIGN_SPACE_AROUND, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_SPACE_EVENLY);
    lv_obj_clear_flag(ui_Panel2, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_Panel2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Panel2, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Panel2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Panel2, &ui_font_MontserratSemiBold14, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label1 = lv_label_create(ui_Panel2);
    lv_obj_set_width(ui_Label1, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label1, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label1, -27);
    lv_obj_set_y(ui_Label1, 4);
    lv_obj_set_align(ui_Label1, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label1, "Switch 1");
    lv_obj_set_style_text_color(ui_Label1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Switch1 = lv_switch_create(ui_Panel2);
    lv_obj_set_width(ui_Switch1, 25);
    lv_obj_set_height(ui_Switch1, 14);
    lv_obj_set_x(ui_Switch1, 41);
    lv_obj_set_y(ui_Switch1, -16);
    lv_obj_set_align(ui_Switch1, LV_ALIGN_CENTER);

    ui_Label2 = lv_label_create(ui_Panel2);
    lv_obj_set_width(ui_Label2, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label2, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label2, -33);
    lv_obj_set_y(ui_Label2, -14);
    lv_obj_set_align(ui_Label2, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label2, "Switch 2");
    lv_obj_set_style_text_color(ui_Label2, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Switch2 = lv_switch_create(ui_Panel2);
    lv_obj_set_width(ui_Switch2, 25);
    lv_obj_set_height(ui_Switch2, 14);
    lv_obj_set_x(ui_Switch2, 41);
    lv_obj_set_y(ui_Switch2, -16);
    lv_obj_set_align(ui_Switch2, LV_ALIGN_CENTER);

    ui_Label3 = lv_label_create(ui_Panel2);
    lv_obj_set_width(ui_Label3, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label3, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label3, -33);
    lv_obj_set_y(ui_Label3, -14);
    lv_obj_set_align(ui_Label3, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label3, "Switch 3");
    lv_obj_set_style_text_color(ui_Label3, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Label3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Switch3 = lv_switch_create(ui_Panel2);
    lv_obj_set_width(ui_Switch3, 25);
    lv_obj_set_height(ui_Switch3, 14);
    lv_obj_set_x(ui_Switch3, 41);
    lv_obj_set_y(ui_Switch3, -16);
    lv_obj_set_align(ui_Switch3, LV_ALIGN_CENTER);

    lv_obj_add_event_cb(ui_Switch_Screen, ui_event_Switch_Screen, LV_EVENT_ALL, NULL);

}
