#include  "stm32f0xx.h"
#include "differentialDC.h"
#include "key.h"
#include "stdio.h"
#include "display.h"
#include "bsp_eeprom_24xx.h"
#include "menu.h"


#define DMA_BUFFER_SIZE     1  

#define _Gpio_12_set  GPIO_WriteBit(GPIOA, GPIO_Pin_12, (BitAction)!GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_12))
#define _Gpio_7_set  GPIO_WriteBit(GPIOA, GPIO_Pin_7, (BitAction)!GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_7))

uint8_t sample_finish = 0;  
int16_t adc_dma_tab[6] = { 0 };  
uint8_t sample_index = 0;  
uint8_t TIM1step=0;

//?????  
int16_t sample_1[128] = { 0 };  
int16_t sample_2[128] = { 0 };  
int16_t sample_3[128] = { 0 };  
int16_t sample_4[128] = { 0 };  
int16_t sample_5[128] = { 0 };  
int16_t sample_6[128] = { 0 };  
  

RCC_ClocksTypeDef   SysClock;

void adc_config(void) ;
void user_adc_init(void)  ;
void adc_gpio_init(void);
void adc_dma_init(void) ;
void adc_timer_init(void)  ;
void USART_Config(void);
void GPIOA_Config(void);
uint8_t FlashCheck(void);
void GPIO_DEINIT_ALL(void);
void ProgramCheck(void);
void IWDG_Config(void);
/*****************************************/
///////////////////////////////////////////////////////////////////////////////////
/**
  * @brief  //加入以下代码,支持printf函数,而不需要选择use MicroLIB
  */
int fputc(int ch, FILE *f)
{
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
		;
	USART_SendData(USART1, (unsigned char) ch);
  return (ch);
}

void user_adc_init(void)  
{  
    adc_gpio_init();  
    adc_config();               // ??????????,????????????????????  
    adc_dma_init();             //  
    adc_timer_init();           //  
		GPIOA_Config();
    //USART_Config();
}

void USART_Config(void)
{
//        USART_InitTypeDef USART_InitStructure;
//        /*???USART1??*/
//        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
//        USART_InitStructure.USART_BaudRate = 115200;//???????
//        USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//?????
//        USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;//??????
//        USART_InitStructure.USART_Parity = USART_Parity_No;//?????
//        USART_InitStructure.USART_StopBits = USART_StopBits_1;//?????
//        USART_InitStructure.USART_WordLength = USART_WordLength_8b;//?????
//        USART_Init(USART1, &USART_InitStructure);
//        USART_Cmd(USART1, ENABLE);//???? 1
}
/*GPIOA????? */
void GPIOA_Config(void)
{
//        GPIO_InitTypeDef GPIO_InitStructure;
//        /*???GPIOA??*/
//        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
//        GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_1);
//        GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_1);
//        /* ??PA9 ,PA10*/
//        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
//        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //??????
//        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
//        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_Level_3;
//        GPIO_Init(GPIOA, &GPIO_InitStructure);
}
/*?? 1 ????*/
void UART_send_byte(uint8_t byte)
{
        while(!((USART1->ISR)&(1<<7)));//?????
        USART1->TDR=byte; //??????
}
/*?????*/
void UART_Send(uint8_t *Buffer, uint32_t Length)
{
        while(Length != 0)
        {
                while(!((USART1->ISR)&(1<<7)));//?????
                USART1->TDR= *Buffer;
                Buffer++;
                Length--;
        }
}

uint8_t UART_Recive(void)
{
        while(!(USART1->ISR & (1<<5)));
        return(USART1->RDR);
}
  
void adc_config()  
{  
    ADC_InitTypeDef adc_init_structure;  

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);            //??ADC?? 
    
		ADC_DeInit(ADC1);                                               //??ADC  
    ADC_StructInit(&adc_init_structure);                            //???ADC???  
  
    adc_init_structure.ADC_ContinuousConvMode = DISABLE;            //????????  
    adc_init_structure.ADC_DataAlign = ADC_DataAlign_Right;         //???????  
    adc_init_structure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T2_TRGO; //???????TIM2  
    adc_init_structure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;//?????  
    adc_init_structure.ADC_Resolution = ADC_Resolution_12b;         //12????  
    adc_init_structure.ADC_ScanDirection = ADC_ScanDirection_Upward;//????0-18??  
		
    ADC_Init(ADC1, &adc_init_structure);  
		
    ADC_OverrunModeCmd(ADC1, ENABLE);                               //????????  
    ADC_ChannelConfig(ADC1, ADC_Channel_2, ADC_SampleTime_28_5Cycles);               //??????,????125nS  
    ADC_GetCalibrationFactor(ADC1);                                 //?????ADC  
    ADC_Cmd(ADC1, ENABLE);                                          //??ADC1  
    while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADEN) == RESET);         //??ADC1????  
  
    ADC_DMACmd(ADC1, ENABLE);                                       //??ADC_DMA  
    ADC_DMARequestModeConfig(ADC1, ADC_DMAMode_Circular);           //??DMA?????????  
    ADC_StartOfConversion(ADC1);                                    //??????(??)  
}  
  
