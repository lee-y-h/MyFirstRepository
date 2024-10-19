//
// Created by 90514 on 24-10-14.
//

#include "control.h"
#include "RC.h"


RC rc;

void controlinit() {
}

void controlloop() {
    rc.ReceiveData();
}