
///**
//  ********************************  STM32F0x1  *********************************
//  * @文件名     ： differentialDC.c
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
#include "display.h"
#include "SelfStudy.h"
#include "menu.h"
#include "bsp_eeprom_24xx.h"

/*DSP库宏定义：ARM_MATH_CM0*/


extern uint8_t sample_finish;
uint32_t DealyBaseTime=23;
uint16_t DEL = 50;
int16_t Threshold=1000;
uint8_t RegisterA=0;
uint8_t RegisterB=1;
uint8_t OUT1=0;
uint8_t OUT2=0;
int16_t OUT2_TimerCounter=0;
uint16_t OUT2_Timer=0;
uint32_t ADCValue=0;
uint8_t ADCValueFlag=0;
uint32_t ADC_ROW_Value[4];
uint32_t ADC_Display_Value[258];
uint32_t ADC_Display=0;
uint16_t ADC_Display_Index=0;
int32_t DACOUT = 1000;
uint32_t CPV = 0;
Button_STATUS KEY=ULOC;
uint8_t 	ConfirmShortCircuit=0;
uint32_t   ShortCircuitCounter = 0;
uint32_t   ShortCircuitLastTime = 0;
uint8_t 	KeyMappingFlag=0;
uint8_t  	EventFlag=0x00; 
uint8_t 	ShortCircuit=0;
uint32_t 	ShortCircuitTimer=0;

void GetADCValue(uint32_t *meanValue);
void DisplayMODE(void);
void DisplayModeONE(void);
void DisplayModeTWO(void);
void DisplayModeTHIRD(void);
void DisplayModeFOUR(void);
void ShortCircuitProtection(void);
void SetOUT1Status(void);
void SetOUT2Status(void);
void ButtonMapping(void);
/*----------------------------------宏定义-------------------------------------*/
extern uint32_t OUTADCValue;

/*连续采样四次*/
void GetADCValue(uint32_t *meanValue)
{
//	uint32_t TempValue=0;
//	uint8_t i=0;
//	
//	if(sample_finish)
//	{
//		sample_finish = 0;
//		for(i=0;i<4;i++)
//			TempValue += ADC_ROW_Value[i];
//	}
	
	*meanValue = OUTADCValue;
}

/*计算OUT1*/
/********************
*
*判断出RegisterA状态
*
**********************/
uint8_t GetRegisterAState(uint32_t ADCValue)
{
	uint8_t A;
		
	if(ADCValue>=Threshold)
		A = 1;
	else if(ADCValue<=Threshold-50)
		A = 0;
	return A;
}



void differenttialDC(void)
{
	GetEEPROM();
	while(1)
	{
//			/*正常显示*/
//			if(EventFlag&ADVtimeFlag) //判断事件
//			{
				EventFlag = EventFlag &(~ADVtimeFlag);  //清楚标志位
				GetADCValue(&ADC_Display_Value[ADC_Display_Index]);		//定时ADC采样
				ADC_Display_Value[256]+=ADC_Display_Value[ADC_Display_Index++];//累加
				if(ADC_Display_Index>128)
				{
					ADC_Display_Index = 0;
					ADCValueFlag = 1;
				}			
				if(ADCValueFlag)
				{
					ADCValueFlag = 0;
					ADC_Display = ADC_Display_Value[256]/128;
					ADC_Display_Value[256] = 0;
				}
		//	}			
				//		ADC_Display++;
				/*短路保护*/
				ShortCircuitProtection();

				while(ConfirmShortCircuit)
				{
						GPIO_WriteBit(OUT1_GPIO_Port,OUT1_Pin,Bit_RESET);
						GPIO_WriteBit(OUT2_GPIO_Port,OUT2_Pin,Bit_RESET);/*马上拉低OUT*/ /*发现短路，将OUT引脚拉低*/
						if((ShortCircuitLastTime - ShortCircuitTimer)>=20000)
						{
								ConfirmShortCircuit = 0;
								ShortCircuitCounter = 0;
								ShortCircuit=0;
						}
				}
				
				/*输入OUT2*/
				//GPIOA->ODR ^= GPIO_Pin_9;
				SetOUT2Status();
				/*显示OUT1和OUT2的状态*/
				//SMG_DisplayOUT_STATUS(OUT1,OUT2);
				/*正常显示模式*/
				DisplayMODE();
			
				/*按键复用*/
				ButtonMapping();
				
				if(KEY==ULOC)/*判断按键是否上锁*/
				{
				/*SET自学习模式*/
					selfstudy();
				/*Mode菜单模式*/
					menu();
				}
	}
}