void adc_gpio_init()  
{  
    GPIO_InitTypeDef gpio_init_structure;  
    //??GPIO??  
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA , ENABLE);  
  
    GPIO_StructInit(&gpio_init_structure);  
    //GPIOA                                                         //PA-0~3??ADC  
    gpio_init_structure.GPIO_Pin = (ADCIN_Pin);  
    gpio_init_structure.GPIO_Mode = GPIO_Mode_AN;                   //????(??)??  
    gpio_init_structure.GPIO_OType = GPIO_OType_PP;                 //????  
    gpio_init_structure.GPIO_Speed = GPIO_Speed_2MHz;              //Fast speed  
    gpio_init_structure.GPIO_PuPd= GPIO_PuPd_UP;                    //??  
    GPIO_Init(ADCIN_GPIO_Port, &gpio_init_structure);  
}  
  
void adc_dma_init()  
{  
    DMA_InitTypeDef dma_init_structure;  
    NVIC_InitTypeDef nvic_init_structure;  
  
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);              //??DMA??  
  
    nvic_init_structure.NVIC_IRQChannel = DMA1_Channel1_IRQn;       //??DMA1????  
    nvic_init_structure.NVIC_IRQChannelCmd = ENABLE;                //????  
    nvic_init_structure.NVIC_IRQChannelPriority = 0;                //?????0  
    NVIC_Init(&nvic_init_structure);  
  
    DMA_DeInit(DMA1_Channel1);                                      //复位DMA1_channel1  
    DMA_StructInit(&dma_init_structure);                            //初始化DMA结构体 
  
    dma_init_structure.DMA_BufferSize = DMA_BUFFER_SIZE;            //DMA缓存数组大小  
    dma_init_structure.DMA_DIR = DMA_DIR_PeripheralSRC;             //DMA方向:外设作为数据源 
    dma_init_structure.DMA_M2M = DISABLE;                           //内存到内存禁用
    dma_init_structure.DMA_MemoryBaseAddr = (uint32_t)&adc_dma_tab[0];//缓存数据数组起始地址
    dma_init_structure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;//数据大小设置为Halfword  
    dma_init_structure.DMA_MemoryInc = DMA_MemoryInc_Enable;        //内存地址递增 
    dma_init_structure.DMA_Mode = DMA_Mode_Circular;                //DMA循环模式,完成后重新开始覆盖 
    dma_init_structure.DMA_PeripheralBaseAddr = (uint32_t) &(ADC1->DR);//取值的外设地址 
    dma_init_structure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//外设取值大小设置为Halfword  
    dma_init_structure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设地址递增禁用
    dma_init_structure.DMA_Priority = DMA_Priority_High;             //DMA优先级设置为高  
    DMA_Init(DMA1_Channel1, &dma_init_structure);  
  
    DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);                  //使能DMA中断
    DMA_ClearITPendingBit(DMA_IT_TC);                                //清除一次DMA中断标志  
    DMA_Cmd(DMA1_Channel1, ENABLE);                                  //使能DMA1  
}  
  
void adc_timer_init()  
{  
    TIM_TimeBaseInitTypeDef timer_init_structure;  
//		TIM_OCInitTypeDef timer_OCinit_structure; 
    NVIC_InitTypeDef nvic_init_structure;  
//  	GPIO_InitTypeDef gpio_init_structure;  
	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);            //??TIM2??  //??GPIO??  
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 , ENABLE);
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);  
  
//    GPIO_StructInit(&gpio_init_structure);  
//    //GPIOA                                                         //PA-0~3??ADC  
//    gpio_init_structure.GPIO_Pin = GPIO_Pin_9;  
//    gpio_init_structure.GPIO_Mode = GPIO_Mode_AF;                   //????(??)??  
//    gpio_init_structure.GPIO_Speed = GPIO_Speed_50MHz;              //Fast speed  
//    gpio_init_structure.GPIO_PuPd= GPIO_PuPd_NOPULL;                    //??  
//    GPIO_Init(GPIOA, &gpio_init_structure);
	
