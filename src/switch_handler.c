#include "switch_handler.h"

void handle(uint8_t pin)
{    
    //assume there is a bounce window and skip past it
    for (volatile uint32_t i = 0; i < debounce_delay_loops; i++){}

    switch (pin)
    {
    case 10:
        permited_directions[MOTOR_X_FORWARD] = GPIO_Read(GPIOA, GPIO_PIN_10);
        //stop motor x
        break;
    case 8:
        permited_directions[MOTOR_X_BACKWARD] = GPIO_Read(GPIOA, GPIO_PIN_8);
        //stop motor x
        break;
    case 9:
        permited_directions[MOTOR_Y_FORWARD] = GPIO_Read(GPIOA, GPIO_PIN_9);
        //stop motor y
        break;
    case 4:
        permited_directions[MOTOR_Y_BACKWARD] = GPIO_Read(GPIOB, GPIO_PIN_4);
        //stop motor y
        break;
    default:
        break;
    }
}