///**
//  ********************************  STM32F0x1  *********************************
//  * @文件名     ： display.c
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
#include "display.h"

/************************* 变量定义 ***********************************/
SMG_Data_Typedef	SMG_Data=		
{
	0,0,
};


const  uint16_t  data_SMG_bit_table[SMG_bit_sum]= {
	
															SMG_data_bit1,
															SMG_data_bit2,
															SMG_data_bit3,
															SMG_data_bit4,
	
															SMG_data_bit5,
															SMG_data_bit6,
															SMG_data_bit7,
															SMG_data_bit8,
															SMG_data_bit9,
	
													 };

const uint16_t  data_SMG_seg_table[]= {
															SMG_seg_Data_0,		//0
															SMG_seg_Data_1,		//1
															SMG_seg_Data_2,		//2
															SMG_seg_Data_3,		//3
															SMG_seg_Data_4,		//4
															SMG_seg_Data_5,		//5
															SMG_seg_Data_6,		//6
															SMG_seg_Data_7,		//7
															SMG_seg_Data_8,		//8
															SMG_seg_Data_9,		//9
															SMG_seg_Data_A,		//10
															SMG_seg_Data_b,		//11
															SMG_seg_Data_C,		//12
															SMG_seg_Data_d,		//13
															SMG_seg_Data_E,		//14
															SMG_seg_Data_f,		//15
															SMG_seg_Data__,  	//16
															SMG_seg_Data_o,		//17
															SMG_seg_Data_n,		//18
															SMG_seg_Data_V,		//19
															SMG_seg_Data_S,		//20
															SMG_seg_Data_t,		//21
															SMG_seg_Data_dark,//22
															SMG_seg_Data_L,		//23
															SMG_seg_Data_P,		//24
															SMG_seg_Data_f,		//25 
															SMG_seg_Data_H,		//26
															SMG_seg_Data_i,		//27
															SMG_seg_Data_r,		//28
															
															
														 SMG_seg_Data_a_,		//29
														 SMG_seg_Data_b_, 	//30			
														 SMG_seg_Data_c_,	 	//31		
														 SMG_seg_Data_d_, 	//32		
														 SMG_seg_Data_e_,	 	//33		
														 SMG_seg_Data_f_, 	//34			
														 SMG_seg_Data_g_, 	//35		
														 SMG_seg_Data_h_, 	//36		
														 SMG_seg_Data_abcdefgh_,	//37 		
														 SMG_seg_Data_dark,	 //38
														 
														 SMG_seg_Data_U,		//39
															
													 };


unsigned char SMG_D4_to_D1_bit=0;
unsigned char SMG_D8_to_D5_bit=4;
uint8_t SMG_BIT=0;
uint8_t SMG_D9_BIT=0;		 
unsigned char SMG_data_Decode_table[1][SMG_bit_sum]=
{ 
     0x00,  0x00,  0x00,  0x00,  0x00,  0x00,		0x00, 	0x00,0x00
};

int SMG_D9_Decode_table=0x00;

/************************* 函数定义 ***********************************/

/***********************************************************************
*
*数码管各种显示菜单
*
************************************************************************/
void SMG_data_update(void)
{ 
		/*测试码，BIT1到BIT8显示1到8的数字*/
		SMG_data_Decode_table[0][0]=data_SMG_seg_table[1];
		SMG_data_Decode_table[0][1]=data_SMG_seg_table[2];
		SMG_data_Decode_table[0][2]=data_SMG_seg_table[3];
		SMG_data_Decode_table[0][3]=data_SMG_seg_table[4];
		SMG_data_Decode_table[0][4]=data_SMG_seg_table[5];
		SMG_data_Decode_table[0][5]=data_SMG_seg_table[6];
		SMG_data_Decode_table[0][6]=data_SMG_seg_table[7];
		SMG_data_Decode_table[0][7]=data_SMG_seg_table[8];
}

/*按键复用显示*/
extern Button_STATUS KEY;
void ButtonMappingDisplay(uint8_t MappingFlag)
{ 
	if(MappingFlag==1)
	{
		if(KEY== ULOC)
		{
			SMG_data_Decode_table[0][0]=data_SMG_seg_table[12];   		//D1	C
			SMG_data_Decode_table[0][1]=data_SMG_seg_table[17];				//D2	O
			SMG_data_Decode_table[0][2]=data_SMG_seg_table[23];				//D3	L
			SMG_data_Decode_table[0][3]=data_SMG_seg_table[39];				//D4	U
		}
		else 
		{
			SMG_data_Decode_table[0][0]=data_SMG_seg_table[12];   		//D1	C
			SMG_data_Decode_table[0][1]=data_SMG_seg_table[17];				//D2	O
			SMG_data_Decode_table[0][2]=data_SMG_seg_table[23];				//D3	L
			SMG_data_Decode_table[0][3]=data_SMG_seg_table[22];				//D4	none
		}
	}
	else if(MappingFlag==2)
	{
		SMG_data_Decode_table[0][0]=data_SMG_seg_table[21];   		//D1	t
		SMG_data_Decode_table[0][1]=data_SMG_seg_table[27];				//D2	i
		SMG_data_Decode_table[0][2]=data_SMG_seg_table[18];				//D3	n
		SMG_data_Decode_table[0][3]=data_SMG_seg_table[27];				//D4	i
	}
	else if(MappingFlag==3)
	{	
		SMG_data_Decode_table[0][0]=data_SMG_seg_table[21];   		//D1	t
		SMG_data_Decode_table[0][1]=data_SMG_seg_table[20];				//D2	s
		SMG_data_Decode_table[0][2]=data_SMG_seg_table[28];				//D3 	r
		SMG_data_Decode_table[0][3]=data_SMG_seg_table[22];				//D4	none
	}
		
		SMG_data_Decode_table[0][4]=data_SMG_seg_table[22];				//D5 	none
		SMG_data_Decode_table[0][5]=data_SMG_seg_table[22];				//D6	none
		SMG_data_Decode_table[0][6]=data_SMG_seg_table[22];				//D7	none
		SMG_data_Decode_table[0][7]=data_SMG_seg_table[22];				//D8	none
	
	SMG_Diplay();
}


