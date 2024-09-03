/**
 * @file custom_countdown.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "countdown.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void click_back_btn_cb(lv_event_t *e); // 返回按钮点击事件
static void start_btn_cb(lv_event_t *e);      // 启动计时按钮点击事件

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
// 自定义倒计时页面
void custom_countdown_page_init(void)
{
    // basic_page_init("自定义计时");              // 状态栏
    lv_obj_t *scr = clock_base_page_init(); // 自定义倒计时 基础页面

    // 添加返回按钮
    lv_obj_t *back_btn = create_back_btn(scr);

    // 自定义时间
    // 动态分配内存并初始化结构体
    alarm_info_t *alarm_timer_info = malloc(sizeof(alarm_info_t));
    alarm_timer_info->hour = 0;
    alarm_timer_info->minute = 0;
    alarm_timer_info->is_valid = true;
    alarm_timer_info->alarm_type = REPEAT_WORKDAYS;
    alarm_timer_info->is_valid = true;

    char hour[3];
    snprintf(hour, sizeof(hour), "%02d", alarm_timer_info->hour);
    char minute[3];
    snprintf(minute, sizeof(minute), "%02d", alarm_timer_info->minute);
    // 添加启动按钮
    lv_obj_t *start_btn = create_clock_public_btn(scr);
    lv_obj_t *start_label = lv_label_create(start_btn);
    lv_label_set_text(start_label, "启动");
    lv_obj_set_style_text_font(start_label, font18.font, 0);
    lv_obj_center(start_label);

    // 新建倒计时 时 - 按钮
    lv_obj_t *h_minus_btn = create_btn_img(scr, "I:/data/images/minus_btn.png");
    lv_obj_set_pos(h_minus_btn, 130, 212);
    lv_obj_set_style_bg_color(h_minus_btn, lv_color_white(), 0);
    lv_obj_set_style_shadow_opa(h_minus_btn, 0, 0);

    // 新建倒计时 分 - 按钮
    lv_obj_t *m_minus_btn = create_btn_img(scr, "I:/data/images/minus_btn.png");
    lv_obj_set_pos(m_minus_btn, 291, 212);
    lv_obj_set_style_bg_color(m_minus_btn, lv_color_white(), 0);
    lv_obj_set_style_shadow_opa(m_minus_btn, 0, 0);

    // 新建倒计时 时 - 文本
    lv_obj_t *h_text = lv_label_create(scr);
    lv_label_set_text(h_text, hour);
    lv_obj_set_pos(h_text, 126, 322);
    lv_obj_set_style_text_font(h_text, font18.font, 0);
    lv_obj_t *h_unit = lv_label_create(scr);
    lv_label_set_text(h_unit, "时");
    lv_obj_set_pos(h_unit, 199, 353);
    lv_obj_set_style_text_font(h_unit, font18.font, 0);

    // 新建倒计时 分 - 文本
    lv_obj_t *m_text = lv_label_create(scr);
    lv_label_set_text(m_text, minute);
    lv_obj_set_pos(m_text, 286, 322);
    lv_obj_set_style_text_font(m_text, font18.font, 0);
    lv_obj_t *m_unit = lv_label_create(scr);
    lv_label_set_text(m_unit, "分");
    lv_obj_set_pos(m_unit, 359, 353);
    lv_obj_set_style_text_font(m_unit, font18.font, 0);

    // 新建倒计时 时 + 按钮
    lv_obj_t *h_add_btn = create_btn_img(scr, "I:/data/images/add_btn.png");
    lv_obj_set_pos(h_add_btn, 130, 432);
    lv_obj_set_style_bg_color(h_add_btn, lv_color_white(), 0);
    lv_obj_set_style_shadow_opa(h_add_btn, 0, 0);

    // 新建倒计时 分 + 按钮
    lv_obj_t *m_add_btn = create_btn_img(scr, "I:/data/images/add_btn.png");
    lv_obj_set_pos(m_add_btn, 291, 432);
    lv_obj_set_style_bg_color(m_add_btn, lv_color_white(), 0);
    lv_obj_set_style_shadow_opa(m_add_btn, 0, 0);

    // 创建 alarm_data_t 并初始化
    alarm_add_data_t *new_timer = malloc(sizeof(alarm_add_data_t));
    new_timer->alarm_info = alarm_timer_info;
    new_timer->h_label = h_text;
    new_timer->m_label = m_text;
    // 添加 + - 按钮事件
    lv_obj_add_event_cb(h_add_btn, h_add_btn_cb, LV_EVENT_CLICKED, new_timer);
    lv_obj_add_event_cb(h_minus_btn, h_minus_btn_cb, LV_EVENT_CLICKED, new_timer);
    lv_obj_add_event_cb(m_add_btn, m_add_btn_cb, LV_EVENT_CLICKED, new_timer);
    lv_obj_add_event_cb(m_minus_btn, m_minus_btn_cb, LV_EVENT_CLICKED, new_timer);

    // 添加返回按钮事件
    lv_obj_add_event_cb(back_btn, click_back_btn_cb, LV_EVENT_CLICKED, NULL);

    // 添加启动按钮事件
    lv_obj_add_event_cb(start_btn, start_btn_cb, LV_EVENT_CLICKED, new_timer);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
// 返回按钮点击事件
static void click_back_btn_cb(lv_event_t *e)
{
    if (cache_timing_page)
    {
        lv_scr_load(cache_timing_page);
    }
}

// 启动计时按钮点击事件
static void start_btn_cb(lv_event_t *e)
{
    // 获取添加事件时传递的用户数据
    alarm_add_data_t *timer_data = (alarm_add_data_t *)lv_event_get_user_data(e);

    int minutes = timer_data->alarm_info->minute + timer_data->alarm_info->hour * 60;
    // 定义一个足够大的字符数组来存储结果
    char minutes_str[10];
    snprintf(minutes_str, sizeof(minutes_str), "%d", minutes);

    // 使用完之后记得释放内存
    free(timer_data);
    countdown_page_init(minutes_str);
}