//		GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_2);
//	
//    nvic_init_structure.NVIC_IRQChannel = TIM1_BRK_UP_TRG_COM_IRQn;                //使能TIM1中断通道  
//    nvic_init_structure.NVIC_IRQChannelCmd = ENABLE;                //使能TIM1中断  
//    nvic_init_structure.NVIC_IRQChannelPriority = 2;                //优先级为0  
//		
//    NVIC_Init(&nvic_init_structure);  
		
		//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
		//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
		nvic_init_structure.NVIC_IRQChannel = TIM2_IRQn;                //使能TIM2中断通道  
    nvic_init_structure.NVIC_IRQChannelCmd = ENABLE;                //使能TIM2中断  
    nvic_init_structure.NVIC_IRQChannelPriority = 0;                //优先级为0  
		
    NVIC_Init(&nvic_init_structure); 
		
		
		nvic_init_structure.NVIC_IRQChannel = TIM3_IRQn;                //使能TIM3中断通道  
    nvic_init_structure.NVIC_IRQChannelCmd = ENABLE;                //使能TIM3中断  
    nvic_init_structure.NVIC_IRQChannelPriority = 1;                //优先级为4
		
    NVIC_Init(&nvic_init_structure); 

		/*TIM3*/
		TIM_DeInit(TIM3);                                               //复位TIM3
    TIM_TimeBaseStructInit(&timer_init_structure);                  //初始化TIM结构体  
  
    timer_init_structure.TIM_ClockDivision = TIM_CKD_DIV1;          //系统时钟,不分频,24M  
    timer_init_structure.TIM_CounterMode = TIM_CounterMode_Up;      //向上计数模式  
    timer_init_structure.TIM_Period = 10;                          //每300 uS触发一次中断,??ADC  
    timer_init_structure.TIM_Prescaler = 47;                      //计数时钟分频,f=1M,systick=1 uS  
    timer_init_structure.TIM_RepetitionCounter = 0x00;              //发生0+1的update事件产生中断 
		
    TIM_TimeBaseInit(TIM3, &timer_init_structure);  
		TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);                       //使能TIM3中断
    TIM_Cmd(TIM3, ENABLE);                                          //使能TIM3
	
		/*TIM2*/
    TIM_DeInit(TIM2);                                               //复位TIM2  
    TIM_TimeBaseStructInit(&timer_init_structure);                  //初始化TIM结构体  
  
    timer_init_structure.TIM_CounterMode = TIM_CounterMode_Up;      //向上计数模式  
    timer_init_structure.TIM_Period = 10;                          //每300 uS触发一次中断,??ADC  
    timer_init_structure.TIM_Prescaler = 47;                      //计数时钟分频,f=1M,systick=1 uS  
    timer_init_structure.TIM_RepetitionCounter = 0;              //发生0+1的update事件产生中断 
		
    TIM_TimeBaseInit(TIM2, &timer_init_structure);  
  
//		timer_OCinit_structure.TIM_OCMode = TIM_OCMode_PWM1;
//		timer_OCinit_structure.TIM_OutputState = TIM_OutputState_Enable;
//		timer_OCinit_structure.TIM_Pulse = 10;
//		timer_OCinit_structure.TIM_OCPolarity = TIM_OCPolarity_High;
		
		
//		TIM_OC4Init(TIM2,&timer_OCinit_structure);
//		TIM_OC4PreloadConfig(TIM2,TIM_OCPreload_Enable);
//		TIM_ARRPreloadConfig(TIM2,ENABLE);
		
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);                      //使能TIM2中断
		TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);           //选择TIM1的update为触发源  
//		TIM_SelectInputTrigger(TIM2, TIM_TS_ITR0);
//		TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_Gated);//触发模式只启动，门控制启停都可以控制
//		TIM_SelectMasterSlaveMode(TIM2, TIM_MasterSlaveMode_Enable);//主从模式MSM

		TIM_Cmd(TIM2, ENABLE);


