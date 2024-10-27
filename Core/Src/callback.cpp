//
// Created by 90514 on 24-10-14.
//


#include <can.h>
#include <stm32f4xx_hal_can.h>

extern uint8_t sendBuf[8];
extern CAN_TxHeaderTypeDef can_Tx;
extern uint32_t box;

extern CAN_RxHeaderTypeDef can_Rx;
extern uint8_t rx_buf[8];

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
    if (hcan == &hcan1) {
        HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &can_Rx, rx_buf);
        if(rx_buf[0] == 1) {
            HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_RESET);
        }else if(rx_buf[0] == 0) {
            HAL_GPIO_WritePin(LED_R_GPIO_Port, LED_R_Pin, GPIO_PIN_SET);
        }
    }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
    if(GPIO_Pin==KEY_Pin) {
        sendBuf[0] = sendBuf[0] ? 0 : 1;
        HAL_CAN_AddTxMessage(&hcan1, &can_Tx, sendBuf, &box);
    }
}