/*显示模式1*/


uint8_t GetIntNumber(uint32_t Number)
{
		uint8_t K1=0;
		while (Number)  
		{  
				Number = Number / 10;  
				K1++;  
		}  
		return K1;
}

void SMG_DisplayModeONE(int16_t Threshold,int16_t ADCvalue)
{ 
		uint8_t k_ADCvalue,k_Threshold;
		
		k_ADCvalue = GetIntNumber(ADCvalue);
		k_Threshold = GetIntNumber(Threshold);
		
		SMG_data_Decode_table[0][0]=data_SMG_seg_table[ADCvalue%10];						//D1
		if(k_ADCvalue>=2)
			SMG_data_Decode_table[0][1]=data_SMG_seg_table[(ADCvalue/10)%10];				//D2
		else
			SMG_data_Decode_table[0][1]=data_SMG_seg_table[22];										//none
		if(k_ADCvalue>=3)
			SMG_data_Decode_table[0][2]=data_SMG_seg_table[(ADCvalue/100)%10];			//D3
		else
			SMG_data_Decode_table[0][2]=data_SMG_seg_table[22];										//none
		if(k_ADCvalue>=4)
			SMG_data_Decode_table[0][3]=data_SMG_seg_table[(ADCvalue/1000)%10];		//D4
		else
			SMG_data_Decode_table[0][3]=data_SMG_seg_table[22];										//none
	
		SMG_data_Decode_table[0][4]=data_SMG_seg_table[Threshold%10];					//D5
		if(k_Threshold>=2)
			SMG_data_Decode_table[0][5]=data_SMG_seg_table[(Threshold/10)%10];		//D6
		else
			SMG_data_Decode_table[0][5]=data_SMG_seg_table[22];										//none	
		if(k_Threshold>=3)
			SMG_data_Decode_table[0][6]=data_SMG_seg_table[(Threshold/100)%10];	//D7
		else
			SMG_data_Decode_table[0][6]=data_SMG_seg_table[22];		
		if(k_Threshold>=4)
			SMG_data_Decode_table[0][7]=data_SMG_seg_table[(Threshold/1000)%10];	//D8
		else
			SMG_data_Decode_table[0][7]=data_SMG_seg_table[22];										//none
}

/*显示模式2*/
void SMG_DisplayModeTWO(int16_t RegisterB)
{ 
		SMG_data_Decode_table[0][0]=data_SMG_seg_table[22];					//D1	none
		SMG_data_Decode_table[0][1]=data_SMG_seg_table[22];					//D2	none
		SMG_data_Decode_table[0][2]=data_SMG_seg_table[22];					//D3	none
		SMG_data_Decode_table[0][3]=data_SMG_seg_table[22];					//D4	none
	
	if(RegisterB)
	{
		SMG_data_Decode_table[0][4]=data_SMG_seg_table[18];					//D5		n
		SMG_data_Decode_table[0][5]=data_SMG_seg_table[17];					//D6		o
		SMG_data_Decode_table[0][6]=data_SMG_seg_table[16];					//D7		-
		SMG_data_Decode_table[0][7]=data_SMG_seg_table[23];					//D8		L	
	}
	else
	{
		SMG_data_Decode_table[0][4]=data_SMG_seg_table[18];					//D5	n
		SMG_data_Decode_table[0][5]=data_SMG_seg_table[17];					//D6	o	
		SMG_data_Decode_table[0][6]=data_SMG_seg_table[16];					//D7	-
		SMG_data_Decode_table[0][7]=data_SMG_seg_table[13];					//D8  D
	}
		
}

/*显示模式3*/
void SMG_DisplayModeTHIRD(int16_t CPV_counter)
{ 
		uint8_t k_CPV_counter;
		
		k_CPV_counter = GetIntNumber(CPV_counter);
	
		
		SMG_data_Decode_table[0][0]=data_SMG_seg_table[CPV_counter%10];							//D1
		if(k_CPV_counter>=2)
			SMG_data_Decode_table[0][1]=data_SMG_seg_table[(CPV_counter/10)%10];				//D2
		else
			SMG_data_Decode_table[0][1]=data_SMG_seg_table[22];										//none
		if(k_CPV_counter>=3)
			SMG_data_Decode_table[0][2]=data_SMG_seg_table[(CPV_counter/100)%10];				//D3
		else
			SMG_data_Decode_table[0][2]=data_SMG_seg_table[22];										//none
		if(k_CPV_counter>=4)
			SMG_data_Decode_table[0][3]=data_SMG_seg_table[(CPV_counter/1000)%10];			//D4
		else
			SMG_data_Decode_table[0][3]=data_SMG_seg_table[22];										//none

		SMG_data_Decode_table[0][4]=data_SMG_seg_table[22];										//none
		SMG_data_Decode_table[0][5]=data_SMG_seg_table[19];													//D6	v
		SMG_data_Decode_table[0][6]=data_SMG_seg_table[24];													//D7	p
		SMG_data_Decode_table[0][7]=data_SMG_seg_table[12];													//D8	C
}



/*功能菜单-1*/
void SMG_DisplayMenuOne(uint32_t CSV_counter)
{ 
		uint8_t k_CSV_counter;
		
		k_CSV_counter = GetIntNumber(CSV_counter);
	
		SMG_data_Decode_table[0][0]=data_SMG_seg_table[CSV_counter%10];						//D1
	if(k_CSV_counter>=2)
		SMG_data_Decode_table[0][1]=data_SMG_seg_table[(CSV_counter/10)%10];			//D2
	else
			SMG_data_Decode_table[0][1]=data_SMG_seg_table[22];										//none
	if(k_CSV_counter>=3)
		SMG_data_Decode_table[0][2]=data_SMG_seg_table[(CSV_counter/100)%10];			//D3
	else
			SMG_data_Decode_table[0][2]=data_SMG_seg_table[22];										//none
	if(k_CSV_counter>=4)
		SMG_data_Decode_table[0][3]=data_SMG_seg_table[(CSV_counter/1000)%10];		//D4
	else
			SMG_data_Decode_table[0][3]=data_SMG_seg_table[22];										//none

		SMG_data_Decode_table[0][4]=data_SMG_seg_table[22];										//none
		SMG_data_Decode_table[0][5]=data_SMG_seg_table[19];												//D6	v
		SMG_data_Decode_table[0][6]=data_SMG_seg_table[20];												//D7	s
		SMG_data_Decode_table[0][7]=data_SMG_seg_table[12];												//D8	C
	
}

