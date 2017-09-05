
///**
//  ********************************  STM32F0x1  *********************************
//  * @文件名     ： SelftStudy.c
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
#include "differentialDC.h"
#include "key.h"
#include "SelfStudy.h"
//#include "arm_math.h"
#include "display.h"
#include "bsp_eeprom_24xx.h"

/*第一次SET按键按下处理函数*/
void ADCINcalibration(void);

/*第二次SET按键按下处理函数*/
void GetMAXADCValue(void);

uint8_t selfDisplayEndFlay=0;
uint32_t  SET1ADC_Value=0;
extern uint8_t sample_finish;
uint32_t CalibrateADCValue=0;
extern int16_t adc_dma_tab[6];
void selfstudy(void)
{
	uint8_t OUT1_STATUS;
	/*保持OUT1的状态*/
	OUT1_STATUS = GPIO_ReadInputDataBit(OUT1_GPIO_Port,OUT1_Pin);/*获取当前的OUT1状态*/
	GPIO_WriteBit(OUT1_GPIO_Port,OUT1_Pin,(BitAction)OUT1_STATUS);/*保持着OUT1状态*/
	
	if(SetButton.Status == Press && ModeButton.Status==Press)
	{
		
	}
	else
	{
			/*第一次进入SET模式*/
			while(SetButton.Status == Press )     //只要在显示模式下第一次按下SET按键
			{	/*按着按键3秒内*/
				ADCINcalibration();
				Test_Delay(1000);
		//		/*这里要显示SET1*/
		//		if(EventFlag&Blink500msFlag) 
		//		{
		//			EventFlag = EventFlag &(~Blink500msFlag);  //清楚标志位
		//			SMG_DisplaySET_Step_1_Mode(0,ADCValue);
		//		}
				//EventFlag = EventFlag &(~ADVtimeFlag);
				//GetADCValue(&SET1ADC_Value);
				
				while(SetButton.Effect == PressShort) /*按下按键已经超过时间，而且没有释放，闪烁提醒*/
				{		
					/*3秒到了,闪烁提醒*/
					//if(EventFlag&Blink500msFlag) 
					//{
					//	EventFlag = EventFlag &(~Blink500msFlag);  //清楚标志位
						//if(EventFlag&ADVtimeFlag) //按下的3秒内，一直定时采集ADC值
						//{
							//EventFlag = EventFlag &(~ADVtimeFlag);  //清楚标志位
							//GetADCValue(&SET1ADC_Value);		//定时ADC采样
							SMG_DisplaySET_Step_1_Mode(2,CalibrateADCValue);
					//	}
						
					//}
					if(SetButton.Effect == PressLong && SetButton.Status==Press ) /*按键达到3秒后，第一次进入自学习，等待第二次按下SET 3秒*/
					{	/*3秒到了，并释放了按键*/
						/*一直等待第二次SET的按下*
						**************************/
						SetButton.LastCounter = SetButton.PressCounter;
						SetButton.Effect = PressShort;
						UpButton.PressCounter=0;
						UpButton.Effect=PressNOEffect;
						DownButton.PressCounter=0;
						DownButton.Effect=PressNOEffect;
						ModeButton.PressCounter=0;
						ModeButton.Effect=PressNOEffect;
						selfDisplayEndFlay =0;
						GetMAXADCValue();
					}
				}
			}
	}
}