/*******************************
*
*显示模式切换
*
*******************************/
uint8_t DisplayModeNo=0;

void DisplayMODE(void)
{
	if(KEY==ULOC)
	{
		if(ModeButton.PressCounter==0)
		{
				DisplayModeNo = 0;
		}
		else if(ModeButton.Effect==PressShort && ModeButton.PressCounter==1&&DownButton.Status==Release)
		{
			DisplayModeNo = 1;
		}
		else if(ModeButton.Effect==PressShort && ModeButton.PressCounter==2&&DownButton.Status==Release)
		{
				DisplayModeNo = 2;
		}
		else if(ModeButton.Effect==PressShort && ModeButton.PressCounter==3&&DownButton.Status==Release)
		{
				DisplayModeNo = 3;
		}
		if(ModeButton.Effect == PressShort && ModeButton.PressCounter==4 &&DownButton.Status==Release)
		{
				ModeButton.PressCounter = 0;
				DisplayModeNo = 0;
		}
	}
	else
	{
		while(ModeButton.Status==Press||SetButton.Status==Press||DownButton.Status==Press||UpButton.Status==Press)
		{
			if(ModeButton.Effect==PressLong &&DownButton.Effect ==PressLong&&DownButton.Status ==Press)
			{
				if(KEY == ULOC)
						KEY = LOC;
				else
				{
					KEY = ULOC;
					KeyMappingFlag = 0;
				}
				ModeButton.PressCounter = 0;
				while((ReadButtonStatus(&ModeButton)) == Press&&(ReadButtonStatus(&DownButton) == Press))
				{
					ButtonMappingDisplay(1);
				}
				ee_WriteBytes((uint8_t*)&KEY, 60, 1);
				ModeButton.Effect=PressNOEffect;
				ModeButton.PressCounter = 0;
				DownButton.PressCounter = 0;
				DownButton.Effect=PressNOEffect;
			}
			else
				ButtonMappingDisplay(1);
		}
	}
	/*显示*/
	if(DisplayModeNo==0)
	{
		DisplayModeONE();
	}
	else if(DisplayModeNo==1)
	{
		DisplayModeTWO();
	}
	else if(DisplayModeNo==2)
	{
		DisplayModeTHIRD();
	}
	else if(DisplayModeNo==3)
	{
		DisplayModeFOUR();
	}
}

/*******************************
*
*显示模式1
*
*******************************/
void DisplayModeONE(void)
{
	static uint8_t lastCounter;
	static int16_t LastThreshold;
		/*数码管显示*/
		SMG_DisplayModeONE(Threshold,ADC_Display);
	
	if(ModeButton.Status==Release && KeyMappingFlag==0 && KEY==ULOC)
	{
		/*Up Button*/
		LastThreshold = Threshold;
		if(UpButton.PressCounter !=lastCounter && UpButton.Effect==PressShort)
		{
			lastCounter = UpButton.PressCounter;
			UpButton.PressCounter = 0;
			Threshold = Threshold+1;
		}
		else 	if(UpButton.Status==Press&&(UpButton.Effect==PressLong))
		{				/*还按着按键，并且时间超过长按时间*/
			UpButton.PressCounter = 0;
			if(UpButton.PressTimer<=KEY_LEVEL_1)
			{
				if(UpButton.PressTimer%KEY_LEVEL_1_SET==0)
					Threshold = Threshold+1;
			}
			else if(UpButton.PressTimer>KEY_LEVEL_1&&UpButton.PressTimer<=KEY_LEVEL_2)
			{
				if(UpButton.PressTimer%KEY_LEVEL_2_SET==0)
					Threshold = Threshold+1;
			}
			else 
			{
				if(UpButton.PressTimer%KEY_LEVEL_3_SET==0)
					Threshold = Threshold+1;
			}
		}	
		else
		{
			UpButton.Effect = PressShort;
		}
			/*Down Button*/
		if(DownButton.PressCounter !=lastCounter && DownButton.Effect==PressShort)
		{
			DownButton.PressCounter = 0;
			Threshold = Threshold-1;
		}
		else if(DownButton.Status==Press&&(DownButton.Effect==PressLong))
		{
			DownButton.PressCounter = 0;
			if(DownButton.PressTimer<KEY_LEVEL_1)
			{
				if(DownButton.PressTimer%KEY_LEVEL_1_SET==0)
					Threshold = Threshold-1;
			}
			else if(DownButton.PressTimer>KEY_LEVEL_1&&DownButton.PressTimer<KEY_LEVEL_2)
			{
				if(DownButton.PressTimer%KEY_LEVEL_2_SET==0)
					Threshold = Threshold-1;
			}
			else 
			{
				if(DownButton.PressTimer%KEY_LEVEL_3_SET==0)
					Threshold = Threshold-1;
			}
		}
		else
		{
			DownButton.Effect = PressShort;
		}
		if(LastThreshold!=Threshold && DownButton.Status==Release && UpButton.Status==Release)
		{
			ee_WriteBytes((uint8_t*)&Threshold, 40, 4);
		}
	}
		if(Threshold>=4000)
				Threshold = 4000;
		else if(Threshold<=0)
				Threshold = 0;
}



