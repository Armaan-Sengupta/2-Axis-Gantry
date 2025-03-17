#include "gpio.h"

void GPIO_Init(GPIO_TypeDef *GPIOx, uint16_t pin, uint32_t mode, uint32_t pull, uint32_t speed)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = pin;
    GPIO_InitStruct.Mode = mode;
    GPIO_InitStruct.Pull = pull;
    GPIO_InitStruct.Speed = speed;
    HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

void GPIO_Write(GPIO_TypeDef *GPIOx, uint16_t pin, bool state)
{
    HAL_GPIO_WritePin(GPIOx, pin, (GPIO_PinState)state);
}

bool GPIO_Read(GPIO_TypeDef *GPIOx, uint16_t pin)
{
    if (HAL_GPIO_ReadPin(GPIOx, pin) == GPIO_PIN_SET) return true;
    else return false;
}

void GPIO_Toggle(GPIO_TypeDef *GPIOx, uint16_t pin)
{
    HAL_GPIO_TogglePin(GPIOx, pin);
}
