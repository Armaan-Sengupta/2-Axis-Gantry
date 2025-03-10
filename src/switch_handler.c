#include "switch_handler.h"

const uint32_t debounce_delay_loops = 50000;


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
        break;
    case 8:
        switch_state = GPIO_Read(GPIOA, GPIO_PIN_8);
        permited_directions[MOTOR_X_BACKWARD] = switch_state;
        break;
    case 9:
        switch_state = GPIO_Read(GPIOA, GPIO_PIN_9);
        permited_directions[MOTOR_Y_FORWARD] = switch_state;
        break;
    case 4:
        switch_state = GPIO_Read(GPIOB, GPIO_PIN_4);
        permited_directions[MOTOR_Y_BACKWARD] = switch_state;
        break;
    default:
        break;
    }

    if (switch_state == false){
        stop_motor(MOTOR_X);
    }
}