/*功能菜单-2-1,TOFF*/
void SMG_DisplayMenuTwo_TOFF(void)
{ 
	static uint8_t Blinkflag=1;
		SMG_data_Decode_table[0][0]=data_SMG_seg_table[35];						//D1	seg->f
		SMG_data_Decode_table[0][1]=data_SMG_seg_table[35];						//D2	seg->f
		SMG_data_Decode_table[0][2]=data_SMG_seg_table[35];						//D3	seg->f
		SMG_data_Decode_table[0][3]=data_SMG_seg_table[35];						//D4	seg->f
		if(Blinkflag)
		{
			SMG_data_Decode_table[0][4]=data_SMG_seg_table[15];						//D5	f
			SMG_data_Decode_table[0][5]=data_SMG_seg_table[15];						//D6	f
			SMG_data_Decode_table[0][6]=data_SMG_seg_table[17];						//D7	o
			SMG_data_Decode_table[0][7]=data_SMG_seg_table[21];						//D8	t
			Blinkflag = 0;
		}
		else
		{
			SMG_data_Decode_table[0][4]=data_SMG_seg_table[22];						//D5	none
			SMG_data_Decode_table[0][5]=data_SMG_seg_table[22];						//D6	none
			SMG_data_Decode_table[0][6]=data_SMG_seg_table[22];						//D7	none
			SMG_data_Decode_table[0][7]=data_SMG_seg_table[22];						//D8	none
			Blinkflag = 1;
		}
}

/*功能菜单-2-2,OFFD*/
void SMG_DisplayMenuTwo_OFFD(void)
{ 
		static uint8_t Blinkflag=1;
		SMG_data_Decode_table[0][0]=data_SMG_seg_table[35];						//D1	seg->f
		SMG_data_Decode_table[0][1]=data_SMG_seg_table[35];						//D2	seg->f
		SMG_data_Decode_table[0][2]=data_SMG_seg_table[35];						//D3	seg->f
		SMG_data_Decode_table[0][3]=data_SMG_seg_table[35];						//D4	seg->f
		if(Blinkflag)
		{
			SMG_data_Decode_table[0][4]=data_SMG_seg_table[13];						//D5	d
			SMG_data_Decode_table[0][5]=data_SMG_seg_table[15];						//D6	f
			SMG_data_Decode_table[0][6]=data_SMG_seg_table[15];						//D7	f
			SMG_data_Decode_table[0][7]=data_SMG_seg_table[17];						//D8	o
			Blinkflag = 0;
		}
		else
		{
			SMG_data_Decode_table[0][4]=data_SMG_seg_table[22];						//D5	none
			SMG_data_Decode_table[0][5]=data_SMG_seg_table[22];						//D6	none
			SMG_data_Decode_table[0][6]=data_SMG_seg_table[22];						//D7	none
			SMG_data_Decode_table[0][7]=data_SMG_seg_table[22];						//D8	none
			Blinkflag = 1;
		}
	
}

/*功能菜单-2-2-1,OFFD设定菜单*/
void SMG_DisplayMenuTwo_OFFD_SET(int16_t OFFD_DELAY,uint8_t BlinkONOFF)
{ 
		static uint8_t Blinkflag=1;
		uint8_t k_OFFD_DELAY;
		
		k_OFFD_DELAY = GetIntNumber(OFFD_DELAY);
	
	if(BlinkONOFF)
	{
		if(Blinkflag)
		{
			SMG_data_Decode_table[0][0]=data_SMG_seg_table[OFFD_DELAY%10];					//D1
			if(k_OFFD_DELAY>=2)
				SMG_data_Decode_table[0][1]=data_SMG_seg_table[(OFFD_DELAY/10)%10];			//D2
			else
				SMG_data_Decode_table[0][1]=data_SMG_seg_table[22];										//none
			if(k_OFFD_DELAY>=3)
				SMG_data_Decode_table[0][2]=data_SMG_seg_table[(OFFD_DELAY/100)%10];		//D3
			else
				SMG_data_Decode_table[0][2]=data_SMG_seg_table[22];										//none
			if(k_OFFD_DELAY>=4)
				SMG_data_Decode_table[0][3]=data_SMG_seg_table[(OFFD_DELAY/1000)%10];		//D4
			else
				SMG_data_Decode_table[0][3]=data_SMG_seg_table[22];										//none
			Blinkflag = 0;
		}
		else
		{
			SMG_data_Decode_table[0][0]=data_SMG_seg_table[22];					//D1	none
			SMG_data_Decode_table[0][1]=data_SMG_seg_table[22];					//D2	none
			SMG_data_Decode_table[0][2]=data_SMG_seg_table[22];					//D3	none
			SMG_data_Decode_table[0][3]=data_SMG_seg_table[22];					//D4	none
			Blinkflag = 1;
		}
	}
	else
	{
			SMG_data_Decode_table[0][0]=data_SMG_seg_table[OFFD_DELAY%10];					//D1
			if(k_OFFD_DELAY>=2)
				SMG_data_Decode_table[0][1]=data_SMG_seg_table[(OFFD_DELAY/10)%10];			//D2
			else
				SMG_data_Decode_table[0][1]=data_SMG_seg_table[22];										//none
			if(k_OFFD_DELAY>=3)
				SMG_data_Decode_table[0][2]=data_SMG_seg_table[(OFFD_DELAY/100)%10];		//D3
			else
				SMG_data_Decode_table[0][2]=data_SMG_seg_table[22];										//none
			if(k_OFFD_DELAY>=4)
				SMG_data_Decode_table[0][3]=data_SMG_seg_table[(OFFD_DELAY/1000)%10];		//D4
			else
				SMG_data_Decode_table[0][3]=data_SMG_seg_table[22];										//none
	}
		SMG_data_Decode_table[0][4]=data_SMG_seg_table[13];											//D5	d
		SMG_data_Decode_table[0][5]=data_SMG_seg_table[15];											//D6	f
		SMG_data_Decode_table[0][6]=data_SMG_seg_table[15];											//D7	f
		SMG_data_Decode_table[0][7]=data_SMG_seg_table[17];											//D8	o
	
}