/* TIM1 ??? ---------------------------------------------------
   TIM1 ????(TIM1CLK) ??? APB2 ?? (PCLK2)    
    => TIM1CLK = PCLK2 = SystemCoreClock
   TIM1CLK = SystemCoreClock, Prescaler = 0, TIM1 counter clock = SystemCoreClock
   SystemCoreClock ?48 MHz 
   
   ??????? 4 ?PWM ???17.57 KHz:
     - TIM1_Period = (SystemCoreClock / 17570) - 1
   ??1??????? 50%
   ??2??????? 37.5%
   ??3??????? 25%
   ??4??????? 12.5%
   ????????????:
     - ChannelxPulse = DutyCycle * (TIM1_Period - 1) / 100
	*/  
    /*???????,???????????????*/
  //TimerPeriod = (SystemCoreClock / 17570 ) - 1;
  //TimerPeriod = (SystemCoreClock / DEF_PWMFRE ) - 1;
  //TimerPeriod = (SystemCoreClock / DEF_PWMFRE);
  /* TIM1 ???? */
  
  
  /* Time ??????*/
//  timer_init_structure.TIM_Prescaler = 47;
//  timer_init_structure.TIM_CounterMode = TIM_CounterMode_Up;  /* Time ????????????*/
//  timer_init_structure.TIM_Period = 25;
//  timer_init_structure.TIM_RepetitionCounter = 0;

//  TIM_TimeBaseInit(TIM1, &timer_init_structure);

//  /* ??1,2,3,4?PWM ???? */
//  timer_OCinit_structure.TIM_OCMode = TIM_OCMode_PWM1;
//  timer_OCinit_structure.TIM_OutputState = TIM_OutputState_Enable ;//TIM_OutputState_Enable; //PWM?????
//  timer_OCinit_structure.TIM_OutputNState = TIM_OutputNState_Disable ;//TIM_OutputNState_Enable; //??PWM?????
//  timer_OCinit_structure.TIM_OCPolarity = TIM_OCPolarity_High;  //PWM 1?????
//  timer_OCinit_structure.TIM_OCNPolarity = TIM_OCNPolarity_Low; //PWM?? 0?????
//  timer_OCinit_structure.TIM_OCIdleState = TIM_OCIdleState_Set;
//  timer_OCinit_structure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

//  timer_OCinit_structure.TIM_Pulse = 3; //?????
//  TIM_OC1Init(TIM1, &timer_OCinit_structure);//????1??
//	TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);
//	
//	timer_OCinit_structure.TIM_Pulse = 3; //?????
//  TIM_OC2Init(TIM1, &timer_OCinit_structure);//????1??
//	TIM_OC2PreloadConfig(TIM1,TIM_OCPreload_Enable);
//	
//	timer_OCinit_structure.TIM_Pulse = 3; //?????
//  TIM_OC3Init(TIM1, &timer_OCinit_structure);//????1??
//	TIM_OC3PreloadConfig(TIM1,TIM_OCPreload_Enable);

//  TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);                      //使能TIM1中断
//	TIM_ARRPreloadConfig(TIM1,ENABLE);
//	
//  /* TIM1 ?????*/
//	TIM_SelectOutputTrigger(TIM1, TIM_TRGOSource_Enable);							//选择TIM1的timer为触发源  
//	//TIM_SelectOutputTrigger(TIM1, TIM_TRGOSource_OC1Ref);							//选择TIM1的timer为触发源  
//	//TIM_SelectOutputTrigger(TIM1, TIM_TRGOSource_OC2Ref);							//选择TIM1的timer为触发源  
//	//TIM_SelectOutputTrigger(TIM1, TIM_TRGOSource_OC3Ref);							//选择TIM1的timer为触发源  
//	TIM_ClearITPendingBit(TIM1, TIM_IT_Update);     //清除update事件中断标志
//	TIM_SelectMasterSlaveMode(TIM1, TIM_MasterSlaveMode_Enable);//主从模式MSM  
//	TIM_CtrlPWMOutputs(TIM1, ENABLE);
//	//TIM_SelectOnePulseMode(TIM1,TIM_OPMode_Single);
//	
//  TIM_Cmd(TIM1, ENABLE);
//	
  /* TIM1 ????? */
  

}  

///*重设TIM1进入OnePule模式，用于控制脉冲个数*/

//void ChangeTIM1ToOnePulse(int Counter)
//{
//	TIM_TimeBaseInitTypeDef timer_init_structure;
//	TIM_Cmd(TIM1, DISABLE);
//	
//	timer_init_structure.TIM_RepetitionCounter = Counter;
//  TIM_TimeBaseInit(TIM1, &timer_init_structure);
//	
//	
//	TIM_SelectOnePulseMode(TIM1,TIM_OPMode_Single);
//	TIM_Cmd(TIM1, DISABLE);
//}

    
/****************************??????****************************/  
extern uint32_t   ShortCircuitLastTime;
volatile uint32_t timenum; 
extern uint8_t  	EventFlag; 
extern uint8_t 	ShortCircuit;
extern uint8_t 	ShortCircuitTimer;
extern int16_t OUT2_TimerCounter;

