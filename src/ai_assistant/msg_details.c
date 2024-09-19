/**
 * @file msg_details.c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include "msg_details.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/
// 创建消息详情页面
void *create_msg_details(void)
{
    // basic_page_init("AI 助手");              // 状态栏
    lv_obj_t *scr = clock_base_page_init(); // clock 基础页面
    // 添加返回按钮
    lv_obj_t *back_btn = create_back_btn(scr);
}

/**********************
 *   STATIC FUNCTIONS
 **********************/
