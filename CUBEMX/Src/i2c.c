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
//功能：写OV7670寄存器
//返回：1-成功	0-失败
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
//	wrOV7670Reg(0x40, 0xd0);  //原来10，通用COM15，设置输出数据范围和输出格式
//	wrOV7670Reg(0x8c, 0x00);
//	wrOV7670Reg(0x12, 0x16); //输出格式QVGA,原设置为0x14
//	wrOV7670Reg(0x32, 0x80);
//	wrOV7670Reg(0x17, 0x16);
//	wrOV7670Reg(0x18, 0x04);
//	wrOV7670Reg(0x19, 0x02);
//	wrOV7670Reg(0x1a, 0x7b);//0x7a,  原来0x7b
//	wrOV7670Reg(0x03, 0x06);//0x0a,  原来0x06
//	wrOV7670Reg(0x0c, 0x04);//原来0x04	   com3,设置缩放，省电，模式等
//	wrOV7670Reg(0x3e, 0x00);//	原来0x00   com14，dcw和pclk缩放设置，以及分频等
//	wrOV7670Reg(0x70, 0x3a); //原来0x04
//	wrOV7670Reg(0x71, 0x35); //原来0x35
//	wrOV7670Reg(0x72, 0x11); //原来0x11
//	wrOV7670Reg(0x73, 0x00);//原来f0 已改这里改变很重要
//	wrOV7670Reg(0xa2, 0x00); //原来0x02
//	wrOV7670Reg(0x11, 0x80); //时钟频率，这里改为最大值，原来为0x81  （改变）
//	//wrOV7670Reg(0x15 , 0x31);
//	wrOV7670Reg(0x7a, 0x20); //原来0x20 寄存器7a--89为伽马曲线设置
//	wrOV7670Reg(0x7b, 0x1c); //原来0x1c
//	wrOV7670Reg(0x7c, 0x28); //原来0x28
//	wrOV7670Reg(0x7d, 0x3c); //原来0x3c
//	wrOV7670Reg(0x7e, 0x55); //原来0x55
//	wrOV7670Reg(0x7f, 0x68); //原来0x68
//	wrOV7670Reg(0x80, 0x76); //原来0x76
//	wrOV7670Reg(0x81, 0x80); //原来0x80
//	wrOV7670Reg(0x82, 0x88); //原来0x88
//	wrOV7670Reg(0x83, 0x8f); //原来0x8f
//	wrOV7670Reg(0x84, 0x96); //原来0x96
//	wrOV7670Reg(0x85, 0xa3); //原来0xa3
//	wrOV7670Reg(0x86, 0xaf); //原来0xaf
//	wrOV7670Reg(0x87, 0xc4); //原来0xc4
//	wrOV7670Reg(0x88, 0xd7); //原来0xd7
//	wrOV7670Reg(0x89, 0xe8); //原来0xe8 
//	 
//	wrOV7670Reg(0x32,0xb6);
//	
//	wrOV7670Reg(0x13, 0x67); //原来0xe0 com13 AGC,AWB,AEC使能控制	 （改变）
//	wrOV7670Reg(0x00, 0x00);//AGC //原来0x00
//	wrOV7670Reg(0x10, 0x00);//原来0x00  曝光值
//	wrOV7670Reg(0x0d, 0x00);//原来0x00	 COM4
//	wrOV7670Reg(0x14, 0x4e);//原来0x28, limit the max gain	自动增益设置 没变化
//	wrOV7670Reg(0xa5, 0x05); //原来0x05 50Hz bangding step limting
//	wrOV7670Reg(0xab, 0x07); //原来0x07 60Hz bangding step limting
//	wrOV7670Reg(0x24, 0x75); //原来0x75 agc/aec-稳定运行区域上限
//	wrOV7670Reg(0x25, 0x63); //原来0x63 agc/aec-稳定运行区域下限
//	wrOV7670Reg(0x26, 0xA5); //原来0xa5 agc/aec-快速运行区域
//	wrOV7670Reg(0x9f, 0x78); //原来0x78 基于直方图的aec/agc的控制1
//	wrOV7670Reg(0xa0, 0x68); //原来0x68 基于直方图的aec/agc的控制2
////	wrOV7670Reg(0xa1, 0x03);//0x0b,
//	wrOV7670Reg(0xa6, 0xdf);//0xd8, 原来0xdf 基于直方图的aec/agc的控制3
//	wrOV7670Reg(0xa7, 0xdf);//0xd8, 原来0xdf 基于直方图的aec/agc的控制4
//	wrOV7670Reg(0xa8, 0xf0); //原来0xf0  基于直方图的aec/agc的控制5
//	wrOV7670Reg(0xa9, 0x90);  //原来0x90  基于直方图的aec/agc的控制6
//	wrOV7670Reg(0xaa, 0x94);  //原来0x94  基于直方图的aec/agc的控制7
//	//wrOV7670Reg(0x13, 0xe5);  //原来0xe5
//	wrOV7670Reg(0x0e, 0x61);  //原来0x61 COM5
//	wrOV7670Reg(0x0f, 0x43);  //原来0x4b COM6
//	wrOV7670Reg(0x16, 0x02);  //原来0x02 保留
//	wrOV7670Reg(0x1e, 0x13);//0x07, 原来0x37 水平镜像/竖直翻转使能	 设置为01后发生翻转
//	wrOV7670Reg(0x21, 0x02);	//原来0x02  保留
//	wrOV7670Reg(0x22, 0x91);	//原来0x91	保留
//	wrOV7670Reg(0x29, 0x07);	//原来0x07	保留
//	wrOV7670Reg(0x33, 0x0b);	//原来0x0b	href控制，改为初始值80时改变
//	wrOV7670Reg(0x35, 0x0b);	//原来0xe0	保留
//	wrOV7670Reg(0x37, 0x3f);	//原来0x1d	adc控制
//	wrOV7670Reg(0x38, 0x01);	//原来0x71	adc和模拟共模控制
//	wrOV7670Reg(0x39, 0x00);	//原来0x2a	adc偏移控制
//	wrOV7670Reg(0x3c, 0x78);	//原来0x78	 COM12
//	wrOV7670Reg(0x4d, 0x40);	//原来0x40	 保留
//	wrOV7670Reg(0x4e, 0x20);	//原来0x20	 保留
//	wrOV7670Reg(0x69, 0x00);	//原来0x00	  固定增益控制
//	wrOV7670Reg(0x6b, 0x0A);   //PLL原来0x00	  pll控制	重要设置
//	wrOV7670Reg(0x74, 0x19);	//原来0x19	手动数字增益
//	wrOV7670Reg(0x8d, 0x4f);	//原来0x4f	保留
//	wrOV7670Reg(0x8e, 0x00);	//原来0x00	保留
//	wrOV7670Reg(0x8f, 0x00);	//原来0x00	保留
//	wrOV7670Reg(0x90, 0x00);	//原来0x00	保留
//	wrOV7670Reg(0x91, 0x00);	//原来0x00	保留
//	wrOV7670Reg(0x92, 0x00);   //0x19,//0x66	 原来0x00 空行低8位
//	wrOV7670Reg(0x96, 0x00);	//原来0x00	  保留
//	wrOV7670Reg(0x9a, 0x80);	//原来0x80	  保留
//	wrOV7670Reg(0xb0, 0x84);	//原来0xe0	  保留
//	wrOV7670Reg(0xb1, 0x0c);	//原来0x0c	 ablc设置
//	wrOV7670Reg(0xb2, 0x0e);	//原来0x0e	  保留
//	wrOV7670Reg(0xb3, 0x82);	//原来0x82	  ablc target
//	wrOV7670Reg(0xb8, 0x0a);	//原来0x0a	   保留
//	wrOV7670Reg(0x43, 0x14);	//原来0x14	  43-48为保留
//	wrOV7670Reg(0x44, 0xf0);	//原来0xf0
//	wrOV7670Reg(0x45, 0x34);	//原来0x34
//	wrOV7670Reg(0x46, 0x58);	//原来0x58
//	wrOV7670Reg(0x47, 0x28);	//原来0x28
//	wrOV7670Reg(0x48, 0x3a);	//原来0x3a
//	
//	wrOV7670Reg(0x59, 0x88);	//原来0x88	 51-5e保留
//	wrOV7670Reg(0x5a, 0x88);	//原来0x88
//	wrOV7670Reg(0x5b, 0x44);	//原来0x44
//	wrOV7670Reg(0x5c, 0x67);	//原来0x67
//	wrOV7670Reg(0x5d, 0x49);	//原来0x49
//	wrOV7670Reg(0x5e, 0x0e);	//原来0xe0
//	
//	wrOV7670Reg(0x64, 0x04);	//原来0x04	  64-66镜头补偿
//	wrOV7670Reg(0x65, 0x20);	//原来0x20
//	wrOV7670Reg(0x66, 0x05);	//原来0x05

