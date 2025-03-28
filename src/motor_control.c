#include "motor_control.h"
#include <stdlib.h>
#include "switch_handler.h"

// [x forward, x backward, y forward, y backward]
bool permited_directions[4] = {true, true, true, true};


eL6470_DirId_t get_dir_id(int32_t speed){
    if (speed >= 0) return L6470_DIR_FWD_ID;
    else return L6470_DIR_REV_ID;
}

AXIS_DIRECTION get_axis_dir(enum MOTORS motor_id, int32_t speed){
    uint8_t direction_id = get_dir_id(speed);    
    if      (motor_id == MOTOR_X && direction_id == L6470_DIR_FWD_ID) return MOTOR_X_FORWARD;
    else if (motor_id == MOTOR_X && direction_id == L6470_DIR_REV_ID) return MOTOR_X_BACKWARD;
    else if (motor_id == MOTOR_Y && direction_id == L6470_DIR_FWD_ID) return MOTOR_Y_FORWARD;
    else if (motor_id == MOTOR_Y && direction_id == L6470_DIR_REV_ID) return MOTOR_Y_BACKWARD;

}

bool move_axis(enum MOTORS motor_id, int32_t speed){
    AXIS_DIRECTION motor_id_dir = get_axis_dir(motor_id, speed);
    check_axis_limit_switch(motor_id_dir);
    if (!permited_directions[motor_id_dir]){
        stop_motor(motor_id);
        return false;
    }

    L6470_Run(motor_id, get_dir_id(speed), abs(speed));
    return true;
}

void stop_motor(enum MOTORS motor_id){
    L6470_HardStop(motor_id);
}