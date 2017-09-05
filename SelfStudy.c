
///**
//  ********************************  STM32F0x1  *********************************
//  * @�ļ���     �� SelftStudy.c
//  * @����       �� HarryZeng
//  * @��汾     �� V1.5.0
//  * @�ļ��汾   �� V1.0.0
//  * @����       �� 2017��05��11��
//  * @ժҪ       �� ���ݴ���
//  ******************************************************************************/
///*----------------------------------------------------------------------------
//  ������־:
//  2017-05-11 V1.0.0:��ʼ�汾
//  ----------------------------------------------------------------------------*/
///* ������ͷ�ļ� --------------------------------------------------------------*/
/* Includes ------------------------------------------------------------------*/
#include "differentialDC.h"
#include "key.h"
#include "SelfStudy.h"
//#include "arm_math.h"
#include "display.h"
#include "bsp_eeprom_24xx.h"

/*��һ��SET�������´�����*/
void ADCINcalibration(void);

/*�ڶ���SET�������´�����*/
void GetMAXADCValue(void);

uint8_t selfDisplayEndFlay=0;
uint32_t  SET1ADC_Value=0;
extern uint8_t sample_finish;
uint32_t CalibrateADCValue=0;
extern int16_t adc_dma_tab[6];
void selfstudy(void)
{
	uint8_t OUT1_STATUS;
	/*����OUT1��״̬*/
	OUT1_STATUS = GPIO_ReadInputDataBit(OUT1_GPIO_Port,OUT1_Pin);/*��ȡ��ǰ��OUT1״̬*/
	GPIO_WriteBit(OUT1_GPIO_Port,OUT1_Pin,(BitAction)OUT1_STATUS);/*������OUT1״̬*/
	
	if(SetButton.Status == Press && ModeButton.Status==Press)
	{
		
	}
	else
	{
			/*��һ�ν���SETģʽ*/
			while(SetButton.Status == Press )     //ֻҪ����ʾģʽ�µ�һ�ΰ���SET����
			{	/*���Ű���3����*/
				ADCINcalibration();
				Test_Delay(1000);
		//		/*����Ҫ��ʾSET1*/
		//		if(EventFlag&Blink500msFlag) 
		//		{
		//			EventFlag = EventFlag &(~Blink500msFlag);  //�����־λ
		//			SMG_DisplaySET_Step_1_Mode(0,ADCValue);
		//		}
				//EventFlag = EventFlag &(~ADVtimeFlag);
				//GetADCValue(&SET1ADC_Value);
				
				while(SetButton.Effect == PressShort) /*���°����Ѿ�����ʱ�䣬����û���ͷţ���˸����*/
				{		
					/*3�뵽��,��˸����*/
					//if(EventFlag&Blink500msFlag) 
					//{
					//	EventFlag = EventFlag &(~Blink500msFlag);  //�����־λ
						//if(EventFlag&ADVtimeFlag) //���µ�3���ڣ�һֱ��ʱ�ɼ�ADCֵ
						//{
							//EventFlag = EventFlag &(~ADVtimeFlag);  //�����־λ
							//GetADCValue(&SET1ADC_Value);		//��ʱADC����
							SMG_DisplaySET_Step_1_Mode(2,CalibrateADCValue);
					//	}
						
					//}
					if(SetButton.Effect == PressLong && SetButton.Status==Press ) /*�����ﵽ3��󣬵�һ�ν�����ѧϰ���ȴ��ڶ��ΰ���SET 3��*/
					{	/*3�뵽�ˣ����ͷ��˰���*/
						/*һֱ�ȴ��ڶ���SET�İ���*
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


/*��һϵ�е�ADCvalue��Ѱ������ADV MAX*/
uint32_t 			ADCMAX=0;
uint32_t 		NewThreshold=0;
uint32_t 		TempMaxADCValue=0;
void GetMAXADCValue(void)
{
		uint32_t 		TempADCValue=0;
//		static uint8_t lastCounter;
	  TempMaxADCValue =0;
		while(selfDisplayEndFlay==0)     //�ڶ��ΰ���SET����
		{
			SMG_DisplaySET_Step_1_Mode(0,ADCValue);
			while(SetButton.Status == Press )
			{
				//if(EventFlag&ADVtimeFlag) //���µ�3���ڣ�һֱ��ʱ�ɼ�ADCֵ
				//{
				//	EventFlag = EventFlag &(~ADVtimeFlag);  //�����־λ
					GetADCValue(&TempADCValue);					//��ʱADC����
					
					if(TempADCValue>=TempMaxADCValue)
							TempMaxADCValue = TempADCValue;
				//}
				/*����Ҫ��ʾSET2*/
				if(EventFlag&Blink500msFlag) 
				{
					EventFlag = EventFlag &(~Blink500msFlag);  //�����־λ
					SMG_DisplaySET_Step_2_Mode(0,ADCValue,0);
				}
				
				while(SetButton.Effect == PressLong) /*���°����Ѿ�����ʱ�䣬����û���ͷţ���˸����*/
				{	/*3�뵽��,��˸����*/
					if(EventFlag&Blink500msFlag) 
					{
						EventFlag = EventFlag &(~Blink500msFlag);  //�����־λ
						SMG_DisplaySET_Step_2_Mode(1,0,0);     
					}				
					while(SetButton.Effect == PressLong && SetButton.Status == Release) /*�����ﵽ3��󣬽����ڶ���SET����*/
					{		/*3�뵽�ˣ����ͷ��˰���*/
					//	arm_max_f32(TempADCValue_f[1],i,&ADCMAX,&MAX_Index);  /*�������ADCINֵ*/
						
						ADCMAX = TempMaxADCValue;
						NewThreshold = (CalibrateADCValue+ADCMAX)/2;				/*��ñ�����ѧϰ��OUT1��ֵ*/
						
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
//							{				/*�����Ű���������ʱ�䳬������ʱ��*/
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
								
//								if(SetButton.Effect == PressShort)	/*��һ�ΰ���SET�����˳�SET��ѧϰģʽ*/
//								{
									Threshold = NewThreshold;
									selfDisplayEndFlay = 1;
									SetButton.PressCounter = 0;					/*�����������*/
									SetButton.Status = Release;					/*�ͷŰ���*/
									SetButton.Effect = PressNOEffect;
									ee_WriteBytes((uint8_t*)&NewThreshold, 40, 4);
									ee_WriteBytes((uint8_t*)&DACOUT, 50, 4);
								//	break;															/*����SET��ѧϰģʽ*/
							//	}
							}
					}
				}
			}
}

/*ADCIN�����ݵ���*/

void ADCINcalibration(void) 
{
	CalibrateADCValue = 0;
	while(CalibrateADCValue>1050 || CalibrateADCValue<950)
	{
		if(sample_finish)
		{
				sample_finish = 0;
				//CalibrateADCValue = adc_dma_tab[0];
				GetADCValue(&CalibrateADCValue);					//��ʱADC����
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