/*功能菜单-2-3,ON Delay*/
void SMG_DisplayMenuTwo_ON_Delay(void)
{ 
	static uint8_t Blinkflag=1;
	
		SMG_data_Decode_table[0][0]=data_SMG_seg_table[35];						//D1	seg->f
		SMG_data_Decode_table[0][1]=data_SMG_seg_table[35];						//D2	seg->f
		SMG_data_Decode_table[0][2]=data_SMG_seg_table[35];						//D3	seg->f
		SMG_data_Decode_table[0][3]=data_SMG_seg_table[35];						//D4	seg->f
	
		if(Blinkflag)
		{
		SMG_data_Decode_table[0][4]=data_SMG_seg_table[13];						//D5	d
		SMG_data_Decode_table[0][5]=data_SMG_seg_table[16];						//D6	-
		SMG_data_Decode_table[0][6]=data_SMG_seg_table[18];						//D7	n
		SMG_data_Decode_table[0][7]=data_SMG_seg_table[17];						//D8	o
			Blinkflag = 0;
		}
		else
		{
			SMG_data_Decode_table[0][4]=data_SMG_seg_table[22];						//D5	none
			SMG_data_Decode_table[0][5]=data_SMG_seg_table[22];						//D6	none
			SMG_data_Decode_table[0][6]=data_SMG_seg_table[22];						//D7	none
			SMG_data_Decode_table[0][7]=data_SMG_seg_table[22];						//D8	none
			Blinkflag = 1;
		}

	
}

/*功能菜单-2-3_1,ON Delay设定菜单*/
void SMG_DisplayMenuTwo_ON_Delay_SET(int16_t ON_DELAY,uint8_t BlinkONOFF)
{ 
		static uint8_t Blinkflag=1;
		uint8_t k_ON_DELAY;
		
		k_ON_DELAY = GetIntNumber(ON_DELAY);
	if(BlinkONOFF)
	{
		if(Blinkflag)
		{
			SMG_data_Decode_table[0][0]=data_SMG_seg_table[ON_DELAY%10];					//D1
			if(k_ON_DELAY>=2)
				SMG_data_Decode_table[0][1]=data_SMG_seg_table[(ON_DELAY/10)%10];			//D2
			else
				SMG_data_Decode_table[0][1]=data_SMG_seg_table[22];										//none
			if(k_ON_DELAY>=3)
				SMG_data_Decode_table[0][2]=data_SMG_seg_table[(ON_DELAY/100)%10];		//D3
			else
				SMG_data_Decode_table[0][2]=data_SMG_seg_table[22];										//none
			if(k_ON_DELAY>=4)
				SMG_data_Decode_table[0][3]=data_SMG_seg_table[(ON_DELAY/1000)%10];		//D4
			else
				SMG_data_Decode_table[0][3]=data_SMG_seg_table[22];										//none
			Blinkflag = 0;
		}
		else
		{
			SMG_data_Decode_table[0][0]=data_SMG_seg_table[22];					//D1	none
			SMG_data_Decode_table[0][1]=data_SMG_seg_table[22];					//D2	none
			SMG_data_Decode_table[0][2]=data_SMG_seg_table[22];					//D3	none
			SMG_data_Decode_table[0][3]=data_SMG_seg_table[22];					//D4	none
			Blinkflag = 1;
		}
	}
	else
	{
			SMG_data_Decode_table[0][0]=data_SMG_seg_table[ON_DELAY%10];					//D1
			if(k_ON_DELAY>=2)
				SMG_data_Decode_table[0][1]=data_SMG_seg_table[(ON_DELAY/10)%10];			//D2
			else
				SMG_data_Decode_table[0][1]=data_SMG_seg_table[22];										//none
			if(k_ON_DELAY>=3)
				SMG_data_Decode_table[0][2]=data_SMG_seg_table[(ON_DELAY/100)%10];		//D3
			else
				SMG_data_Decode_table[0][2]=data_SMG_seg_table[22];										//none
			if(k_ON_DELAY>=4)
				SMG_data_Decode_table[0][3]=data_SMG_seg_table[(ON_DELAY/1000)%10];		//D4
			else
				SMG_data_Decode_table[0][3]=data_SMG_seg_table[22];										//none
	}
		SMG_data_Decode_table[0][4]=data_SMG_seg_table[13];						//D5	d
		SMG_data_Decode_table[0][5]=data_SMG_seg_table[16];						//D6	-
		SMG_data_Decode_table[0][6]=data_SMG_seg_table[18];						//D7	n
		SMG_data_Decode_table[0][7]=data_SMG_seg_table[17];						//D8	o
	
}

