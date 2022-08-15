/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dcmi.h"
#include "dma.h"
#include "fatfs.h"
#include "i2c.h"
#include "sdmmc.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "bmp.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define RxBuffer_MaxSize 256

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
FATFS fs; 
FIL fil;	


uint32_t byteswritten;                /* File write counts */
uint32_t bytesread;                   /* File read counts */
uint8_t wtext[] = "This is STM32 working with FatFs"; /* File write buffer */
uint8_t rtext[100];                     /* File read buffers */
char filename[] = "STM.txt";
FRESULT a[5];
uint8_t d=0x00,cd=0x8c; //d发送数据；cd地址，u地址
uint8_t collect[151]; //收集数据
uint32_t buffer[2*320*240/4] __attribute__((section(".ARM.__at_0x24000000")));
HAL_StatusTypeDef b;
uint16_t tmp=0;
char RxBuffer[RxBuffer_MaxSize];
uint8_t aRxBuffer;
uint8_t UART5_Rx_Cnt =0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void read_Data(uint8_t regadd,uint8_t *data);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	int i;
	
	BMP bmp;

  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM2_Init();
  MX_SDMMC1_SD_Init();
  MX_FATFS_Init();
  MX_DCMI_Init();
  MX_I2C1_Init();
  MX_UART5_Init();
  /* USER CODE BEGIN 2 */
	
//写8c寄存器
	HAL_I2C_Mem_Write(&hi2c1,0x42,0x8c,1,&d,1,1);
	
	HAL_I2C_Master_Transmit(&hi2c1,0x42,&cd,1,1);
	HAL_I2C_Master_Receive(&hi2c1,0x42,collect,1,1);
	

//写8c寄存器
  d=0x01;
	HAL_I2C_Mem_Write(&hi2c1,0x42,0x8c,1,&d,1,1);
	
	HAL_I2C_Master_Transmit(&hi2c1,0x42,&cd,1,1);
	HAL_I2C_Master_Receive(&hi2c1,0x42,collect,1,1);
	
	wrOV7670Reg(0x12,0x80);
	//延时函数
	i=10000;
	while(i>0)
	{
		i--;
		i++;
		i--;
		i++;
		i--;
		i++;
		i--;
		i++;
		i--;
		i++;
		i--;
		i++;
		i--;
		i++;
		i--;
		i++;
		i--;
		i++;
		i--;
		i++;
		i--;
		i++;
		i--;
		i++;
		i--;
	}
	set_OV7670reg();

	
	HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_15);
  HAL_DCMI_Start_DMA(&hdcmi,DCMI_MODE_SNAPSHOT,(uint32_t)&buffer[0],2*320*240/4);
	HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_15);
	
//  HAL_DMA_Start_IT(((&hdcmi)->DMA_Handle),0x48020028,(uint32_t)&buffer[0],1);
  a[0]=f_mount(&fs, "", 0);
	
	bmp.bitmapfileheader.bfType=0x4D42;
//	bmp.bitmapfileheader.bfType=0xFFFF;
	bmp.bitmapfileheader.bfSize=40+14+320*240*2;
//	bmp.bitmapfileheader.bfSize=0xFFFFFFFF;

	bmp.bitmapfileheader.bfReserved1=0;
	bmp.bitmapfileheader.bfReserved2=0;
	bmp.bitmapfileheader.bfOffBits=54;
	
	bmp.bitmapinfoheader.biSize=40;
	bmp.bitmapinfoheader.biWidth=320;
	bmp.bitmapinfoheader.biHeight=240;
	bmp.bitmapinfoheader.biPlanes=1;
	bmp.bitmapinfoheader.biBitCount=16;
	bmp.bitmapinfoheader.biCompression=0;
	bmp.bitmapinfoheader.biSizeImage=0;
	bmp.bitmapinfoheader.biXPelsPerMeter=0;
	bmp.bitmapinfoheader.biYPelsPerMeter=0;
	bmp.bitmapinfoheader.biClrUsed=0;
	bmp.bitmapinfoheader.biClrImportant=0;
	
	bmp.pixel=(unsigned short *)buffer;
	
  a[1]=f_open(&fil, "1.bmp", FA_CREATE_ALWAYS | FA_WRITE);
  a[2]=f_write(&fil, &bmp.bitmapfileheader, sizeof(bmp.bitmapfileheader), (void *)&byteswritten);
  a[2]=f_write(&fil, &bmp.bitmapinfoheader, sizeof(bmp.bitmapinfoheader), (void *)&byteswritten);
	
