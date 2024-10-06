//
// Created by 90514 on 24-10-6.
//
#include <usart.h>

#include "main.h"

extern uint8_t buffer[];
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART6) {

        HAL_UART_Transmit(&huart6,buffer,1,10);
        HAL_UART_Receive_DMA(&huart6,buffer,256);
    }
}