/*******************************
*
*显示模式2
*
*******************************/
void DisplayModeTWO(void)
{
	static uint8_t lastCounter;
		/*数码管显示*/
		SMG_DisplayModeTWO(RegisterB);
	
		/*Up Button*/
		if(UpButton.PressCounter !=lastCounter && UpButton.Effect==PressShort)
		{
			lastCounter = UpButton.PressCounter;
			UpButton.PressCounter = 0;
			if(RegisterB==0)
				RegisterB = 1;
			else 
				RegisterB=0;
			ee_WriteBytes((uint8_t*)&RegisterB, 70, 1);
		}

		/*Down Button*/
		if(DownButton.PressCounter !=lastCounter && DownButton.Effect==PressShort)
		{
			DownButton.PressCounter = 0;
			if(RegisterB==0)
				RegisterB = 1;
			else 
				RegisterB=0;
			ee_WriteBytes((uint8_t*)&RegisterB, 70, 1);
		}
}


/*******************************
*
*显示模式3
*
*******************************/
void DisplayModeTHIRD(void)
{
		/*数码管显示*/
		SMG_DisplayModeTHIRD(CPV);
}

/*******************************
*
*显示模式4
*
*******************************/
void DisplayModeFOUR(void)
{
	static uint8_t lastCounter;
	static int16_t LastDEL;
		/*数码管显示*/
		SMG_DisplayMenuTwo_DEL_SET(DEL,0);
	
	if(ModeButton.Status==Release && KeyMappingFlag==0 && KEY==ULOC)
	{
		/*Up Button*/
		LastDEL = DEL;
		if(UpButton.PressCounter !=lastCounter && UpButton.Effect==PressShort)
		{
			lastCounter = UpButton.PressCounter;
			UpButton.PressCounter = 0;
			DEL = DEL+1;
		}
		else 	if(UpButton.Status==Press&&(UpButton.Effect==PressLong))
		{				/*还按着按键，并且时间超过长按时间*/
			UpButton.PressCounter = 0;
			if(UpButton.PressTimer<=KEY_LEVEL_1)
			{
				if(UpButton.PressTimer%KEY_LEVEL_1_SET==0)
					DEL = DEL+1;
			}
			else if(UpButton.PressTimer>KEY_LEVEL_1&&UpButton.PressTimer<=KEY_LEVEL_2)
			{
				if(UpButton.PressTimer%KEY_LEVEL_2_SET==0)
					DEL = DEL+1;
			}
			else 
			{
				if(UpButton.PressTimer%KEY_LEVEL_3_SET==0)
					DEL = DEL+1;
			}
		}	
		else
		{
			UpButton.Effect = PressShort;
		}
			/*Down Button*/
		if(DownButton.PressCounter !=lastCounter && DownButton.Effect==PressShort)
		{
			DownButton.PressCounter = 0;
			DEL = DEL-1;
		}
		else if(DownButton.Status==Press&&(DownButton.Effect==PressLong))
		{
			DownButton.PressCounter = 0;
			if(DownButton.PressTimer<KEY_LEVEL_1)
			{
				if(DownButton.PressTimer%KEY_LEVEL_1_SET==0)
					DEL = DEL-1;
			}
			else if(DownButton.PressTimer>KEY_LEVEL_1&&DownButton.PressTimer<KEY_LEVEL_2)
			{
				if(DownButton.PressTimer%KEY_LEVEL_2_SET==0)
					DEL = DEL-1;
			}
			else 
			{
				if(DownButton.PressTimer%KEY_LEVEL_3_SET==0)
					DEL = DEL-1;
			}
		}
		else
		{
			DownButton.Effect = PressShort;
		}
		if(LastDEL!=Threshold && DownButton.Status==Release && UpButton.Status==Release)
		{
			ee_WriteBytes((uint8_t*)&DEL, 80, 2);
		}
	}
		if(DEL>=300)
				DEL = 300;
		else if(DEL<=30)
				DEL = 30;
}


