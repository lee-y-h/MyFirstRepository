//
// Created by 90514 on 24-11-2.
//
#include "control.h"

uint8_t rx_data_G[6];
uint8_t tx_data[1] = {0x02};
uint8_t tx_data_init1 = 0xB6;
uint8_t tx_data_init2 = 0x04;
uint8_t rx_data_A[7];
uint8_t rx_data_A1[7];
uint8_t rx_data_A2[7];
float rx_G[3];
float rx_A[3];

void BMI088_ACCEL_NS_L() { HAL_GPIO_WritePin(CS_A_GPIO_Port, CS_A_Pin, GPIO_PIN_RESET); }

void BMI088_ACCEL_NS_H() { HAL_GPIO_WritePin(CS_A_GPIO_Port, CS_A_Pin, GPIO_PIN_SET); }

void BMI088_GYRO_NS_L() { HAL_GPIO_WritePin(CS_G_GPIO_Port, CS_G_Pin, GPIO_PIN_RESET); }

void BMI088_GYRO_NS_H() { HAL_GPIO_WritePin(CS_G_GPIO_Port, CS_G_Pin, GPIO_PIN_SET); }



void BMI088_read_from_G(uint8_t addr, uint8_t *return_data,uint16_t num) {
    uint8_t tx_buffer = addr | 0x80;
    HAL_SPI_Transmit(&hspi1,&tx_buffer,1,10);
    HAL_SPI_Receive(&hspi1,return_data,num,10);
}

void BMI088_read_from_A(uint8_t addr, uint8_t *return_data,uint16_t num) {
    uint8_t tx_buffer = addr | 0x80;
    HAL_SPI_Transmit(&hspi1,&tx_buffer,1,10);
    HAL_SPI_Receive(&hspi1,return_data,num+1,10);
}

void BMI088_write_to_addr(uint8_t addr, uint8_t *tx_data) {
    uint8_t tx_buffer = addr | 0x00;
    HAL_SPI_Transmit(&hspi1,&tx_buffer,1,10);
    HAL_SPI_Transmit(&hspi1,tx_data,1,10);
}

void controlInit() {
    BMI088_ACCEL_NS_H();
    BMI088_GYRO_NS_H();

    // Soft Reset ACCEL
    BMI088_ACCEL_NS_L();
    BMI088_write_to_addr(0x7E, &tx_data_init1); // Write 0xB6 to ACC_SOFTRESET(0x7E)
    HAL_Delay(1);
    BMI088_ACCEL_NS_H();

    // Soft Reset GYRO
    BMI088_GYRO_NS_L();
    BMI088_write_to_addr(0x14, &tx_data_init1); // Write 0xB6 to GYRO_SOFTRESET(0x14)
    HAL_Delay(30);
    BMI088_GYRO_NS_H();

    // Switch ACCEL to Normal Mode
    BMI088_ACCEL_NS_L();
    HAL_Delay(1);
    BMI088_write_to_addr(0x7D, &tx_data_init2); // Write 0x04 to ACC_PWR_CTRL(0x7D)
    HAL_Delay(1);
    BMI088_ACCEL_NS_H();
}

void map_G(uint8_t *rx,float *processed) {
    int16_t tmp = rx[1] << 8 | rx[0];
    processed[0] = static_cast<float>(tmp)/32767*2000;
    tmp = rx[3] << 8 | rx[2];
    processed[1] = static_cast<float>(tmp)/32767*2000;
    tmp = rx[5] << 8 | rx[4];
    processed[2] = static_cast<float>(tmp)/32767*2000;
}

void map_A(uint8_t *rx,float *processed) {
    int16_t tmp = rx[2] << 8 | rx[1];
    processed[0] = static_cast<float>(tmp)/32767*12;
    tmp = rx[4] << 8 | rx[3];
    processed[1] = static_cast<float>(tmp)/32767*12;
    tmp = rx[6] << 8 | rx[5];
    processed[2] = static_cast<float>(tmp)/32767*12;
}

void control1() {
    //读imu的0x00
    BMI088_GYRO_NS_L();
    BMI088_read_from_G(0x00,rx_data_G,1);
    BMI088_GYRO_NS_H();

    //写0x41 ACC_RANGE寄存器，设置为+-12g（0x02）
    BMI088_ACCEL_NS_L();
    HAL_Delay(1);
    BMI088_write_to_addr(0x41,tx_data);
    HAL_Delay(1);
    BMI088_ACCEL_NS_H();

    //读取该寄存器的值，看看是不是0x02
    BMI088_ACCEL_NS_L();
    HAL_Delay(1);
    BMI088_read_from_A(0x41,rx_data_A,1);
    HAL_Delay(1);
    BMI088_ACCEL_NS_H();
}

void control2() {
    BMI088_GYRO_NS_L();
    BMI088_read_from_G(0x02,rx_data_G,6);
    BMI088_GYRO_NS_H();
    map_G(rx_data_G,rx_G);

    BMI088_ACCEL_NS_L();
    HAL_Delay(1);
    BMI088_read_from_A(0x00,rx_data_A1,6);
    HAL_Delay(1);
    BMI088_ACCEL_NS_H();

    HAL_Delay(10);
    BMI088_ACCEL_NS_L();
    HAL_Delay(1);
    BMI088_read_from_A(0x12,rx_data_A2,6);
    HAL_Delay(1);
    BMI088_ACCEL_NS_H();
    map_A(rx_data_A2,rx_A);
}