/*功能菜单-2-4,SHOT*/
void SMG_DisplayMenuTwo_SHOT(void)
{ 
		static uint8_t Blinkflag=1;
	
		SMG_data_Decode_table[0][0]=data_SMG_seg_table[35];						//D1	seg->f
		SMG_data_Decode_table[0][1]=data_SMG_seg_table[35];						//D2	seg->f
		SMG_data_Decode_table[0][2]=data_SMG_seg_table[35];						//D3	seg->f
		SMG_data_Decode_table[0][3]=data_SMG_seg_table[35];						//D4	seg->f
	
		if(Blinkflag)
		{
		SMG_data_Decode_table[0][4]=data_SMG_seg_table[21];						//D5	t
		SMG_data_Decode_table[0][5]=data_SMG_seg_table[17];						//D6	o
		SMG_data_Decode_table[0][6]=data_SMG_seg_table[26];						//D7	H
		SMG_data_Decode_table[0][7]=data_SMG_seg_table[20];						//D8	s
			Blinkflag = 0;
		}
		else
		{
			SMG_data_Decode_table[0][4]=data_SMG_seg_table[22];						//D5	none
			SMG_data_Decode_table[0][5]=data_SMG_seg_table[22];						//D6	none
			SMG_data_Decode_table[0][6]=data_SMG_seg_table[22];						//D7	none
			SMG_data_Decode_table[0][7]=data_SMG_seg_table[22];						//D8	none
			Blinkflag = 1;
		}	
}
/*功能菜单-2-4-1,SHOT设定菜单*/
void SMG_DisplayMenuTwo_SHOT_SET(int16_t SHOT,uint8_t BlinkONOFF)
{
		static uint8_t Blinkflag=1;
		uint8_t k_SHOT;
		
		k_SHOT = GetIntNumber(SHOT);
	if(BlinkONOFF)
	{
		if(Blinkflag)
		{
			SMG_data_Decode_table[0][0]=data_SMG_seg_table[SHOT%10];					//D1
			if(k_SHOT>=2)
				SMG_data_Decode_table[0][1]=data_SMG_seg_table[(SHOT/10)%10];			//D2
			else
				SMG_data_Decode_table[0][1]=data_SMG_seg_table[22];										//none
			if(k_SHOT>=3)
				SMG_data_Decode_table[0][2]=data_SMG_seg_table[(SHOT/100)%10];		//D3
			else
				SMG_data_Decode_table[0][2]=data_SMG_seg_table[22];										//none
			if(k_SHOT>=4)
				SMG_data_Decode_table[0][3]=data_SMG_seg_table[(SHOT/1000)%10];		//D4
			else
				SMG_data_Decode_table[0][3]=data_SMG_seg_table[22];										//none
			Blinkflag = 0;
		}
		else
		{
			SMG_data_Decode_table[0][0]=data_SMG_seg_table[22];					//D1	none
			SMG_data_Decode_table[0][1]=data_SMG_seg_table[22];					//D2	none
			SMG_data_Decode_table[0][2]=data_SMG_seg_table[22];					//D3	none
			SMG_data_Decode_table[0][3]=data_SMG_seg_table[22];					//D4	none
			Blinkflag = 1;
		}
	}
	else
	{
			SMG_data_Decode_table[0][0]=data_SMG_seg_table[SHOT%10];					//D1
			if(k_SHOT>=2)
				SMG_data_Decode_table[0][1]=data_SMG_seg_table[(SHOT/10)%10];			//D2
			else
				SMG_data_Decode_table[0][1]=data_SMG_seg_table[22];										//none
			if(k_SHOT>=3)
				SMG_data_Decode_table[0][2]=data_SMG_seg_table[(SHOT/100)%10];		//D3
			else
				SMG_data_Decode_table[0][2]=data_SMG_seg_table[22];										//none
			if(k_SHOT>=4)
				SMG_data_Decode_table[0][3]=data_SMG_seg_table[(SHOT/1000)%10];		//D4
			else
				SMG_data_Decode_table[0][3]=data_SMG_seg_table[22];										//none
	}
		SMG_data_Decode_table[0][4]=data_SMG_seg_table[21];								//D5	t
		SMG_data_Decode_table[0][5]=data_SMG_seg_table[17];								//D6	o
		SMG_data_Decode_table[0][6]=data_SMG_seg_table[26];								//D7	H
		SMG_data_Decode_table[0][7]=data_SMG_seg_table[20];								//D8	s
	
}


/*功能菜单-2-5,DEL*/
void SMG_DisplayMenuTwo_DEL(void)
{
			static uint8_t Blinkflag=1;
	
		SMG_data_Decode_table[0][0]=data_SMG_seg_table[35];						//D1	seg->f
		SMG_data_Decode_table[0][1]=data_SMG_seg_table[35];						//D2	seg->f
		SMG_data_Decode_table[0][2]=data_SMG_seg_table[35];						//D3	seg->f
		SMG_data_Decode_table[0][3]=data_SMG_seg_table[35];						//D4	seg->f
	
		if(Blinkflag)
		{
		SMG_data_Decode_table[0][4]=data_SMG_seg_table[22];						//D5	none
		SMG_data_Decode_table[0][5]=data_SMG_seg_table[23];						//D6	L
		SMG_data_Decode_table[0][6]=data_SMG_seg_table[14];						//D7	E
		SMG_data_Decode_table[0][7]=data_SMG_seg_table[13];						//D8	D
			Blinkflag = 0;
		}
		else
		{
			SMG_data_Decode_table[0][4]=data_SMG_seg_table[22];						//D5	none
			SMG_data_Decode_table[0][5]=data_SMG_seg_table[22];						//D6	none
			SMG_data_Decode_table[0][6]=data_SMG_seg_table[22];						//D7	none
			SMG_data_Decode_table[0][7]=data_SMG_seg_table[22];						//D8	none
			Blinkflag = 1;
		}
}

