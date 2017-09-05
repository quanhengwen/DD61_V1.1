
///**
//  ********************************  STM32F0x1  *********************************
//  * @文件名     ： menu.h
//  * @作者       ： HarryZeng
//  * @库版本     ： V1.5.0
//  * @文件版本   ： V1.0.0
//  * @日期       ： 2017年05月15日
//  * @摘要       ： 数据处理
//  ******************************************************************************/
///*----------------------------------------------------------------------------
//  更新日志:
//  2017-05-15 V1.0.0:初始版本
//  ----------------------------------------------------------------------------*/
///* 包含的头文件 --------------------------------------------------------------*/
/* Includes ------------------------------------------------------------------*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __menu_H
#define __menu_H
#ifdef __cplusplus
 extern "C" {
#endif
#include  "main.h"
#include 	"stdbool.h"
#include 	"stdint.h"
	 

typedef enum
{
		TOFF = 31U,
		OFFD,
		ON_D,
		SHOT
}DELAY_MODE;	 
	 
	 
typedef struct 
{
	DELAY_MODE 	DelayMode;		
	int16_t 		DelayValue;	
	uint32_t    DelayCounter;
}OUT1_DELAY_MODE_STRUCT;
	 

extern int32_t CSV;	 
extern OUT1_DELAY_MODE_STRUCT OUT1_Mode;

void menu(void);

#ifdef __cplusplus
}
#endif
#endif /*__menu_H */
