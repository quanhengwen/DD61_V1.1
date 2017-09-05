
///**
//  ********************************  STM32F0x1  *********************************
//  * @�ļ���     �� key.h
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __i2c_simu_H
#define __i2c_simu_H
#ifdef __cplusplus
 extern "C" {
#endif
#include  "stm32f0xx.h"
#include "differentialDC.h"
#include 	"stdbool.h"

#define SCL_H         SCL_GPIO_Port->BSRR = SCL_Pin
#define SCL_L         SCL_GPIO_Port->BRR  = SCL_Pin 
   
#define SDA_H         SDA_GPIO_Port->BSRR = SDA_Pin
#define SDA_L         SDA_GPIO_Port->BRR  = SDA_Pin

#define SCL_read      SCL_GPIO_Port->IDR  & SCL_Pin
#define SDA_read      SDA_GPIO_Port->IDR  & SDA_Pin

#define I2C_PageSize  8  //24C02ÿҳ8�ֽ�

#define ADDRESS    0xA0 

#define FALSE					1
#define TRUE					0	 
	 
bool I2C_WriteByte(uint8_t SendByte, uint16_t WriteAddress, uint8_t DeviceAddress);
bool I2C_ReadByte(uint32_t* pBuffer,   uint8_t length,     uint16_t ReadAddress,  uint8_t DeviceAddress);
 
void IIC_Init(void);

#ifdef __cplusplus
}
#endif
#endif /*__i2c_simu_H */