//	wrOV7670Reg(0x94, 0x04);	//原来0x04	94-95镜头补偿
//	wrOV7670Reg(0x95, 0x08);	//原来0x08	 

//	wrOV7670Reg(0x6c, 0x0a);	//原来0x0a	6c-6fawb设置
//	wrOV7670Reg(0x6d, 0x55);	//原来0x55
//	wrOV7670Reg(0x6e, 0x11);	//原来0x11
//	wrOV7670Reg(0x6f, 0x9f);   //0x9e for advance AWB	  原来9f

//	wrOV7670Reg(0x6a, 0x40);	//原来0x40	g通道awb增益
//	wrOV7670Reg(0x01, 0x40);	//原来0x40	b通道awb增益控制
//	wrOV7670Reg(0x02, 0x40);	//原来0x40	r通道awb增益控制
//	
//	//wrOV7670Reg(0x13, 0xe7);	//原来0xe7
//	wrOV7670Reg(0x15, 0x00);   //00  cmos10  含有pclk输出选择
//	wrOV7670Reg(0x4f, 0x80);	//原来0x80	以下为色彩矩阵系数设置
//	wrOV7670Reg(0x50, 0x80);	//原来0x80
//	wrOV7670Reg(0x51, 0x00);	//原来0x00
//	wrOV7670Reg(0x52, 0x22);	//原来0x22
//	wrOV7670Reg(0x53, 0x5e);	//原来0x5e
//	wrOV7670Reg(0x54, 0x80);	//原来0x80
//	wrOV7670Reg(0x58, 0x9e);	//原来0x9e

