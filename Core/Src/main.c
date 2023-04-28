/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
// inicializar perifericos
#define MPU9255_ADDR 0xD0

#define SMPLRT_DIV_REG 0x19
#define ACCEL_CONFIG_REG 0x1C
#define ACCEL_XOUT_H_REG 0x3B
#define TEMP_OUT_H_REG 0x41
#define PWR_MGMT_1_REG 0x6B
#define WHO_AM_I_REG 0x75

//Datos para acelerometro
int16_t Accel_X_RAW = 0;
int16_t Accel_Y_RAW = 0;
int16_t Accel_Z_RAW = 0;

float Ax, Ay, Az;

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
uint8_t Buffer[25] = {0};
uint8_t Space[] = " - ";
uint8_t StartMSG[] = "Starting I2C Scanning: \r\n";
uint8_t EndMSG[] = "Done! \r\n\r\n";
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */

//-------------- CHEQUEO FUNCIONAL-------------------------------------------------------------

void MPU9255_Init(void)
{
	uint8_t check;
	uint8_t Data;

	// chequeo WHO_AM_I

	HAL_I2C_Mem_Read(&hi2c1, MPU9255_ADDR, WHO_AM_I_REG, 1, &check, 1, 1000);

	if (check == 104) // retorna 0x68 si esta todo ok
	{
		// 0X6B despierta el sensor
		Data = 0;
		HAL_I2C_Mem_Write(&hi2c1, MPU9255_ADDR, PWR_MGMT_1_REG, 1, &Data, 1, 1000);

		// Seteo DATA RATE a 1KHz - SMPLRT_DIV register
		Data = 0x07;
		HAL_I2C_Mem_Write(&hi2c1, MPU9255_ADDR, SMPLRT_DIV_REG, 1, &Data, 1, 1000);

		// Configuracion del ACCEL_CONFIG Register
		// XA_ST=0,YA_ST=0,ZA_ST=0, FS_SEL=0 -> � 2g
		Data = 0x00;
		HAL_I2C_Mem_Write(&hi2c1, MPU9255_ADDR, ACCEL_CONFIG_REG, 1, &Data, 1, 1000);

	}
}

void MPU9255_Read_Accel(void)
{
	uint8_t Rec_Data[6];

	// Read 6 BYTES of data starting from ACCEL_XOUT_H register

	HAL_I2C_Mem_Read(&hi2c1, MPU9255_ADDR, ACCEL_XOUT_H_REG, 1, Rec_Data, 6, 1000);

	Accel_X_RAW = (int16_t)(Rec_Data[0] << 8 | Rec_Data[1]);
	Accel_Y_RAW = (int16_t)(Rec_Data[2] << 8 | Rec_Data[3]);
	Accel_Z_RAW = (int16_t)(Rec_Data[4] << 8 | Rec_Data[5]);

	/*** convert the RAW values into acceleration in 'g'
       we have to divide according to the Full scale value set in FS_SEL
       I have configured FS_SEL = 0. So I am dividing by 16384.0****/

	Ax = Accel_X_RAW / 16384.0;
	Ay = Accel_Y_RAW / 16384.0;
	Az = Accel_Z_RAW / 16384.0;
}

int main(void)
{
	uint8_t i = 0, ret;
	HAL_Init();

	SystemClock_Config();

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_I2C1_Init();
	MX_USART3_UART_Init();

	// Inicializo I2c
	MPU9255_Init();

	//Identifica los perifericos existentes
	HAL_Delay(1000);

	/*-[ I2C Bus Scanning ]-*/
	HAL_UART_Transmit(&huart3, StartMSG, sizeof(StartMSG), 10000);
	uartSendString(StartMSG);
	for(i=1; i<128; i++)
	{
		ret = HAL_I2C_IsDeviceReady(&hi2c1, (uint16_t)(i<<1), 3, 5);
		if (ret != HAL_OK) /* No ACK Received At That Address */
		{
			HAL_UART_Transmit(&huart3, Space, sizeof(Space), 10000);
			uartSendString(Space);
		}
		else if(ret == HAL_OK)
		{
			sprintf(Buffer, "0x%X", i);
			HAL_UART_Transmit(&huart3, Buffer, sizeof(Buffer), 10000);
			uartSendString(Buffer);
		}
	}
	HAL_UART_Transmit(&huart3, EndMSG, sizeof(EndMSG), 10000);
	//iniciar debounce estado MEF
	//inicia estado MEF QUIETO
	debounceFSM_init();

	///
	while (1)
	{
		MPU9255_Read_Accel();
		uartSendString(" Medicion Accelerometro: ");

		//leer variable X accelerometro
		int varx = Ax;
		char int_str1[20];
		sprintf(int_str1, "%d", varx);
		uartSendString(int_str1);

		//leer variable Y accelerometro
		int vary = Ay;
		char int_str2[20];
		sprintf(int_str2, "%d", vary);
		uartSendString(int_str2);

		//leer variable Z accelerometro
		int varz = Az;
		char int_str3[20];
		sprintf(int_str3, "%d", varz);
		uartSendString(int_str3);

		//checkeo de dato transmitido I2C
		uint8_t check = 0;
		HAL_I2C_Mem_Read(&hi2c1, MPU9255_ADDR, WHO_AM_I_REG, 1, &check, 1, 1000);
		uartSendString(" prueba ");
		uartSendString(check);

		//actualiza el estado
		debounceFSM_update(Ax,Ay,Az);
	}
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	/** Configure the main internal regulator output voltage
	 */
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
	{
		Error_Handler();
	}
}


/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1)
	{
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
