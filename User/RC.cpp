//
// Created by 90514 on 24-10-14.
//

#include "RC.h"

#include <usart.h>

RC::RC() {
    for(uint8_t i=0;i<18;i++) {
        rx_data_[i] = rx_data_[i] = 0;
    }
    channel_.l_col = 0;
    channel_.l_row = 0;
    channel_.r_col = 0;
    channel_.r_row = 0;
    switch_.l = MIDDLE;
    switch_.r = MIDDLE;
}

float RC::Mapping(uint16_t data) {
    return static_cast<float>(data-1024)/660;
}

void RC::SaveData() {
    for(uint8_t i=0;i<18;i++) {
        rx_data_[i] = rx_buf_[i];
    }
}

void RC::ReceiveData() {
    HAL_UART_Receive_DMA(&huart1, rx_buf_, 18);
}

void RC::ProcessData() {
    channel_.r_row = Mapping((rx_data_[0] | rx_data_[1] << 8) & 0x07FF);
    channel_.r_col = Mapping((rx_data_[1] >> 3 | rx_data_[2] << 5) & 0x07FF);
    channel_.l_row = Mapping((rx_data_[2] >> 6 | rx_data_[3] << 2 | rx_data_[4] << 10) & 0x07FF);
    channel_.l_col = Mapping((rx_data_[4] >> 1 | rx_data_[5] << 7) & 0x07FF);
    switch_.r = static_cast<RCSwitchState_e>((rx_data_[5] >> 4) & 0x3);
    switch_.l = static_cast<RCSwitchState_e>(rx_data_[5] >> 6);
}
