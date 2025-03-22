#include "switch_handler.h"

const uint32_t debounce_delay_loops = 800000;

void init_switch_directions(){
    permited_directions[MOTOR_X_FORWARD] = !GPIO_Read(GPIOA, GPIO_PIN_10);
    permited_directions[MOTOR_X_BACKWARD] = !GPIO_Read(GPIOA, GPIO_PIN_8);
    permited_directions[MOTOR_Y_FORWARD] = !GPIO_Read(GPIOA, GPIO_PIN_9);
    permited_directions[MOTOR_Y_BACKWARD] = !GPIO_Read(GPIOB, GPIO_PIN_4);;
}

bool check_axis_limit_switch(AXIS_DIRECTION axis){
    bool switch_state;
    switch (axis)
    {
    case MOTOR_X_FORWARD:
        switch_state = !GPIO_Read(GPIOA, GPIO_PIN_10);
        break;
        
    case MOTOR_X_BACKWARD:
        switch_state = !GPIO_Read(GPIOA, GPIO_PIN_8);
        break;

    case MOTOR_Y_FORWARD:
        switch_state = !GPIO_Read(GPIOA, GPIO_PIN_9);
        break;

    case MOTOR_Y_BACKWARD:
        switch_state = !GPIO_Read(GPIOB, GPIO_PIN_4);
        break;

    default:
        assert_param(0); //invalid axis
    }
    permited_directions[axis] = switch_state;
    return switch_state;
}

void handle(uint8_t pin)
{    
    //assume there is a bounce window and skip past it
    volatile uint32_t x = 0;
    for (volatile uint32_t i = 0; i < debounce_delay_loops; i++){
        asm("nop");
        x++; //trying to make sure loop is not optimized out
    }
    switch (pin)
    {
    case 10:
        if (check_axis_limit_switch(MOTOR_X_FORWARD)) stop_motor(MOTOR_X);
        break;
    case 8:
        if (check_axis_limit_switch(MOTOR_X_BACKWARD)) stop_motor(MOTOR_X);
        break;
    case 9:
        if (check_axis_limit_switch(MOTOR_Y_FORWARD)) stop_motor(MOTOR_Y);
        break;
    case 4:
        if (check_axis_limit_switch(MOTOR_Y_BACKWARD)) stop_motor(MOTOR_Y);
        break;
    default:
        break;
    }


}