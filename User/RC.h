//
// Created by 90514 on 24-10-14.
//

#ifndef RC_H
#define RC_H

#include <cstdint>


#define RC_RX_BUF_SIZE 18
#define RC_RX_DATA_SIZE 18

enum RCSwitchState_e{UP=1,DOWN,MIDDLE};

class RC{
private:
    uint8_t rx_buf_[RC_RX_BUF_SIZE];
    uint8_t rx_data_[RC_RX_DATA_SIZE];

public:
    struct RCChannel{
        float r_row;
        float r_col;
        float l_row;
        float l_col;
    } channel_;
    struct RCSwitch {
        RCSwitchState_e l;
        RCSwitchState_e r;
    } switch_;

    RC();
    float Mapping(uint16_t data);
    void ReceiveData();
    void SaveData();
    void ProcessData();
};

#endif //RC_H