//	for(i=0;i<320*240;i++)
//	{
////		a[2]=f_write(&fil, &bmp.pixel[i], sizeof(bmp.pixel[i]), (void *)&byteswritten);
//		
//		if(i<=600 && i>=0)
//		{
//			tmp=0xf800;
//			f_write(&fil, &tmp, sizeof(tmp), (void *)&byteswritten);
//		}
//		
//		if(i<=32000 && i>600)
//		{
//			tmp=0x7e0;
//			f_write(&fil, &tmp, sizeof(tmp), (void *)&byteswritten);
//		}
//		
//		if(i>32000)
//		{
//		  tmp=0x1f;
//	  	f_write(&fil, &tmp, sizeof(tmp), (void *)&byteswritten);
//		}
//	}
	
	a[3]=f_close(&fil);
	
	f_open(&fil, "2.bmp", FA_CREATE_ALWAYS | FA_WRITE);
  f_write(&fil, &bmp.bitmapfileheader, sizeof(bmp.bitmapfileheader), (void *)&byteswritten);
  f_write(&fil, &bmp.bitmapinfoheader, sizeof(bmp.bitmapinfoheader), (void *)&byteswritten);
	for(i=0;i<320*240;i++)
	{
		f_write(&fil, &bmp.pixel[i], sizeof(bmp.pixel[i]), (void *)&byteswritten);
	}	
	f_close(&fil);

	

	a[1]=f_open(&fil,filename,FA_CREATE_ALWAYS | FA_WRITE);
  a[2]=f_write(&fil,&bmp.bitmapfileheader.bfType, sizeof(bmp.bitmapfileheader.bfType), (void *)&byteswritten);
  a[3]=f_close(&fil);

	
	HAL_UART_Receive_IT(&huart5,(uint8_t *)&aRxBuffer,1);

