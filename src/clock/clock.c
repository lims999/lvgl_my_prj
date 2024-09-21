// 基础页面 ，状态栏标题等内容。
/*********************
 *      INCLUDES
 *********************/
#include "clock.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/
// 用于 tabview 按钮事件回调传入的参数结构体
#define TAB_COUNT 2 // Fixed to 2 tabs

typedef struct
{
    // lv_obj_t *container;    // Main container
    lv_obj_t *tab_btns[TAB_COUNT]; // Array of tab buttons
    lv_obj_t *content[TAB_COUNT];  // Array of content containers
    lv_obj_t *add_clock_btn;
    uint32_t active_tab; // Index of the active tab
} my_tabview_t;

// // 闹钟重复类型
// typedef enum
// {
//     REPEAT_EVERY_DAY,
//     REPEAT_WORKDAYS,
//     REPEAT_ONCE
// } repeat_type;
// // 闹钟信息结构体
// typedef struct
// {
//     int hour;               // 小时（0-23）
//     int minute;             // 分钟（0-59）
//     bool is_enabled;        // 闹钟开关状态
//     repeat_type alarm_type; // 重复类型
//     bool is_valid;          // 是否为有效闹钟
// } alarm_info_t;

// // 闹钟删除事件数据结构体
// typedef struct
// {
//     int index;
//     lv_obj_t *parent;
// } alarm_del_data_t;

// // 新增闹钟时的数据结构体
// typedef struct
// {
//     alarm_info_t *alarm_info; // 闹钟信息
//     lv_obj_t *h_label;        // 显示时的标签
//     lv_obj_t *m_label;        // 显示时分的标签
// } alarm_add_data_t;

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void click_back_btn_cb(lv_event_t *e);                                                         // 点击返回按钮回调函数
static void add_clock_btn_cb(lv_event_t *e);                                                          // 点击添加闹钟按钮回调函数
static void click_tv_btn_cb(lv_event_t *e);                                                           // 点击 tabview 按钮切换内容
static void click_timer_btn_cb(lv_event_t *e);                                                        // 点击计时按钮 item 回调函数
static lv_obj_t *create_alarm_item(lv_obj_t *parent, alarm_info_t *alarm_info, uint32_t alarm_index); // 创建闹钟项
static void alarm_del_event_cb(lv_event_t *e);                                                        // 闹钟删除事件回调函数
static void switch_btn_event_cb(lv_event_t *e);                                                       // 闹钟 switch 事件回调函数
static void render_alarm_list(lv_obj_t *parent);                                                      // 闹钟列表渲染
static void create_timer_btn(lv_obj_t *scr, char *text, lv_coord_t x, lv_coord_t y);                  // 创建倒计时按钮
static void free_all_mem(void);                                                                       // 释放内存

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
// mock 闹钟数组
alarm_info_t alarms[20] = {
    // {7, 0, true, REPEAT_EVERY_DAY, true},
    // {8, 0, false, REPEAT_WORKDAYS, true},
    // {6, 15, true, REPEAT_ONCE, true},
    // {7, 15, true, REPEAT_ONCE, true},
    // {9, 15, true, REPEAT_ONCE, true},
    // {10, 15, true, REPEAT_ONCE, false},
};
// uint32_t alarm_count = sizeof(alarms) / sizeof(alarms[0]);
uint32_t alarm_count = 0; // 闹钟数量计数

// 创建参数数据
my_tabview_t *my_tabview = NULL;         // 用于 tabview 切换的参数
alarm_del_data_t *alarm_del_data = NULL; // 用于闹钟删除时的参数

lv_obj_t *cache_timing_page = NULL; // 缓存当前页面，用于返回到计时页面不清除 tabview 选项

