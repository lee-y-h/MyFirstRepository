//
// Created by 90514 on 24-10-12.
//
#include "Motor.h"
#include "stm32f4xx_hal.h"
#include "can.h"
#include "tim.h"
#include "callback.h"
#include "PID.h"

M2006_Motor motor1;
CAN_TxHeaderTypeDef can_Tx;
uint8_t sendBuf[8];
uint32_t box;
//PID
float d_angle = 0.0;
float ref_angle = 0.0;
float ref_speed = 0.0;
float pid_speed = 0.0;
uint8_t tx_data[2];
PID speed_pid(5,0,0,10,2457);
PID position_pid(0.5,0,0,10,400*36);

void controlInit() {

    //Filter
    CAN_FilterTypeDef FilterConfig;
    FilterConfig.FilterActivation = ENABLE;
    FilterConfig.FilterIdHigh = 0;
    FilterConfig.FilterIdLow = 0;
    FilterConfig.FilterMaskIdHigh = 0;
    FilterConfig.FilterMaskIdLow = 0;
    FilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    FilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    FilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
    FilterConfig.FilterBank = 0;

    //Transmit
    sendBuf[0] = 0;
    sendBuf[1] = 0;
    sendBuf[2] = 0;
    sendBuf[3] = 0;
    sendBuf[4] = 0;
    sendBuf[5] = 0;
    sendBuf[6] = 0;
    sendBuf[7] = 0;
    can_Tx.StdId = 0x200;
    can_Tx.IDE = CAN_ID_STD;
    can_Tx.RTR = CAN_RTR_DATA;
    can_Tx.DLC = 8;
    can_Tx.TransmitGlobalTime = DISABLE;

    HAL_CAN_ConfigFilter(&hcan1, &FilterConfig);
    HAL_CAN_Start(&hcan1);
    HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);

    HAL_TIM_Base_Start_IT(&htim1);
    HAL_TIM_Base_Start_IT(&htim6);
}

void controlLoop1() {

    motor1.canRxMsgCallback_v1(rx_buf);
    motor1.canRxMsgCallback_v2(rx_buf);
    motor1.canRxMsgCallback_v3(rx_buf);

    float angle = motor1.getAngle();
    if(d_angle!=0) {
        ref_angle = angle+d_angle;
        d_angle = 0;
    }

    ref_speed = position_pid.calc(ref_angle,angle);

    pid_speed = speed_pid.calc(ref_speed, motor1.getRotateSpeed());
    motor1.setRotateSpeed(pid_speed, tx_data);
    sendBuf[6] = tx_data[0];
    sendBuf[7] = tx_data[1];

    HAL_CAN_AddTxMessage(&hcan1, &can_Tx, sendBuf, &box);

}

void controlLoop2() {
    // HAL_CAN_AddTxMessage(&hcan1, &can_Tx, sendBuf, &box);
}