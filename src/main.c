#include "example.h"
#include "example_usart.h"
#include "L6470.h"
#include "GPIO.h"

// #define MICROSTEPPING_MOTOR_EXAMPLE        //!< Uncomment to performe the standalone example
// #define MICROSTEPPING_MOTOR_USART_EXAMPLE //!< Uncomment to performe the USART example
#define ADC_EXAMPLE //!< Uncomment to performe the ADC example
#if ((defined(MICROSTEPPING_MOTOR_EXAMPLE)) && (defined(MICROSTEPPING_MOTOR_USART_EXAMPLE)))
#error "Please select an option only!"
#endif
#if (defined(MICROSTEPPING_MOTOR_USART_EXAMPLE) && (!defined(NUCLEO_USE_USART)))
#error "Please define "NUCLEO_USE_USART" in "stm32fxxx_x-nucleo-ihm02a1.h"!"
#endif

void init_switches()
{

  GPIO_Init(GPIOA, GPIO_PIN_10, GPIO_MODE_IT_RISING_FALLING, GPIO_PULLDOWN, GPIO_SPEED_FREQ_MEDIUM); //works
  GPIO_Init(GPIOA, GPIO_PIN_8, GPIO_MODE_IT_RISING_FALLING, GPIO_PULLDOWN, GPIO_SPEED_FREQ_MEDIUM);  //works
  GPIO_Init(GPIOA, GPIO_PIN_9, GPIO_MODE_IT_RISING_FALLING, GPIO_PULLDOWN, GPIO_SPEED_FREQ_MEDIUM);  //works
  GPIO_Init(GPIOB, GPIO_PIN_4, GPIO_MODE_IT_RISING_FALLING, GPIO_PULLDOWN, GPIO_SPEED_FREQ_MEDIUM); //works

  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);
}

int main(void)
{
  /* NUCLEO board initialization */
  NUCLEO_Board_Init();

  /* X-NUCLEO-IHM02A1 initialization */
  BSP_Init();
  init_switches();

#ifdef NUCLEO_USE_USART
  /* Transmit the initial message to the PC via UART */
  USART_TxWelcomeMessage();
  USART_Transmit(&huart2, " X-CUBE-SPN2 v1.0.0\n\r");
#endif

#if defined(ADC_EXAMPLE)

__HAL_RCC_GPIOB_CLK_ENABLE();
  GPIO_Init(GPIOB, GPIO_PIN_0, GPIO_MODE_ANALOG, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW); //PB 0 is ADC1_IN8 from page 40 of the datasheet
  
  volatile uint32_t adc_value = 0;
  /* Infinite loop */
  while (1){
    HAL_ADC_Start(&hadc1);  // Start conversion
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);  // Wait for conversion
    adc_value = HAL_ADC_GetValue(&hadc1);  // Get ADC value
  }

#elif defined(MICROSTEPPING_MOTOR_EXAMPLE)
  /* Perform a batch commands for X-NUCLEO-IHM02A1 */
  MicrosteppingMotor_Example_01();

  /* Infinite loop */
  while (1);
#elif defined(MICROSTEPPING_MOTOR_USART_EXAMPLE)
  /* Fill the L6470_DaisyChainMnemonic structure */
  Fill_L6470_DaisyChainMnemonic();

  /*Initialize the motor parameters */
  Motor_Param_Reg_Init();

  /* Infinite loop */
  while (1)
  {
    /* Check if any Application Command for L6470 has been entered by USART */
    USART_CheckAppCmd();
    // L6470_Run(0, L6470_DIR_FWD_ID, 5000);
    // L6470_Run(1, L6470_DIR_FWD_ID, 5000);
    // HAL_Delay(2000);
    // L6470_Run(0, L6470_DIR_REV_ID, 5000);
    // L6470_Run(1, L6470_DIR_REV_ID, 5000);
    // HAL_Delay(2000);
    // L6470_HardStop(0);
    // L6470_HardStop(1);
    // HAL_Delay(2000);

    // L6470_Move(0, L6470_DIR_FWD_ID, 10000);
    // L6470_Move(1, L6470_DIR_FWD_ID, 10000);
    // HAL_Delay(10000);
    // L6470_Move(0, L6470_DIR_REV_ID, 10000);
    // L6470_Move(1, L6470_DIR_REV_ID, 10000);
    // HAL_Delay(10000);
  }
#endif
}

#ifdef USE_FULL_ASSERT

/**
 * @brief Reports the name of the source file and the source line number
 * where the assert_param error has occurred.
 * @param file: pointer to the source file name
 * @param line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}

#endif