extern uint8_t SelfGetADCWell;
extern uint32_t ADC_ROW_Value[4];
extern uint8_t OUT2;
extern uint8_t OUT1;
extern uint32_t CPV;
uint8_t ADCIndex=0;
uint8_t DMAIndex=0;
static uint8_t DMA_GetADC_Counter;
int16_t selfADCValue[12];
uint32_t OUTSumADCValue=0;
uint32_t OUTADCValue=0;
uint8_t OUTADCValueIndex=0;
uint8_t LastRegisterA=0;
int ProgramCounter=0;
void TIM2_IRQHandler()  
{
    if(TIM_GetITStatus(TIM2, TIM_FLAG_Update))            //判断发生update事件中断  
    {  
        TIM_ClearITPendingBit(TIM2, TIM_FLAG_Update);     //清除update事件中断标志
    }  
}  

void TIM1_BRK_UP_TRG_COM_IRQHandler()  
{  
    if(TIM_GetITStatus(TIM1, TIM_FLAG_Update))            //判断发生update事件中断  
    {

      TIM_ClearITPendingBit(TIM1, TIM_FLAG_Update);     //清除update事件中断标志
    }  
} 

void TIM3_IRQHandler()
{
	  if(TIM_GetITStatus(TIM3, TIM_IT_Update))            //判断发生update事件中断  
    { 
				timenum++;
				//GPIOA->ODR ^= GPIO_Pin_9;
				if(timenum%10==0) /*5*10us=50us*/
				{
					//EventFlag = EventFlag | ADVtimeFlag;
					//GPIOA->ODR ^= GPIO_Pin_9;
					if(OUT2)
						OUT2_TimerCounter++;
					
					SMG_Diplay();
					Key_Scan();									//定时扫描按键
					ShortCircuitLastTime++;	
					IWDG_ReloadCounter();
					if(ShortCircuit)
						ShortCircuitCounter++;
					else
						ShortCircuitCounter=0;
				}
//				if(timenum%80==0)  /*80*10us = 800us*/
//				{
//					OUT2_TimerCounter++;
//					EventFlag = EventFlag | D4toD1timeFlag;
//					
//				}
//				if(timenum%100==0)	/*120*10us = 1,000us*/
//				{
//					EventFlag = EventFlag | Blink500msFlag;
//					ShortCircuitLastTime++;				
//					if(ShortCircuit)
//						ShortCircuitCounter++;
//					else
//						ShortCircuitCounter=0;
//					Key_Scan();									//定时扫描按键
//				}
//				if(timenum%2000==0)	/*2000*10us = 20,000us = 20ms*/
//				{
//					EventFlag = EventFlag | ButtontimeFlag;
//				}	
//				if(timenum%10000==0)	/*10000*10us = 100,000us = 100ms*/
//				{
//					EventFlag = EventFlag | Blink100msFlag;
//				}	
				if(timenum>=5000)	/*5000*10us = 500,000us = 500ms*/
				{
					EventFlag = EventFlag | Blink500msFlag;
//					EventFlag = EventFlag &(~EndDispalyFlag);

					timenum = 0;
				}
			TIM_ClearITPendingBit(TIM3, TIM_IT_Update);     //清除update事件中断标志
		}
}

void DMA1_Channel1_IRQHandler()  
{  
//		if(DMA_GetITStatus(DMA_IT_HT))
//				;
    if(DMA_GetITStatus(DMA_IT_TC))                      //判断DMA传输完成中断  
    {   
				//GPIOA->ODR ^= GPIO_Pin_9;
				ADC_ROW_Value[DMA_GetADC_Counter] = adc_dma_tab[0];
				OUTSumADCValue += adc_dma_tab[0];
				DMA_GetADC_Counter++;
				
				if(DMA_GetADC_Counter>3)
				{
					DMA_GetADC_Counter=0;
					OUTADCValue= OUTSumADCValue/4;
					OUTSumADCValue = 0;
					sample_finish = 1;
					if(OUTADCValue>=Threshold)
						RegisterA = 1;
					else if(OUTADCValue<=Threshold-DEL)
						RegisterA = 0;
					
					OUT1_Mode.DelayCounter++;

					/*设置OUT1的状态*/
					SetOUT1Status();
					
					if(LastRegisterA==0&&RegisterA==1)
					{
						CPV++;
						if(CPV>=CSV) /*如果计数器达到预先设定的CSV，清零，OUT2输出一个高电平*/
						{
							OUT2 = 1;
							CPV = 0;
						}
					}
					/*显示OUT1和OUT2的状态*/
					SMG_DisplayOUT_STATUS(OUT1,OUT2);
					LastRegisterA = RegisterA;
				}
    }  
    DMA_ClearITPendingBit(DMA_IT_TC);                   //清楚DMA中断标志位  
}  


