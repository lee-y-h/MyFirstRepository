//
// Created by 90514 on 24-10-12.
//

#ifndef CALLBACK_H
#define CALLBACK_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx_hal.h"

extern CAN_RxHeaderTypeDef can_Rx;
extern uint8_t rx_buf[8];

#ifdef __cplusplus
}
#endif

#endif //CALLBACK_H
