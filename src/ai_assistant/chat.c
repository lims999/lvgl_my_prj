// 基础页面 ，状态栏标题等内容。
/*********************
 *      INCLUDES
 *********************/
#include "chat.h"

/*********************
 *      DEFINES
 *********************/
#define MAX_MESSAGES 100 // 最大消息数量

/**********************
 *      TYPEDEFS
 **********************/
// 枚举，消息发送者（用户或AI）
typedef enum
{
    SENDER_USER,
    SENDER_AI
} sender_type_t;

// 消息结构体
typedef struct
{
    sender_type_t sender; // 消息发送者（用户或AI）
    char *text;           // 文本内容（文本消息或AI回复）
    char *voice_path;     // 语音文件路径（用户语音或AI语音）
} chat_msg_t;

// 定义一个聊天列表数组
chat_msg_t chat_list[MAX_MESSAGES] = {
    {SENDER_AI, "你好，我是AI助手，请问有什么可以帮到你？", "xxx.mp3"},
    {SENDER_USER, NULL, "xxx.mp3"},
    {SENDER_AI, "你好，我是AI助手，请问有什么可以帮到你？", "xxx.mp3"},
    {SENDER_USER, NULL, "xxx.mp3"},
    {SENDER_AI, "你好，我是AI助手，请问有什么可以帮到你？", "xxx.mp3"},
    {SENDER_AI, "你好，我是AI助手，请问有什么可以帮到你？你好，我是AI助手，请问有什么可以帮到你？你好，我是AI助手，请问有什么可以帮到你？你好，我是AI助手，请问有什么可以帮到你？你好，我是AI助手，请问有什么可以帮到你？你好，我是AI助手，请问有什么可以帮到你？你好，我是AI助手，请问有什么可以帮到你？", "xxx.mp3"},
    {SENDER_AI, "你好，我是AI助手，请问有什么可以帮到你？", "xxx.mp3"},
    {SENDER_AI, "你好，我是AI助手，请问有什么可以帮到你？", "xxx.mp3"},
    {SENDER_AI, "你好，我是AI助手，请问有什么可以帮到你？", "xxx.mp3"},
    {SENDER_AI, "你好，我是AI助手，请问有什么可以帮到你？", "xxx.mp3"},
};
int chat_count = 10; // 记录聊天消息的数量

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void click_back_btn_cb(lv_event_t *e);                                        // 返回按钮点击事件
static void click_speech_btn_cb(lv_event_t *e);                                      // 语音输入按钮长按事件
static void click_mute_btn_cb(lv_event_t *e);                                        // 静音按钮点击事件
static void click_tip_btn_cb(lv_event_t *e);                                         // 联网、绑定按钮点击事件
static void click_play_btn_cb(lv_event_t *e);                                        // 点击 播放语音消息 按钮
static void click_knowledge_btn_cb(lv_event_t *e);                                   // 点击 添加至知识库 按钮
static void click_view_all_btn_cb(lv_event_t *e);                                    // 点击 查看完整内容 按钮
static void add_msg(sender_type_t sender, const char *text, const char *voice_path); // 添加消息
static void update_chat_list_ui(void);                                               // 更新聊天列表
static void init_chat_ui(void);                                                      // 初始化聊天内容列表

void toast_init(); // 初始化 toast
void toast_show(); // 显示 toast
void toast_hide(); // 隐藏 toast

/**********************
 *  STATIC VARIABLES
 **********************/
