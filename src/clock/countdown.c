/**
 * @file countdown.c
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
static void click_back_btn_cb(lv_event_t *e);     // 返回按钮点击事件
static void start_btn_event_cb(lv_event_t *e);    // 开始按钮的事件回调
static void pause_btn_event_cb(lv_event_t *e);    // 暂停按钮的事件回调
static void continue_btn_event_cb(lv_event_t *e); // 继续按钮的事件回调
static void countdown_timer_cb();                 // 定时器回调函数
static void update_countdown();                   // 更新倒计时进度、内容

/**********************
 *  STATIC VARIABLES
 **********************/
static int total_time;            // 总计时时间，单位秒
static int remaining_time;        // 剩余时间，单位秒
static lv_timer_t *timer;         // 定时器
static lv_obj_t *arc;             // 倒计时圆弧
static lv_obj_t *countdown_label; // 倒计时标签
static lv_obj_t *start_timer_btn; // 倒计时开始按钮

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
// 倒计时页面
void countdown_page_init(char *minutes_str)
{
    timer = NULL;                    // 初始化定时器为NULL
    int minutes = atoi(minutes_str); // 将传入的字符串转换为整数
    total_time = minutes * 60;       // 设置总时间
    remaining_time = total_time;     // 初始化剩余时间
    // basic_page_init("倒计时");              // 状态栏
    lv_obj_t *scr = clock_base_page_init(); // 倒计时 基础页面
    // 添加返回按钮
    lv_obj_t *back_btn = create_back_btn(scr);

    // 添加开始计时按钮
    start_timer_btn = create_clock_public_btn(scr);
    lv_obj_set_layout(start_timer_btn, LV_LAYOUT_FLEX);                                                       // 设置 flex 布局
    lv_obj_set_flex_flow(start_timer_btn, LV_FLEX_FLOW_ROW);                                                  // 设置为行布局
    lv_obj_set_flex_align(start_timer_btn, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER); // 设置对齐方式（居中）

    lv_obj_t *btn_icon = lv_img_create(start_timer_btn);
    lv_img_set_src(btn_icon, "I:/data/images/start_timer.png"); // 设置PNG图像的路径
    lv_obj_set_style_pad_right(btn_icon, 10, 0);
    lv_obj_set_style_pad_left(btn_icon, -5, 0);
    // lv_obj_align(btn_icon, LV_ALIGN_LEFT_MID, 10, 0);

    lv_obj_t *start_label = lv_label_create(start_timer_btn);
    lv_label_set_text(start_label, "开始计时");
    lv_obj_set_style_text_font(start_label, font22.font, 0);
    // lv_obj_center(start_label);

    // 标题
    lv_obj_t *titel_label = lv_label_create(scr);
    char title[20];
    snprintf(title, sizeof(title), "%d Min 计时", minutes);
    lv_label_set_text(titel_label, title);
    lv_obj_align(titel_label, LV_ALIGN_TOP_MID, 0, 156);
    lv_obj_set_style_text_font(titel_label, font22.font, 0);

    // 创建外部圆弧（作为边框）
    // lv_obj_t *outer_arc = lv_arc_create(scr);
    // lv_obj_set_size(outer_arc, 320, 320);  // 外圆弧比内圆弧稍大
    // lv_arc_set_bg_angles(outer_arc, 0, 0); // 背景圆弧
    // lv_arc_set_angles(outer_arc, 0, 360);
    // lv_obj_set_style_arc_color(outer_arc, lv_color_black(), LV_PART_INDICATOR); // 设置外圆弧颜色为黑色
    // lv_obj_set_style_arc_width(outer_arc, 1, LV_PART_INDICATOR);                // 设置边框宽度
    // lv_obj_center(outer_arc);
    // lv_obj_remove_style(outer_arc, NULL, LV_PART_KNOB);  // 移除默认的指示器样式
    // lv_obj_clear_flag(outer_arc, LV_OBJ_FLAG_CLICKABLE); // 移除点击事件

    // 创建圆弧倒计时动画
    arc = lv_arc_create(scr);
    lv_obj_set_size(arc, 314, 314);
    lv_arc_set_bg_angles(arc, 0, 0); // 背景圆弧
    lv_arc_set_rotation(arc, 270);   // 从顶部开始
    lv_arc_set_value(arc, 30);       // 初始角度
    lv_obj_center(arc);
    lv_arc_set_angles(arc, 0, 360);                                       // 初始设置为满弧
    lv_obj_set_style_arc_color(arc, lv_color_black(), LV_PART_INDICATOR); // 设置圆弧颜色为黑色
    lv_obj_set_style_arc_width(arc, 18, LV_PART_INDICATOR);               // 设置边框宽度
    lv_obj_remove_style(arc, NULL, LV_PART_KNOB);                         // 移除默认的指示器样式
    lv_obj_clear_flag(arc, LV_OBJ_FLAG_CLICKABLE);                        // 移除点击事件

    // 创建内部圆弧（作为内边框）
    // lv_obj_t *in_arc = lv_arc_create(scr);
    // lv_obj_set_size(in_arc, 274, 274);
    // lv_arc_set_bg_angles(in_arc, 0, 0); // 背景圆弧
    // lv_arc_set_angles(in_arc, 0, 360);
    // lv_obj_set_style_arc_color(in_arc, lv_color_black(), LV_PART_INDICATOR); // 设置外圆弧颜色为黑色
    // lv_obj_set_style_arc_width(in_arc, 1, LV_PART_INDICATOR);                // 设置边框宽度
    // lv_obj_center(in_arc);
    // lv_obj_remove_style(in_arc, NULL, LV_PART_KNOB);  // 移除默认的指示器样式
    // lv_obj_clear_flag(in_arc, LV_OBJ_FLAG_CLICKABLE); // 移除点击事件

    // 创建圆弧内部倒计时标签
    countdown_label = lv_label_create(scr);
    lv_obj_set_style_text_font(countdown_label, font60.font, 0);
    update_countdown(); // 初始化倒计时内容
    lv_obj_center(countdown_label);

    // 添加返回按钮事件
    lv_obj_add_event_cb(back_btn, click_back_btn_cb, LV_EVENT_CLICKED, NULL);

    // 添加开始计时按钮点击事件
    lv_obj_add_event_cb(start_timer_btn, start_btn_event_cb, LV_EVENT_CLICKED, NULL);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
// 返回按钮点击事件
static void click_back_btn_cb(lv_event_t *e)
{
    if (timer && remaining_time > 0)
    {
        printf("lv_timer_del(timer);\n");
        lv_timer_del(timer);
    }

    // 返回时间管理页面（计时）
    if (cache_timing_page)
    {
        lv_scr_load(cache_timing_page);
    }
}
// 开始按钮的事件回调
static void start_btn_event_cb(lv_event_t *e)
{
    remaining_time = total_time; // 初始化剩余时间
    update_countdown();
    printf("start_btn_event_cb%d--%d\n", remaining_time, total_time);
    lv_obj_t *btn = lv_event_get_target(e);
    lv_label_set_text(lv_obj_get_child(btn, 1), "暂停");
    lv_img_set_src(lv_obj_get_child(btn, 0), "I:/data/images/pause.png"); // 设置PNG图像的路径
    lv_obj_remove_event_cb_with_user_data(btn, start_btn_event_cb, NULL);
    lv_obj_add_event_cb(btn, pause_btn_event_cb, LV_EVENT_CLICKED, NULL);

    // 启动定时器
    timer = lv_timer_create(countdown_timer_cb, 100, NULL);
}

// 暂停按钮的事件回调
static void pause_btn_event_cb(lv_event_t *e)
{
    lv_obj_t *btn = lv_event_get_target(e);
    lv_label_set_text(lv_obj_get_child(btn, 1), "继续");
    lv_img_set_src(lv_obj_get_child(btn, 0), "I:/data/images/start_timer.png"); // 设置PNG图像的路径
    lv_obj_remove_event_cb_with_user_data(btn, pause_btn_event_cb, NULL);
    lv_obj_add_event_cb(btn, continue_btn_event_cb, LV_EVENT_CLICKED, NULL);

    // 暂停定时器
    lv_timer_pause(timer);
}

// 继续按钮的事件回调
static void continue_btn_event_cb(lv_event_t *e)
{
    printf("continue_btn_event_cb-----------\n");
    lv_obj_t *btn = lv_event_get_target(e);
    lv_label_set_text(lv_obj_get_child(btn, 1), "暂停");
    lv_img_set_src(lv_obj_get_child(btn, 0), "I:/data/images/pause.png"); // 设置PNG图像的路径
    lv_obj_remove_event_cb_with_user_data(btn, continue_btn_event_cb, NULL);
    lv_obj_add_event_cb(btn, pause_btn_event_cb, LV_EVENT_CLICKED, NULL);

    // 恢复定时器
    lv_timer_resume(timer);
}

// 定时器回调函数
static void countdown_timer_cb()
{
    remaining_time--;
    if (remaining_time <= 0)
    {
        lv_timer_del(timer);
        lv_label_set_text(lv_obj_get_child(start_timer_btn, 1), "重新计时");
        lv_img_set_src(lv_obj_get_child(start_timer_btn, 0), "I:/data/images/refresh.png"); // 设置PNG图像的路径
        // lv_obj_remove_event_cb_with_user_data(start_timer_btn, start_btn_event_cb, NULL);
        lv_obj_remove_event_cb_with_user_data(start_timer_btn, pause_btn_event_cb, NULL);
        lv_obj_add_event_cb(start_timer_btn, start_btn_event_cb, LV_EVENT_CLICKED, NULL);
        // remaining_time = 0;
    }
    update_countdown();
}

// 更新倒计时进度、内容
static void update_countdown()
{
    printf("update_countdown:%d\n", remaining_time);
    int minutes = remaining_time / 60;
    int seconds = remaining_time % 60;
    char buf[6];
    snprintf(buf, sizeof(buf), "%02d:%02d", minutes, seconds);
    lv_label_set_text(countdown_label, buf);

    // 更新圆弧进度
    // 计算圆弧的结束角度（顺时针减少）
    int angle = 360 * (total_time - remaining_time) / total_time;
    lv_arc_set_angles(arc, angle, 360);
}