/*******************************
*
*判断OUT1的输出状态
*
*******************************/
uint8_t SHOTflag=0;
void SetOUT1Status(void)
{
	if(ShortCircuit!=1)/*不是短路保护的情况下才判断OUT1的输出*/
	{
		if(OUT1_Mode.DelayMode==TOFF)
		{
			//GPIOA->ODR ^= GPIO_Pin_9;
			if(OUT1)
			{
				GPIO_WriteBit(OUT1_GPIO_Port, OUT1_Pin, Bit_SET);
				OUT1_Mode.DelayCounter = 0;
			}
			else
			{
				GPIO_WriteBit(OUT1_GPIO_Port, OUT1_Pin, Bit_RESET);
				OUT1_Mode.DelayCounter=0;
			}
		}
		/*OFFD*/
		else if(OUT1_Mode.DelayMode==OFFD)
		{
			if(OUT1)
			{
				GPIO_WriteBit(OUT1_GPIO_Port, OUT1_Pin, Bit_SET);
				OUT1_Mode.DelayCounter = 0;
			}
			else
			{
				if(OUT1_Mode.DelayCounter>(OUT1_Mode.DelayValue*DealyBaseTime))
				{
					GPIO_WriteBit(OUT1_GPIO_Port, OUT1_Pin, Bit_RESET);
				}
			}
		}
		/*ON_D*/
		else if(OUT1_Mode.DelayMode==ON_D)
		{
			if(OUT1)
			{
				if(OUT1_Mode.DelayCounter>(OUT1_Mode.DelayValue*DealyBaseTime))
				{
					GPIO_WriteBit(OUT1_GPIO_Port, OUT1_Pin, Bit_SET);
				}
			}
			else
			{
				GPIO_WriteBit(OUT1_GPIO_Port, OUT1_Pin, Bit_RESET);
				OUT1_Mode.DelayCounter = 0;
			}
		}
		/*SHOT*/
		else if(OUT1_Mode.DelayMode==SHOT)
		{
			if(OUT1 || SHOTflag == 1)
			{
				if(OUT1_Mode.DelayCounter<(OUT1_Mode.DelayValue*DealyBaseTime))
				{
					GPIO_WriteBit(OUT1_GPIO_Port, OUT1_Pin, Bit_SET);
					SHOTflag = 1;
				}
				else
				{
					GPIO_WriteBit(OUT1_GPIO_Port, OUT1_Pin, Bit_RESET);
					SHOTflag = 0;
				}
			}
			else
			{
				OUT1_Mode.DelayCounter = 0;
				GPIO_WriteBit(OUT1_GPIO_Port, OUT1_Pin, Bit_RESET);
			}
		}
	}
}
/*******************************
*
*判断OUT2的输出状态
*
*******************************/
void SetOUT2Status(void)
{
	if(ShortCircuit!=1)/*不是短路保护的情况下才判断OUT2的输出*/
	{
		if(OUT2)
		{
			GPIO_WriteBit(OUT2_GPIO_Port,OUT2_Pin,Bit_SET);/*拉高*/
			//GPIO_WriteBit(OUT1_GPIO_Port,OUT1_Pin,Bit_SET);
		}
	 if(OUT2_TimerCounter >=250)
		{
			OUT2 = 0;
			OUT2_TimerCounter = 0;  /*获取当前时间*/
			//GPIO_WriteBit(OUT1_GPIO_Port,OUT1_Pin,Bit_RESET);
			GPIO_WriteBit(OUT2_GPIO_Port,OUT2_Pin,Bit_RESET);/*80ms后拉低*/
		}
	}
}

