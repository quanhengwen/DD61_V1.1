///**
//  ********************************  STM32F0x1  *********************************
//  * @�ļ���     �� menu.c
//  * @����       �� HarryZeng
//  * @��汾     �� V1.5.0
//  * @�ļ��汾   �� V1.0.0
//  * @����       �� 2017��05��15��
//  * @ժҪ       �� ���ݴ���
//  ******************************************************************************/
///*----------------------------------------------------------------------------
//  ������־:
//  2017-05-15 V1.0.0:��ʼ�汾
//  ----------------------------------------------------------------------------*/
///* ������ͷ�ļ� --------------------------------------------------------------*/
/* Includes ------------------------------------------------------------------*/
#include "differentialDC.h"
#include "menu.h"
#include "key.h"
#include "display.h"
#include "bsp_eeprom_24xx.h"

int32_t CSV = 1000;
OUT1_DELAY_MODE_STRUCT OUT1_Mode={TOFF,10};
int8_t DispalyNo=0;
void MenuOne_CounterSET(void);
void MenuTwo_OUT1_DelaySET(void);

void MenuTwo_OUT1_TOFF(void);
void MenuTwo_OUT1_OFFD(void);
void MenuTwo_OUT1_ON_D(void);	
void MenuTwo_OUT1_SHOT(void);
void MenuTwo_DEL(void);

void menu(void)
{
	static uint8_t lastCounter;
	/*Mode������һ�ΰ���,��������ʱ��*/
	lastCounter = ModeButton.PressCounter;
	if((DownButton.Status==Press && ModeButton.Status ==Press )|| SetButton.Status == Press||UpButton.Status==Press)
	{
		
	}
	else
	{
		while(((ModeButton.Status==Press&&(ModeButton.PressTimer>=ModeButton.LongTime)) || (ModeButton.Effect==PressLong)))//&&DownButton.Status==Release)
		{
				MenuOne_CounterSET();

				if(ModeButton.PressCounter !=lastCounter && ModeButton.Effect==PressShort)
				{
					ModeButton.PressCounter = 3;
					if(OUT1_Mode.DelayMode == TOFF)
						DispalyNo = 0;
					else if(OUT1_Mode.DelayMode == OFFD)
						DispalyNo = 1;
					else if(OUT1_Mode.DelayMode == ON_D)
						DispalyNo = 2;
					else if(OUT1_Mode.DelayMode == SHOT)
						DispalyNo = 3;
				}
				while(ModeButton.Effect==PressShort && ModeButton.PressCounter==3)
				{
						MenuTwo_OUT1_DelaySET();
						/*�ٶ̰�MODE�����˳��˵�*/
						if(ModeButton.Effect==PressShort && ModeButton.PressCounter>=4) 
						{
							ModeButton.PressCounter = 0;
							ModeButton.Status = Release;
							ee_WriteBytes((uint8_t*)&OUT1_Mode.DelayMode, 10, 4);
							ee_WriteBytes((uint8_t*)&OUT1_Mode.DelayValue, 20, 4);
							ee_WriteBytes((uint8_t*)&CSV, 30, 4);
							ee_WriteBytes((uint8_t*)&DEL, 80, 2);
							//I2C_WriteByte(OUT1_Mode.DelayMode,10,EE_DEV_ADDR);
							//I2C_WriteByte(OUT1_Mode.DelayValue,20,EE_DEV_ADDR);
							//I2C_WriteByte(CSV,30,EE_DEV_ADDR);
							break;
						}
				}
		}
	}
}