/*在一系列的ADCvalue中寻找最大的ADV MAX*/
uint32_t 			ADCMAX=0;
uint32_t 		NewThreshold=0;
uint32_t 		TempMaxADCValue=0;
void GetMAXADCValue(void)
{
		uint32_t 		TempADCValue=0;
//		static uint8_t lastCounter;
	  TempMaxADCValue =0;
		while(selfDisplayEndFlay==0)     //第二次按下SET按键
		{
			SMG_DisplaySET_Step_1_Mode(0,ADCValue);
			while(SetButton.Status == Press )
			{
				//if(EventFlag&ADVtimeFlag) //按下的3秒内，一直定时采集ADC值
				//{
				//	EventFlag = EventFlag &(~ADVtimeFlag);  //清楚标志位
					GetADCValue(&TempADCValue);					//定时ADC采样
					
					if(TempADCValue>=TempMaxADCValue)
							TempMaxADCValue = TempADCValue;
				//}
				/*这里要显示SET2*/
				if(EventFlag&Blink500msFlag) 
				{
					EventFlag = EventFlag &(~Blink500msFlag);  //清楚标志位
					SMG_DisplaySET_Step_2_Mode(0,ADCValue,0);
				}
				
				while(SetButton.Effect == PressLong) /*按下按键已经超过时间，而且没有释放，闪烁提醒*/
				{	/*3秒到了,闪烁提醒*/
					if(EventFlag&Blink500msFlag) 
					{
						EventFlag = EventFlag &(~Blink500msFlag);  //清楚标志位
						SMG_DisplaySET_Step_2_Mode(1,0,0);     
					}				
					while(SetButton.Effect == PressLong && SetButton.Status == Release) /*按键达到3秒后，结束第二次SET按键*/
					{		/*3秒到了，并释放了按键*/
					//	arm_max_f32(TempADCValue_f[1],i,&ADCMAX,&MAX_Index);  /*获得最大的ADCIN值*/
						
						ADCMAX = TempMaxADCValue;
						NewThreshold = (CalibrateADCValue+ADCMAX)/2;				/*获得本次自学习的OUT1阈值*/
						
						GPIO_WriteBit(OUT1_GPIO_Port,OUT1_Pin,(BitAction)GPIO_ReadInputDataBit(OUT1_GPIO_Port,OUT1_Pin));
						GPIO_WriteBit(OUT2_GPIO_Port,OUT2_Pin,(BitAction)GPIO_ReadInputDataBit(OUT2_GPIO_Port,OUT2_Pin));
						
//							while(1)
//							{
//								SMG_DisplaySET_Step_2_Mode(2,ADCValue,NewThreshold);
								
//								
//													/*Up Button*/
//							if(UpButton.PressCounter !=lastCounter && UpButton.Effect==PressShort)
//							{
//								lastCounter = UpButton.PressCounter;
//								UpButton.PressCounter = 0;
//								NewThreshold = NewThreshold+1;
//							}
//							else 	if(UpButton.Status==Press&&(UpButton.Effect==PressLong))
//							{				/*还按着按键，并且时间超过长按时间*/
//								UpButton.PressCounter = 0;
//								if(UpButton.PressTimer<1000)
//								{
//									if(UpButton.PressTimer%40==0)
//										NewThreshold = NewThreshold+1;
//								}
//								else if(UpButton.PressTimer>1000&&UpButton.PressTimer<3000)
//								{
//									if(UpButton.PressTimer%10==0)
//										NewThreshold = NewThreshold+1;
//								}
//								else 
//								{
//									if(UpButton.PressTimer%2==0)
//										NewThreshold = NewThreshold+1;
//								}
//							}	
//							else
//							{
//								UpButton.Effect = PressShort;
//							}
//								/*Down Button*/
//							if(DownButton.PressCounter !=lastCounter && DownButton.Effect==PressShort)
//							{
//								DownButton.PressCounter = 0;
//								NewThreshold = NewThreshold-1;
//							}
//							else if(DownButton.Status==Press&&(DownButton.Effect==PressLong))
//							{
//								DownButton.PressCounter = 0;
//								if(DownButton.PressTimer<1000)
//								{
//									if(DownButton.PressTimer%40==0)
//										NewThreshold = NewThreshold-1;
//								}
//								else if(DownButton.PressTimer>1000&&DownButton.PressTimer<3000)
//								{
//									if(DownButton.PressTimer%10==0)
//										NewThreshold = NewThreshold-1;
//								}
//								else 
//								{
//									if(DownButton.PressTimer%2==0)
//										NewThreshold = NewThreshold-1;
//								}
//							}
//							else
//							{
//								DownButton.Effect = PressShort;
//							}
//							if(NewThreshold>4000)
//									NewThreshold = 0;
//							else if(NewThreshold<=0)
//									NewThreshold = 4000;
								
//								if(SetButton.Effect == PressShort)	/*再一次按下SET键，退出SET自学习模式*/
//								{
									Threshold = NewThreshold;
									selfDisplayEndFlay = 1;
									SetButton.PressCounter = 0;					/*清楚按键次数*/
									SetButton.Status = Release;					/*释放按键*/
									SetButton.Effect = PressNOEffect;
									ee_WriteBytes((uint8_t*)&NewThreshold, 40, 4);
									ee_WriteBytes((uint8_t*)&DACOUT, 50, 4);
								//	break;															/*结束SET自学习模式*/
							//	}
							}
					}
				}
			}
}

/*ADCIN的数据调零*/

void ADCINcalibration(void) 
{
	CalibrateADCValue = 0;
	while(CalibrateADCValue>1050 || CalibrateADCValue<950)
	{
		if(sample_finish)
		{
				sample_finish = 0;
				//CalibrateADCValue = adc_dma_tab[0];
				GetADCValue(&CalibrateADCValue);					//定时ADC采样
			if(CalibrateADCValue > 1000)
			{
				//DACOUT = 1000- (CalibrateADCValue - 1000)/3;
				DACOUT = DACOUT-1;
			}
			else if(CalibrateADCValue < 1000)
			{
				//DACOUT = 1000+ (1000 - CalibrateADCValue)/3;
				DACOUT = DACOUT+1;
			}
			if(DACOUT>=4095)
				DACOUT = 4095;
			else if(DACOUT<=0)
				DACOUT = 0;
			DAC_SetChannel1Data(DAC_Align_12b_R,(uint16_t)DACOUT);
			DAC_SoftwareTriggerCmd(DAC_Channel_1,ENABLE);
		}
	}
}