static lv_obj_t *chat_container = NULL; // 聊天列表容器（全局定义用于后续更新）
static lv_obj_t *mic_modal = NULL;      // 语音输入时模态框

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
bool is_online = false;         // 是否联网
bool is_bind = false;           // 是否绑定
lv_obj_t *toast = NULL;         // toast 对象 配合 toast_init() toast_show() toast_hide() 使用
lv_timer_t *toast_timer = NULL; // toast 显示定时器 配合 toast_init() toast_show() toast_hide() 使用
// 初始化 Toast
void toast_init()
{
    // 当前屏幕上创建 Toast 对象
    toast = lv_obj_create(lv_scr_act());
    lv_obj_remove_style_all(toast);                                                                 // 移除默认样式
    lv_obj_set_size(toast, LV_SIZE_CONTENT, LV_SIZE_CONTENT);                                       // 根据内容自动调整大小
    lv_obj_align(toast, LV_ALIGN_BOTTOM_MID, 0, -140);                                              // 底部居中
    lv_obj_set_style_bg_color(toast, lv_color_white(), 0);                                          // 设置背景颜色为白色
    lv_obj_set_style_bg_opa(toast, LV_OPA_COVER, 0);                                                // 设置背景透明度
    lv_obj_set_style_radius(toast, LV_RADIUS_CIRCLE, 0);                                            // 设置圆角
    lv_obj_set_style_border_width(toast, 1, 0);                                                     // 设置边框
    lv_obj_set_style_border_color(toast, lv_color_hex(0x000000), 0);                                // 设置边框颜色
    lv_obj_set_style_pad_all(toast, 30, 0);                                                         // 内边距
    lv_obj_set_style_pad_top(toast, 10, 0);                                                         // 上边距
    lv_obj_set_style_pad_bottom(toast, 10, 0);                                                      // 下边距
    lv_obj_set_layout(toast, LV_LAYOUT_FLEX);                                                       // 使用Flex布局（默认横向布局）
    lv_obj_set_flex_align(toast, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER); // 设置对齐方式
    lv_obj_add_flag(toast, LV_OBJ_FLAG_HIDDEN);                                                     // 隐藏 toast

    // 创建一个icon
    lv_obj_t *icon = lv_img_create(toast);
    lv_img_set_src(icon, "I:/data/images/AI/failed.png");

    // 创建一个文本标签
    lv_obj_t *label = lv_label_create(toast);
    lv_label_set_text(label, "This is a toast box.");            // 设置 toast 文本内容
    lv_obj_set_style_text_color(label, lv_color_black(), 0);     // 设置文本颜色为白色
    lv_obj_set_style_text_align(label, LV_TEXT_ALIGN_CENTER, 0); // 居中对齐文本
    lv_obj_center(label);                                        // 文本在容器中居中
    lv_obj_set_style_pad_left(label, 10, 0);                     // 左边距
    lv_obj_set_style_text_font(label, font22.font, 0);           // 设置字体
}
// 显示 Toast
void toast_show()
{
    if (toast_timer)
    { // 判断当前是否有 Toast 显示，有的话先关闭当前
        lv_timer_del(toast_timer);
        toast_timer = NULL;
    }

    // 显示 Toast
    lv_obj_clear_flag(toast, LV_OBJ_FLAG_HIDDEN);

    // 设置定时器 2秒后隐藏
    toast_timer = lv_timer_create(toast_hide, 2000, NULL);
}

