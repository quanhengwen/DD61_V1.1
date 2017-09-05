
///**
//  ********************************  STM32F0x1  *********************************
//  * @文件名     ： display.h
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
#ifndef __display_H
#define __display_H
#ifdef __cplusplus
 extern "C" {
#endif
#include  "main.h"
#include 	"stdint.h"
	 
//位指令宏定义
/* 位操作指令宏定义 用于对变量 或 可位寻址的特殊功能寄存器X进行具体第N位置1或置零操作(屏蔽其他位): X|=BITN(N位置1)	 X&=~BITN(N位置零) */
#define BIT0			(0x0001)
#define BIT1			(0x0002)
#define BIT2			(0x0004)
#define BIT3			(0x0008)
#define BIT4			(0x0010)
#define BIT5			(0x0020)
#define BIT6			(0x0040)
#define BIT7			(0x0080)

#define BIT8			(0x0100)
#define BIT9			(0x0200)
#define BIT10			(0x0400)
#define BIT11			(0x0800)
#define BIT12			(0x1000)
#define BIT13			(0x2000)
#define BIT14			(0x4000)
#define BIT15			(0x8000)
	 
	 
/*低电平有效*/

//#define SMG_bit1(a)	if (a)	\
//					GPIO_WriteBit(D1_GPIO_Port, D1_Pin, Bit_SET);\
//					else		\
//					GPIO_WriteBit(D1_GPIO_Port, D1_Pin, Bit_RESET)

//#define SMG_bit2(a)	if (a)	\
//					GPIO_WriteBit(D2_GPIO_Port, D2_Pin, Bit_SET);\
//					else		\
//					GPIO_WriteBit(D2_GPIO_Port, D2_Pin, Bit_RESET)
//					
//#define SMG_bit3(a)	if (a)	\
//					D3_GPIO_Port->BRR  = D3_Pin;\
//					else		\
//					D3_GPIO_Port->BSRR = D3_Pin
//					
//#define SMG_bit4(a)	if (a)	\
//					D4_GPIO_Port->BRR  = D4_Pin;\
//					else		\
//					D4_GPIO_Port->BSRR = D4_Pin

//#define SMG_bit5(a)	if (a)	\
//					D5_GPIO_Port->BRR  = D5_Pin;\
//					else		\
//					D5_GPIO_Port->BSRR = D5_Pin

//#define SMG_bit6(a)	if (a)	\
//					D6_GPIO_Port->BRR  = D6_Pin;\
//					else		\
//					D6_GPIO_Port->BSRR = D6_Pin		

//#define SMG_bit7(a)	if (a)	\
//					D7_GPIO_Port->BRR  = D7_Pin;\
//					else		\
//					D7_GPIO_Port->BSRR = D7_Pin	

//#define SMG_bit8(a)	if (a)	\
//					D8_GPIO_Port->BRR  = D8_Pin;\
//					else		\
//					D8_GPIO_Port->BSRR = D8_Pin	

//#define SMG_bit9(a)	if (a)	\
//					D9_GPIO_Port->BRR  = D9_Pin;\
//					else		\
//					D9_GPIO_Port->BSRR = D9_Pin	
//					
//	
//#define SMG_seg_a(a)	if (a)	\
//					GPIO_WriteBit(A_GPIO_Port, A_Pin, Bit_SET);\
//					else		\
//					GPIO_WriteBit(A_GPIO_Port, A_Pin, Bit_RESET)

//#define SMG_seg_b(a)	if (a)	\
//					GPIO_WriteBit(B_GPIO_Port, B_Pin, Bit_SET);\
//					else		\
//					GPIO_WriteBit(B_GPIO_Port, B_Pin, Bit_RESET)				

//#define SMG_seg_c(a)	if (a)	\
//					GPIO_WriteBit(C_GPIO_Port, C_Pin, Bit_SET);\
//					else		\
//					GPIO_WriteBit(C_GPIO_Port, C_Pin, Bit_RESET)					

//#define SMG_seg_d(a)	if (a)	\
//					GPIO_WriteBit(D_GPIO_Port, D_Pin, Bit_SET);\
//					else		\
//					GPIO_WriteBit(D_GPIO_Port, D_Pin, Bit_RESET)	

//#define SMG_seg_e(a)	if (a)	\
//					GPIO_WriteBit(E_GPIO_Port, E_Pin, Bit_SET);\
//					else		\
//					GPIO_WriteBit(E_GPIO_Port, E_Pin, Bit_RESET)	
//	
//#define SMG_seg_f(a)	if (a)	\
//					GPIO_WriteBit(F_GPIO_Port, F_Pin, Bit_SET);\
//					else		\
//					GPIO_WriteBit(F_GPIO_Port, F_Pin, Bit_RESET)		

//#define SMG_seg_g(a)	if (a)	\
//					GPIO_WriteBit(G_GPIO_Port, G_Pin, Bit_SET);\
//					else		\
//					GPIO_WriteBit(G_GPIO_Port, G_Pin, Bit_RESET)


#define SMG_bit1(a)	if (a)	\
					D1_GPIO_Port->BRR  = D1_Pin;\
					else		\
					D1_GPIO_Port->BSRR = D1_Pin

#define SMG_bit2(a)	if (a)	\
					D2_GPIO_Port->BRR  = D2_Pin;\
					else		\
					D2_GPIO_Port->BSRR = D2_Pin
					
#define SMG_bit3(a)	if (a)	\
					D3_GPIO_Port->BRR  = D3_Pin;\
					else		\
					D3_GPIO_Port->BSRR = D3_Pin
					
#define SMG_bit4(a)	if (a)	\
					D4_GPIO_Port->BRR  = D4_Pin;\
					else		\
					D4_GPIO_Port->BSRR = D4_Pin

#define SMG_bit5(a)	if (a)	\
					D5_GPIO_Port->BRR  = D5_Pin;\
					else		\
					D5_GPIO_Port->BSRR = D5_Pin

#define SMG_bit6(a)	if (a)	\
					D6_GPIO_Port->BRR  = D6_Pin;\
					else		\
					D6_GPIO_Port->BSRR = D6_Pin		

#define SMG_bit7(a)	if (a)	\
					D7_GPIO_Port->BRR  = D7_Pin;\
					else		\
					D7_GPIO_Port->BSRR = D7_Pin	

#define SMG_bit8(a)	if (a)	\
					D8_GPIO_Port->BRR  = D8_Pin;\
					else		\
					D8_GPIO_Port->BSRR = D8_Pin	

#define SMG_bit9(a)	if (a)	\
					D9_GPIO_Port->BRR  = D9_Pin;\
					else		\
					D9_GPIO_Port->BSRR = D9_Pin	


#define SMG_seg_a(a)	if (a)	\
					GPIO_WriteBit(A_GPIO_Port, A_Pin, Bit_RESET);\
					else		\
					GPIO_WriteBit(A_GPIO_Port, A_Pin, Bit_SET)

#define SMG_seg_b(a)	if (a)	\
					GPIO_WriteBit(B_GPIO_Port, B_Pin, Bit_RESET);\
					else		\
					GPIO_WriteBit(B_GPIO_Port, B_Pin, Bit_SET)				

#define SMG_seg_c(a)	if (a)	\
					GPIO_WriteBit(C_GPIO_Port, C_Pin, Bit_RESET);\
					else		\
					GPIO_WriteBit(C_GPIO_Port, C_Pin, Bit_SET)				

#define SMG_seg_d(a)	if (a)	\
					GPIO_WriteBit(D_GPIO_Port, D_Pin, Bit_RESET);\
					else		\
					GPIO_WriteBit(D_GPIO_Port, D_Pin, Bit_SET)

#define SMG_seg_e(a)	if (a)	\
					GPIO_WriteBit(E_GPIO_Port, E_Pin, Bit_RESET);\
					else		\
					GPIO_WriteBit(E_GPIO_Port, E_Pin, Bit_SET)
	
#define SMG_seg_f(a)	if (a)	\
					GPIO_WriteBit(F_GPIO_Port, F_Pin, Bit_RESET);\
					else		\
					GPIO_WriteBit(F_GPIO_Port, F_Pin, Bit_SET)			

#define SMG_seg_g(a)	if (a)	\
					GPIO_WriteBit(G_GPIO_Port, G_Pin, Bit_RESET);\
					else		\
					GPIO_WriteBit(G_GPIO_Port, G_Pin, Bit_SET)
					
	
//#define SMG_seg_a(a)	if (a)	\
//					GPIO_WriteBit(A_GPIO_Port, A_Pin, Bit_RESET);\
//					else		\
//					GPIO_WriteBit(A_GPIO_Port, A_Pin, Bit_SET)

//#define SMG_seg_b(a)	if (a)	\
//					GPIO_WriteBit(B_GPIO_Port, B_Pin, Bit_RESET);\
//					else		\
//					GPIO_WriteBit(B_GPIO_Port, B_Pin, Bit_SET)				

//#define SMG_seg_c(a)	if (a)	\
//					C_GPIO_Port->BRR  = C_Pin;\
//					else		\
//					C_GPIO_Port->BRR  = C_Pin					

//#define SMG_seg_d(a)	if (a)	\
//					D_GPIO_Port->BRR  = D_Pin;\
//					else		\
//					D_GPIO_Port->BRR  = D_Pin	

//#define SMG_seg_e(a)	if (a)	\
//					E_GPIO_Port->BRR  = E_Pin;\
//					else		\
//					E_GPIO_Port->BRR  = E_Pin	
//	
//#define SMG_seg_f(a)	if (a)	\
//					F_GPIO_Port->BRR  = F_Pin;\
//					else		\
//					F_GPIO_Port->BRR  = F_Pin				

//#define SMG_seg_g(a)	if (a)	\
//					G_GPIO_Port->BRR  = G_Pin;\
//					else		\
//					G_GPIO_Port->BRR  = G_Pin	
					 
//#define D1_H         D1_GPIO_Port->BSRR = D1_Pin
//#define D1_L         D1_GPIO_Port->BRR  = D1_Pin 	 
//	 
//#define D2_H         D2_GPIO_Port->BSRR = D2_Pin
//#define D2_L         D2_GPIO_Port->BRR  = D2_Pin 	 

//#define D3_H         D3_GPIO_Port->BSRR = D3_Pin
//#define D3_L         D3_GPIO_Port->BRR  = D3_Pin 	 

//#define D4_H         D4_GPIO_Port->BSRR = D4_Pin
//#define D4_L         D4_GPIO_Port->BRR  = D4_Pin 	 

//#define D5_H         D5_GPIO_Port->BSRR = D5_Pin
//#define D5_L         D5_GPIO_Port->BRR  = D5_Pin 	 

//#define D6_H         D6_GPIO_Port->BSRR = D6_Pin
//#define D6_L         D6_GPIO_Port->BRR  = D6_Pin 	

//#define D7_H         D7_GPIO_Port->BSRR = D7_Pin
//#define D7_L         D7_GPIO_Port->BRR  = D7_Pin 	 

//#define D8_H         D8_GPIO_Port->BSRR = D8_Pin
//#define D8_L         D8_GPIO_Port->BRR  = D8_Pin 	 

//#define D9_H         D9_GPIO_Port->BSRR = D9_Pin
//#define D9_L         D9_GPIO_Port->BRR  = D9_Pin 	 

//#define A_H         A_GPIO_Port->BSRR = A_Pin
//#define A_L         A_GPIO_Port->BRR  = A_Pin 	

//#define B_H         B_GPIO_Port->BSRR = B_Pin
//#define B_L         B_GPIO_Port->BRR  = B_Pin

//#define C_H         C_GPIO_Port->BSRR = C_Pin
//#define C_L         C_GPIO_Port->BRR  = C_Pin

//#define D_H         D_GPIO_Port->BSRR = D_Pin
//#define D_L         D_GPIO_Port->BRR  = D_Pin

//#define E_H         E_GPIO_Port->BSRR = E_Pin
//#define E_L         E_GPIO_Port->BRR  = E_Pin

//#define F_H        	F_GPIO_Port->BSRR = F_Pin
//#define F_L         F_GPIO_Port->BRR  = F_Pin

//#define G_H         G_GPIO_Port->BSRR = G_Pin
//#define G_L         G_GPIO_Port->BRR  = G_Pin

#define		SMG_ON  	0
#define 	SMG_OFF 	1
	
#define		QD_zhengxiang		1
#define		QD_fanxiang			0
#define 	SMG_seg_QD  		QD_zhengxiang		
#define 	SMG_bit_QD  		QD_zhengxiang	 

#if SMG_bit_QD
	#define SMG_data_bit1  BIT0
	#define SMG_data_bit2  BIT1
	#define SMG_data_bit3  BIT2
	#define SMG_data_bit4  BIT3
	#define SMG_data_bit5  BIT4
	#define SMG_data_bit6  BIT5
	#define SMG_data_bit7  BIT6
	#define SMG_data_bit8  BIT7
	#define SMG_data_bit9  BIT8
	
	#define SMG_data_bit987654321  (BIT8+BIT7+BIT6+BIT5+BIT4+BIT3+BIT2+BIT1+BIT0)
	#define SMG_data_bit_allnot  (BIT8+BIT7+BIT6+BIT5+BIT4+BIT3+BIT2+BIT1+BIT0)	
#else
	#define SMG_data_bit1  (~BIT0)
	#define SMG_data_bit2  (~BIT1)
	#define SMG_data_bit3  (~BIT2)
	#define SMG_data_bit4  (~BIT3)
	#define SMG_data_bit5  (~BIT4)
	#define SMG_data_bit6  (~BIT5)
	#define SMG_data_bit7  (~BIT6)
	#define SMG_data_bit8  (~BIT7)
	#define SMG_data_bit9  (~BIT8)
	
	#define SMG_data_bit987654321  (~(BIT8+BIT7+BIT6+BIT5+BIT4+BIT3+BIT2+BIT1+BIT0))
	#define SMG_data_bit_allnot  (BIT8+BIT7+BIT6+BIT5+BIT4+BIT3+BIT2+BIT1+BIT0)	

#endif
 
 
#if SMG_seg_QD
	#define SMG_seg_Data_0  		0x3F
	#define SMG_seg_Data_1  		0x30
	#define SMG_seg_Data_2  		0x5B
	#define SMG_seg_Data_3  		0x79
	#define SMG_seg_Data_4  		0x74
	#define SMG_seg_Data_5  		0x6d
	#define SMG_seg_Data_6  		0x6f
	#define SMG_seg_Data_7  		0x38
	#define SMG_seg_Data_8  		0x7f
	#define SMG_seg_Data_9  		0x7d
		
	#define SMG_seg_Data_A	 		0x7e  //20170715		
	#define SMG_seg_Data_b	 		0x67	//20170715
	#define SMG_seg_Data_C	 		0x0f	//20170715
	#define SMG_seg_Data_d			0x73  //20170715
	#define SMG_seg_Data_E	 		0x4f  //20170715	
	#define SMG_seg_Data_H	 		0x76  //20170715
	#define SMG_seg_Data_P	 		0x5e  //20170715
	#define SMG_seg_Data_U	 		0x37	//20170720
	#define SMG_seg_Data_y	 		0x6E	
	#define SMG_seg_Data__     	0x40	//20170715 -
	#define SMG_seg_Data_o			0x63					
	#define SMG_seg_Data_n			0x62  //20170715
	#define SMG_seg_Data_V			0x23	
	#define SMG_seg_Data_S			0x6d	//20170715
	#define SMG_seg_Data_f			0x4e	//20170715
	#define SMG_seg_Data_t			0x47  //20170715
	#define SMG_seg_Data_L			0x07	//20170715
	#define SMG_seg_Data_i			0x0b	//20170825
	#define SMG_seg_Data_r			0x0e	//20170715

	/*单独某段显示*/
	#define SMG_seg_Data_a_	 					0x01
	#define SMG_seg_Data_b_	 					0x02	
	#define SMG_seg_Data_c_	 					0x04
	#define SMG_seg_Data_d_	 					0x08
	#define SMG_seg_Data_e_	 					0x10
	#define SMG_seg_Data_f_	 					0x20
	#define SMG_seg_Data_g_	 					0x40
	#define SMG_seg_Data_h_	 					0x80
	#define SMG_seg_Data_abcdefgh_	 	0xFF	
	#define SMG_seg_Data_dark	 				0x00		
#else
	/*数字*/
	#define SMG_seg_Data_0  		0xC0
	#define SMG_seg_Data_1  		0xF9
	#define SMG_seg_Data_2  		0xA4
	#define SMG_seg_Data_3  		0xB0
	#define SMG_seg_Data_4  		0x99
	#define SMG_seg_Data_5  		0x92
	#define SMG_seg_Data_6  		0x82
	#define SMG_seg_Data_7  		0xF8
	#define SMG_seg_Data_8  		0x80
	#define SMG_seg_Data_9  		0x90
	/*字母*/
	#define SMG_seg_Data_A	 		0x88		
	#define SMG_seg_Data_B	 		0x83	
	#define SMG_seg_Data_C	 		0xC6	
	#define SMG_seg_Data_D	 		0xA1	
	#define SMG_seg_Data_E	 		0x86
	#define SMG_seg_Data_F	 		0x8E
	#define SMG_seg_Data_H	 		0x89
	#define SMG_seg_Data_L	 		0xC7
	#define SMG_seg_Data_P	 		0x8C
	#define SMG_seg_Data_U	 		0xC1
	#define SMG_seg_Data_y	 		0x91	
	/**/
	#define SMG_seg_Data___     0xBF
	#define SMG_seg_Data_o_			0xA3					
	#define SMG_seg_Data_n_			0xAB
	#define SMG_seg_Data_V_			0x0D
	#define SMG_seg_Data_S_			0x12
	#define SMG_seg_Data_t_			0x07
	#define SMG_seg_Data_i_			0x4f
	#define SMG_seg_Data_r_			0x4e
	
//	/*单独某段显示*/
//	#define SMG_seg_Data_a_	 					0xFE
//	#define SMG_seg_Data_b_	 					0xFD	
//	#define SMG_seg_Data_c_	 					0xFB
//	#define SMG_seg_Data_d_	 					0xF7
//	#define SMG_seg_Data_e_	 					0xEF
//	#define SMG_seg_Data_f_	 					0xDF
//	#define SMG_seg_Data_g_	 					0xBF
//	#define SMG_seg_Data_h_	 					0x7F
	/*多段显示*/
	#define SMG_seg_Data_ab_	 				0xFC
	#define SMG_seg_Data_abc_	 				0xF8	
	#define SMG_seg_Data_abcd_	 			0xF0
	#define SMG_seg_Data_abcde_	 			0xE0
	#define SMG_seg_Data_abcdef_	 		0xC0
	#define SMG_seg_Data_abcdefg_	 		0x80
	#define SMG_seg_Data_abcdefgh_	 	0x00	
	#define SMG_seg_Data_dark	 				0xFF	
	
#endif 



#define     change_STOP		0	
#define     change_START	1

#define     PAGE_sum			(8*2)
#define     SMG_bit_sum		9



/***************************************************************************/
// 外部定义变量声明
typedef struct 
{
	unsigned char		SMG_seg;		
	int 	SMG_bit;							
}SMG_Data_Typedef;

extern SMG_Data_Typedef		SMG_Data;


extern const uint16_t  data_SMG_bit_table[SMG_bit_sum];
extern const uint16_t  data_SMG_seg_table[];

extern unsigned char SMG_bit;

extern unsigned char SMG_data_Decode_table[1][SMG_bit_sum];

extern int SMG_D9_Decode_table;
/***************************************************************************/
/*外部定义函数声明*/

/*显示模式1*/
void SMG_DisplayModeONE(int16_t Threshold,int16_t ADCvalue);
/*显示模式2*/
void SMG_DisplayModeTWO(int16_t RegisterB);
/*显示模式3*/
void SMG_DisplayModeTHIRD(int16_t CPV_counter);
/*功能菜单-1*/
void SMG_DisplayMenuOne(uint32_t CSV_counter);
/*功能菜单-2-1,TOFF*/
void SMG_DisplayMenuTwo_TOFF(void);
/*功能菜单-2-2,OFFD*/
void SMG_DisplayMenuTwo_OFFD(void);
/*功能菜单-2-2-1,OFFD设定菜单*/
void SMG_DisplayMenuTwo_OFFD_SET(int16_t OFFD_DELAY,uint8_t BlinkONOFF);
/*功能菜单-2-3,ON Delay*/
void SMG_DisplayMenuTwo_ON_Delay(void);
/*功能菜单-2-3_1,ON Delay设定菜单*/
void SMG_DisplayMenuTwo_ON_Delay_SET(int16_t ON_DELAY,uint8_t BlinkONOFF);
/*功能菜单-2-4,SHOT*/
void SMG_DisplayMenuTwo_SHOT(void);
/*功能菜单-2-4-1,SHOT设定菜单*/
void SMG_DisplayMenuTwo_SHOT_SET(int16_t SHOT,uint8_t BlinkONOFF);
/*功能菜单-2-5,DEL*/
void SMG_DisplayMenuTwo_DEL(void);
/*功能菜单-2-5-1,SHOT设定菜单*/
void SMG_DisplayMenuTwo_DEL_SET(int16_t DEL,uint8_t BlinkONOFF);

/*SET-step-1模式菜单*/
void SMG_DisplaySET_Step_1_Mode(int16_t modeflag,uint32_t ADCINvalue);
/*SET-step-2模式菜单*/
void SMG_DisplaySET_Step_2_Mode(int16_t modeflag,uint32_t ADCINvalue,uint32_t ADCMAX);
/*OUT1,OUT2状态显示*/
void SMG_DisplayOUT_STATUS(uint8_t OUT1_STATUS,uint8_t OUT2_STATUS);
/*END界面*/
void ENDDisplay(void);
	
/*按键复用显示*/
void ButtonMappingDisplay(uint8_t MappingFlag);

/*内部函数*/
void SMG_data_update(void);
void SMG_GPIO_Config(void);
void SMG_Init(void);

void send_seg_RP0(int32_t);
void send_bit_RP1(int32_t);

void SMG_D4_to_D1_Display(void);
void SMG_D8_to_D5_Display(void);
void SMG_D9_Display(void);
void SMG_Diplay(void);
#ifdef __cplusplus
}
#endif
#endif /*__display_H */
