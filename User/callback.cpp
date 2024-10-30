//
// Created by 90514 on 24-10-12.
//
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_tim.h"
#include "control.h"

CAN_RxHeaderTypeDef can_Rx;
uint8_t rx_buf[8];

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &can_Rx, rx_buf);


}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM1) {
        controlLoop1();
    }
    if (htim->Instance == TIM6) {
        controlLoop2();
    }
}