//	HAL_GPIO_WritePin(GPIOD,GPIO_PIN_7,GPIO_PIN_SET);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		if(tmp)
		{
//			HAL_UART_Transmit(&huart5, (uint8_t *)buffer,100,0xFFFF);
			HAL_DCMI_Stop(&hdcmi);
//			HAL_DCMI_Start_DMA(&hdcmi,DCMI_MODE_SNAPSHOT,(uint32_t)&buffer[0],2*320*240/4);
//			while(HAL_DCMI_GetState(&hdcmi)==HAL_DCMI_STATE_BUSY)
//			{}

			b=HAL_UART_Transmit(&huart5, (uint8_t *)buffer,60000,0xFFFF);
		  HAL_UART_Transmit(&huart5, (uint8_t *)&buffer[15000],60000,0xFFFF);
		  HAL_UART_Transmit(&huart5, (uint8_t *)&buffer[30000],33600,0xFFFF);
//			xxx=buffer[15000];
//			xxx=buffer[30000];
//			xxx=0xFFFF2211;
//			b=HAL_UART_Transmit(&huart5, (uint8_t *)&xxx,4,0xFFFF);	
//			xxx=0x33EEEEEE;
//			HAL_UART_Transmit(&huart5, (uint8_t *)&xxx,4,0xFFFF);
//			xxx=0xCCCCCCCC;
//			HAL_UART_Transmit(&huart5, (uint8_t *)&xxx,4,0xFFFF);	
      HAL_DCMI_Start_DMA(&hdcmi,DCMI_MODE_SNAPSHOT,(uint32_t)&buffer[0],2*320*240/4);
			
//			HAL_GPIO_WritePin(GPIOD,GPIO_PIN_7,GPIO_PIN_RESET);
			
			HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_15);
			i=30000;
			while(i>0)
			{
				i--;
				i++;
				i--;
				i++;
				i--;
				i++;
				i--;
				i++;
				i--;
				i++;
				i--;
				i++;
				i--;
				i++;
				i--;
				i++;
				i--;
				i++;
				i--;
				i++;
				i--;
				i++;
				i--;
				i++;
				i--;
			}
			HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_15);
			
			
			read_Data(0x3a, &collect[0]);
			read_Data(0x40, &collect[1]);
			read_Data(0x12, &collect[2]);
			read_Data(0x32, &collect[3]);
			read_Data(0x17, &collect[4]);
			read_Data(0x18, &collect[5]);
			read_Data(0x19, &collect[6]);
			read_Data(0x1a, &collect[7]);
			read_Data(0x03, &collect[8]);
			read_Data(0x0c, &collect[9]);
			read_Data(0x3e, &collect[10]);
			read_Data(0x70, &collect[11]);
			read_Data(0x71, &collect[12]);
			read_Data(0x72, &collect[13]);
			read_Data(0x73, &collect[14]);
			read_Data(0xa2, &collect[15]);
			read_Data(0x11, &collect[16]);
			
			read_Data(0x7a, &collect[17]);
			read_Data(0x7b, &collect[18]);
			read_Data(0x7c, &collect[19]);
			read_Data(0x7d, &collect[20]);
			read_Data(0x7e, &collect[21]);
			read_Data(0x7f, &collect[22]);
			read_Data(0x80, &collect[23]);
			read_Data(0x81, &collect[24]);
			read_Data(0x82, &collect[25]);
			read_Data(0x83, &collect[26]);
			read_Data(0x84, &collect[27]);
			read_Data(0x85, &collect[28]);
			read_Data(0x86, &collect[29]);
			read_Data(0x87, &collect[30]);
			read_Data(0x88, &collect[31]);
			read_Data(0x89, &collect[32]);

			read_Data(0x13, &collect[33]);
			read_Data(0x00, &collect[34]);
			
			read_Data(0x10, &collect[35]);
			read_Data(0x0d, &collect[36]);
			read_Data(0x14, &collect[37]);
			read_Data(0xa5, &collect[38]);
			read_Data(0xab, &collect[39]);
			read_Data(0x24, &collect[40]);
			read_Data(0x25, &collect[41]);
			read_Data(0x26, &collect[42]);
			read_Data(0x9f, &collect[43]);
			read_Data(0xa0, &collect[44]);
			read_Data(0xa1, &collect[45]);
			read_Data(0xa6, &collect[46]);
			read_Data(0xa7, &collect[47]);
			read_Data(0xa8, &collect[48]);
			read_Data(0xa9, &collect[49]);
			read_Data(0xaa, &collect[50]);
			read_Data(0x13, &collect[51]);

			read_Data(0x0e, &collect[52]);
			read_Data(0x0f, &collect[53]);
			read_Data(0x16, &collect[54]);
			read_Data(0x1e, &collect[55]);
			read_Data(0x21, &collect[56]);
			read_Data(0x22, &collect[57]);
			read_Data(0x29, &collect[58]);
			read_Data(0x33, &collect[59]);
			read_Data(0x35, &collect[60]);
			read_Data(0x37, &collect[61]);
			read_Data(0x38, &collect[62]);
			read_Data(0x39, &collect[63]);
			read_Data(0x3c, &collect[64]);
			read_Data(0x4d, &collect[65]);
			read_Data(0x4e, &collect[66]);
			read_Data(0x69, &collect[67]);
			read_Data(0x6b, &collect[68]);
			read_Data(0x74, &collect[69]);
			read_Data(0x8d, &collect[70]);
			read_Data(0x8e, &collect[71]);
			read_Data(0x8f, &collect[72]);
			read_Data(0x90, &collect[73]);
			read_Data(0x91, &collect[74]);
			read_Data(0x92, &collect[75]);
			read_Data(0x96, &collect[76]);
			read_Data(0x9a, &collect[77]);
			read_Data(0xb0, &collect[78]);
			read_Data(0xb1, &collect[79]);
			read_Data(0xb2, &collect[80]);
			read_Data(0xb3, &collect[81]);
			read_Data(0xb8, &collect[82]);



			read_Data(0x43, &collect[83]);
			read_Data(0x44, &collect[84]);
			read_Data(0x45, &collect[85]);
			read_Data(0x46, &collect[86]);
			read_Data(0x47, &collect[87]);
			read_Data(0x48, &collect[88]);
			read_Data(0x59, &collect[89]);
			read_Data(0x5a, &collect[90]);
			read_Data(0x5b, &collect[91]);
			read_Data(0x5c, &collect[92]);
			read_Data(0x5d, &collect[93]);
			read_Data(0x5e, &collect[94]);
			read_Data(0x64, &collect[95]);
			read_Data(0x65, &collect[96]);
			read_Data(0x66, &collect[97]);
			read_Data(0x94, &collect[98]);
			read_Data(0x95, &collect[99]);
			read_Data(0x6c, &collect[100]);
			read_Data(0x6d, &collect[101]);
			read_Data(0x6e, &collect[102]);
			read_Data(0x6f, &collect[103]);
			read_Data(0x6a, &collect[104]);
			read_Data(0x01, &collect[105]);
			read_Data(0x02, &collect[106]);
			read_Data(0x13, &collect[107]);
			read_Data(0x15, &collect[108]);
			
			
			read_Data(0x4f, &collect[109]);
			read_Data(0x50, &collect[110]);
			read_Data(0x51, &collect[111]);
			read_Data(0x52, &collect[112]);
			read_Data(0x53, &collect[113]);
			read_Data(0x54, &collect[114]);
			read_Data(0x58, &collect[115]);
			
			read_Data(0x41, &collect[116]);
			read_Data(0x3f, &collect[117]);
			read_Data(0x75, &collect[118]);
			read_Data(0x76, &collect[119]);
			read_Data(0x4c, &collect[120]);
			read_Data(0x77, &collect[121]);
			read_Data(0x3d, &collect[122]);
			read_Data(0x4b, &collect[123]);
			read_Data(0xc9, &collect[124]);
			read_Data(0x41, &collect[125]);
			read_Data(0x56, &collect[126]);
			
			read_Data(0x34, &collect[127]);
			read_Data(0x3b, &collect[128]);
										
			read_Data(0xa4, &collect[129]);
			read_Data(0x96, &collect[130]);
			read_Data(0x97, &collect[131]);
			read_Data(0x98, &collect[132]);
			read_Data(0x99, &collect[133]);
			read_Data(0x9a, &collect[134]);
			read_Data(0x9b, &collect[135]);
			read_Data(0x9c, &collect[136]);
			read_Data(0x9d, &collect[137]);
			read_Data(0x9e, &collect[138]);
			read_Data(0x78, &collect[139]);		
			
			
			
			tmp=0;
		}

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Supply configuration update enable 
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);
  /** Configure the main internal regulator output voltage 
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
  /** Macro to configure the PLL clock source 
  */
  __HAL_RCC_PLL_PLLSOURCE_CONFIG(RCC_PLLSOURCE_HSE);
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_UART5|RCC_PERIPHCLK_SDMMC
                              |RCC_PERIPHCLK_I2C1;
  PeriphClkInitStruct.PLL2.PLL2M = 1;
  PeriphClkInitStruct.PLL2.PLL2N = 40;
  PeriphClkInitStruct.PLL2.PLL2P = 2;
  PeriphClkInitStruct.PLL2.PLL2Q = 2;
  PeriphClkInitStruct.PLL2.PLL2R = 10;
  PeriphClkInitStruct.PLL2.PLL2RGE = RCC_PLL2VCIRANGE_3;
  PeriphClkInitStruct.PLL2.PLL2VCOSEL = RCC_PLL2VCOWIDE;
  PeriphClkInitStruct.PLL2.PLL2FRACN = 0;
  PeriphClkInitStruct.SdmmcClockSelection = RCC_SDMMCCLKSOURCE_PLL2;
  PeriphClkInitStruct.Usart234578ClockSelection = RCC_USART234578CLKSOURCE_D2PCLK1;
  PeriphClkInitStruct.I2c123ClockSelection = RCC_I2C123CLKSOURCE_D2PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_HSI48, RCC_MCODIV_6);
  /** Enables the Clock Security System 
  */
  HAL_RCC_EnableCSS();
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  /* Prevent unused argument(s) compilation warning */
  UNUSED(huart);
  /* NOTE: This function Should not be modified, when the callback is needed,
           the HAL_UART_TxCpltCallback could be implemented in the user file
   */
 
    if(UART5_Rx_Cnt >= 255)  //????
    {
        UART5_Rx_Cnt = 0;
        memset(RxBuffer,0x00,sizeof(RxBuffer));
        HAL_UART_Transmit(&huart5, (uint8_t *)"数据溢出", 10,0xFFFF);   
        
    }
    else
    {
        RxBuffer[UART5_Rx_Cnt++] = aRxBuffer;   //??????
    
        if((RxBuffer[UART5_Rx_Cnt-1] == 0x0A)&&(RxBuffer[UART5_Rx_Cnt-2] == 0x0D)) //?????
        {
					
//					HAL_UART_Transmit(&huart5, (uint8_t *)RxBuffer, UART5_Rx_Cnt,0xFFFF); //??**????**????????
					tmp=1;
					if(RxBuffer[0] != 0xFF)
					{
						wrOV7670Reg(RxBuffer[0], RxBuffer[1]);
					}
					while(HAL_UART_GetState(&huart5) == HAL_UART_STATE_BUSY_TX);//??UART????
					UART5_Rx_Cnt = 0;
					memset(RxBuffer,0x00,sizeof(RxBuffer)); //????
        }
		}
    HAL_UART_Receive_IT(&huart5, (uint8_t *)&aRxBuffer, 1);   //??????
}

void read_Data(uint8_t regadd,uint8_t *data)
{
	HAL_I2C_Master_Transmit(&hi2c1,0x42,&regadd,1,1);
	HAL_I2C_Master_Receive(&hi2c1,0x42,data,1,1);
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
