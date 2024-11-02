//
// Created by 90514 on 24-11-2.
//

#ifndef CONTROL_H
#define CONTROL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "gpio.h"
#include "spi.h"
void BMI088_ACCEL_NS_L();

void BMI088_ACCEL_NS_H();

void BMI088_GYRO_NS_L();

void BMI088_GYRO_NS_H();

void controlInit();

void BMI088_read_from_G(uint8_t addr, uint8_t *return_data,uint16_t num);

void BMI088_read_from_A(uint8_t addr, uint8_t *return_data,uint16_t num);

void BMI088_write_to_addr(uint8_t addr, uint8_t *tx_data);

void map_G(uint8_t *rx,float *processed);

void map_A(uint8_t *rx,float *processed);

void control1();

void control2();


#ifdef __cplusplus
}
#endif

#endif //CONTROL_H
