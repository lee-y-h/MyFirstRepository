//
// Created by 90514 on 24-10-26.
//
#include "PID.h"

PID::PID(float kp, float ki, float kd, float i_max, float out_max): kp_(kp), ki_(ki), kd_(kd), i_max_(i_max),
                                                                    out_max_(out_max) {
    ref_ = fdb_ = 0;
    err_ = err_sum_ = last_err_ = 0;
    pout_ = dout_ = iout_ = output_ = 0;
}

float PID::calc(float ref, float fdb) {
    //算err
    last_err_ = err_;
    err_ = ref - fdb;
    err_sum_ += err_;
    if (err_sum_ > i_max_ || err_sum_ < -i_max_) err_sum_ = i_max_;

    //算output
    pout_ = kp_ * err_;
    dout_ = kd_ * (err_ - last_err_);
    iout_ = ki_ * err_sum_;
    output_ = pout_ + dout_ + iout_;
    if (output_ > out_max_ || output_ < -out_max_) output_= out_max_;

    ref_ = ref/36;
    fdb_ = fdb/36;
    
    return output_;
}
