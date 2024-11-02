//
// Created by 90514 on 24-10-26.
//


#ifndef MOTOR_H
#define MOTOR_H

#include "stdint.h"

class Motor {
    float ratio_;// 电机减速比
    float angle_;// deg 输出端累计转动角度
    float delta_angle_;// deg 输出端新转动的角度
    float ecd_angle_;// deg 当前电机编码器角度
    float last_ecd_angle_;  // deg 上次电机编码器角度
    float delta_ecd_angle_; // deg 编码器端新转动的角度
    float rotate_speed_;// rpm 反馈转子转速
    float current_;// A   反馈转矩电流
    float temp_;// °C  反馈电机温度
    int16_t speed;
public:
    //构造函数
    Motor(float ratio = static_cast<float>(3591) / 187) {
        ratio_ = ratio;
        angle_ = 0, delta_angle_ = 0;
        ecd_angle_ = 0, last_ecd_angle_ = 0, delta_ecd_angle_ = 0;
        rotate_speed_ = 0;
        current_ = 0.78;
        temp_ = 25;
    }

    //解码
    //线性映射函数
    float  linearMapping(int in, int in_min, int in_max, float out_min, float out_max) {
        return (out_max - out_min) / static_cast<float>(in_max - in_min) * static_cast<float>(in - in_min) + out_min;
    }
    //报文解析函数1
    void canRxMsgCallback_v1(uint8_t rx_data[8]) {

        //解析转子角度
        uint16_t temp_ecd_angle = rx_data[0];
        temp_ecd_angle <<= 8;
        temp_ecd_angle += rx_data[1];
        //将报文映射到角度
        ecd_angle_ = linearMapping(temp_ecd_angle, 0, 8191, 0.0, 360.0);

        //解析转子转速
        int16_t temp_rotate_speed_ = rx_data[2] <<8 | rx_data[3];

        rotate_speed_ = static_cast<float>(temp_rotate_speed_)/ratio_;

        //解析转矩电流
        int16_t temp_current_ = rx_data[4];
        temp_current_ <<= 8;
        temp_current_ += rx_data[5];
        current_ = linearMapping(temp_current_, 0, 8191, 0.0, 360.0);

        //解析电机温度
        temp_ = rx_data[6];
    }
    //报文解析函数2
    void canRxMsgCallback_v2(uint8_t rx_data[8]) {
        if(ecd_angle_ - last_ecd_angle_<-180) {
            delta_ecd_angle_ = ecd_angle_ - last_ecd_angle_+360;
        }else if(ecd_angle_ - last_ecd_angle_>180) {
            delta_ecd_angle_ = ecd_angle_ - last_ecd_angle_-360;
        }else {
            delta_ecd_angle_ = ecd_angle_-last_ecd_angle_;
        }
    }
    //计算电机的输出轴角度
    void canRxMsgCallback_v3(uint8_t rx_data[8]) {
        delta_angle_ = delta_ecd_angle_;
        angle_ += delta_angle_;
        last_ecd_angle_ = ecd_angle_;
    }

    //pid位置
    float getAngle() const {//rpm
        return angle_;
    }

    //pid速度
    float getRotateSpeed() const {//rpm
        return rotate_speed_;
    }
    void setRotateSpeed(float speed, uint8_t tx_data[2]) {//rpm
        int16_t temp_speed = static_cast<int16_t>(speed);
        tx_data[0] = temp_speed >> 8 & 0xFF;
        tx_data[1] = temp_speed & 0xFF;
    }
};

class M3508_Motor: public Motor {
    public:
    M3508_Motor() :Motor(static_cast<float>(3591) / 187) {}
};

class M2006_Motor: public Motor {
public:
    M2006_Motor() :Motor(static_cast<float>(36)){}
};


#endif //MOTOR_H