/*功能菜单-2-5-1,DEL设定菜单*/
void SMG_DisplayMenuTwo_DEL_SET(int16_t DEL,uint8_t BlinkONOFF)
{
		static uint8_t Blinkflag=1;
		uint8_t k_SHOT;
		
		k_SHOT = GetIntNumber(DEL);
	if(BlinkONOFF)
	{
		if(Blinkflag)
		{
			SMG_data_Decode_table[0][0]=data_SMG_seg_table[DEL%10];					//D1
			if(k_SHOT>=2)
				SMG_data_Decode_table[0][1]=data_SMG_seg_table[(DEL/10)%10];			//D2
			else
				SMG_data_Decode_table[0][1]=data_SMG_seg_table[22];										//none
			if(k_SHOT>=3)
				SMG_data_Decode_table[0][2]=data_SMG_seg_table[(DEL/100)%10];		//D3
			else
				SMG_data_Decode_table[0][2]=data_SMG_seg_table[22];										//none
			if(k_SHOT>=4)
				SMG_data_Decode_table[0][3]=data_SMG_seg_table[(DEL/1000)%10];		//D4
			else
				SMG_data_Decode_table[0][3]=data_SMG_seg_table[22];										//none
			Blinkflag = 0;
		}
		else
		{
			SMG_data_Decode_table[0][0]=data_SMG_seg_table[22];					//D1	none
			SMG_data_Decode_table[0][1]=data_SMG_seg_table[22];					//D2	none
			SMG_data_Decode_table[0][2]=data_SMG_seg_table[22];					//D3	none
			SMG_data_Decode_table[0][3]=data_SMG_seg_table[22];					//D4	none
			Blinkflag = 1;
		}
	}
	else
	{
			SMG_data_Decode_table[0][0]=data_SMG_seg_table[DEL%10];					//D1
			if(k_SHOT>=2)
				SMG_data_Decode_table[0][1]=data_SMG_seg_table[(DEL/10)%10];			//D2
			else
				SMG_data_Decode_table[0][1]=data_SMG_seg_table[22];										//none
			if(k_SHOT>=3)
				SMG_data_Decode_table[0][2]=data_SMG_seg_table[(DEL/100)%10];		//D3
			else
				SMG_data_Decode_table[0][2]=data_SMG_seg_table[22];										//none
			if(k_SHOT>=4)
				SMG_data_Decode_table[0][3]=data_SMG_seg_table[(DEL/1000)%10];		//D4
			else
				SMG_data_Decode_table[0][3]=data_SMG_seg_table[22];										//none
	}
		SMG_data_Decode_table[0][4]=data_SMG_seg_table[22];						//D5	none
		SMG_data_Decode_table[0][5]=data_SMG_seg_table[23];						//D6	L
		SMG_data_Decode_table[0][6]=data_SMG_seg_table[14];						//D7	E
		SMG_data_Decode_table[0][7]=data_SMG_seg_table[13];						//D8	D
	
}


/*SET-step-1模式菜单*/
void SMG_DisplaySET_Step_1_Mode(int16_t modeflag,uint32_t ADCINvalue)
{ 
	static uint8_t bilinkflag=0;
		uint8_t k_ADCINvalue;
		
		k_ADCINvalue = GetIntNumber(ADCINvalue);
		/* 3秒提醒，闪烁*/
		if(modeflag==1)   
		{
			SMG_data_Decode_table[0][0]=data_SMG_seg_table[22];								//D1	none
			SMG_data_Decode_table[0][1]=data_SMG_seg_table[22];								//D2	none
			SMG_data_Decode_table[0][2]=data_SMG_seg_table[22];								//D3	none
			SMG_data_Decode_table[0][3]=data_SMG_seg_table[22];								//D4	none
			
			/*闪烁效果*/
			if(bilinkflag)
			{
				SMG_data_Decode_table[0][4]=data_SMG_seg_table[1];								//D5	1
				SMG_data_Decode_table[0][5]=data_SMG_seg_table[21];								//D6	t
				SMG_data_Decode_table[0][6]=data_SMG_seg_table[14];								//D7	E
				SMG_data_Decode_table[0][7]=data_SMG_seg_table[20];								//D8	s	
				bilinkflag = 0;
			}
			else
			{
				SMG_data_Decode_table[0][4]=data_SMG_seg_table[22];								//D5	none
				SMG_data_Decode_table[0][5]=data_SMG_seg_table[22];								//D6	none
				SMG_data_Decode_table[0][6]=data_SMG_seg_table[22];								//D7	none
				SMG_data_Decode_table[0][7]=data_SMG_seg_table[22];								//D8	none
				bilinkflag = 1;
			}
		}
		/*3秒后*/
		else if(modeflag==2)
		{
			SMG_data_Decode_table[0][0]=data_SMG_seg_table[ADCINvalue%10];					//D1
			if(k_ADCINvalue>=2)
				SMG_data_Decode_table[0][1]=data_SMG_seg_table[(ADCINvalue/10)%10];			//D2
			else
				SMG_data_Decode_table[0][1]=data_SMG_seg_table[22];										//none
			if(k_ADCINvalue>=3)
				SMG_data_Decode_table[0][2]=data_SMG_seg_table[(ADCINvalue/100)%10];		//D3
			else
				SMG_data_Decode_table[0][2]=data_SMG_seg_table[22];										//none
			if(k_ADCINvalue>=4)
				SMG_data_Decode_table[0][3]=data_SMG_seg_table[(ADCINvalue/1000)%10];		//D4
			else
				SMG_data_Decode_table[0][3]=data_SMG_seg_table[22];										//none
			SMG_data_Decode_table[0][4]=data_SMG_seg_table[1];								//D5	1
			SMG_data_Decode_table[0][5]=data_SMG_seg_table[21];								//D6	t
			SMG_data_Decode_table[0][6]=data_SMG_seg_table[14];								//D7	E
			SMG_data_Decode_table[0][7]=data_SMG_seg_table[20];								//D8	s	
		}
		/*3秒内*/
		else
		{
			SMG_data_Decode_table[0][0]=data_SMG_seg_table[22];								//D1	none
			SMG_data_Decode_table[0][1]=data_SMG_seg_table[22];								//D2	none
			SMG_data_Decode_table[0][2]=data_SMG_seg_table[22];								//D3	none
			SMG_data_Decode_table[0][3]=data_SMG_seg_table[22];								//D4	none
			SMG_data_Decode_table[0][4]=data_SMG_seg_table[1];								//D5	1
			SMG_data_Decode_table[0][5]=data_SMG_seg_table[21];								//D6	t
			SMG_data_Decode_table[0][6]=data_SMG_seg_table[14];								//D7	E
			SMG_data_Decode_table[0][7]=data_SMG_seg_table[20];								//D8	s
		}
}