void SMG_GPIO_INIT(void)
{
	
	   GPIO_InitTypeDef gpio_init_structure;  
    //??GPIO??  
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE); 
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);  
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);  
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);  
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);  
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF, ENABLE);  
	
    GPIO_StructInit(&gpio_init_structure);  
	
			//GPIOD3
    gpio_init_structure.GPIO_Pin = D3_Pin;  
    gpio_init_structure.GPIO_Mode = GPIO_Mode_OUT;                  
    gpio_init_structure.GPIO_OType = GPIO_OType_PP;                
    gpio_init_structure.GPIO_Speed = GPIO_Speed_2MHz;              
    gpio_init_structure.GPIO_PuPd= GPIO_PuPd_UP;                    
		GPIO_Init(D3_GPIO_Port, &gpio_init_structure);  

		//GPIOD2~4
    gpio_init_structure.GPIO_Pin = D4_Pin|D3_Pin|D2_Pin|D1_Pin;  
    gpio_init_structure.GPIO_Mode = GPIO_Mode_OUT;                  
    gpio_init_structure.GPIO_OType = GPIO_OType_PP;                
    gpio_init_structure.GPIO_Speed = GPIO_Speed_2MHz;              
    gpio_init_structure.GPIO_PuPd= GPIO_PuPd_UP;                    
    GPIO_Init(D4_GPIO_Port, &gpio_init_structure);  
		GPIO_Init(D3_GPIO_Port, &gpio_init_structure);  
		GPIO_Init(D2_GPIO_Port, &gpio_init_structure); 
		GPIO_Init(D1_GPIO_Port, &gpio_init_structure); 
	  //GPIOD5~7                                                       
    gpio_init_structure.GPIO_Pin = D5_Pin|D6_Pin|D7_Pin;  
    gpio_init_structure.GPIO_Mode = GPIO_Mode_OUT;                   
    gpio_init_structure.GPIO_OType = GPIO_OType_PP;                
    gpio_init_structure.GPIO_Speed = GPIO_Speed_2MHz;             
    gpio_init_structure.GPIO_PuPd= GPIO_PuPd_UP;                  
    GPIO_Init(D5_GPIO_Port, &gpio_init_structure);  
		GPIO_Init(D6_GPIO_Port, &gpio_init_structure);  
		GPIO_Init(D7_GPIO_Port, &gpio_init_structure);  
		//GPIOA                                                        
    gpio_init_structure.GPIO_Pin = D8_Pin|D9_Pin;  
    gpio_init_structure.GPIO_Mode = GPIO_Mode_OUT;                  
    gpio_init_structure.GPIO_OType = GPIO_OType_PP;                 
    gpio_init_structure.GPIO_Speed = GPIO_Speed_2MHz;              
    gpio_init_structure.GPIO_PuPd= GPIO_PuPd_UP;                    
    GPIO_Init(D8_GPIO_Port, &gpio_init_structure);  
		GPIO_Init(D9_GPIO_Port, &gpio_init_structure);  

		//GPIOB
	  gpio_init_structure.GPIO_Pin = A_Pin|B_Pin|C_Pin|D_Pin|E_Pin|F_Pin|G_Pin;  
    gpio_init_structure.GPIO_Mode = GPIO_Mode_OUT;                  
    gpio_init_structure.GPIO_OType = GPIO_OType_PP;                 
    gpio_init_structure.GPIO_Speed = GPIO_Speed_2MHz;              
    gpio_init_structure.GPIO_PuPd= GPIO_PuPd_DOWN; 
		GPIO_Init(A_GPIO_Port, &gpio_init_structure);
		GPIO_Init(B_GPIO_Port, &gpio_init_structure);
		GPIO_Init(C_GPIO_Port, &gpio_init_structure);
		GPIO_Init(D_GPIO_Port, &gpio_init_structure);
		GPIO_Init(E_GPIO_Port, &gpio_init_structure);
		GPIO_Init(F_GPIO_Port, &gpio_init_structure);
		GPIO_Init(G_GPIO_Port, &gpio_init_structure); 
		
		GPIO_WriteBit(D1_GPIO_Port, D1_Pin, Bit_RESET);
		GPIO_WriteBit(D2_GPIO_Port, D2_Pin, Bit_RESET);
		GPIO_WriteBit(D3_GPIO_Port, D3_Pin, Bit_RESET);
		GPIO_WriteBit(D4_GPIO_Port, D4_Pin, Bit_RESET);
		GPIO_WriteBit(D5_GPIO_Port, D5_Pin, Bit_RESET);
		GPIO_WriteBit(D6_GPIO_Port, D6_Pin, Bit_RESET);
		GPIO_WriteBit(D7_GPIO_Port, D7_Pin, Bit_RESET);
		GPIO_WriteBit(D8_GPIO_Port, D8_Pin, Bit_RESET);
		GPIO_WriteBit(D9_GPIO_Port, D9_Pin, Bit_RESET);

		GPIO_WriteBit(A_GPIO_Port, A_Pin, Bit_RESET);
		GPIO_WriteBit(B_GPIO_Port, B_Pin, Bit_RESET);
		GPIO_WriteBit(C_GPIO_Port, C_Pin, Bit_RESET);
		GPIO_WriteBit(D_GPIO_Port, D_Pin, Bit_RESET);
		GPIO_WriteBit(E_GPIO_Port, E_Pin, Bit_RESET);
		GPIO_WriteBit(F_GPIO_Port, F_Pin, Bit_RESET);
		GPIO_WriteBit(G_GPIO_Port, G_Pin, Bit_RESET);

}

