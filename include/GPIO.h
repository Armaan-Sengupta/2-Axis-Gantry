#ifndef GPIO_H
#define GPIO_H

#include "stm32f4xx_hal.h" // Include the HAL library
#include <stdbool.h>

void GPIO_Init(GPIO_TypeDef *GPIOx, uint16_t pin, uint32_t mode, uint32_t pull, uint32_t speed);
void GPIO_Write(GPIO_TypeDef *GPIOx, uint16_t pin, bool state);
bool GPIO_Read(GPIO_TypeDef *GPIOx, uint16_t pin);
void GPIO_Toggle(GPIO_TypeDef *GPIOx, uint16_t pin);

#endif // GPIO_H
