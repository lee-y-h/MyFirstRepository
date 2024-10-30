//
// Created by 90514 on 24-10-12.
//

#ifndef M3508_MOTOR_H
#define M3508_MOTOR_H

#include "stdint.h"

class M3508_Motor {
private:
	float ratio_;// 电机减速比
	float angle_;// deg 输出端累计转动角度
	float delta_angle_;// deg 输出端新转动的角度
	float ecd_angle_;// deg 当前电机编码器角度
	float last_ecd_angle_;  // deg 上次电机编码器角度
	float delta_ecd_angle_; // deg 编码器端新转动的角度
	float rotate_speed_;// dps 反馈转子转速
	float current_;// A   反馈转矩电流
	float temp_;// °C  反馈电机温度

public:
	//构造函数
	M3508_Motor() {
		ratio_ = static_cast<float>(3591) / 187;
		angle_ = 0, delta_angle_ = 0;
		ecd_angle_ = 0, last_ecd_angle_ = 0, delta_ecd_angle_ = 0;
		rotate_speed_ = 0;
		current_ = 0.78;
		temp_ = 25;
	}

	//线性映射函数
	float  linearMapping(int in, int in_min, int in_max, float out_min, float out_max) {
		return (out_max - out_min) / (in_max - in_min) * (in - in_min) + out_min;
	}

	//报文解析函数1
	void canRxMsgCallback_v1(uint8_t rx_data[8]) {

		//解析转子角度
		int temp_ecd_angle = rx_data[0];
		temp_ecd_angle <<= 8;
		temp_ecd_angle += rx_data[1];
		//将报文映射到角度
		ecd_angle_ = linearMapping(temp_ecd_angle, 0, 8191, 0, 360);

		//解析转子转速
		int temp_rotate_speed_ = rx_data[2];
		temp_rotate_speed_ <<= 8;
		temp_rotate_speed_ += rx_data[3];
		//将RPM转化为dps(乘6)
		rotate_speed_ = temp_rotate_speed_ *6;

		//解析转矩电流
		int temp_current_ = rx_data[4];
		temp_current_ <<= 8;
		temp_current_ += rx_data[5];
		current_ = temp_current_;

		//解析电机温度
		temp_ = rx_data[6];
	}

	//报文解析函数2
	void canRxMsgCallback_v2(uint8_t rx_data[8]) {
		if (ecd_angle_ - last_ecd_angle_ >= 0) {
			delta_ecd_angle_ = ecd_angle_ - last_ecd_angle_;
		}
		else {
			delta_ecd_angle_ = ecd_angle_ - last_ecd_angle_+360;
		}

	}

	//计算电机的输出轴角度
	void canRxMsgCallback_v3(uint8_t rx_data[8]) {
		if (delta_ecd_angle_> 0) {
			delta_angle_ = delta_ecd_angle_ * ratio_;
			angle_ += delta_angle_;
			last_ecd_angle_ = ecd_angle_;
		}
		if (angle_ > 360) angle_ -= 360;
	}

	float getRotateSpeed() const {//rpm
		return rotate_speed_/6;
	}
	void setRotateSpeed(float speed, uint8_t tx_data[2]) {//rpm
		int16_t tmp_speed = speed;
		tx_data[0] = (tmp_speed >> 8) & 0xFF;
		tx_data[1] = tmp_speed & 0xFF;
	}
};
#endif //M3508_MOTOR_H