// 隐藏 Toast
void toast_hide(void)
{
    if (toast_timer)
    { // 如果当前存在toast，就关闭当前toast
        lv_timer_del(toast_timer);
        toast_timer = NULL;
    }
    // 隐藏 Toast
    lv_obj_add_flag(toast, LV_OBJ_FLAG_HIDDEN);
}
// 创建ai助手聊天页面
void *create_chat_page(void)
{
    // basic_page_init("AI 助手");              // 状态栏
    lv_obj_t *scr = clock_base_page_init(); // clock 基础页面
    // 添加返回按钮
    lv_obj_t *back_btn = create_back_btn(scr);

    // 语音输入按钮
    lv_obj_t *speech_btn = create_clock_public_btn(scr);
    lv_obj_set_width(speech_btn, 272);
    lv_obj_t *mic_icon = lv_img_create(speech_btn);
    lv_img_set_src(mic_icon, "I:/data/images/AI/mic_icon.png");
    lv_obj_center(mic_icon);

    // 静音按钮
    lv_obj_t *mute_btn = create_btn_img(scr, "I:/data/images/AI/voiced.png");
    lv_obj_align(mute_btn, LV_ALIGN_BOTTOM_RIGHT, -20, -20);

    // 聊天内容容器
    chat_container = lv_obj_create(scr);
    lv_obj_set_size(chat_container, 480, 600);
    lv_obj_set_pos(chat_container, 0, 106);
    lv_obj_set_layout(chat_container, LV_LAYOUT_FLEX);         // 使用 Flex 布局
    lv_obj_set_flex_flow(chat_container, LV_FLEX_FLOW_COLUMN); // 纵向布局

    lv_obj_set_style_border_width(chat_container, 0, 0); // 调试
    lv_obj_set_style_border_color(chat_container, lv_color_hex(0x0000ff), 0);

    // 调整 聊天内容容器顶部 padding
    if (is_online && is_bind)
    {
        lv_obj_set_style_pad_top(chat_container, 0, 0);
    }
    else
    {
        lv_obj_set_style_pad_top(chat_container, 56, 0);
    }

    update_chat_list_ui(); // 初始化聊天列表

    // 页面顶部提示
    lv_obj_t *topTip = lv_obj_create(scr);
    lv_obj_set_style_bg_color(topTip, lv_color_hex(0x000000), 0);
    lv_obj_set_size(topTip, 480, 56);
    lv_obj_set_pos(topTip, 0, 76);
    lv_obj_set_style_border_side(topTip, LV_BORDER_SIDE_TOP, 0); // 显示上边框
    lv_obj_set_style_border_width(topTip, 1, 0);
    lv_obj_set_style_border_color(topTip, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_border_opa(topTip, LV_OPA_50, 0);
    lv_obj_set_style_radius(topTip, 0, 0);
    lv_obj_set_style_pad_all(topTip, 0, 0);
    lv_obj_set_layout(topTip, LV_LAYOUT_FLEX);      // 使用Flex布局
    lv_obj_set_flex_flow(topTip, LV_FLEX_FLOW_ROW); // 横向布局
    lv_obj_set_flex_align(topTip, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    // 提示 icon
    lv_obj_t *topTipIcon = lv_img_create(topTip);
    lv_img_set_src(topTipIcon, "I:/data/images/AI/tip_icon.png");

    // 提示文字
    lv_obj_t *topTipText = lv_label_create(topTip);
    lv_obj_set_style_text_color(topTipText, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(topTipText, font18.font, 0);

    lv_obj_set_style_border_width(topTipText, 1, 0); // 调试
    lv_obj_set_style_border_color(topTipText, lv_color_hex(0xff0000), 0);

    // 操作按钮
    lv_obj_t *topTipBtn = lv_obj_create(topTip);
    lv_obj_set_size(topTipBtn, 100, 56);
    lv_obj_set_style_bg_color(topTipBtn, lv_color_hex(0x000000), 0);
    lv_obj_set_layout(topTipBtn, LV_LAYOUT_FLEX);      // 使用Flex布局
    lv_obj_set_flex_flow(topTipBtn, LV_FLEX_FLOW_ROW); // 横向布局
    lv_obj_set_flex_align(topTipBtn, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_set_style_border_width(topTipBtn, 1, 0); // 调试
    lv_obj_set_style_border_color(topTipBtn, lv_color_hex(0xff0000), 0);

    // 操作按钮文字
    lv_obj_t *topTipBtnText = lv_label_create(topTipBtn);
    lv_obj_set_style_text_color(topTipBtnText, lv_color_hex(0xffffff), 0);
    lv_obj_set_style_text_font(topTipBtnText, font18.font, 0);
    lv_obj_set_style_text_decor(topTipBtnText, LV_TEXT_DECOR_UNDERLINE, 0);

    // 根据联网、绑定状态显示不同的提示
    if (!is_online)
    {
        lv_label_set_text(topTipText, "当前未联网，请先连接网络");
        lv_label_set_text(topTipBtnText, "点击联网");
    }
    else if (!is_bind)
    {
        lv_label_set_text(topTipText, "您未绑定Palmi账号，部分功能无法启用哟");
        lv_label_set_text(topTipBtnText, "点击绑定");
    }
    // 联网且绑定时不显示提示
    if (is_online && is_bind)
    {
        lv_obj_add_flag(topTip, LV_OBJ_FLAG_HIDDEN);
    }
    // lv_obj_add_flag(topTip, LV_OBJ_FLAG_HIDDEN);

    // 创建模态框，长按语音输入按钮时显示
    mic_modal = lv_obj_create(scr);
    lv_obj_set_size(mic_modal, 327, 217);
    lv_obj_align(mic_modal, LV_ALIGN_BOTTOM_MID, 0, -140);
    lv_obj_set_style_border_color(mic_modal, lv_color_hex(0x000000), 0);
    lv_obj_set_style_radius(mic_modal, 20, 0);
    lv_obj_set_layout(mic_modal, LV_LAYOUT_FLEX);
    lv_obj_set_flex_flow(mic_modal, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(mic_modal, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_add_flag(mic_modal, LV_OBJ_FLAG_HIDDEN); // 隐藏语音输入模态框（长按时改变状态）

    // 模态框内部控件
    lv_obj_t *mic_tip_icon = lv_img_create(mic_modal);
    lv_img_set_src(mic_tip_icon, "I:/data/images/AI/mic_icon_big.png");
    lv_obj_t *mic_tip_text = lv_label_create(mic_modal);
    lv_obj_set_style_text_color(mic_tip_text, lv_color_hex(0x000000), 0);
    lv_obj_set_style_text_font(mic_tip_text, font22.font, 0);
    lv_label_set_text(mic_tip_text, "我在听...");

    init_chat_ui(); // 添加初始聊天界面消息（AI助手欢迎词）
    toast_init();   // 初始化 Toast 最后调用确保 Toast 在最上层显示

    // 禁用滚动条
    lv_obj_set_scrollbar_mode(topTip, LV_SCROLLBAR_MODE_OFF);         // 顶部提示栏禁用滚动条
    lv_obj_set_scrollbar_mode(topTipBtn, LV_SCROLLBAR_MODE_OFF);      // 顶部操作栏禁用滚动条
    lv_obj_set_scrollbar_mode(chat_container, LV_SCROLLBAR_MODE_OFF); // 聊天列表禁用滚动条
    // 禁用滚动
    lv_obj_set_scroll_dir(topTip, LV_DIR_NONE);
    lv_obj_set_scroll_dir(topTipBtn, LV_DIR_NONE);

    // 添加返回按钮事件
    lv_obj_add_event_cb(back_btn, click_back_btn_cb, LV_EVENT_CLICKED, NULL);
    // 添加操作按钮事件
    lv_obj_add_event_cb(topTipBtn, click_tip_btn_cb, LV_EVENT_CLICKED, NULL);

    // 添加长按语音输入按钮事件
    lv_obj_add_event_cb(speech_btn, click_speech_btn_cb, LV_EVENT_ALL, NULL);
    // 添加静音按钮事件
    lv_obj_add_event_cb(mute_btn, click_mute_btn_cb, LV_EVENT_CLICKED, NULL);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
// 返回按钮点击事件
static void click_back_btn_cb(lv_event_t *e)
{
    // 返回首页
    create_palmi_ui();
}

// 语音输入按钮长按事件
static void click_speech_btn_cb(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    // TODO: 长按事件暂未处理
    switch (code)
    {
    case LV_EVENT_LONG_PRESSED:
        lv_obj_clear_flag(mic_modal, LV_OBJ_FLAG_HIDDEN); // 长按时显示模态框
        // TODO: 长按时开始录音
        break;
    case LV_EVENT_RELEASED:
        lv_obj_add_flag(mic_modal, LV_OBJ_FLAG_HIDDEN); // 松开时隐藏模态框
        // TODO: 判断是否正在录音，停止录音
        break;

    default:
        break;
    }
}

// 静音按钮点击事件
static void click_mute_btn_cb(lv_event_t *e)
{
    lv_obj_t *btn = lv_event_get_target(e);

    // 使用 strstr 判断是 "voiced" / "mute"
    if (strstr(lv_img_get_src(lv_obj_get_child(btn, 0)), "voiced") != NULL)
    {
        lv_img_set_src(lv_obj_get_child(btn, 0), "I:/data/images/AI/mute.png");
        // TODO: 关闭音量
    }
    else
    {
        lv_img_set_src(lv_obj_get_child(btn, 0), "I:/data/images/AI/voiced.png");
        // TODO: 打开音量
    }
}

// 联网、绑定按钮点击事件
static void click_tip_btn_cb(lv_event_t *e)
{
    lv_obj_t *obj = lv_event_get_target(e);
    lv_obj_t *label = lv_obj_get_child(obj, 0); // 触发事件的按钮 label
    // 判断 label 文字进行联网或绑定
    if (strcmp(lv_label_get_text(label), "点击联网") == 0)
    {
        printf("click online.\n");
        // TODO: 联网
    }
    else if (strcmp(lv_label_get_text(label), "点击绑定") == 0)
    {
        printf("click bind.\n");
        // TODO: 绑定
    }
}

// 点击 播放语音消息 按钮
static void click_play_btn_cb(lv_event_t *e)
{
    printf("click play.\n");
    // TODO: 播放语音消息
}

// 点击 添加至知识库 按钮
static void click_knowledge_btn_cb(lv_event_t *e)
{
    if (!is_bind)
    {
        toast_show();
    }
    else
    {
        // TODO: 添加至知识库
    }
}

// 点击 查看完整内容 按钮
static void click_view_all_btn_cb(lv_event_t *e)
{
    printf("click_view_all_btn_cb.\n");
    // TODO: 查看完整内容
}

// 初始化聊天内容列表
static void init_chat_ui(void)
{
    // 添加AI欢迎词
    if (!is_online)
    {
        add_msg(SENDER_AI, "你好我是Palmi，你的AI助手。当前未联网,联网后您可以问我问题，或者让我帮您定制日程，记录知识。", NULL);
    }
    else
    {
        add_msg(SENDER_AI, "你好我是你的AI助手。您可以问我问题，或者让我帮您定制日程，记录知识。", NULL);
    }
}

// 添加消息
static void add_msg(sender_type_t sender, const char *text, const char *voice_path)
{
    if (chat_count >= MAX_MESSAGES)
    {
        // todo: 处理聊天记录溢出（聊天记录满了，可以选择清除旧消息或扩展数组大小）
        return;
    }

    chat_list[chat_count].sender = sender;                 // 设置发送者（类型）
    chat_list[chat_count].text = strdup(text);             // 复制消息文本
    chat_list[chat_count].voice_path = strdup(voice_path); // 复制语音路径

    chat_count++;
    update_chat_list_ui();
}

// 更新聊天列表
static void update_chat_list_ui(void)
{
    // 清除旧的聊天界面，重新渲染
    lv_obj_clean(chat_container);

    for (int i = 0; i < chat_count; i++)
    {

        if (chat_list[i].sender == SENDER_USER)
        {

            // 创建用户的聊天消息容器
            lv_obj_t *msg_container = lv_obj_create(chat_container);
            lv_obj_set_size(msg_container, LV_PCT(100), LV_SIZE_CONTENT);
            lv_obj_set_style_pad_all(msg_container, 0, 0);
            lv_obj_set_layout(msg_container, LV_LAYOUT_FLEX);                                                    // 使用Flex布局
            lv_obj_set_flex_align(msg_container, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER); // 将内容右对齐
            lv_obj_set_style_border_width(msg_container, 0, 0);

            // 用户发送的消息（语音）
            // lv_label_set_text(msg_text, chat_list[i].text);
            lv_obj_t *voice_msg_btn = create_btn_img(msg_container, "I:/data/images/AI/voice_msg.png");

            lv_obj_add_event_cb(voice_msg_btn, click_play_btn_cb, LV_EVENT_CLICKED, NULL); // 添加播放按钮事件
        }
        else if (chat_list[i].sender == SENDER_AI)
        {

            // 创建AI的聊天消息容器
            lv_obj_t *msg_container = lv_obj_create(chat_container);
            lv_obj_set_size(msg_container, LV_PCT(95), LV_SIZE_CONTENT);
            lv_obj_set_style_border_color(msg_container, lv_color_hex(0x000000), 0);
            lv_obj_set_style_border_width(msg_container, 1, 0);
            lv_obj_set_style_radius(msg_container, 20, 0);
            lv_obj_set_layout(msg_container, LV_LAYOUT_FLEX);         // 使用Flex布局
            lv_obj_set_flex_flow(msg_container, LV_FLEX_FLOW_COLUMN); // 垂直布局
            // lv_obj_set_flex_align(msg_container, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER); // 将内容右对齐

            // 创建聊天消息文本
            lv_obj_t *msg_text = lv_label_create(msg_container);
            lv_obj_set_size(msg_text, LV_PCT(100), LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(msg_text, font18.font, 0);

            lv_obj_set_style_border_width(msg_text, 1, 0); // 调试
            lv_obj_set_style_border_color(msg_text, lv_color_hex(0xff0000), 0);

            lv_label_set_long_mode(msg_text, LV_LABEL_LONG_DOT); // 超出部分显示"..."
            lv_obj_set_style_max_height(msg_text, 168, 0);       // 限制高度（根据 UED 6行文本高度为 168）
            // AI 回复的消息（语音 + 文本 + 操作选项）
            lv_label_set_text(msg_text, chat_list[i].text); // 显示 AI 回复的文本内容

            if (i != 0)
            {
                // 操作按钮
                lv_obj_t *msg_btns_box = lv_obj_create(msg_container);
                lv_obj_set_size(msg_btns_box, LV_PCT(100), LV_SIZE_CONTENT);
                lv_obj_set_layout(msg_btns_box, LV_LAYOUT_FLEX);                                                              // 使用Flex布局
                lv_obj_set_flex_flow(msg_btns_box, LV_FLEX_FLOW_ROW);                                                         // 水平布局
                lv_obj_set_flex_align(msg_btns_box, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER); // space-between
                lv_obj_set_style_pad_all(msg_btns_box, 0, 0);

                lv_obj_set_style_border_side(msg_btns_box, LV_BORDER_SIDE_TOP, 0); // 只显示上边框
                lv_obj_set_style_border_width(msg_btns_box, 1, 0);                 // 调试
                lv_obj_set_style_border_color(msg_btns_box, lv_color_hex(0x000000), 0);
                // 播放按钮
                lv_obj_t *play_btn = create_btn_img(msg_btns_box, "I:/data/images/AI/play_icon.png");
                // lv_obj_set_size(play_btn, 48, 48);

                // 添加至知识库按钮
                lv_obj_t *add_to_knowledge_btn = create_btn_img(msg_btns_box, "I:/data/images/AI/add_to_knowledge.png");

                // 查看完整内容按钮
                lv_obj_t *view_all_btn = create_btn_img(msg_btns_box, "I:/data/images/AI/view_all_icon.png");

                lv_obj_add_event_cb(play_btn, click_play_btn_cb, LV_EVENT_CLICKED, NULL);                  // 添加播放按钮事件
                lv_obj_add_event_cb(add_to_knowledge_btn, click_knowledge_btn_cb, LV_EVENT_CLICKED, NULL); // 添加知识库按钮事件
                lv_obj_add_event_cb(view_all_btn, click_view_all_btn_cb, LV_EVENT_CLICKED, NULL);          // 添加查看完整内容按钮事件
            }
        }
    }
}