/*******************************
*
*短路保护
*
*******************************/
void ShortCircuitProtection(void)
{
	uint8_t SCState;
	
	/*读取SC引脚的状态*/
	if(ShortCircuit!=1)
	{
		SCState = GPIO_ReadInputDataBit(SC_GPIO_Port ,SC_Pin);
		if((BitAction)SCState == Bit_RESET)
		{
			/*拉低FB_SC*/
			ShortCircuit= 1;
		}
		else
		{
			ShortCircuit = 0;
			ConfirmShortCircuit = 0;
		}
	}
	if(ShortCircuit && ShortCircuitCounter>=100)
	{
		ConfirmShortCircuit=1;
		GPIO_WriteBit(OUT1_GPIO_Port,OUT1_Pin,Bit_RESET);
		GPIO_WriteBit(OUT2_GPIO_Port,OUT2_Pin,Bit_RESET);/*马上拉低OUT*/
		ShortCircuitTimer = ShortCircuitLastTime;
	}	
}

/*******************************
*
*按键复用
*
*******************************/
void ButtonMapping(void)
{
	/*按键上锁*/
	if(ModeButton.Effect==PressLong &&DownButton.Effect ==PressLong&&DownButton.Status ==Press)
	{
		if(KEY == ULOC)
				KEY = LOC;
		else
			KEY = ULOC;
		ModeButton.PressCounter = 0;
		while((ReadButtonStatus(&ModeButton)) == Press&&(ReadButtonStatus(&DownButton) == Press))
		{
			ButtonMappingDisplay(1);
			KeyMappingFlag = 1;
		}
		ee_WriteBytes((uint8_t*)&KEY, 60, 1);
		ModeButton.Effect=PressNOEffect;
		ModeButton.PressCounter = 0;
		DownButton.PressCounter = 0;
		DownButton.Effect=PressNOEffect;
		//ee_WriteBytes((uint8_t*)&KEY, 60, 4);
		//I2C_WriteByte(KEY,60,EE_DEV_ADDR);
	}
	/*软件初始化*/
	if(ModeButton.Effect==PressLong &&SetButton.Effect ==PressLong&&SetButton.Status ==Press)
	{
		CSV = 1000;
		Threshold = 1000;
		KEY = ULOC;
		OUT1_Mode.DelayMode = TOFF;
		OUT1_Mode.DelayValue = 10;
		DACOUT = 1000;
		DEL = 50;
		RegisterB = 1;
		while((ReadButtonStatus(&ModeButton)) == Press&&(ReadButtonStatus(&SetButton) == Press))
		{
			ButtonMappingDisplay(2);
			KeyMappingFlag = 0;
		}
		ee_WriteBytes((uint8_t*)&OUT1_Mode.DelayMode, 10, 4);
		ee_WriteBytes((uint8_t*)&OUT1_Mode.DelayValue, 20, 4);
		ee_WriteBytes((uint8_t*)&CSV, 30, 4);
		ee_WriteBytes((uint8_t*)&Threshold, 40, 4);
		ee_WriteBytes((uint8_t*)&DACOUT, 50, 4);
		ee_WriteBytes((uint8_t*)&KEY, 60, 1);	
		ee_WriteBytes((uint8_t*)&RegisterB, 70, 1);	
		ee_WriteBytes((uint8_t*)&DEL, 80, 2);	
		
		ModeButton.Effect=PressNOEffect;
		ModeButton.PressTimer = 0;
		ModeButton.PressCounter = 0;
		SetButton.Effect=PressNOEffect;
		SetButton.PressCounter = 0;
		
		DAC_SetChannel1Data(DAC_Align_12b_R,(uint16_t)DACOUT);
		DAC_SoftwareTriggerCmd(DAC_Channel_1,ENABLE);

	}
	/*计算器清零*/
	if(ModeButton.Effect==PressLong &&UpButton.Effect ==PressLong&&UpButton.Status ==Press)
	{
		CPV = 0;
		while((ReadButtonStatus(&ModeButton)) == Press&&(ReadButtonStatus(&UpButton) == Press))
		{
			ButtonMappingDisplay(3);
			KeyMappingFlag=0;
		}
		ModeButton.Effect=PressNOEffect;
		ModeButton.PressTimer = 0;
		ModeButton.PressCounter = 0;
		UpButton.PressCounter = 0;
		UpButton.Effect=PressNOEffect;
	}	
}

