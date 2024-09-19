/**
 * @file new_alarm.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "new_alarm.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void click_back_btn_cb(lv_event_t *e);        // 点击返回按钮回调函数
static void draw_event_cb(lv_event_t *e);            // 重复类型按钮绘制时回调，用于自定义按钮
static void add_clock_confirm_btn_cb(lv_event_t *e); // 添加闹钟确认按钮点击事件

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
// 创建闹钟公共按钮
lv_obj_t *create_clock_public_btn(lv_obj_t *parent)
{
    lv_obj_t *clock_public_btn = lv_btn_create(parent);
    lv_obj_set_size(clock_public_btn, 348, 64);
    lv_obj_set_pos(clock_public_btn, 112, 716);
    lv_obj_set_style_bg_color(clock_public_btn, lv_color_black(), 0);
    lv_obj_set_style_radius(clock_public_btn, 100, 0);

    return clock_public_btn;
}

// 闹钟 h + 按钮点击事件
void h_add_btn_cb(lv_event_t *e)
{
    // 获取添加事件时传递的用户数据
    alarm_add_data_t *alarm_data = (alarm_add_data_t *)lv_event_get_user_data(e);

    alarm_data->alarm_info->hour = (alarm_data->alarm_info->hour + 1) % 24; // 确保小时不超过24
    char hour[3];
    snprintf(hour, sizeof(hour), "%02d", alarm_data->alarm_info->hour);
    lv_label_set_text(alarm_data->h_label, hour);
}

// 闹钟 h - 按钮点击事件
void h_minus_btn_cb(lv_event_t *e)
{
    // 获取添加事件时传递的用户数据
    alarm_add_data_t *alarm_data = (alarm_add_data_t *)lv_event_get_user_data(e);

    alarm_data->alarm_info->hour = (alarm_data->alarm_info->hour - 1 + 24) % 24; // 确保小时不小于0
    char hour[3];
    snprintf(hour, sizeof(hour), "%02d", alarm_data->alarm_info->hour);
    lv_label_set_text(alarm_data->h_label, hour);
}

// 闹钟 m + 按钮点击事件
void m_add_btn_cb(lv_event_t *e)
{
    // 获取添加事件时传递的用户数据
    alarm_add_data_t *alarm_data = (alarm_add_data_t *)lv_event_get_user_data(e);

    alarm_data->alarm_info->minute = (alarm_data->alarm_info->minute + 1) % 60; // 确保分钟不超过60
    char minute[3];
    snprintf(minute, sizeof(minute), "%02d", alarm_data->alarm_info->minute);
    lv_label_set_text(alarm_data->m_label, minute);
}

// 闹钟 m - 按钮点击事件
void m_minus_btn_cb(lv_event_t *e)
{
    // 获取添加事件时传递的用户数据
    alarm_add_data_t *alarm_data = (alarm_add_data_t *)lv_event_get_user_data(e);

    alarm_data->alarm_info->minute = (alarm_data->alarm_info->minute - 1 + 60) % 60; // 确保分钟不小于0
    char minute[3];
    snprintf(minute, sizeof(minute), "%02d", alarm_data->alarm_info->minute);
    lv_label_set_text(alarm_data->m_label, minute);
}

// 新建闹钟页面
void create_new_alarm_page()
{
    // basic_page_init("新建闹钟");              // 状态栏
    lv_obj_t *scr = clock_base_page_init(); // clock 基础页面
    // 添加返回按钮
    lv_obj_t *back_btn = create_back_btn(scr);

    // 添加确定按钮
    lv_obj_t *confirm_btn = create_clock_public_btn(scr);
    lv_obj_t *confirm_label = lv_label_create(confirm_btn);
    lv_label_set_text(confirm_label, "确定");
    lv_obj_set_style_text_font(confirm_label, font18.font, 0);
    lv_obj_center(confirm_label);

    // 新建闹钟 时 - 按钮
    lv_obj_t *h_minus_btn = create_btn_img(scr, "A:/data/images/minus_btn.png");
    lv_obj_set_pos(h_minus_btn, 130, 136);
    lv_obj_set_style_bg_color(h_minus_btn, lv_color_white(), 0);
    lv_obj_set_style_shadow_opa(h_minus_btn, 0, 0);

    // 新建闹钟 分 - 按钮
    lv_obj_t *m_minus_btn = create_btn_img(scr, "A:/data/images/minus_btn.png");
    lv_obj_set_pos(m_minus_btn, 291, 136);
    lv_obj_set_style_bg_color(m_minus_btn, lv_color_white(), 0);
    lv_obj_set_style_shadow_opa(m_minus_btn, 0, 0);

    // 动态分配内存并初始化结构体
    alarm_info_t *alarm_info = malloc(sizeof(alarm_info_t));
    alarm_info->hour = 0;
    alarm_info->minute = 0;
    alarm_info->is_enabled = true;
    alarm_info->alarm_type = REPEAT_WORKDAYS;
    alarm_info->is_valid = true;

    char hour[3];
    snprintf(hour, sizeof(hour), "%02d", alarm_info->hour);
    char minute[3];
    snprintf(minute, sizeof(minute), "%02d", alarm_info->minute);

    // 新建闹钟 时 - 文本
    lv_obj_t *h_text = lv_label_create(scr);
    lv_label_set_text(h_text, hour);
    lv_obj_set_pos(h_text, 126, 246);
    lv_obj_set_style_text_font(h_text, font60.font, 0);
    lv_obj_t *h_unit = lv_label_create(scr);
    lv_label_set_text(h_unit, "时");
    lv_obj_set_pos(h_unit, 199, 277);
    lv_obj_set_style_text_font(h_unit, font22.font, 0);

    // 新建闹钟 分 - 文本
    lv_obj_t *m_text = lv_label_create(scr);
    lv_label_set_text(m_text, minute);
    lv_obj_set_pos(m_text, 286, 246);
    lv_obj_set_style_text_font(m_text, font60.font, 0);
    lv_obj_t *m_unit = lv_label_create(scr);
    lv_label_set_text(m_unit, "分");
    lv_obj_set_pos(m_unit, 359, 277);
    lv_obj_set_style_text_font(m_unit, font22.font, 0);

    // 新建闹钟 时 + 按钮
    lv_obj_t *h_add_btn = create_btn_img(scr, "A:/data/images/add_btn.png");
    lv_obj_set_pos(h_add_btn, 130, 356);
    lv_obj_set_style_bg_color(h_add_btn, lv_color_white(), 0);
    lv_obj_set_style_shadow_opa(h_add_btn, 0, 0);

    // 新建闹钟 分 + 按钮
    lv_obj_t *m_add_btn = create_btn_img(scr, "A:/data/images/add_btn.png");
    lv_obj_set_pos(m_add_btn, 291, 356);
    lv_obj_set_style_bg_color(m_add_btn, lv_color_white(), 0);
    lv_obj_set_style_shadow_opa(m_add_btn, 0, 0);

    // 新建闹钟 重复类型
    lv_obj_t *repeat_type_label = lv_label_create(scr);
    lv_label_set_text(repeat_type_label, "重复:");
    lv_obj_set_pos(repeat_type_label, 48, 491);
    lv_obj_set_style_text_font(repeat_type_label, font22.font, 0);

    // 创建一个按钮矩阵 用于 重复类型选项按钮
    lv_obj_t *btnm = lv_btnmatrix_create(scr);

    // 定义按钮矩阵的按钮内容
    static const char *btnm_map[] = {"每天", "工作日", "一次", ""}; // 最后一个空字符串表示结束
    lv_btnmatrix_set_map(btnm, btnm_map);                           // 设置按钮矩阵的按钮内容
    lv_obj_set_style_text_font(btnm, font22.font, 0);               // 设置按钮矩阵的字体为中文字体

    // 调整按钮矩阵的样式和布局
    lv_obj_set_size(btnm, 304, 40);       // 根据需要调整大小
    lv_obj_set_pos(btnm, 128, 486);       // 根据需要调整位置
    lv_obj_set_style_pad_all(btnm, 0, 0); // 按钮的内边距（用于调整按钮大小）
    lv_obj_set_style_border_opa(btnm, 0, 0);

    lv_btnmatrix_set_btn_ctrl_all(btnm, LV_BTNMATRIX_CTRL_CHECKABLE);
    lv_btnmatrix_set_one_checked(btnm, true);
    lv_btnmatrix_set_btn_ctrl(btnm, 0, LV_BTNMATRIX_CTRL_CHECKED); // 将第一个按钮设置为选中状态

    // 添加选中重复类型按钮事件
    lv_obj_add_event_cb(btnm, draw_event_cb, LV_EVENT_ALL, alarm_info);

    // 创建 alarm_data_t 并初始化
    alarm_add_data_t *new_alarm = malloc(sizeof(alarm_add_data_t));
    new_alarm->alarm_info = alarm_info;
    new_alarm->h_label = h_text;
    new_alarm->m_label = m_text;

    // // 添加 + - 按钮事件
    lv_obj_add_event_cb(h_add_btn, h_add_btn_cb, LV_EVENT_CLICKED, new_alarm);
    lv_obj_add_event_cb(h_minus_btn, h_minus_btn_cb, LV_EVENT_CLICKED, new_alarm);
    lv_obj_add_event_cb(m_add_btn, m_add_btn_cb, LV_EVENT_CLICKED, new_alarm);
    lv_obj_add_event_cb(m_minus_btn, m_minus_btn_cb, LV_EVENT_CLICKED, new_alarm);

    // 添加返回按钮事件
    lv_obj_add_event_cb(back_btn, click_back_btn_cb, LV_EVENT_CLICKED, NULL);

    // 添加确定按钮事件
    lv_obj_add_event_cb(confirm_btn, add_clock_confirm_btn_cb, LV_EVENT_CLICKED, alarm_info);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
// 返回按钮点击事件
static void click_back_btn_cb(lv_event_t *e)
{
    // char *back_path = lv_event_get_user_data(e); // 获取添加事件时传递的用户数据
    // if (strcmp(back_path, "home") == 0)
    // {
    //     // 返回首页
    //     printf("back home.\n");
    // }
    // else if (strcmp(back_path, "clock") == 0)
    // {
    //     // 返回闹钟列表页面
    //     click_clock(e);
    // }
    printf("click back btn.\n");
    // 返回闹钟列表页面
    create_time_manage();
}

// 添加闹钟确认按钮点击事件
static void add_clock_confirm_btn_cb(lv_event_t *e)
{
    printf("add_clock_confirm_btn_cb\n");
    // 获取添加事件时传递的用户数据
    alarm_info_t *alarm_info = (alarm_info_t *)lv_event_get_user_data(e);
    if (alarm_count >= 20)
    {
        printf("Cannot add more alarms. Maximum limit reached. alarm_count: %d\n", alarm_count);
        return;
    }

    // 添加新的闹钟到数组的末尾
    alarms[alarm_count] = *alarm_info;

    // 增加有效闹钟的计数
    alarm_count++;

    free(alarm_info);
    // 返回闹钟列表页面
    // click_clock(e);
    create_time_manage();
}

// 重复类型按钮绘制时回调，用于自定义按钮
static void draw_event_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *btn = lv_event_get_target(e);

    if (code == LV_EVENT_DRAW_PART_BEGIN)
    {
        // lv_obj_draw_part_dsc_t * dsc = lv_event_get_draw_part_dsc(e);  这个api不对（版本问题？）
        lv_obj_draw_part_dsc_t *dsc = lv_event_get_param(e); // 8.1文档示例中使用的

        // 检查 dsc 和相关指针是否有效
        if (dsc == NULL || dsc->rect_dsc == NULL || dsc->label_dsc == NULL)
        {
            return;
        }
        // if(lv_btnmatrix_get_selected_btn(btn) == dsc->id) {
        //     // 选中的按钮背景色为黑色，文本色为白色
        //     dsc->rect_dsc->bg_color = lv_color_hex(0x000000);
        //     dsc->label_dsc->color = lv_color_white();
        // } else {
        //     // 未选中的按钮背景色为白色，文本色为黑色
        //     dsc->rect_dsc->bg_color = lv_color_white();
        //     dsc->label_dsc->color = lv_color_black();
        // }
    }
    else if (code == LV_EVENT_CLICKED)
    {
        // 获取添加事件时传递的用户数据
        alarm_info_t *alarm_info = (alarm_info_t *)lv_event_get_user_data(e);
        uint16_t selcte_id = lv_btnmatrix_get_selected_btn(btn);

        // 根据按钮ID更新 alarm_info->alarm_type
        if (selcte_id == 0)
        {
            alarm_info->alarm_type = REPEAT_EVERY_DAY;
        }
        else if (selcte_id == 1)
        {
            alarm_info->alarm_type = REPEAT_WORKDAYS;
        }
        else if (selcte_id == 2)
        {
            alarm_info->alarm_type = REPEAT_ONCE;
        }
    }
}