/*COUNTER�趨�˵�*/
void MenuOne_CounterSET(void)
{
	static uint8_t lastCounter;
	
	SMG_DisplayMenuOne(CSV);
	
	/*Up Button*/
	if(UpButton.PressCounter !=lastCounter && UpButton.Effect==PressShort)
	{
		lastCounter = UpButton.PressCounter;
		UpButton.PressCounter = 0;
		CSV = CSV+1;
	}
	else 	if(UpButton.Status==Press&&(UpButton.Effect==PressLong))
	{				/*�����Ű���������ʱ�䳬������ʱ��*/
		UpButton.PressCounter = 0;
		if(UpButton.PressTimer<KEY_LEVEL_1)
		{
			if(UpButton.PressTimer%KEY_LEVEL_1_SET==0)
				CSV = CSV+1;
		}
		else if(UpButton.PressTimer>KEY_LEVEL_1&&UpButton.PressTimer<KEY_LEVEL_2)
		{
			if(UpButton.PressTimer%KEY_LEVEL_2_SET==0)
				CSV = CSV+1;
		}
		else 
		{
			if(UpButton.PressTimer%KEY_LEVEL_3_SET==0)
				CSV = CSV+1;
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
		CSV = CSV-1;
	}
	else 	if(DownButton.Status==Press&&(DownButton.Effect==PressLong))
	{				/*�����Ű���������ʱ�䳬������ʱ��*/
		DownButton.PressCounter = 0;
		if(DownButton.PressTimer<KEY_LEVEL_1)
		{
			if(DownButton.PressTimer%KEY_LEVEL_1_SET==0)
				CSV = CSV-1;
		}
		else if(DownButton.PressTimer>KEY_LEVEL_1&&DownButton.PressTimer<KEY_LEVEL_2)
		{
			if(DownButton.PressTimer%KEY_LEVEL_2_SET==0)
				CSV = CSV-1;
		}
		else 
		{
			if(DownButton.PressTimer%KEY_LEVEL_3_SET==0)
				CSV = CSV-1;
		}
	}	
	else
	{
		DownButton.Effect = PressShort;
	}
	
	if(CSV<=0)
			CSV =0;
	else if(CSV>=9999)
			CSV =9999;
}


/*OUT1 DELAY�趨�˵�*/

void MenuTwo_OUT1_DelaySET(void)
{
		static uint8_t lastCounter;
	
		/*TOFF Mode*/
		while(DispalyNo==0)
		{
			MenuTwo_OUT1_TOFF();
			if(UpButton.PressCounter !=lastCounter && UpButton.Effect==PressShort)
			{
				lastCounter = UpButton.PressCounter;
				UpButton.PressCounter = 0;
				DispalyNo++;
				if(DispalyNo>3)
					DispalyNo = 0;
			}
			if(DownButton.PressCounter !=lastCounter && DownButton.Effect==PressShort)
			{
				//lastCounter = DownButton.PressCounter;
				DownButton.PressCounter = 0;
				DispalyNo--;
				if(DispalyNo<0)
					DispalyNo = 3;
			}
			if(ModeButton.PressCounter>=3) break;
		}
		
		/*OFFD mode*/
		while(DispalyNo==1)
		{
				MenuTwo_OUT1_OFFD();
				if(UpButton.PressCounter !=lastCounter && UpButton.Effect==PressShort)
				{
					//lastCounter = UpButton.PressCounter;
					UpButton.PressCounter = 0;
					DispalyNo++;
					if(DispalyNo>3)
						DispalyNo = 0;
				}
				if(DownButton.PressCounter !=lastCounter && DownButton.Effect==PressShort)
				{
					//lastCounter = DownButton.PressCounter;
					DownButton.PressCounter = 0;
					DispalyNo--;
					if(DispalyNo<0)
						DispalyNo = 3;
				}
				if(ModeButton.PressCounter>3) break;
		}
		/*ON_D mode*/
		while(DispalyNo==2)
		{
				MenuTwo_OUT1_ON_D();
				if(UpButton.PressCounter !=lastCounter && UpButton.Effect==PressShort)
				{
					//lastCounter = UpButton.PressCounter;
					UpButton.PressCounter = 0;
					DispalyNo++;
					if(DispalyNo>3)
						DispalyNo = 0;
				}
				if(DownButton.PressCounter !=lastCounter && DownButton.Effect==PressShort)
				{
					//lastCounter = DownButton.PressCounter;
					DownButton.PressCounter = 0;
					DispalyNo--;
					if(DispalyNo<0)
						DispalyNo = 3;
				}
				if(ModeButton.PressCounter>3) break;
		}
		while(DispalyNo==3)
		{
				MenuTwo_OUT1_SHOT();
				if(UpButton.PressCounter !=lastCounter && UpButton.Effect==PressShort)
				{
					//lastCounter = UpButton.PressCounter;
					UpButton.PressCounter = 0;
					DispalyNo++;
					if(DispalyNo>3)
						DispalyNo = 0;
				}
				if(DownButton.PressCounter !=lastCounter && DownButton.Effect==PressShort)
				{
					//lastCounter = DownButton.PressCounter;
					DownButton.PressCounter = 0;
					DispalyNo--;
					if(DispalyNo<0)
						DispalyNo = 3;
				}
				if(ModeButton.PressCounter>3) break;
		}
//		
//		while(DispalyNo==4)
//		{
//				MenuTwo_DEL();
//				if(UpButton.PressCounter !=lastCounter && UpButton.Effect==PressShort)
//				{
//					//lastCounter = UpButton.PressCounter;
//					UpButton.PressCounter = 0;
//					DispalyNo++;
//					if(DispalyNo>4)
//						DispalyNo = 0;
//				}
//				if(DownButton.PressCounter !=lastCounter && DownButton.Effect==PressShort)
//				{
//					//lastCounter = DownButton.PressCounter;
//					DownButton.PressCounter = 0;
//					DispalyNo--;
//					if(DispalyNo<0)
//						DispalyNo = 4;
//				}
//				if(ModeButton.PressCounter>4) break;
//			}
}


/*******************************
*
*OUT1_TOFF
*
*******************************/
void MenuTwo_OUT1_TOFF(void)
{
		if(EventFlag&Blink500msFlag) 
		{
			EventFlag = EventFlag &(~Blink500msFlag);  //�����־λ
			SMG_DisplayMenuTwo_TOFF();
		}
		//if(ModeButton.Effect==PressShort && ModeButton.PressCounter==4)
			OUT1_Mode.DelayMode = TOFF;
}

/*******************************
*
*OUT1_OFFD
*
*******************************/
void MenuTwo_OUT1_OFFD(void)
{
		static uint8_t lastCounter;
	
		if(EventFlag&Blink500msFlag) 
		{
			EventFlag = EventFlag &(~Blink500msFlag);  //�����־λ
			/*��ʾOFFD,��˸*/
			SMG_DisplayMenuTwo_OFFD();
		}
		/*�̰�MODE�󣬽���OFFD�������Ӳ˵�*/
		while(ModeButton.Effect==PressShort && ModeButton.PressCounter==4)
		{
			OUT1_Mode.DelayMode = OFFD;
			/*��ʾOFFD value*/
			if(UpButton.Status==Release&&DownButton.Status==Release)
			{
				if(EventFlag&Blink500msFlag) 
				{
					EventFlag = EventFlag &(~Blink500msFlag);  //�����־λ
					/*��ʾOFFD,��˸*/
					SMG_DisplayMenuTwo_OFFD_SET(OUT1_Mode.DelayValue,1);
				}
			}
			else
			{
				EventFlag = EventFlag &(~Blink500msFlag);
				SMG_DisplayMenuTwo_OFFD_SET(OUT1_Mode.DelayValue,0);
			}
			/*Up Button*/
			if(UpButton.PressCounter !=lastCounter && UpButton.Effect==PressShort)
			{
				lastCounter = UpButton.PressCounter;
				UpButton.PressCounter = 0;
				OUT1_Mode.DelayValue = OUT1_Mode.DelayValue+1;
			}
			else 	if(UpButton.Status==Press&&(UpButton.Effect==PressLong))
			{				/*�����Ű���������ʱ�䳬������ʱ��*/
				UpButton.PressCounter = 0;
				if(UpButton.PressTimer<KEY_LEVEL_1)
				{
					if(UpButton.PressTimer%KEY_LEVEL_1_SET==0)
						OUT1_Mode.DelayValue = OUT1_Mode.DelayValue+1;
				}
				else if(UpButton.PressTimer>KEY_LEVEL_1&&UpButton.PressTimer<KEY_LEVEL_2)
				{
					if(UpButton.PressTimer%KEY_LEVEL_2_SET==0)
						OUT1_Mode.DelayValue = OUT1_Mode.DelayValue+1;
				}
				else 
				{
					if(UpButton.PressTimer%KEY_LEVEL_3_SET==0)
						OUT1_Mode.DelayValue = OUT1_Mode.DelayValue+1;
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
				OUT1_Mode.DelayValue = OUT1_Mode.DelayValue-1;
			}
			else if(DownButton.Status==Press&&(DownButton.Effect==PressLong))
			{				/*�����Ű���������ʱ�䳬������ʱ��*/
				DownButton.PressCounter = 0;
				if(DownButton.PressTimer<KEY_LEVEL_1)
				{
					if(DownButton.PressTimer%KEY_LEVEL_1_SET==0)
						OUT1_Mode.DelayValue = OUT1_Mode.DelayValue-1;
				}
				else if(DownButton.PressTimer>KEY_LEVEL_1&&DownButton.PressTimer<KEY_LEVEL_2)
				{
					if(DownButton.PressTimer%KEY_LEVEL_2_SET==0)
						OUT1_Mode.DelayValue = OUT1_Mode.DelayValue-1;
				}
				else 
				{
					if(DownButton.PressTimer%KEY_LEVEL_3_SET==0)
						OUT1_Mode.DelayValue = OUT1_Mode.DelayValue-1;
				}
			}	
			else
			{
				DownButton.Effect = PressShort;
			}
			if(OUT1_Mode.DelayValue>=9999)
				OUT1_Mode.DelayValue = 9999;
			else if(OUT1_Mode.DelayValue<=0)
					OUT1_Mode.DelayValue = 0;
		}
}

/*******************************
*
*OUT1_ON_D
*
*******************************/
void MenuTwo_OUT1_ON_D(void)
{
	static uint8_t lastCounter;
		if(EventFlag&Blink500msFlag) 
		{
			EventFlag = EventFlag &(~Blink500msFlag);  //�����־λ
			
			SMG_DisplayMenuTwo_ON_Delay();
		}
		/*�̰�MODE�󣬽���ON_D�������Ӳ˵�*/
		while(ModeButton.Effect==PressShort && ModeButton.PressCounter==4)
		{
			OUT1_Mode.DelayMode = ON_D;
			/*��ʾON_D value*/
			if(UpButton.Status==Release&&DownButton.Status==Release)
			{
				if(EventFlag&Blink500msFlag) 
				{
					EventFlag = EventFlag &(~Blink500msFlag);  //�����־λ
					/*��ʾON_D,��˸*/
					SMG_DisplayMenuTwo_ON_Delay_SET(OUT1_Mode.DelayValue,1);
				}
			}
			else
			{
				EventFlag = EventFlag &(~Blink100msFlag);
				SMG_DisplayMenuTwo_ON_Delay_SET(OUT1_Mode.DelayValue,0);
			}
			
			/*Up Button*/
			if(UpButton.PressCounter !=lastCounter && UpButton.Effect==PressShort)
			{
				lastCounter = UpButton.PressCounter;
				UpButton.PressCounter = 0;
				OUT1_Mode.DelayValue = OUT1_Mode.DelayValue+1;
			}
			else 	if(UpButton.Status==Press&&(UpButton.Effect==PressLong))
			{				/*�����Ű���������ʱ�䳬������ʱ��*/
				UpButton.PressCounter = 0;
				if(UpButton.PressTimer<KEY_LEVEL_1)
				{
					if(UpButton.PressTimer%KEY_LEVEL_1_SET==0)
						OUT1_Mode.DelayValue = OUT1_Mode.DelayValue+1;
				}
				else if(UpButton.PressTimer>KEY_LEVEL_1&&UpButton.PressTimer<KEY_LEVEL_2)
				{
					if(UpButton.PressTimer%KEY_LEVEL_2_SET==0)
						OUT1_Mode.DelayValue = OUT1_Mode.DelayValue+1;
				}
				else 
				{
					if(UpButton.PressTimer%KEY_LEVEL_3_SET==0)
						OUT1_Mode.DelayValue = OUT1_Mode.DelayValue+1;
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
				OUT1_Mode.DelayValue = OUT1_Mode.DelayValue-1;

			}
			else if(DownButton.Status==Press&&(DownButton.Effect==PressLong))
			{				/*�����Ű���������ʱ�䳬������ʱ��*/
				DownButton.PressCounter = 0;
				if(DownButton.PressTimer<KEY_LEVEL_1)
				{
					if(DownButton.PressTimer%KEY_LEVEL_1_SET==0)
						OUT1_Mode.DelayValue = OUT1_Mode.DelayValue-1;
				}
				else if(DownButton.PressTimer>KEY_LEVEL_1&&DownButton.PressTimer<KEY_LEVEL_2)
				{
					if(DownButton.PressTimer%KEY_LEVEL_2_SET==0)
						OUT1_Mode.DelayValue = OUT1_Mode.DelayValue-1;
				}
				else 
				{
					if(DownButton.PressTimer%KEY_LEVEL_3_SET==0)
						OUT1_Mode.DelayValue = OUT1_Mode.DelayValue-1;
				}
			}	
			else
			{
				DownButton.Effect = PressShort;
			}
			
			if(OUT1_Mode.DelayValue>=9999)
				OUT1_Mode.DelayValue = 9999;
			else if(OUT1_Mode.DelayValue<=0)
					OUT1_Mode.DelayValue = 0;
		}
}


/*******************************
*
*OUT1_SHOT
*
*******************************/
void MenuTwo_OUT1_SHOT(void)
{
		static uint8_t lastCounter;
		if(EventFlag&Blink500msFlag) 
		{
			EventFlag = EventFlag &(~Blink500msFlag);  //�����־λ
			
			SMG_DisplayMenuTwo_SHOT();
		}
		/*�̰�MODE�󣬽���SHOT�������Ӳ˵�*/
		while(ModeButton.Effect==PressShort && ModeButton.PressCounter==4)
		{
			OUT1_Mode.DelayMode = SHOT;
			/*��ʾSHOT value*/			
			if(UpButton.Status==Release&&DownButton.Status==Release)
			{
				if(EventFlag&Blink500msFlag) 
				{
					EventFlag = EventFlag &(~Blink500msFlag);  //�����־λ
					/*��ʾSHOT,��˸*/
					SMG_DisplayMenuTwo_SHOT_SET(OUT1_Mode.DelayValue,1);
				}
			}
			else
			{
				EventFlag = EventFlag &(~Blink500msFlag);
				SMG_DisplayMenuTwo_SHOT_SET(OUT1_Mode.DelayValue,0);
			}
			
			/*Up Button*/
			if(UpButton.PressCounter !=lastCounter && UpButton.Effect==PressShort)
			{
				lastCounter = UpButton.PressCounter;
				UpButton.PressCounter = 0;
				OUT1_Mode.DelayValue = OUT1_Mode.DelayValue+1;
			}
			else 	if(UpButton.Status==Press&&(UpButton.Effect==PressLong))
			{				/*�����Ű���������ʱ�䳬������ʱ��*/
				UpButton.PressCounter = 0;
				if(UpButton.PressTimer<KEY_LEVEL_1)
				{
					if(UpButton.PressTimer%KEY_LEVEL_1_SET==0)
						OUT1_Mode.DelayValue = OUT1_Mode.DelayValue+1;
				}
				else if(UpButton.PressTimer>KEY_LEVEL_1&&UpButton.PressTimer<KEY_LEVEL_2)
				{
					if(UpButton.PressTimer%KEY_LEVEL_2_SET==0)
						OUT1_Mode.DelayValue = OUT1_Mode.DelayValue+1;
				}
				else 
				{
					if(UpButton.PressTimer%KEY_LEVEL_3_SET==0)
						OUT1_Mode.DelayValue = OUT1_Mode.DelayValue+1;
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
				OUT1_Mode.DelayValue = OUT1_Mode.DelayValue-1;
			}
			else if(DownButton.Status==Press&&(DownButton.Effect==PressLong))
			{				/*�����Ű���������ʱ�䳬������ʱ��*/
				DownButton.PressCounter = 0;
				if(DownButton.PressTimer<KEY_LEVEL_1)
				{
					if(DownButton.PressTimer%KEY_LEVEL_1_SET==0)
						OUT1_Mode.DelayValue = OUT1_Mode.DelayValue-1;
				}
				else if(DownButton.PressTimer>KEY_LEVEL_1&&DownButton.PressTimer<KEY_LEVEL_2)
				{
					if(DownButton.PressTimer%KEY_LEVEL_2_SET==0)
						OUT1_Mode.DelayValue = OUT1_Mode.DelayValue-1;
				}
				else 
				{
					if(DownButton.PressTimer%KEY_LEVEL_3_SET==0)
						OUT1_Mode.DelayValue = OUT1_Mode.DelayValue-1;
				}
			}	
			else
			{
				DownButton.Effect = PressShort;
			}
			
			if(OUT1_Mode.DelayValue>=9999)
				OUT1_Mode.DelayValue = 9999;
			else if(OUT1_Mode.DelayValue<=0)
					OUT1_Mode.DelayValue = 0;
		}
}



/*******************************
*
*OUT1_DEL Ӧ��ֵ����
*
*******************************/
void MenuTwo_DEL(void)
{
		static uint8_t lastCounter;
		if(EventFlag&Blink500msFlag) 
		{
			EventFlag = EventFlag &(~Blink500msFlag);  //�����־λ
			
			SMG_DisplayMenuTwo_DEL();
		}
		/*�̰�MODE�󣬽���SHOT�������Ӳ˵�*/
		while(ModeButton.Effect==PressShort && ModeButton.PressCounter==4)
		{
			/*��ʾSHOT value*/			
			if(UpButton.Status==Release&&DownButton.Status==Release)
			{
				if(EventFlag&Blink500msFlag) 
				{
					EventFlag = EventFlag &(~Blink500msFlag);  //�����־λ
					/*��ʾSHOT,��˸*/
					SMG_DisplayMenuTwo_DEL_SET(DEL,1);
				}
			}
			else
			{
				EventFlag = EventFlag &(~Blink500msFlag);
				SMG_DisplayMenuTwo_DEL_SET(DEL,0);
			}
			
			/*Up Button*/
			if(UpButton.PressCounter !=lastCounter && UpButton.Effect==PressShort)
			{
				lastCounter = UpButton.PressCounter;
				UpButton.PressCounter = 0;
				DEL = DEL+1;
			}
			else 	if(UpButton.Status==Press&&(UpButton.Effect==PressLong))
			{				/*�����Ű���������ʱ�䳬������ʱ��*/
				UpButton.PressCounter = 0;
				if(UpButton.PressTimer<KEY_LEVEL_1)
				{
					if(UpButton.PressTimer%KEY_LEVEL_1_SET==0)
						DEL = DEL+1;
				}
				else if(UpButton.PressTimer>KEY_LEVEL_1&&UpButton.PressTimer<KEY_LEVEL_2)
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
			{				/*�����Ű���������ʱ�䳬������ʱ��*/
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
			
			if(DEL>=300)
				DEL = 300;
			else if(DEL<=30)
					DEL = 30;
		}
}



