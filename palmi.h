/**
 * @file palmi.h
 * Include all palmi related headers
 */

// 头文件保护 确保头文件不会被多次包含
#ifndef PALMI_H
#define PALMI_H

// 这段代码确保在 C++ 编译环境中，C 语言的函数声明不会被名字修饰，从而实现 C 和 C++ 代码之间的正确链接。这对于编写可以在 C++ 项目中使用的 C 语言库非常重要。
#ifdef __cplusplus // 检查代码是否在 C++ 编译环境中编译。__cplusplus 是一个预定义的宏，仅在 C++ 编译器中定义。
extern "C"         // extern "C" 是一个 C++ 的特性，用于告诉 C++ 编译器按照 C 语言的方式来处理链接符号，从而避免名字修饰。
{
#endif

/*********************
 *      INCLUDES
 *********************/
#include <stdio.h>
#include <stdlib.h>
#include "../lvgl/lvgl.h"
#include "src/clock/clock.h"            // 时间管理
#include "src/clock/new_alarm.h"        // 新建闹钟
#include "src/clock/countdown.h"        // 倒计时
#include "src/clock/custom_countdown.h" // 自定义倒计时

#include "src/ai_assistant/chat.h" // AI 助手

/*********************
 *      DEFINES
 *********************/
/*Test  lvgl version*/
#if LV_VERSION_CHECK(8, 0, 0) == 0
#error "palmi: Wrong lvgl version"
#endif

    /**********************
     *      TYPEDEFS
     **********************/

    /**********************
     * GLOBAL PROTOTYPES
     **********************/

    /**********************
     *      MACROS
     **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif // 结束 extern "C" 块。

#endif /*LV_100ASK_TECH_DEMOS_H*/
