
#ifndef NEW_ALARM_H
#define NEW_ALARM_H

#ifdef __cplusplus
extern "C"
{
#endif

/*********************
 *      INCLUDES
 *********************/
#include "../../palmi.h"

    /*********************
     *      DEFINES
     *********************/

    /**********************
     *      TYPEDEFS
     **********************/

    /**********************
     * GLOBAL PROTOTYPES
     **********************/
    lv_obj_t *create_clock_public_btn(lv_obj_t *parent); // 创建闹钟公共按钮
    void create_new_alarm_page();                        // 新建闹钟页面
    void h_add_btn_cb(lv_event_t *e);                    // 点击小时加减按钮回调函数
    void h_minus_btn_cb(lv_event_t *e);                  // 点击分钟加减按钮回调函数
    void m_add_btn_cb(lv_event_t *e);                    // 点击分钟加减按钮回调函数
    void m_minus_btn_cb(lv_event_t *e);                  // 点击分钟加减按钮回调函数

    /**********************
     *      MACROS
     **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* NEW_ALARM_H */
