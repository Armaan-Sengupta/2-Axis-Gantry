#include "motor_control.h"

// [x forward, x backward, y forward, y backward]
bool permited_directions[4] = {true, true, true, true};

uint8_t get_(AXIS_DIRECTION axis_dir);

eL6470_DirId_t get_dir_id(int32_t speed){
    if (speed >= 0) return L6470_DIR_FWD_ID;
    else return L6470_DIR_REV_ID;
}

AXIS_DIRECTION get_axis_dir(enum MOTORS motor_id, int32_t speed){
    uint8_t direction_id = get_dir(speed);    
    if      (motor_id == MOTOR_X && direction_id == L6470_DIR_FWD_ID) return MOTOR_X_FORWARD;
    else if (motor_id == MOTOR_X && direction_id == L6470_DIR_REV_ID) return MOTOR_X_BACKWARD;
    else if (motor_id == MOTOR_Y && direction_id == L6470_DIR_FWD_ID) return MOTOR_Y_FORWARD;
    else if (motor_id == MOTOR_Y && direction_id == L6470_DIR_REV_ID) return MOTOR_Y_BACKWARD;
}

bool move_axis(enum MOTORS motor_id, int32_t speed){
    uint8_t direction_id = get_dir(speed);
    AXIS_DIRECTION axis_dir = get_axis_dir(motor_id, speed);    

    if (!permited_directions[axis_dir]) return false;

    L6470_Run(motor_id, direction_id, speed);
    return true;
}