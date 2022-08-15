/**
  ******************************************************************************
  * File Name          : I2C.c
  * Description        : This file provides code for the configuration
  *                      of the I2C instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "i2c.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

I2C_HandleTypeDef hi2c1;

/* I2C1 init function */
void MX_I2C1_Init(void)
{

  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x307075B1;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Analogue filter 
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure Digital filter 
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(i2cHandle->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspInit 0 */

  /* USER CODE END I2C1_MspInit 0 */
  
    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**I2C1 GPIO Configuration    
    PB8     ------> I2C1_SCL
    PB9     ------> I2C1_SDA 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* I2C1 clock enable */
    __HAL_RCC_I2C1_CLK_ENABLE();
  /* USER CODE BEGIN I2C1_MspInit 1 */

  /* USER CODE END I2C1_MspInit 1 */
  }
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* i2cHandle)
{

  if(i2cHandle->Instance==I2C1)
  {
  /* USER CODE BEGIN I2C1_MspDeInit 0 */

  /* USER CODE END I2C1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_I2C1_CLK_DISABLE();
  
    /**I2C1 GPIO Configuration    
    PB8     ------> I2C1_SCL
    PB9     ------> I2C1_SDA 
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_8|GPIO_PIN_9);

  /* USER CODE BEGIN I2C1_MspDeInit 1 */

  /* USER CODE END I2C1_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */

////////////////////////////
//���ܣ�дOV7670�Ĵ���
//���أ�1-�ɹ�	0-ʧ��
unsigned char wrOV7670Reg(unsigned char regID, unsigned char regDat)
{
	if(HAL_I2C_Mem_Write(&hi2c1,0x42,regID,1,&regDat,1,1)==HAL_OK)
	{
  	return(1);
	}	
	return(0);

}

void set_OV7670reg(void)
{
//	wrOV7670Reg(0x3a, 0x04);
//	wrOV7670Reg(0x40, 0xd0);  //ԭ��10��ͨ��COM15������������ݷ�Χ�������ʽ
//	wrOV7670Reg(0x8c, 0x00);
//	wrOV7670Reg(0x12, 0x16); //�����ʽQVGA,ԭ����Ϊ0x14
//	wrOV7670Reg(0x32, 0x80);
//	wrOV7670Reg(0x17, 0x16);
//	wrOV7670Reg(0x18, 0x04);
//	wrOV7670Reg(0x19, 0x02);
//	wrOV7670Reg(0x1a, 0x7b);//0x7a,  ԭ��0x7b
//	wrOV7670Reg(0x03, 0x06);//0x0a,  ԭ��0x06
//	wrOV7670Reg(0x0c, 0x04);//ԭ��0x04	   com3,�������ţ�ʡ�磬ģʽ��
//	wrOV7670Reg(0x3e, 0x00);//	ԭ��0x00   com14��dcw��pclk�������ã��Լ���Ƶ��
//	wrOV7670Reg(0x70, 0x3a); //ԭ��0x04
//	wrOV7670Reg(0x71, 0x35); //ԭ��0x35
//	wrOV7670Reg(0x72, 0x11); //ԭ��0x11
//	wrOV7670Reg(0x73, 0x00);//ԭ��f0 �Ѹ�����ı����Ҫ
//	wrOV7670Reg(0xa2, 0x00); //ԭ��0x02
//	wrOV7670Reg(0x11, 0x80); //ʱ��Ƶ�ʣ������Ϊ���ֵ��ԭ��Ϊ0x81  ���ı䣩
//	//wrOV7670Reg(0x15 , 0x31);
//	wrOV7670Reg(0x7a, 0x20); //ԭ��0x20 �Ĵ���7a--89Ϊ٤����������
//	wrOV7670Reg(0x7b, 0x1c); //ԭ��0x1c
//	wrOV7670Reg(0x7c, 0x28); //ԭ��0x28
//	wrOV7670Reg(0x7d, 0x3c); //ԭ��0x3c
//	wrOV7670Reg(0x7e, 0x55); //ԭ��0x55
//	wrOV7670Reg(0x7f, 0x68); //ԭ��0x68
//	wrOV7670Reg(0x80, 0x76); //ԭ��0x76
//	wrOV7670Reg(0x81, 0x80); //ԭ��0x80
//	wrOV7670Reg(0x82, 0x88); //ԭ��0x88
//	wrOV7670Reg(0x83, 0x8f); //ԭ��0x8f
//	wrOV7670Reg(0x84, 0x96); //ԭ��0x96
//	wrOV7670Reg(0x85, 0xa3); //ԭ��0xa3
//	wrOV7670Reg(0x86, 0xaf); //ԭ��0xaf
//	wrOV7670Reg(0x87, 0xc4); //ԭ��0xc4
//	wrOV7670Reg(0x88, 0xd7); //ԭ��0xd7
//	wrOV7670Reg(0x89, 0xe8); //ԭ��0xe8 
//	 
//	wrOV7670Reg(0x32,0xb6);
//	
//	wrOV7670Reg(0x13, 0x67); //ԭ��0xe0 com13 AGC,AWB,AECʹ�ܿ���	 ���ı䣩
//	wrOV7670Reg(0x00, 0x00);//AGC //ԭ��0x00
//	wrOV7670Reg(0x10, 0x00);//ԭ��0x00  �ع�ֵ
//	wrOV7670Reg(0x0d, 0x00);//ԭ��0x00	 COM4
//	wrOV7670Reg(0x14, 0x4e);//ԭ��0x28, limit the max gain	�Զ��������� û�仯
//	wrOV7670Reg(0xa5, 0x05); //ԭ��0x05 50Hz bangding step limting
//	wrOV7670Reg(0xab, 0x07); //ԭ��0x07 60Hz bangding step limting
//	wrOV7670Reg(0x24, 0x75); //ԭ��0x75 agc/aec-�ȶ�������������
//	wrOV7670Reg(0x25, 0x63); //ԭ��0x63 agc/aec-�ȶ�������������
//	wrOV7670Reg(0x26, 0xA5); //ԭ��0xa5 agc/aec-������������
//	wrOV7670Reg(0x9f, 0x78); //ԭ��0x78 ����ֱ��ͼ��aec/agc�Ŀ���1
//	wrOV7670Reg(0xa0, 0x68); //ԭ��0x68 ����ֱ��ͼ��aec/agc�Ŀ���2
////	wrOV7670Reg(0xa1, 0x03);//0x0b,
//	wrOV7670Reg(0xa6, 0xdf);//0xd8, ԭ��0xdf ����ֱ��ͼ��aec/agc�Ŀ���3
//	wrOV7670Reg(0xa7, 0xdf);//0xd8, ԭ��0xdf ����ֱ��ͼ��aec/agc�Ŀ���4
//	wrOV7670Reg(0xa8, 0xf0); //ԭ��0xf0  ����ֱ��ͼ��aec/agc�Ŀ���5
//	wrOV7670Reg(0xa9, 0x90);  //ԭ��0x90  ����ֱ��ͼ��aec/agc�Ŀ���6
//	wrOV7670Reg(0xaa, 0x94);  //ԭ��0x94  ����ֱ��ͼ��aec/agc�Ŀ���7
//	//wrOV7670Reg(0x13, 0xe5);  //ԭ��0xe5
//	wrOV7670Reg(0x0e, 0x61);  //ԭ��0x61 COM5
//	wrOV7670Reg(0x0f, 0x43);  //ԭ��0x4b COM6
//	wrOV7670Reg(0x16, 0x02);  //ԭ��0x02 ����
//	wrOV7670Reg(0x1e, 0x13);//0x07, ԭ��0x37 ˮƽ����/��ֱ��תʹ��	 ����Ϊ01������ת
//	wrOV7670Reg(0x21, 0x02);	//ԭ��0x02  ����
//	wrOV7670Reg(0x22, 0x91);	//ԭ��0x91	����
//	wrOV7670Reg(0x29, 0x07);	//ԭ��0x07	����
//	wrOV7670Reg(0x33, 0x0b);	//ԭ��0x0b	href���ƣ���Ϊ��ʼֵ80ʱ�ı�
//	wrOV7670Reg(0x35, 0x0b);	//ԭ��0xe0	����
//	wrOV7670Reg(0x37, 0x3f);	//ԭ��0x1d	adc����
//	wrOV7670Reg(0x38, 0x01);	//ԭ��0x71	adc��ģ�⹲ģ����
//	wrOV7670Reg(0x39, 0x00);	//ԭ��0x2a	adcƫ�ƿ���
//	wrOV7670Reg(0x3c, 0x78);	//ԭ��0x78	 COM12
//	wrOV7670Reg(0x4d, 0x40);	//ԭ��0x40	 ����
//	wrOV7670Reg(0x4e, 0x20);	//ԭ��0x20	 ����
//	wrOV7670Reg(0x69, 0x00);	//ԭ��0x00	  �̶��������
//	wrOV7670Reg(0x6b, 0x0A);   //PLLԭ��0x00	  pll����	��Ҫ����
//	wrOV7670Reg(0x74, 0x19);	//ԭ��0x19	�ֶ���������
//	wrOV7670Reg(0x8d, 0x4f);	//ԭ��0x4f	����
//	wrOV7670Reg(0x8e, 0x00);	//ԭ��0x00	����
//	wrOV7670Reg(0x8f, 0x00);	//ԭ��0x00	����
//	wrOV7670Reg(0x90, 0x00);	//ԭ��0x00	����
//	wrOV7670Reg(0x91, 0x00);	//ԭ��0x00	����
//	wrOV7670Reg(0x92, 0x00);   //0x19,//0x66	 ԭ��0x00 ���е�8λ
//	wrOV7670Reg(0x96, 0x00);	//ԭ��0x00	  ����
//	wrOV7670Reg(0x9a, 0x80);	//ԭ��0x80	  ����
//	wrOV7670Reg(0xb0, 0x84);	//ԭ��0xe0	  ����
//	wrOV7670Reg(0xb1, 0x0c);	//ԭ��0x0c	 ablc����
//	wrOV7670Reg(0xb2, 0x0e);	//ԭ��0x0e	  ����
//	wrOV7670Reg(0xb3, 0x82);	//ԭ��0x82	  ablc target
//	wrOV7670Reg(0xb8, 0x0a);	//ԭ��0x0a	   ����
//	wrOV7670Reg(0x43, 0x14);	//ԭ��0x14	  43-48Ϊ����
//	wrOV7670Reg(0x44, 0xf0);	//ԭ��0xf0
//	wrOV7670Reg(0x45, 0x34);	//ԭ��0x34
//	wrOV7670Reg(0x46, 0x58);	//ԭ��0x58
//	wrOV7670Reg(0x47, 0x28);	//ԭ��0x28
//	wrOV7670Reg(0x48, 0x3a);	//ԭ��0x3a
//	
//	wrOV7670Reg(0x59, 0x88);	//ԭ��0x88	 51-5e����
//	wrOV7670Reg(0x5a, 0x88);	//ԭ��0x88
//	wrOV7670Reg(0x5b, 0x44);	//ԭ��0x44
//	wrOV7670Reg(0x5c, 0x67);	//ԭ��0x67
//	wrOV7670Reg(0x5d, 0x49);	//ԭ��0x49
//	wrOV7670Reg(0x5e, 0x0e);	//ԭ��0xe0
//	
//	wrOV7670Reg(0x64, 0x04);	//ԭ��0x04	  64-66��ͷ����
//	wrOV7670Reg(0x65, 0x20);	//ԭ��0x20
//	wrOV7670Reg(0x66, 0x05);	//ԭ��0x05

//	wrOV7670Reg(0x94, 0x04);	//ԭ��0x04	94-95��ͷ����
//	wrOV7670Reg(0x95, 0x08);	//ԭ��0x08	 

//	wrOV7670Reg(0x6c, 0x0a);	//ԭ��0x0a	6c-6fawb����
//	wrOV7670Reg(0x6d, 0x55);	//ԭ��0x55
//	wrOV7670Reg(0x6e, 0x11);	//ԭ��0x11
//	wrOV7670Reg(0x6f, 0x9f);   //0x9e for advance AWB	  ԭ��9f

//	wrOV7670Reg(0x6a, 0x40);	//ԭ��0x40	gͨ��awb����
//	wrOV7670Reg(0x01, 0x40);	//ԭ��0x40	bͨ��awb�������
//	wrOV7670Reg(0x02, 0x40);	//ԭ��0x40	rͨ��awb�������
//	
//	//wrOV7670Reg(0x13, 0xe7);	//ԭ��0xe7
//	wrOV7670Reg(0x15, 0x00);   //00  cmos10  ����pclk���ѡ��
//	wrOV7670Reg(0x4f, 0x80);	//ԭ��0x80	����Ϊɫ�ʾ���ϵ������
//	wrOV7670Reg(0x50, 0x80);	//ԭ��0x80
//	wrOV7670Reg(0x51, 0x00);	//ԭ��0x00
//	wrOV7670Reg(0x52, 0x22);	//ԭ��0x22
//	wrOV7670Reg(0x53, 0x5e);	//ԭ��0x5e
//	wrOV7670Reg(0x54, 0x80);	//ԭ��0x80
//	wrOV7670Reg(0x58, 0x9e);	//ԭ��0x9e

//	wrOV7670Reg(0x41, 0x08);	//ԭ��0x08	com16 ����Ϊ��Ե����
//	wrOV7670Reg(0x3f, 0x00);	//ԭ��0x00  ��Ե��ǿ����
//	wrOV7670Reg(0x75, 0x05);	//ԭ��0x05
//	wrOV7670Reg(0x76, 0xe1);	//ԭ��0xe1

//	wrOV7670Reg(0x4c, 0x00);	//ԭ��0x00  ��������ǿ��
//	wrOV7670Reg(0x77, 0x01);	//ԭ��0x01	����ȥ��ƫ��
//	
//	wrOV7670Reg(0x3d, 0xc2);	//0xc0,	   com13 �������
//	wrOV7670Reg(0x4b, 0x09);	//ԭ��0x09 �Ĵ���4b����
//	wrOV7670Reg(0xc9, 0x60);	//ԭ��0x60	 ���Ͷȿ���
//	//wrOV7670Reg(0x41, 0x38);	//ԭ��0x38	  com16��Ե��������
//	wrOV7670Reg(0x55, 0x00);
//	wrOV7670Reg(0x56, 0x40);//0x40,  change according to Jim's request	�Աȶȿ���
//	wrOV7670Reg(0x34, 0x11);	//ԭ��0x11	  �й����вο���ѹ����
//	wrOV7670Reg(0x3b, 0x02);//0x00,//ԭ��0x02,	  com11����
//	wrOV7670Reg(0xa4, 0x89);//0x88, ԭ��89 nt����  
//	
//	wrOV7670Reg(0x96, 0x00);	//ԭ��0x00  ����96-9c����
//	wrOV7670Reg(0x97, 0x30);	//ԭ��0x30
//	wrOV7670Reg(0x98, 0x20);	//ԭ��0x20
//	wrOV7670Reg(0x99, 0x30);	//ԭ��0x30
//	wrOV7670Reg(0x9a, 0x84);	//ԭ��0x84
//	wrOV7670Reg(0x9b, 0x29);	//ԭ��0x29
//	wrOV7670Reg(0x9c, 0x03);	//ԭ��0x03
//	wrOV7670Reg(0x9d, 0x4c);	//ԭ��0x4c	50HZ�����˲���ֵ
//	wrOV7670Reg(0x9e, 0x3f);	//ԭ��0x3f	60HZ�����˲���ֵ

//	wrOV7670Reg(0x09, 0x00);	//ԭ��0x00 ͨ�ÿ�����com2
//	wrOV7670Reg(0x3b, 0xc2);//0x82,//0xc0,//ԭ��0xc2,	//night mode ԭ��0xc2
//	=====================================================================================================
	wrOV7670Reg(0x3a, 0x04);
	wrOV7670Reg(0x40, 0xd0);
	wrOV7670Reg(0x12, 0x14);
	wrOV7670Reg(0x32, 0x80);
	wrOV7670Reg(0x17, 0x16);
	wrOV7670Reg(0x18, 0x04);
	wrOV7670Reg(0x19, 0x02);
	wrOV7670Reg(0x1a, 0x7b);
	wrOV7670Reg(0x03, 0x06);
	wrOV7670Reg(0x0c, 0x04);
	wrOV7670Reg(0x3e, 0x00);
	wrOV7670Reg(0x70, 0x3a);
	wrOV7670Reg(0x71, 0x35);
	wrOV7670Reg(0x72, 0x11);
	wrOV7670Reg(0x73, 0x00);
	wrOV7670Reg(0xa2, 0x02);
	wrOV7670Reg(0x11, 0x81);
	
	wrOV7670Reg(0x7a, 0x20);
	wrOV7670Reg(0x7b, 0x1c);
	wrOV7670Reg(0x7c, 0x28);
	wrOV7670Reg(0x7d, 0x3c);
	wrOV7670Reg(0x7e, 0x55);
	wrOV7670Reg(0x7f, 0x68);
	wrOV7670Reg(0x80, 0x76);
	wrOV7670Reg(0x81, 0x80);
	wrOV7670Reg(0x82, 0x88);
	wrOV7670Reg(0x83, 0x8f);
	wrOV7670Reg(0x84, 0x96);
	wrOV7670Reg(0x85, 0xa3);
	wrOV7670Reg(0x86, 0xaf);
	wrOV7670Reg(0x87, 0xc4);
	wrOV7670Reg(0x88, 0xd7);
	wrOV7670Reg(0x89, 0xe8);
	
	wrOV7670Reg(0x13, 0xe0);
	wrOV7670Reg(0x00, 0x00);
	
	wrOV7670Reg(0x10, 0x00);
	wrOV7670Reg(0x0d, 0x00);
	wrOV7670Reg(0x14, 0x28);
	wrOV7670Reg(0xa5, 0x05);
	wrOV7670Reg(0xab, 0x07);
	wrOV7670Reg(0x24, 0x75);
	wrOV7670Reg(0x25, 0x63);
	wrOV7670Reg(0x26, 0xA5);
	wrOV7670Reg(0x9f, 0x78);
	wrOV7670Reg(0xa0, 0x68);
	wrOV7670Reg(0xa1, 0x03);
	wrOV7670Reg(0xa6, 0xdf);
	wrOV7670Reg(0xa7, 0xdf);
	wrOV7670Reg(0xa8, 0xf0);
	wrOV7670Reg(0xa9, 0x90);
	wrOV7670Reg(0xaa, 0x94);
	wrOV7670Reg(0x13, 0xe5);

	wrOV7670Reg(0x0e, 0x61);
	wrOV7670Reg(0x0f, 0x4b);
	wrOV7670Reg(0x16, 0x02);
	wrOV7670Reg(0x1e, 0x27);
	wrOV7670Reg(0x21, 0x02);
	wrOV7670Reg(0x22, 0x91);
	wrOV7670Reg(0x29, 0x07);
	wrOV7670Reg(0x33, 0x0b);
	wrOV7670Reg(0x35, 0x0b);
	wrOV7670Reg(0x37, 0x1d);
	wrOV7670Reg(0x38, 0x71);
	wrOV7670Reg(0x39, 0x2a);
	wrOV7670Reg(0x3c, 0x78);
	wrOV7670Reg(0x4d, 0x40);
	wrOV7670Reg(0x4e, 0x20);
	wrOV7670Reg(0x69, 0x00);
	wrOV7670Reg(0x6b, 0x60);
	wrOV7670Reg(0x74, 0x19);
	wrOV7670Reg(0x8d, 0x4f);
	wrOV7670Reg(0x8e, 0x00);
	wrOV7670Reg(0x8f, 0x00);
	wrOV7670Reg(0x90, 0x00);
	wrOV7670Reg(0x91, 0x00);
	wrOV7670Reg(0x92, 0x00);
	wrOV7670Reg(0x96, 0x00);
	wrOV7670Reg(0x9a, 0x80);
	wrOV7670Reg(0xb0, 0x84);
	wrOV7670Reg(0xb1, 0x0c);
	wrOV7670Reg(0xb2, 0x0e);
	wrOV7670Reg(0xb3, 0x82);
	wrOV7670Reg(0xb8, 0x0a);



	wrOV7670Reg(0x43, 0x14);
	wrOV7670Reg(0x44, 0xf0);
	wrOV7670Reg(0x45, 0x34);
	wrOV7670Reg(0x46, 0x58);
	wrOV7670Reg(0x47, 0x28);
	wrOV7670Reg(0x48, 0x3a);
	wrOV7670Reg(0x59, 0x88);
	wrOV7670Reg(0x5a, 0x88);
	wrOV7670Reg(0x5b, 0x44);
	wrOV7670Reg(0x5c, 0x67);
	wrOV7670Reg(0x5d, 0x49);
	wrOV7670Reg(0x5e, 0x0e);
	wrOV7670Reg(0x64, 0x04);
	wrOV7670Reg(0x65, 0x20);
	wrOV7670Reg(0x66, 0x05);
	wrOV7670Reg(0x94, 0x04);
	wrOV7670Reg(0x95, 0x08);
	wrOV7670Reg(0x6c, 0x0a);
	wrOV7670Reg(0x6d, 0x55);
	wrOV7670Reg(0x6e, 0x11);
	wrOV7670Reg(0x6f, 0x9f);
	wrOV7670Reg(0x6a, 0x40);
	wrOV7670Reg(0x01, 0x40);
	wrOV7670Reg(0x02, 0x40);
	wrOV7670Reg(0x13, 0xe7);
	wrOV7670Reg(0x15, 0x00);  
	
	
	wrOV7670Reg(0x4f, 0x80);
	wrOV7670Reg(0x50, 0x80);
	wrOV7670Reg(0x51, 0x00);
	wrOV7670Reg(0x52, 0x22);
	wrOV7670Reg(0x53, 0x5e);
	wrOV7670Reg(0x54, 0x80);
	wrOV7670Reg(0x58, 0x9e);
	
	wrOV7670Reg(0x41, 0x08);
	wrOV7670Reg(0x3f, 0x00);
	wrOV7670Reg(0x75, 0x05);
	wrOV7670Reg(0x76, 0xe1);
	wrOV7670Reg(0x4c, 0x00);
	wrOV7670Reg(0x77, 0x01);
	wrOV7670Reg(0x3d, 0xc2);	
	wrOV7670Reg(0x4b, 0x09);
	wrOV7670Reg(0xc9, 0x60);
	wrOV7670Reg(0x41, 0x38);
	wrOV7670Reg(0x56, 0x40);
	
	wrOV7670Reg(0x34, 0x11);
	wrOV7670Reg(0x3b, 0x02); 
								
	wrOV7670Reg(0xa4, 0x89);
	wrOV7670Reg(0x96, 0x00);
	wrOV7670Reg(0x97, 0x30);
	wrOV7670Reg(0x98, 0x20);
	wrOV7670Reg(0x99, 0x30);
	wrOV7670Reg(0x9a, 0x84);
	wrOV7670Reg(0x9b, 0x29);
	wrOV7670Reg(0x9c, 0x03);
	wrOV7670Reg(0x9d, 0x4c);
	wrOV7670Reg(0x9e, 0x3f);
	wrOV7670Reg(0x78, 0x04);



}


/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
