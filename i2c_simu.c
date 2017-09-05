///**
//  ********************************  STM32F0x1  *********************************
//  * @文件名     ： i2c_simu.c
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
#include "i2c_simu.h"

#define BYTE  uint8_t  
	
GPIO_InitTypeDef gpio_init_structure;  


/** 
  * @brief  IIC Init 
  * @param  A: 
  * @retval None 
  */  
void IIC_Init(void)  
{                          
		/*Configure GPIO pin : PtPin */
    gpio_init_structure.GPIO_Pin = SDA_Pin|SCL_Pin;  
    gpio_init_structure.GPIO_Mode = GPIO_Mode_OUT;                   //????(??)??  
    gpio_init_structure.GPIO_OType = GPIO_OType_PP;                 //????  
    gpio_init_structure.GPIO_Speed = GPIO_Speed_2MHz;              //Fast speed  
    gpio_init_structure.GPIO_PuPd= GPIO_PuPd_DOWN;                    //??  
		GPIO_Init(SDA_GPIO_Port, &gpio_init_structure);
	
		GPIO_WriteBit(SDA_GPIO_Port, SDA_Pin, Bit_SET);
		GPIO_WriteBit(SCL_GPIO_Port, SCL_Pin, Bit_SET);

}  
   
/** 
* @brief  Simulate IIC conmunication :Create Start signal 
  * @retval None 
  */  
/*******************************************************************************
* File Name             : I2C_24c02_simulink_driver.c
* Author                : Rasar Yao
* Date First Issued     : 04/26/2010
* Description           : I2C_24c02_simulink_driver
********************************************************************************/

void I2C_delay(void)
{	
   uint8_t i=10; //这里可以优化速度	，经测试最低到5还能写入
   while(i) 
   { 
     i--; 
   } 
}

bool I2C_Start(void)
{
	SDA_H;
	SCL_H;
	I2C_delay();
	if(!SDA_read)return FALSE;	//SDA线为低电平则总线忙,退出
	SDA_L;
	I2C_delay();
	if(SDA_read) return FALSE;	//SDA线为高电平则总线出错,退出
	SDA_L;
	I2C_delay();
	return TRUE;
}

void I2C_Stop(void)
{
	SCL_L;
	I2C_delay();
	SDA_L;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SDA_H;
	I2C_delay();
}

void I2C_Ack(void)
{	
	SCL_L;
	I2C_delay();
	SDA_L;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SCL_L;
	I2C_delay();
}

void I2C_NoAck(void)
{	
	SCL_L;
	I2C_delay();
	SDA_H;
	I2C_delay();
	SCL_H;
	I2C_delay();
	SCL_L;
	I2C_delay();
}

bool I2C_WaitAck(void) 	 //返回为:=1有ACK,=0无ACK
{
	SCL_L;
	I2C_delay();
	SDA_H;			
	I2C_delay();
	SCL_H;
	I2C_delay();
	if(SDA_read)
	{
      SCL_L;
      return FALSE;
	}
	SCL_L;
	return TRUE;
}

void I2C_SendByte(uint8_t SendByte) //数据从高位到低位//
{
    uint8_t i=8;
    while(i--)
    {
        SCL_L;
        I2C_delay();
      if(SendByte&0x80)
        SDA_H;  
      else 
        SDA_L;   
    SendByte<<=1;
    I2C_delay();
		SCL_H;
    I2C_delay();
    }
    SCL_L;
}

uint8_t I2C_ReceiveByte(void)  //数据从高位到低位//
{ 
    uint8_t i=8;
    uint8_t ReceiveByte=0;

    SDA_H;				
    while(i--)
    {
      ReceiveByte<<=1;      
      SCL_L;
      I2C_delay();
			SCL_H;
      I2C_delay();	
      if(SDA_read)
      {
        ReceiveByte|=0x01;
      }
    }
    SCL_L;
    return ReceiveByte;
}

//写入1字节数据       待写入数据    待写入地址       器件类型(24c16或SD2403)
bool I2C_WriteByte(uint8_t SendByte, uint16_t WriteAddress, uint8_t DeviceAddress)
{		
    if(!I2C_Start())return FALSE;
    I2C_SendByte(((WriteAddress & 0x0700) >>7) | (DeviceAddress & 0xFFFE));//设置高起始地址+器件地址 
    if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}
    I2C_SendByte((uint8_t)(WriteAddress & 0x00FF));   //设置低起始地址      
    I2C_WaitAck();	
    I2C_SendByte(SendByte);
    I2C_WaitAck(); 
    I2C_Stop(); 
	//注意：因为这里要等待EEPROM写完，可以采用查询或延时方式(10ms)
    //Systick_Delay_1ms(10);
    return TRUE;
}

//读出1串数据         存放读出数据  待读出长度      待读出地址       器件类型(24c16或SD2403)	
bool I2C_ReadByte(uint32_t* pBuffer,   uint8_t length,     uint16_t ReadAddress,  uint8_t DeviceAddress)
{		
    if(!I2C_Start())return FALSE;
    I2C_SendByte(((ReadAddress & 0x0700) >>7) | (DeviceAddress & 0xFFFE));//设置高起始地址+器件地址 
    if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}
    I2C_SendByte((uint8_t)(ReadAddress & 0x00FF));   //设置低起始地址      
    I2C_WaitAck();
    I2C_Start();
    I2C_SendByte(((ReadAddress & 0x0700) >>7) | DeviceAddress | 0x0001);
    I2C_WaitAck();
    while(length)
    {
      *pBuffer = I2C_ReceiveByte();
      if(length == 1)I2C_NoAck();
      else I2C_Ack(); 
      pBuffer++;
      length--;
    }
    I2C_Stop();
    return TRUE;
}