/*****************************
*
*初始化所有参数
*
****************************/
void ResetParameter(void)
{
		CSV = 1000;
		Threshold = 1000;
		KEY = ULOC;
		OUT1_Mode.DelayMode = TOFF;
		OUT1_Mode.DelayValue = 10;
		DACOUT = 1000;
		DEL = 50;
		RegisterB = 1;

		ee_WriteBytes((uint8_t*)&OUT1_Mode.DelayMode, 10, 4);
		ee_WriteBytes((uint8_t*)&OUT1_Mode.DelayValue, 20, 4);
		ee_WriteBytes((uint8_t*)&CSV, 30, 4);
		ee_WriteBytes((uint8_t*)&Threshold, 40, 4);
		ee_WriteBytes((uint8_t*)&DACOUT, 50, 4);
		ee_WriteBytes((uint8_t*)&KEY, 60, 1);	
		ee_WriteBytes((uint8_t*)&RegisterB, 70, 1);	
		ee_WriteBytes((uint8_t*)&DEL, 80, 2);	
		
		
		ModeButton.Effect=PressNOEffect;
		ModeButton.PressTimer = 0;
		ModeButton.PressCounter = 0;
		SetButton.Effect=PressNOEffect;
		SetButton.PressCounter = 0;
		
		DAC_SetChannel1Data(DAC_Align_12b_R,(uint16_t)DACOUT);
		DAC_SoftwareTriggerCmd(DAC_Channel_1,ENABLE);	

}



 void Test_Delay(uint32_t ms)
{
	uint32_t i;

	/*　
		CPU主频168MHz时，在内部Flash运行, MDK工程不优化。用台式示波器观测波形。
		循环次数为5时，SCL频率 = 1.78MHz (读耗时: 92ms, 读写正常，但是用示波器探头碰上就读写失败。时序接近临界)
		循环次数为10时，SCL频率 = 1.1MHz (读耗时: 138ms, 读速度: 118724B/s)
		循环次数为30时，SCL频率 = 440KHz， SCL高电平时间1.0us，SCL低电平时间1.2us

		上拉电阻选择2.2K欧时，SCL上升沿时间约0.5us，如果选4.7K欧，则上升沿约1us

		实际应用选择400KHz左右的速率即可
	*/
	for (i = 0; i < ms*10; i++);
}
/*******************************
*
*获取EEPROM参数
*
*******************************/

	uint32_t _test_send_buf[30];
	uint32_t _test_recv_buf[30];
	short _test_size = 30;

	char statusW=2;
	char statusR=2;

void GetEEPROM(void)
{
	uint8_t tempKEY=0;
	
	while(ee_CheckOk()==0)//检测不到24c02
	{
	}
			Test_Delay(6000);
	
		while(ee_ReadBytes((uint8_t*)&OUT1_Mode.DelayMode,10,4)==0);
		while(ee_ReadBytes((uint8_t*)&OUT1_Mode.DelayValue,20,4)==0);
		while(ee_ReadBytes((uint8_t*)&CSV,30,4)==0);
		while(ee_ReadBytes((uint8_t*)&Threshold,40,4)==0);
		while(ee_ReadBytes((uint8_t*)&DACOUT,50,4)==0);
		while(ee_ReadBytes((uint8_t*)&KEY,60,1)==0);
		while(ee_ReadBytes((uint8_t*)&RegisterB,70,1)==0);
		while(ee_ReadBytes((uint8_t*)&DEL,80,2)==0);
		while(ee_ReadBytes((uint8_t*)&ADCMAX,100,4)==0);
	
		DAC_SetChannel1Data(DAC_Align_12b_R,(uint16_t)DACOUT);
		DAC_SoftwareTriggerCmd(DAC_Channel_1,ENABLE);
		
}


