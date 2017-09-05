/*********************************************************************************************************/
#ifndef __I2C_Driver_H
#define __I2C_Driver_H
/*********************************************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include  "stm32f0xx.h"
#include "differentialDC.h"
#define TRUE	1
#define FALSE	0


#define ADDRESS    0xA0 

//#define SCL_H         Port_IO_Config(C_Port_IOA, 8, 8, C_Port_OH)
//#define SCL_L         Port_IO_Config(C_Port_IOA, 8, 8, C_Port_OL)
//   
//#define SDA_H         Port_IO_Config(C_Port_IOA, 9, 9, C_Port_OH)
//#define SDA_L         Port_IO_Config(C_Port_IOA, 9, 9, C_Port_OL)

//#define SCL_read      (*P_IOA_Data & 0x100)
//#define SDA_read      (*P_IOA_Data & 0x200)


#define SCL_H         SCL_GPIO_Port->BSRR = SCL_Pin
#define SCL_L         SCL_GPIO_Port->BRR  = SCL_Pin 
   
#define SDA_H         SDA_GPIO_Port->BSRR = SDA_Pin
#define SDA_L         SDA_GPIO_Port->BRR  = SDA_Pin

#define SCL_read      SCL_GPIO_Port->IDR  & SCL_Pin
#define SDA_read      SDA_GPIO_Port->IDR  & SDA_Pin

void I2C_GPIO_Config(void);
int I2C_WriteByte(uint32_t SendByte, int WriteAddress, int DeviceAddress);
int I2C_ReceiveByte(void);

int I2C_BufferWrite(uint32_t* pBuffer,int length,int WriteAddress,int DeviceAddress);
int I2C_ReadBuffer(uint32_t* pBuffer,   int length,int ReadAddress,int DeviceAddress);
#endif 