//	wrOV7670Reg(0x41, 0x08);	//原来0x08	com16 以下为边缘设置
//	wrOV7670Reg(0x3f, 0x00);	//原来0x00  边缘增强调整
//	wrOV7670Reg(0x75, 0x05);	//原来0x05
//	wrOV7670Reg(0x76, 0xe1);	//原来0xe1

//	wrOV7670Reg(0x4c, 0x00);	//原来0x00  噪声抑制强度
//	wrOV7670Reg(0x77, 0x01);	//原来0x01	噪声去除偏移
//	
//	wrOV7670Reg(0x3d, 0xc2);	//0xc0,	   com13 输出设置
//	wrOV7670Reg(0x4b, 0x09);	//原来0x09 寄存器4b设置
//	wrOV7670Reg(0xc9, 0x60);	//原来0x60	 饱和度控制
//	//wrOV7670Reg(0x41, 0x38);	//原来0x38	  com16边缘设置设置
//	wrOV7670Reg(0x55, 0x00);
//	wrOV7670Reg(0x56, 0x40);//0x40,  change according to Jim's request	对比度控制
//	wrOV7670Reg(0x34, 0x11);	//原来0x11	  感光阵列参考电压控制
//	wrOV7670Reg(0x3b, 0x02);//0x00,//原来0x02,	  com11控制
//	wrOV7670Reg(0xa4, 0x89);//0x88, 原来89 nt控制  
//	
//	wrOV7670Reg(0x96, 0x00);	//原来0x00  以下96-9c保留
//	wrOV7670Reg(0x97, 0x30);	//原来0x30
//	wrOV7670Reg(0x98, 0x20);	//原来0x20
//	wrOV7670Reg(0x99, 0x30);	//原来0x30
//	wrOV7670Reg(0x9a, 0x84);	//原来0x84
//	wrOV7670Reg(0x9b, 0x29);	//原来0x29
//	wrOV7670Reg(0x9c, 0x03);	//原来0x03
//	wrOV7670Reg(0x9d, 0x4c);	//原来0x4c	50HZ条纹滤波的值
//	wrOV7670Reg(0x9e, 0x3f);	//原来0x3f	60HZ条纹滤波的值

//	wrOV7670Reg(0x09, 0x00);	//原来0x00 通用控制器com2
//	wrOV7670Reg(0x3b, 0xc2);//0x82,//0xc0,//原来0xc2,	//night mode 原来0xc2
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