void IO_GPIO_INIT(void)
{
		GPIO_InitTypeDef gpio_init_structure;  
    //??GPIO??  
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE); 
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);  
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);  
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOD, ENABLE);  
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);  
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF, ENABLE);  
	
    GPIO_StructInit(&gpio_init_structure);  
	
		//OUT1_GPIO_Port,OUT2_GPIO_Port
    gpio_init_structure.GPIO_Pin = OUT1_Pin|OUT2_Pin;  
    gpio_init_structure.GPIO_Mode = GPIO_Mode_OUT;                  
    gpio_init_structure.GPIO_OType = GPIO_OType_PP;                
    gpio_init_structure.GPIO_Speed = GPIO_Speed_50MHz;              
    gpio_init_structure.GPIO_PuPd= GPIO_PuPd_UP;                    
		GPIO_Init(OUT1_GPIO_Port, &gpio_init_structure);
		GPIO_Init(OUT2_GPIO_Port, &gpio_init_structure); 
	
		//SC_GPIO_Port
    gpio_init_structure.GPIO_Pin = SC_Pin;  
    gpio_init_structure.GPIO_Mode = GPIO_Mode_IN;                                
    gpio_init_structure.GPIO_Speed = GPIO_Speed_2MHz;              
    gpio_init_structure.GPIO_PuPd= GPIO_PuPd_NOPULL;                    
		GPIO_Init(SC_GPIO_Port, &gpio_init_structure);

}

void DAC_GPIO_Init(void)
{
		GPIO_InitTypeDef gpio_init_structure;  
    //??GPIO??  
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE); 
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	
	
	  gpio_init_structure.GPIO_Pin = DACOUT_Pin;  
    gpio_init_structure.GPIO_Mode = GPIO_Mode_AN;                                
    gpio_init_structure.GPIO_Speed = GPIO_Speed_2MHz;              
    gpio_init_structure.GPIO_PuPd= GPIO_PuPd_NOPULL;                    
		GPIO_Init(DACOUT_GPIO_Port, &gpio_init_structure);
}

void DAC_OUT_Init(void)
{
		DAC_InitTypeDef DAC_InitStructure;
		
		DAC_GPIO_Init();
		
		DAC_InitStructure.DAC_Trigger = DAC_Trigger_Software ; 
		DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
		DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bits11_0;
		DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Enable;
		DAC_Init(DAC_Channel_1,&DAC_InitStructure);
		
		DAC_Cmd(DAC_Channel_1,ENABLE);
}

	
void RCC_Configuration(void)
{
			RCC_DeInit();//??? RCC?????????
			RCC_HSICmd(ENABLE);//??HSI
			while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET)//??HSI???? 
			{
			}
			RCC_HCLKConfig(RCC_SYSCLK_Div1);
			RCC_PCLKConfig(RCC_HCLK_Div1);
			
			RCC_PLLConfig(RCC_PLLSource_HSI_Div2,RCC_PLLMul_12);
			RCC_PLLCmd(ENABLE);
			RCC_ADCCLKConfig(RCC_ADCCLK_PCLK_Div4);
			while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)==RESET)
			{
				
			}
			
			RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
			
			while(RCC_GetSYSCLKSource()!=0x08)
			{
			}
		