/*SET-step-2模式菜单*/
void SMG_DisplaySET_Step_2_Mode(int16_t modeflag,uint32_t ADCINvalue,uint32_t NewThreshold)
{ 
	static uint8_t bilinkflag=0;
	uint8_t k_NewThreshold;
		
	k_NewThreshold = GetIntNumber(NewThreshold);
		/* 3秒提醒，闪烁*/
		if(modeflag==1)   
		{
			SMG_data_Decode_table[0][0]=data_SMG_seg_table[22];								//D1	none
			SMG_data_Decode_table[0][1]=data_SMG_seg_table[22];								//D2	none
			SMG_data_Decode_table[0][2]=data_SMG_seg_table[22];								//D3	none
			SMG_data_Decode_table[0][3]=data_SMG_seg_table[22];								//D4	none
			
			/*闪烁效果*/
			if(bilinkflag)
			{
				SMG_data_Decode_table[0][4]=data_SMG_seg_table[2];								//D5	2
				SMG_data_Decode_table[0][5]=data_SMG_seg_table[21];								//D6	t
				SMG_data_Decode_table[0][6]=data_SMG_seg_table[14];								//D7	E
				SMG_data_Decode_table[0][7]=data_SMG_seg_table[20];								//D8	s	
				bilinkflag = 0;
			}
			else
			{
				SMG_data_Decode_table[0][4]=data_SMG_seg_table[22];								//D5	none
				SMG_data_Decode_table[0][5]=data_SMG_seg_table[22];								//D6	none
				SMG_data_Decode_table[0][6]=data_SMG_seg_table[22];								//D7	none
				SMG_data_Decode_table[0][7]=data_SMG_seg_table[22];								//D8	none
				bilinkflag = 1;
			}
		}
		/*3秒后*/
		else if(modeflag==2)  //modeflag  ==2
		{
			SMG_data_Decode_table[0][0]=data_SMG_seg_table[NewThreshold%10];					//D1
			if(k_NewThreshold>=2)
				SMG_data_Decode_table[0][1]=data_SMG_seg_table[(NewThreshold/10)%10];			//D2
			else
				SMG_data_Decode_table[0][1]=data_SMG_seg_table[22];										//none
			if(k_NewThreshold>=3)
				SMG_data_Decode_table[0][2]=data_SMG_seg_table[(NewThreshold/100)%10];		//D3
			else
				SMG_data_Decode_table[0][2]=data_SMG_seg_table[22];										//none
			if(k_NewThreshold>=4)
				SMG_data_Decode_table[0][3]=data_SMG_seg_table[(NewThreshold/1000)%10];		//D4
			else
				SMG_data_Decode_table[0][3]=data_SMG_seg_table[22];										//none			
			
				SMG_data_Decode_table[0][4]=data_SMG_seg_table[2];								//D5	2
				SMG_data_Decode_table[0][5]=data_SMG_seg_table[21];								//D6	t
				SMG_data_Decode_table[0][6]=data_SMG_seg_table[14];								//D7	E
				SMG_data_Decode_table[0][7]=data_SMG_seg_table[20];								//D8	s	
		}
		/*3秒内*/
		else
		{
			SMG_data_Decode_table[0][0]=data_SMG_seg_table[22];					//D1	
			SMG_data_Decode_table[0][1]=data_SMG_seg_table[22];					//D2
			SMG_data_Decode_table[0][2]=data_SMG_seg_table[22];					//D3
			SMG_data_Decode_table[0][3]=data_SMG_seg_table[22];					//D4
			SMG_data_Decode_table[0][4]=data_SMG_seg_table[2];								//D5	2
			SMG_data_Decode_table[0][5]=data_SMG_seg_table[21];								//D6	t
			SMG_data_Decode_table[0][6]=data_SMG_seg_table[14];								//D7	E
			SMG_data_Decode_table[0][7]=data_SMG_seg_table[20];								//D8	s
		}
}

/*OUT1,OUT2状态显示*/
void SMG_DisplayOUT_STATUS(uint8_t OUT1_STATUS,uint8_t OUT2_STATUS)
{ 
		OUT1_STATUS = GPIO_ReadInputDataBit(OUT1_GPIO_Port,OUT1_Pin);
		if(OUT1_STATUS==0 && OUT2_STATUS==1)
			SMG_data_Decode_table[0][8]=data_SMG_seg_table[33]|data_SMG_seg_table[34];						//D1	e,f seg	
		else if(OUT1_STATUS==1 && OUT2_STATUS==0)
			SMG_data_Decode_table[0][8]=data_SMG_seg_table[30]|data_SMG_seg_table[31];						//D1	b,c seg
		else if(OUT1_STATUS==1 && OUT1_STATUS==1)
		{
			SMG_data_Decode_table[0][8]=data_SMG_seg_table[30]|data_SMG_seg_table[31]|data_SMG_seg_table[33]|data_SMG_seg_table[34];//OUT1，OUT2同时亮
		}
		else
		{
			SMG_data_Decode_table[0][8] = data_SMG_seg_table[22];
		}
}

void ENDDisplay(void)
{
			/*测试码，BIT1到BIT8显示1到8的数字*/
		SMG_data_Decode_table[0][0]=data_SMG_seg_table[38];
		SMG_data_Decode_table[0][1]=data_SMG_seg_table[38];
		SMG_data_Decode_table[0][2]=data_SMG_seg_table[38];
		SMG_data_Decode_table[0][3]=data_SMG_seg_table[38];
		SMG_data_Decode_table[0][4]=data_SMG_seg_table[13];
		SMG_data_Decode_table[0][5]=data_SMG_seg_table[18];
		SMG_data_Decode_table[0][6]=data_SMG_seg_table[14];
		SMG_data_Decode_table[0][7]=data_SMG_seg_table[38];
}

void ParameterDisplay(void)
{
			/*测试码，BIT1到BIT8显示1到8的数字*/
		SMG_data_Decode_table[0][0]=data_SMG_seg_table[21];
		SMG_data_Decode_table[0][1]=data_SMG_seg_table[27];
		SMG_data_Decode_table[0][2]=data_SMG_seg_table[18];
		SMG_data_Decode_table[0][3]=data_SMG_seg_table[27];
		SMG_data_Decode_table[0][4]=data_SMG_seg_table[22];
		SMG_data_Decode_table[0][5]=data_SMG_seg_table[22];
		SMG_data_Decode_table[0][6]=data_SMG_seg_table[22];
		SMG_data_Decode_table[0][7]=data_SMG_seg_table[22];
}



