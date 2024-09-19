
#ifndef CHAT_DETAILS_H
#define CHAT_DETAILS_H

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

    /**********************
     * GLOBAL PROTOTYPES
     **********************/
    void *create_msg_details(void); // 创建消息详情页面

    /**********************
     *      MACROS
     **********************/

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* CHAT_DETAILS_H */
