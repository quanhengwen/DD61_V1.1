
///**
//  ********************************  STM32F0x1  *********************************
//  * @文件名     ： key.h
//  * @作者       ： HarryZeng
//  * @库版本     ： V1.5.0
//  * @文件版本   ： V1.0.0
//  * @日期       ： 2017年05月11日
//  * @摘要       ： 数据处理
//  ******************************************************************************/
///*----------------------------------------------------------------------------
//  更新日志:
//  2017-05-11 V1.0.0:初始版本
//  ----------------------------------------------------------------------------*/
///* 包含的头文件 --------------------------------------------------------------*/
/* Includes ------------------------------------------------------------------*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __key_H
#define __key_H
#ifdef __cplusplus
 extern "C" {
#endif
#include  "stm32f0xx.h"
#include "differentialDC.h"
#include 	"stdbool.h"
#include 	"stdint.h"
	 
	 
#define KEY_LEVEL_1   		6000
#define KEY_LEVEL_1_SET   20	 
	 
#define KEY_LEVEL_2   		12000
#define KEY_LEVEL_2_SET   10

#define KEY_LEVEL_3   		10
#define KEY_LEVEL_3_SET   2 
	 
	 
typedef enum
{
		Shortpress = 11U,
		Longpress,
		ShortAndLong
}Button_MODE;

typedef enum
{
		WorkHigh = 21U,
		WorkLow
}Button_Work;
	 
typedef enum
{
		Release = 0U,
		Press,
}Button_Status;

typedef enum
{
		PressNOEffect = 31U,
		PressShort,
		PressLong
}Button_Effect;
	 
typedef  struct Button
{
		GPIO_TypeDef* ButtonPort;
		uint16_t ButtonPin;
		Button_MODE  Mode;
		Button_Status Status;
		Button_Effect Effect;
		int16_t PressCounter;
		int16_t LastCounter;
		uint32_t PressTimer;
		uint32_t ShortTime;
		uint32_t LongTime;
		Button_Work WorkIn;
//		void (* PressCallback)(struct Button* Button);
//		void (* ReleaseCallback)(struct Button* Button);
	
}ButtonStruct;	 


extern ButtonStruct SetButton;
extern ButtonStruct ModeButton;
extern ButtonStruct UpButton;
extern ButtonStruct DownButton;	

void PressCallback(ButtonStruct* Button);
void Key_Scan(void) ;
void Button_Init(void);
Button_Status ReadButtonStatus(ButtonStruct *Button);
#ifdef __cplusplus
}
#endif
#endif /*__key_H */
