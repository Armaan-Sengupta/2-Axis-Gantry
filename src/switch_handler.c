#include "switch_handler.h"

const uint32_t debounce_delay_loops = 50000;

void init_switch_directions(){
    permited_directions[MOTOR_X_FORWARD] = GPIO_Read(GPIOA, GPIO_PIN_10);
    permited_directions[MOTOR_X_BACKWARD] = GPIO_Read(GPIOA, GPIO_PIN_8);
    permited_directions[MOTOR_Y_FORWARD] = GPIO_Read(GPIOA, GPIO_PIN_9);
    permited_directions[MOTOR_Y_BACKWARD] = GPIO_Read(GPIOB, GPIO_PIN_4);;
}

void handle(uint8_t pin)
{    
    //assume there is a bounce window and skip past it
    for (volatile uint32_t i = 0; i < debounce_delay_loops; i++){}
    bool switch_state;
    switch (pin)
    {
    case 10:
        switch_state = GPIO_Read(GPIOA, GPIO_PIN_10);
        permited_directions[MOTOR_X_FORWARD] = switch_state;
        if (switch_state == false){
            stop_motor(MOTOR_X);
        }
        break;
    case 8:
        switch_state = GPIO_Read(GPIOA, GPIO_PIN_8);
        permited_directions[MOTOR_X_BACKWARD] = switch_state;
        if (switch_state == false){
            stop_motor(MOTOR_X);
        }
        break;
    case 9:
        switch_state = GPIO_Read(GPIOA, GPIO_PIN_9);
        permited_directions[MOTOR_Y_FORWARD] = switch_state;
        if (switch_state == false){
            stop_motor(MOTOR_Y);
        }
        break;
    case 4:
        switch_state = GPIO_Read(GPIOB, GPIO_PIN_4);
        permited_directions[MOTOR_Y_BACKWARD] = switch_state;
        if (switch_state == false){
            stop_motor(MOTOR_Y);
        }
        break;
    default:
        break;
    }


}