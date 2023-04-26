
#include <stdio.h>
#include "API_uart.h"

/* UART handler declaration */
//Se le coloca static para encapsulamiento
static UART_HandleTypeDef huart3;


//implementacion
void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

void uartSendString(uint8_t *pstring) {
	do {
		HAL_UART_Transmit(&huart3, pstring, 1, 0xFFFF);
		pstring++;
	} while (*pstring != '\0');
}

void uartSendStringSize(uint8_t *pstring, uint16_t size) {
	HAL_UART_Transmit(&huart3, pstring, size, 0xFFFF);
}

void uartReceiveStringSize(uint8_t *pstring, uint16_t size) {
	HAL_UART_Receive(&huart3, pstring, size, 0xFFFF);
}

PUTCHAR_PROTOTYPE {
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART3 and Loop until the end of transmission */
	HAL_UART_Transmit(&huart3, (uint8_t*) &ch, 1, 0xFFFF);

	return ch;
}