//		PWR_BackupAccessCmd(ENABLE);//允许修改RTC和后备寄存器
//		RCC_LSEConfig(RCC_LSE_OFF);//关闭外部低速时钟
//		PWR_BackupAccessCmd(DISABLE);//禁止修改RTC和后备寄存器
}

void DelaymsSet(int16_t ms)
{
		while(1)
		{
			ms--;
			if(ms<=0)
				break;
		}
}

uint8_t CheckFLag;

int main(void)
{
		uint32_t checkcouter;
		RCC_Configuration();
		SMG_GPIO_INIT();
		user_adc_init();
		RCC_GetClocksFreq(&SysClock);
		bsp_InitI2C();
		DAC_OUT_Init();		
		IO_GPIO_INIT();
		Button_Init();
		DelaymsSet(500); 
		IWDG_Config();
		CheckFLag = FlashCheck();
	
		if(CheckFLag)
		{
			/*程序运行次数检测*/
			//ee_WriteBytes((uint8_t*)&ProgramCounter, 90, 4);
			ProgramCheck();
			/*主要运行函数*/
			differenttialDC();
		}
		else
			while(1)
			{
				checkcouter++;
			}
}

#define 	FLASH_START_ADDR1 	 						0x08008000
uint8_t* UID=(uint8_t*)0x1FFFF7AC;  //获取UID  
uint32_t Fml_Constant 	= 0x19101943;//输入到公式的常熟
uint8_t *C= (uint8_t*)&Fml_Constant;//把常量转化成数组
uint8_t FormulaResult[4];
uint32_t FormulaCheck;
uint32_t UIDFlashResult;
uint16_t Fml_CRC16;
uint8_t D[12];

void ProgramCheck(void)
{
	while(ee_ReadBytes((uint8_t*)&ProgramCounter,90,4)==0);
	if(ProgramCounter>65535)
		ProgramCounter = 0;
	ProgramCounter = ProgramCounter+1;
	ee_WriteBytes((uint8_t*)&ProgramCounter, 90, 4);
	DelaymsSet(50); 
	if(ProgramCounter<=1)
	{
		ResetParameter();
	}
}

void IWDG_Config(void)
{
	if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
	{
		RCC_ClearFlag();
	}
	
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(IWDG_Prescaler_64);
	IWDG_SetReload(40000/20);
	IWDG_ReloadCounter();
	
	IWDG_Enable();
}

void Formula_100(uint8_t *D,uint8_t *Result)
{
	D[0] = UID[4];
	D[1] = UID[8];
	D[2] = UID[1];
	D[3] = UID[3];
	D[4] = UID[0];
	D[5] = UID[5];
	D[6] = UID[10];
	D[7] = UID[7];
	D[8] = UID[9];
	D[9] = UID[2];
	D[10] = UID[11];
	D[11] = UID[6];
	
	Result[0] = C[0] ^ D[0];
	Result[1] = C[1] ^ D[6] ^ D[7] ^ D[8] ^ D[9] ^ D[10] ^ D[11] ;
	Result[2] = C[2] ^ D[4] ;
	Result[3] = C[3] ^ D[2] ^ D[1];
}

uint8_t FlashCheck(void)
{
		uint8_t FlashFlag;
		Formula_100(D,FormulaResult);
		FormulaCheck = FormulaResult[0]+(FormulaResult[1]<<8)+(FormulaResult[2]<<16)+(FormulaResult[3]<<24);
		UIDFlashResult = *(__IO uint32_t*)(FLASH_START_ADDR1);
		if(UIDFlashResult==FormulaCheck)
			FlashFlag =1;
		else
			FlashFlag =0;
		
		return FlashFlag;
			
}
uint16_t Formula_CRC16(uint8_t *p,uint8_t len)
{
	uint8_t i;
	while(len--)
	{
		for(i=0x80; i!=0; i>>=1)
		{
			if((Fml_CRC16 & 0x8000) != 0)
			{
				Fml_CRC16 <<= 1;
				Fml_CRC16 ^= 0x1021;
			}
			else
			{
				Fml_CRC16 <<= 1;
			}
			if((*p&i)!=0)
			{
				Fml_CRC16 ^= 0x1021;
			}
		}
		p++;
	}
	return Fml_CRC16;
}