// 闹钟统一背景
lv_obj_t *clock_base_page_init(void)
{
    // 设置背景颜色为黑色
    lv_obj_t *scr = lv_obj_create(NULL);
    lv_obj_set_style_bg_color(scr, lv_color_black(), 0);
    lv_scr_load(scr);

    // 创建一个对象用于表示矩形
    lv_obj_t *rect = lv_obj_create(scr);

    // 设置矩形的宽度为屏幕宽度，高度从y=20到屏幕底部
    lv_coord_t screen_width = lv_obj_get_width(scr);
    lv_coord_t screen_height = lv_obj_get_height(scr);

    // 设置矩形的大小和位置（往下移动遮住下边圆角）
    lv_obj_set_size(rect, screen_width, screen_height - 20);
    lv_obj_set_pos(rect, 0, 76); // 从y=20开始

    // 设置矩形的样式
    lv_obj_set_style_radius(rect, 40, 0);                 // 圆角半径
    lv_obj_set_style_bg_color(rect, lv_color_white(), 0); // 背景色白色
    lv_obj_set_style_bg_opa(rect, LV_OPA_COVER, 0);       // 不透明

    // 禁用滚动条
    lv_obj_set_scrollbar_mode(scr, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_scrollbar_mode(rect, LV_SCROLLBAR_MODE_OFF);

    // 禁用滚动
    lv_obj_set_scroll_dir(scr, LV_DIR_NONE);
    lv_obj_set_scroll_dir(rect, LV_DIR_NONE);

    return scr;
}

// 创建返回按钮
lv_obj_t *create_back_btn(lv_obj_t *parent)
{
    lv_obj_t *back_btn = create_btn_img(parent, "A:/data/images/back_btn.png");
    lv_obj_set_pos(back_btn, 0, 716);
    lv_obj_set_style_bg_color(back_btn, lv_color_white(), 0);
    lv_obj_set_style_pad_all(back_btn, 20, LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(back_btn, 0, 0);
    return back_btn;
}

// 时间管理页面
void create_time_manage(void)
{
    // lvgl_page_basic_t *title = lvgl_page_basic_init(scr);
    // lvgl_page_basic_set_text(title, "时间管理"); // 设置标题
    lv_obj_t *scr = clock_base_page_init(); // clock 基础页面
    // 添加返回按钮
    lv_obj_t *back_btn = create_back_btn(scr);

    // 右下角新增闹钟按钮
    lv_obj_t *add_clock_btn = create_btn_img(scr, "A:/data/images/add_btn.png");
    lv_obj_set_pos(add_clock_btn, 400, 722);

    // 时间管理主要内容
    // tabview 按钮
    lv_obj_t *tv_btns = lv_obj_create(scr);
    lv_obj_set_size(tv_btns, 440, 54);
    lv_obj_align(tv_btns, LV_ALIGN_TOP_MID, 0, 96);
    lv_obj_set_style_radius(tv_btns, 30, 0);
    lv_obj_set_style_border_color(tv_btns, lv_color_black(), 0);

    // 禁用滚动条
    lv_obj_set_scrollbar_mode(tv_btns, LV_SCROLLBAR_MODE_OFF);
    // 禁用滚动
    lv_obj_set_scroll_dir(tv_btns, LV_DIR_NONE);

    lv_obj_t *tv_btn1 = lv_btn_create(tv_btns);
    lv_obj_set_size(tv_btn1, 214, 42);
    lv_obj_align(tv_btn1, LV_ALIGN_LEFT_MID, -13, 0);
    lv_obj_set_style_radius(tv_btn1, 30, 0);
    lv_obj_set_style_bg_color(tv_btn1, lv_color_black(), 0);
    lv_obj_t *text1 = lv_label_create(tv_btn1);
    lv_obj_center(text1);
    lv_obj_set_style_text_font(text1, font18.font, LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(text1, lv_color_white(), LV_STATE_DEFAULT);
    lv_label_set_text(text1, "闹钟");

    lv_obj_t *tv_btn2 = lv_btn_create(tv_btns);
    lv_obj_set_size(tv_btn2, 214, 42);
    lv_obj_align(tv_btn2, LV_ALIGN_RIGHT_MID, 13, 0);
    lv_obj_set_style_radius(tv_btn2, 30, 0);
    lv_obj_set_style_bg_color(tv_btn2, lv_color_white(), 0);
    lv_obj_set_style_shadow_opa(tv_btn2, LV_OPA_TRANSP, 0);
    lv_obj_t *text2 = lv_label_create(tv_btn2);
    lv_obj_center(text2);
    lv_obj_set_style_text_font(text2, font18.font, LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(text2, lv_color_black(), LV_STATE_DEFAULT);
    lv_label_set_text(text2, "计时");

    // tabview 内容
    // 闹钟
    lv_obj_t *tv_cont1 = lv_obj_create(scr);
    lv_obj_set_size(tv_cont1, 480, 570);
    lv_obj_align(tv_cont1, LV_ALIGN_TOP_MID, 0, 150);
    lv_obj_set_style_border_width(tv_cont1, 0, 0);
    // lv_obj_set_style_border_color(tv_cont1, lv_color_hex(0xff0000), 0); // 调试用
    lv_obj_set_style_pad_all(tv_cont1, 0, 0);

    render_alarm_list(tv_cont1);

    // 计时
    lv_obj_t *tv_cont2 = lv_obj_create(scr);
    lv_obj_set_size(tv_cont2, 480, 575);
    lv_obj_align(tv_cont2, LV_ALIGN_TOP_MID, 0, 150);
    lv_obj_set_style_border_width(tv_cont2, 0, 0);
    // lv_obj_set_style_border_color(tv_cont2, lv_color_hex(0x00ff00), 0); // 调试用
    lv_obj_set_style_pad_all(tv_cont2, 0, 0);
    lv_obj_add_flag(tv_cont2, LV_OBJ_FLAG_HIDDEN);

    create_timer_btn(tv_cont2, "60", 80, 30);
    create_timer_btn(tv_cont2, "30", 280, 30);
    create_timer_btn(tv_cont2, "25", 80, 170);
    create_timer_btn(tv_cont2, "15", 280, 170);
    create_timer_btn(tv_cont2, "10", 80, 310);
    create_timer_btn(tv_cont2, "5", 280, 310);
    create_timer_btn(tv_cont2, "1", 80, 450);
    create_timer_btn(tv_cont2, "自定义", 280, 450);

    // 禁用滚动条
    lv_obj_set_scrollbar_mode(tv_cont1, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_scrollbar_mode(tv_cont2, LV_SCROLLBAR_MODE_OFF);

    // 内存分配
    my_tabview = malloc(sizeof(my_tabview_t));
    my_tabview->tab_btns[0] = tv_btn1;
    my_tabview->tab_btns[1] = tv_btn2;

    my_tabview->content[0] = tv_cont1;
    my_tabview->content[1] = tv_cont2;

    my_tabview->add_clock_btn = add_clock_btn;

    my_tabview->active_tab = 0;

    // 顶部切换按钮添加事件
    lv_obj_add_event_cb(tv_btn1, click_tv_btn_cb, LV_EVENT_CLICKED, my_tabview);
    lv_obj_add_event_cb(tv_btn2, click_tv_btn_cb, LV_EVENT_CLICKED, my_tabview);

    // 添加返回按钮事件
    lv_obj_add_event_cb(back_btn, click_back_btn_cb, LV_EVENT_CLICKED, "home");
    // 添加新建闹钟按钮事件
    lv_obj_add_event_cb(add_clock_btn, add_clock_btn_cb, LV_EVENT_CLICKED, NULL);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
// 释放内存
static void free_all_mem(void)
{
    free(my_tabview);
    // free(alarm_del_data); // 点击删除时已经释放
}
// 返回按钮点击事件
static void click_back_btn_cb(lv_event_t *e)
{
    char *back_path = lv_event_get_user_data(e); // 获取添加事件时传递的用户数据
    if (strcmp(back_path, "home") == 0)
    {
        // 返回首页
        printf("back home.\n");
        free_all_mem(); // 释放内存
        create_palmi_ui();
    }
    // else if (strcmp(back_path, "clock") == 0)
    // {
    //     // 返回闹钟列表页面
    //     click_clock(e);
    // }
}
// 闹钟顶部栏点击事件
static void click_tv_btn_cb(lv_event_t *e)
{
    lv_obj_t *obj = lv_event_get_target(e);      // 获取触发事件的部件(对象)
    lv_event_code_t code = lv_event_get_code(e); // 获取当前部件(对象)触发的事件代码
    // lv_obj_t * data = lv_event_get_user_data(e); // 获取添加事件时传递的用户数据
    my_tabview_t *data = lv_event_get_user_data(e);
    // 找到被点击的按钮的索引
    uint32_t tab_index = 0;
    for (uint32_t i = 0; i < TAB_COUNT; i++)
    {
        if (data->tab_btns[i] == obj)
        {
            tab_index = i;
            break;
        }
    }

    // 更新激活的标签页 (active_tab 是之前被激活的, tab_index 是当前被点击的)
    if (data->active_tab != tab_index)
    {
        // 获取对应按钮的文本
        lv_obj_t *label = lv_obj_get_child(obj, 0);                               // 触发事件的按钮 label
        lv_obj_t *label2 = lv_obj_get_child(data->tab_btns[data->active_tab], 0); // 另一按钮 label2
        // lv_obj_set_style_text_color(label, lv_color_hex(0xff0000), 0);

        // 修改自身样式
        lv_obj_set_style_bg_color(obj, lv_color_black(), 0);
        lv_obj_set_style_shadow_opa(obj, LV_OPA_COVER, 0);
        lv_obj_set_style_text_color(label, lv_color_white(), 0);

        // 恢复另一按钮样式
        lv_obj_set_style_bg_color(data->tab_btns[data->active_tab], lv_color_white(), 0);
        lv_obj_set_style_shadow_opa(data->tab_btns[data->active_tab], LV_OPA_TRANSP, 0);
        lv_obj_set_style_text_color(label2, lv_color_black(), 0);

        // 隐藏旧内容并取消旧按钮的选中状态
        lv_obj_add_flag(data->content[data->active_tab], LV_OBJ_FLAG_HIDDEN);
        lv_obj_set_style_border_color(data->tab_btns[data->active_tab], lv_color_hex(0xff0000), 0);

        // 显示新内容并选中新按钮
        lv_obj_clear_flag(data->content[tab_index], LV_OBJ_FLAG_HIDDEN);
        lv_obj_set_style_border_color(data->tab_btns[tab_index], lv_color_hex(0x00ff00), 0);

        if (tab_index == 0)
        {
            lv_obj_clear_flag(data->add_clock_btn, LV_OBJ_FLAG_HIDDEN);
        }
        else
        {
            lv_obj_add_flag(data->add_clock_btn, LV_OBJ_FLAG_HIDDEN);
        }

        data->active_tab = tab_index;
    }
}

// 点击添加闹钟按钮跳转至新建闹钟页面
static void add_clock_btn_cb(lv_event_t *e)
{
    printf("add clock.\n");
    free_all_mem(); // 释放内存
    create_new_alarm_page();
}

// 计时按钮点击事件回调函数
static void click_timer_btn_cb(lv_event_t *e)
{
    lv_obj_t *btn = lv_event_get_target(e);
    char *btn_text = lv_event_get_user_data(e); // 获取添加事件时传递的用户数据
    cache_timing_page = lv_scr_act();           // 进入计时页面前保存当前页面
    if (strcmp(btn_text, "自定义") == 0)
    {
        printf("render custom timer \n");
        custom_countdown_page_init();
    }
    else
    {
        printf("render start timer \n");
        countdown_page_init(btn_text);
    }
}

// 创建闹钟 item
static lv_obj_t *create_alarm_item(lv_obj_t *parent, alarm_info_t *alarm_info, uint32_t alarm_index)
{
    // 创建一个包含闹钟信息的容器
    lv_obj_t *alarm_item = lv_obj_create(parent);
    lv_obj_set_size(alarm_item, 480, 120);
    lv_obj_align(alarm_item, LV_ALIGN_TOP_MID, 0, (120 * alarm_index)); // 设置位置，依据 alarmindex 调整垂直偏移
    // lv_obj_set_style_bg_color(alarm_item, lv_color_white(), 0);
    // lv_obj_set_style_border_color(alarm_item, lv_color_hex(0x0000ff), 0);
    lv_obj_set_style_pad_all(alarm_item, 20, 0);

    // 设置底部边框
    lv_obj_set_style_border_width(alarm_item, 1, 0);                    // 设置边框宽度
    lv_obj_set_style_border_color(alarm_item, lv_color_black(), 0);     // 设置边框颜色
    lv_obj_set_style_border_side(alarm_item, LV_BORDER_SIDE_BOTTOM, 0); // 仅设置底部边框

    // 创建显示闹钟时间的标签
    char time_str[6]; // "HH:MM"
    snprintf(time_str, sizeof(time_str), "%02d:%02d", alarm_info->hour, alarm_info->minute);
    lv_obj_t *time_label = lv_label_create(alarm_item);
    lv_label_set_text(time_label, time_str);
    lv_obj_align(time_label, LV_ALIGN_TOP_LEFT, 0, 0); // 调整位置
    lv_obj_set_style_text_font(time_label, font18.font, LV_STATE_DEFAULT);

    // 创建显示重复类型的标签
    lv_obj_t *repeat_btn = lv_obj_create(alarm_item);
    lv_obj_set_size(repeat_btn, 58, 27);
    lv_obj_align(repeat_btn, LV_ALIGN_BOTTOM_LEFT, 0, 0); // 调整位置
    lv_obj_set_style_border_width(repeat_btn, 1, 0);      // 设置边框宽度
    lv_obj_set_style_border_color(repeat_btn, lv_color_black(), 0);
    lv_obj_set_style_radius(repeat_btn, 8, 0);

    // 禁用滚动条
    lv_obj_set_scrollbar_mode(repeat_btn, LV_SCROLLBAR_MODE_OFF);
    // 禁用滚动
    lv_obj_set_scroll_dir(repeat_btn, LV_DIR_NONE);

    const char *repeat_str = NULL;
    switch (alarm_info->alarm_type)
    {
    case REPEAT_EVERY_DAY:
        repeat_str = "每天";
        break;
    case REPEAT_WORKDAYS:
        repeat_str = "工作日";
        lv_obj_set_size(repeat_btn, 73, 27);
        break;
    case REPEAT_ONCE:
        repeat_str = "一次";
        break;
    }
    lv_obj_t *repeat_label = lv_label_create(repeat_btn);
    lv_label_set_text(repeat_label, repeat_str);
    lv_obj_center(repeat_label);
    lv_obj_set_style_text_font(repeat_label, font18.font, LV_STATE_DEFAULT);

    // 删除按钮
    lv_obj_t *del_btn = lv_obj_create(alarm_item);
    lv_obj_set_size(del_btn, 64, 32);
    lv_obj_align(del_btn, LV_ALIGN_RIGHT_MID, -80, 0);
    lv_obj_t *del_label = lv_label_create(del_btn);
    lv_label_set_text(del_label, "删除");
    lv_obj_center(del_label);
    lv_obj_set_style_text_font(del_label, font18.font, LV_STATE_DEFAULT);
    // 禁用滚动条
    lv_obj_set_scrollbar_mode(del_btn, LV_SCROLLBAR_MODE_OFF);
    // 禁用滚动
    lv_obj_set_scroll_dir(del_btn, LV_DIR_NONE);

    // 创建闹钟开关
    lv_obj_t *switch_btn = lv_switch_create(alarm_item);
    lv_obj_align(switch_btn, LV_ALIGN_RIGHT_MID, 0, 0); // 调整位置
    lv_obj_set_style_bg_color(switch_btn, lv_color_black(), 0);
    // 开关文本
    lv_obj_t *switch_label = lv_label_create(switch_btn);
    lv_obj_set_style_text_color(switch_label, lv_color_white(), 0);
    lv_obj_set_style_text_font(switch_label, font18.font, 0);
    // 修改开关状态指示器部分，打开状态时的背景颜色
    lv_obj_set_style_bg_color(switch_btn, lv_color_hex(0x000000), LV_PART_INDICATOR | LV_STATE_CHECKED);

    if (alarm_info->is_enabled)
    {
        lv_obj_add_state(switch_btn, LV_STATE_CHECKED);
        lv_label_set_text(switch_label, "开");
        lv_obj_align(switch_label, LV_ALIGN_LEFT_MID, 4, 0);
    }
    else
    {
        lv_obj_clear_state(switch_btn, LV_STATE_CHECKED);
        lv_label_set_text(switch_label, "关");
        lv_obj_align(switch_label, LV_ALIGN_RIGHT_MID, -4, 0);
    }

    // 动态分配内存并设置 index 和 parent
    alarm_del_data = malloc(sizeof(alarm_del_data_t));
    alarm_del_data->index = alarm_index;
    alarm_del_data->parent = parent;
    lv_obj_add_event_cb(del_btn, alarm_del_event_cb, LV_EVENT_CLICKED, alarm_del_data);
    lv_obj_add_event_cb(switch_btn, switch_btn_event_cb, LV_EVENT_VALUE_CHANGED, alarm_info);
}

// 闹钟删除事件
static void alarm_del_event_cb(lv_event_t *e)
{
    lv_obj_t *obj = lv_event_get_target(e);
    alarm_del_data_t *data = (alarm_del_data_t *)lv_event_get_user_data(e);
    if (data != NULL)
    {
        if (data->index < 0 || data->index >= alarm_count)
            return;

        // 将数组中的元素向前移动，覆盖被删除的元素
        for (int i = data->index; i < alarm_count - 1; i++)
        {
            alarms[i] = alarms[i + 1];
        }
        alarm_count--;                   // 减少计数
        render_alarm_list(data->parent); // 重新渲染闹钟列表
        free(data);                      // 释放内存
    }
}

// switch 事件
static void switch_btn_event_cb(lv_event_t *e)
{
    lv_obj_t *obj = lv_event_get_target(e);
    alarm_info_t *alarm_info = lv_event_get_user_data(e);
    bool is_checked = lv_obj_has_state(obj, LV_STATE_CHECKED);

    lv_obj_t *switch_label = lv_obj_get_child(obj, 0);
    if (is_checked)
    {
        lv_label_set_text(switch_label, "开");
        lv_obj_align(switch_label, LV_ALIGN_LEFT_MID, 4, 0);
        alarm_info->is_enabled = true;
    }
    else
    {
        lv_label_set_text(switch_label, "关");
        lv_obj_align(switch_label, LV_ALIGN_RIGHT_MID, -4, 0);
        alarm_info->is_enabled = false;
    }
}

// 闹钟列表渲染
static void render_alarm_list(lv_obj_t *parent)
{
    lv_obj_clean(parent); // 清空父对象中的所有子对象

    // 检查是否有有效的闹钟
    bool has_valid_alarm = false;
    for (uint32_t i = 0; i < alarm_count; i++)
    {
        if (alarms[i].is_valid)
        {
            has_valid_alarm = true;
            break;
        }
    }
    // 根据闹钟列表数据创建相应内容
    if (!has_valid_alarm)
    {
        // 当没有闹钟时显示的内容
        lv_obj_t *no_alarm_label = lv_label_create(parent);
        lv_obj_align(no_alarm_label, LV_ALIGN_TOP_MID, 0, 230);
        lv_obj_set_style_text_font(no_alarm_label, font18.font, 0);
        lv_label_set_text(no_alarm_label, "无闹钟");

        // 创建设置闹钟按钮
        lv_obj_t *set_alarm_btn = lv_btn_create(parent);
        lv_obj_set_size(set_alarm_btn, 160, 50);
        lv_obj_align(set_alarm_btn, LV_ALIGN_TOP_MID, 0, 280);
        lv_obj_set_style_radius(set_alarm_btn, 100, 0);
        lv_obj_set_style_bg_color(set_alarm_btn, lv_color_white(), 0);
        lv_obj_set_style_border_width(set_alarm_btn, 1, 0);

        lv_obj_t *set_alarm_text = lv_label_create(set_alarm_btn);
        lv_obj_center(set_alarm_text);
        lv_obj_set_style_text_font(set_alarm_text, font22.font, 0);
        lv_obj_set_style_text_color(set_alarm_text, lv_color_black(), 0);
        lv_label_set_text(set_alarm_text, "设置");

        lv_obj_add_event_cb(set_alarm_btn, add_clock_btn_cb, LV_EVENT_CLICKED, NULL);
    }
    else
    {
        for (uint32_t i = 0; i < alarm_count; i++)
        {
            if (alarms[i].is_valid)
            {
                lv_obj_t *alarm_item = create_alarm_item(parent, &alarms[i], i);
            }
        }
    }
}

// 渲染计时按钮
static void create_timer_btn(lv_obj_t *parent, char *btn_text, lv_coord_t x, lv_coord_t y)
{
    // 1. 创建按钮
    lv_obj_t *btn = lv_btn_create(parent); // 创建按钮
    lv_obj_set_size(btn, 120, 120);        // 设置按钮大小为 120x120
    lv_obj_set_pos(btn, x, y);             // 按给定的 x 和 y 位置对齐按钮

    // 2. 设置按钮样式
    lv_obj_set_style_radius(btn, LV_RADIUS_CIRCLE, 0);       // 设置圆角半径为圆形
    lv_obj_set_style_bg_color(btn, lv_color_white(), 0);     // 设置背景颜色为白色
    lv_obj_set_style_border_color(btn, lv_color_black(), 0); // 设置边框颜色为黑色
    lv_obj_set_style_border_width(btn, 2, 0);                // 设置边框宽度
    lv_obj_set_layout(btn, LV_LAYOUT_FLEX);                  // 使用Flex布局
    lv_obj_set_flex_flow(btn, LV_FLEX_FLOW_COLUMN);          // 垂直布局
    lv_obj_set_flex_align(btn, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    // 3. 添加自定义文本
    lv_obj_t *label1 = lv_label_create(btn); // 在容器内创建第一个标签
    lv_label_set_text(label1, btn_text);     // 设置标签1的文本
    lv_obj_set_style_text_color(label1, lv_color_black(), 0);
    lv_obj_set_style_text_align(label1, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_text_font(label1, font30.font, 0);

    // 4. 如果文本不是 "自定义"，再添加 "min" 文本
    if (strcmp(btn_text, "自定义") != 0)
    {
        lv_obj_t *label2 = lv_label_create(btn); // 在容器内创建第二个标签
        lv_label_set_text(label2, "min");        // 设置标签2的文本
        lv_obj_set_style_text_color(label2, lv_color_black(), 0);
        lv_obj_set_style_text_align(label2, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_set_style_text_font(label2, font18.font, 0);
    }

    lv_obj_add_event_cb(btn, click_timer_btn_cb, LV_EVENT_CLICKED, btn_text);
}