/***********************************************************************
*
*数码管的基本操作函数
*
************************************************************************/
void SMG_Init(void)
{
	//消隐处理	
//	send_seg_RP0(SMG_seg_Data_dark);
//	send_bit_RP1(SMG_data_bit_allnot);
}	
	

/*段码*/

void send_seg_RP0(int32_t RP0_data)
{
		if(RP0_data & BIT0)	
			SMG_seg_a( SMG_OFF );
	else	SMG_seg_a( SMG_ON );
		
		if(RP0_data & BIT1)	
			SMG_seg_b( SMG_OFF );
	else	SMG_seg_b( SMG_ON );
		
		if(RP0_data & BIT2)	
			SMG_seg_c( SMG_OFF );
	else	SMG_seg_c( SMG_ON );
		
		if(RP0_data & BIT3)	
			SMG_seg_d( SMG_OFF );
	else	SMG_seg_d( SMG_ON );
		
		if(RP0_data & BIT4)	
			SMG_seg_e( SMG_OFF );
	else	SMG_seg_e( SMG_ON );
		
		if(RP0_data & BIT5)	
			SMG_seg_f( SMG_OFF );
	else	SMG_seg_f( SMG_ON );
		
		if(RP0_data & BIT6)	
			SMG_seg_g( SMG_OFF );
	else	SMG_seg_g( SMG_ON );
	
}

/*位片选*/
void send_bit_RP1(int32_t RP1_data)
{
		if(RP1_data & BIT0)	
			SMG_bit1( SMG_ON );
	else	SMG_bit1( SMG_OFF);
		
		if(RP1_data & BIT1)	
			SMG_bit2( SMG_ON );
	else	SMG_bit2( SMG_OFF);
		
		if(RP1_data & BIT2)	
			SMG_bit3( SMG_ON );
	else	SMG_bit3( SMG_OFF);
		
		if(RP1_data & BIT3)	
			SMG_bit4( SMG_ON );
	else	SMG_bit4( SMG_OFF );
		
		if(RP1_data & BIT4)	
			SMG_bit5( SMG_ON );
	else	SMG_bit5( SMG_OFF );
		
		if(RP1_data & BIT5)	
			SMG_bit6( SMG_ON );
	else	SMG_bit6( SMG_OFF );
		
		if(RP1_data & BIT6)	
			SMG_bit7( SMG_ON );
	else	SMG_bit7( SMG_OFF );
		
		if(RP1_data & BIT7)	
			SMG_bit8( SMG_ON );
	else	SMG_bit8( SMG_OFF );
		
		if(RP1_data & BIT8)	
			SMG_bit9( SMG_ON );
	else	SMG_bit9( SMG_OFF );

}

///*D4 到 D1的动态扫描*/
//void SMG_D4_to_D1_Display(void)
//{
//		//SMG_Data.SMG_seg=SMG_data_Decode_table[0][SMG_D4_to_D1_bit];	 //段码，填充数据
//		SMG_Data.SMG_seg = 	SMG_seg_Data_0;
//		SMG_Data.SMG_bit=data_SMG_bit_table[SMG_D4_to_D1_bit];				//位片选，填充数据
//	
//		/*失能，消隐处理*/
// 		send_seg_RP0(SMG_seg_Data_dark);
// 		send_bit_RP1(SMG_Data.SMG_bit);
//		/*使能，并显示*/
//		send_seg_RP0(SMG_Data.SMG_seg);
//		send_bit_RP1(SMG_Data.SMG_bit);
//		/*循环显示*/
//		if(SMG_D4_to_D1_bit++>=3)	
//			SMG_D4_to_D1_bit=0;
//}

///*D8 到 D5的动态扫描*/
//void SMG_D8_to_D5_Display(void)
//{
//		//SMG_Data.SMG_seg=SMG_data_Decode_table[0][SMG_D8_to_D5_bit];	 //段码，填充数据
//		SMG_Data.SMG_seg = 	SMG_seg_Data_0;
//		SMG_Data.SMG_bit=data_SMG_bit_table[SMG_D8_to_D5_bit];				//位片选，填充数据
//	
//		/*失能，消隐处理*/
// 		send_seg_RP0(SMG_seg_Data_dark);
// 		send_bit_RP1(SMG_Data.SMG_bit);
//		/*使能，并显示*/
//		send_seg_RP0(SMG_Data.SMG_seg);
//		send_bit_RP1(SMG_Data.SMG_bit);

//		/*循环显示*/
//		if(SMG_D8_to_D5_bit++>=SMG_bit_sum-1)	
//			SMG_D8_to_D5_bit=4;
//}

///*D9显示*/
//void SMG_D9_Display(void)
//{
//		SMG_Data.SMG_seg=SMG_D9_Decode_table;	 												//段码，填充数据
//		SMG_Data.SMG_bit=SMG_data_bit9;				//位片选，填充数据
//	
//		/*失能，消隐处理*/
// 		send_seg_RP0(SMG_seg_Data_dark);
// 		send_bit_RP1(SMG_Data.SMG_bit);
//		/*使能，并显示*/
//		send_seg_RP0(SMG_Data.SMG_seg);
//		send_bit_RP1(SMG_Data.SMG_bit);
//}



void SMG_Diplay(void)
{
		SMG_Data.SMG_seg=SMG_data_Decode_table[0][SMG_BIT];
		SMG_Data.SMG_bit=data_SMG_bit_table[SMG_BIT];				//位片选，填充数据
	
		/*失能，消隐处理*/
 		send_seg_RP0(SMG_seg_Data_dark);
 		send_bit_RP1(SMG_Data.SMG_bit);
		/*使能，并显示*/
		send_seg_RP0(SMG_Data.SMG_seg);
		send_bit_RP1(SMG_Data.SMG_bit);

		/*循环显示*/
		if(SMG_BIT++>=SMG_bit_sum-1)	
			SMG_BIT=0;
}


