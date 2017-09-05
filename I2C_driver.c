
#include "I2C_driver.h"


void I2C_GPIO_Config(void)
{
	  GPIO_InitTypeDef  GPIO_InitStructure; 
			/*Configure GPIO pin : PtPin */
    GPIO_InitStructure.GPIO_Pin = SDA_Pin|SCL_Pin;  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;                   //????(??)??  
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                 //????  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;              //Fast speed  
    GPIO_InitStructure.GPIO_PuPd= GPIO_PuPd_DOWN;                    //??  
		GPIO_Init(SDA_GPIO_Port, &GPIO_InitStructure);
	
		GPIO_WriteBit(SDA_GPIO_Port, SDA_Pin, Bit_SET);
		GPIO_WriteBit(SCL_GPIO_Port, SCL_Pin, Bit_SET);
	
//  GPIO_InitTypeDef  GPIO_InitStructure; 
//  /* Configure I2C1 pins: SCL and SDA */
//  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  
//  GPIO_Init(GPIOB, &GPIO_InitStructure);
//  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
//  GPIO_Init(GPIOB, &GPIO_InitStructure);
//	Port_IO_Config(C_Port_IOA,  8, 9, C_Port_OH_INV);	//�����������������
}

void I2C_delay(void)
{	
   int i=40; //��������Ż��ٶ�	����������͵�5����д��
   while(i) 
   { 
     i--; 
   } 
}

/*********************************************************************************************************/
int I2C_Start(void)
{
	SDA_H;
	SCL_H;
	I2C_delay();
	if(!SDA_read)
	{
		return FALSE;	//SDA��Ϊ�͵�ƽ������æ,�˳�
	}
	SDA_L;
	I2C_delay();
	if(SDA_read) return FALSE;	//SDA��Ϊ�ߵ�ƽ�����߳���,�˳�
	SDA_L;
	I2C_delay();
	return TRUE;
}
/*********************************************************************************************************/
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
/*********************************************************************************************************/
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
/*********************************************************************************************************/
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
/*********************************************************************************************************/
int I2C_WaitAck(void) 	 //����Ϊ:=1��ACK,=0��ACK
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
/*********************************************************************************************************/
void I2C_SendByte(int SendByte) //���ݴӸ�λ����λ//
{
    int i=8;
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
/*********************************************************************************************************/
int I2C_ReceiveByte(void)  //���ݴӸ�λ����λ//
{ 
    int i=8;
    int ReceiveByte=0;

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
/*********************************************************************************************************/
//д��1�ֽ�����       ��д������    ��д���ַ       ��������(DS3231)
int I2C_WriteByte(uint32_t SendByte, int WriteAddress, int DeviceAddress)
{		
    if(!I2C_Start())return FALSE;
    I2C_SendByte(DeviceAddress & 0xFE);//������ַ
    if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}
    I2C_SendByte(WriteAddress);   //��д�����ݵ���ʼ��ַ
    I2C_WaitAck();	
    I2C_SendByte(SendByte);
    I2C_WaitAck();   
    I2C_Stop(); 
    //Systick_Delay_1ms(10);
    return TRUE;
}
/*********************************************************************************************************/
//ע�ⲻ�ܿ�ҳд
//д��1������      ��д�������ַ    ��д�볤��      ��д���ַ       ��������(DS3231)
int I2C_BufferWrite(uint32_t* pBuffer, int length, int WriteAddress, int DeviceAddress)
{
    if(!I2C_Start())return FALSE;
    I2C_SendByte(DeviceAddress & 0xFE);//������ַ 
    if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}
    I2C_SendByte(WriteAddress);   //��д�����ݵ���ʼ��ַ      
	  I2C_WaitAck();	  
		while(length--)
		{
		  I2C_SendByte(*pBuffer);
		  I2C_WaitAck();
          pBuffer++;
		}
	  I2C_Stop();
	  //Systick_Delay_1ms(10);
	  return TRUE;
}
/*********************************************************************************************************/
//����1������         ��Ŷ�������  ����������      ��������ַ       ��������(DS3231)
int I2C_ReadBuffer(uint32_t* pBuffer,   int length,     int ReadAddress,  int DeviceAddress)
{		
    if(length == 0)
    return FALSE;
    if(!I2C_Start())return FALSE;
    I2C_SendByte(DeviceAddress & 0xFE);//������ַ 
    if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}
    I2C_SendByte(ReadAddress);        //���������ݵ���ʼ��ַ      
    I2C_WaitAck();
    I2C_Stop();                       //selectable
    I2C_Start();                      
    I2C_SendByte(DeviceAddress | 0x01);
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


/*********************************************************************************************************/

