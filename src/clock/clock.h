
#ifndef CLOCK_H
#define CLOCK_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************
 *      INCLUDES
 *********************/
#include "main.h"
#include "../../palmi.h"

    /*********************
     *      DEFINES
     *********************/

    /**********************
     *      TYPEDEFS    在头文件中定义结构体，以在多个源文件（.c 文件）之间共享和访问该结构体
     **********************/
    // 闹钟重复类型
    typedef enum
    {
        REPEAT_EVERY_DAY,
        REPEAT_WORKDAYS,
        REPEAT_ONCE
    } repeat_type;

    // 闹钟信息结构体
    typedef struct
    {
        int hour;               // 小时（0-23）
        int minute;             // 分钟（0-59）
        bool is_enabled;        // 闹钟开关状态
        repeat_type alarm_type; // 重复类型
        bool is_valid;          // 是否为有效闹钟
    } alarm_info_t;

    // 闹钟删除事件数据结构体
    typedef struct
    {
        int index;
        lv_obj_t *parent;
    } alarm_del_data_t;

    // 新增闹钟时的数据结构体
    typedef struct
    {
        alarm_info_t *alarm_info; // 闹钟信息
        lv_obj_t *h_label;        // 显示时的标签
        lv_obj_t *m_label;        // 显示时分的标签
    } alarm_add_data_t;

    /**********************
     * GLOBAL PROTOTYPES
     **********************/
    lv_obj_t *clock_base_page_init(void);        // 闹钟统一背景
    lv_obj_t *create_back_btn(lv_obj_t *parent); // 创建返回按钮
    void create_time_manage(void);               // 创建时间管理页面

    extern alarm_info_t alarms[20];
    extern uint32_t alarm_count;
    extern lv_obj_t *cache_timing_page; // 缓存当前页面，用于返回到计时页面不清除 tabview 选项

    /**********************
     *      MACROS
     **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* CLOCK_H */
