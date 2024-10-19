//
// Created by 90514 on 24-10-14.
//

#include "callback.h"
#include "RC.h"
#include "usart.h"


extern RC rc;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1) {
        rc.SaveData();
        rc.ProcessData();
        rc.ReceiveData();
    }
}
