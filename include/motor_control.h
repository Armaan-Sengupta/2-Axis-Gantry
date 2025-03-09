#ifndef MOTOR_CONTROL
#define MOTOR_CONTROL

#include "stdbool.h"
#include "stdint.h"
#include "L6470.h"

extern permited_directions;

typedef enum{
    MOTOR_X_FORWARD,
    MOTOR_X_BACKWARD,
    MOTOR_Y_FORWARD,
    MOTOR_Y_BACKWARD,
}AXIS_DIRECTION;

enum MOTORS{
    MOTOR_X,
    MOTOR_Y
};

bool move_axis(AXIS_DIRECTION axis_dir, uint32_t speed);

void init_system(); //check for edge case of switch being pressed upon program startup, and set permited directions accordingly

#endif