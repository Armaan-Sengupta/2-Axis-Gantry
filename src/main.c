#include "example.h"
#include "example_usart.h"
#include "L6470.h"
#include "GPIO.h"
#include <stdio.h>
#include "motor_control.h"

// #define MICROSTEPPING_MOTOR_EXAMPLE        //!< Uncomment to performe the standalone example
// #define MICROSTEPPING_MOTOR_USART_EXAMPLE //!< Uncomment to performe the USART example
// #define ADC_EXAMPLE //!< Uncomment to performe the ADC example
#define LIIMIT_SWITCHES_EXAMPLE //!< Uncomment to performe the Limit Switches example
// #define ARMAAN_MOTOR_EXAMPLE //! Uncomment to perform this test to see if motor wiring is correct
#if ((defined(MICROSTEPPING_MOTOR_EXAMPLE)) && (defined(MICROSTEPPING_MOTOR_USART_EXAMPLE)))
#error "Please select an option only!"
#endif
#if (defined(MICROSTEPPING_MOTOR_USART_EXAMPLE) && (!defined(NUCLEO_USE_USART)))
#error "Please define "NUCLEO_USE_USART" in "stm32fxxx_x-nucleo-ihm02a1.h"!"
#endif

void init_switches()
{

  GPIO_Init(GPIOA, GPIO_PIN_10, GPIO_MODE_IT_RISING_FALLING, GPIO_PULLUP, GPIO_SPEED_FREQ_MEDIUM); //works
  GPIO_Init(GPIOA, GPIO_PIN_8, GPIO_MODE_IT_RISING_FALLING, GPIO_PULLUP, GPIO_SPEED_FREQ_MEDIUM);  //works
  GPIO_Init(GPIOA, GPIO_PIN_9, GPIO_MODE_IT_RISING_FALLING, GPIO_PULLUP, GPIO_SPEED_FREQ_MEDIUM); //works

  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
  HAL_NVIC_SetPriority(EXTI4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);
}

uint32_t get_adc_value(uint32_t channel) {
  ADC_ChannelConfTypeDef sConfig = {0};
  

  sConfig.Channel = channel;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;

  HAL_ADC_ConfigChannel(&hadc1, &sConfig);  // This is what tells ADC which pin/channel to use

  HAL_ADC_Start(&hadc1);
  HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
  return HAL_ADC_GetValue(&hadc1);
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
  
  char output_str[100];
  uint32_t adc_value = 0;
  double voltage = 0.0;
  const double vref = 3.3;
  const double offset = 0.034;

  while (1)
  {
    adc_value = get_adc_value();
    voltage = (((float)adc_value / 4095.0) * vref)-offset;

    sprintf(output_str, ">ADC Value:%lu,Voltage:%.3f V\r\n", adc_value, voltage);
    HAL_UART_Transmit(&huart2, (uint8_t *)output_str, strlen(output_str), HAL_MAX_DELAY);

    HAL_Delay(500);
  }
#elif defined(LIIMIT_SWITCHES_EXAMPLE)

  /* Fill the L6470_DaisyChainMnemonic structure */
  Fill_L6470_DaisyChainMnemonic();

  /*Initialize the motor parameters */
  Motor_Param_Reg_Init();

  init_switches();
  init_switch_directions();

  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();

  GPIO_Init(GPIOB, GPIO_PIN_0, GPIO_MODE_ANALOG, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW); //PB 0 is ADC1_IN8 from page 40 of the datasheet
  GPIO_Init(GPIOC, GPIO_PIN_1, GPIO_MODE_ANALOG, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW); //PC 1 is ADC1_IN11


  char output_str[100];
  uint32_t adc_value_pot_one = 0;
  uint32_t adc_value_pot_two = 0;
  double voltage = 0.0;
  int32_t motor_speed_one = 0;
  int32_t motor_speed_two = 0;

  const double vref = 3.3;
  const uint32_t max_adc_value = 4095; // 12-bit ADC
  const uint32_t mid_adc_value = max_adc_value / 2; // Midpoint
  const int32_t max_motor_speed = 10000; // Max motor speed in steps per second
  const int32_t deadband_threshold = 1; // Adjust this value as needed

  while (1)
  {
    adc_value_pot_one = get_adc_value(ADC_CHANNEL_8);
    adc_value_pot_two = get_adc_value(ADC_CHANNEL_11);

    int32_t raw_motor_speed_one = (int32_t)((((int32_t)adc_value_pot_one) - ((int32_t)mid_adc_value)) * max_motor_speed / (double)mid_adc_value);
    int32_t raw_motor_speed_two = (int32_t)((((int32_t)adc_value_pot_two) - ((int32_t)mid_adc_value)) * max_motor_speed / (double)mid_adc_value);
 
    // Apply deadband
    if (abs(adc_value_pot_one - mid_adc_value) < deadband_threshold) {
        motor_speed_one = 0;
    } else {
        motor_speed_one = raw_motor_speed_one;
    }

    if (abs(adc_value_pot_two - mid_adc_value) < deadband_threshold) {
        motor_speed_two = 0;
    } else {
        motor_speed_two = raw_motor_speed_two;
    }

    move_axis(MOTOR_X, motor_speed_one); //motor x is base, forward in x is away from the motor
    move_axis(MOTOR_Y, motor_speed_two); //motor y is upper

    sprintf(output_str, ">ADC1:%lu,Speed1:%ld\r\n", adc_value_pot_one, motor_speed_one);
    sprintf(output_str + strlen(output_str), ">ADC2:%lu,Speed2:%ld\r\n", adc_value_pot_two, motor_speed_two);

    HAL_UART_Transmit(&huart2, (uint8_t *)output_str, strlen(output_str), HAL_MAX_DELAY);

    HAL_Delay(500);
  }

#elif defined(ARMAAN_MOTOR_EXAMPLE)
  /* Fill the L6470_DaisyChainMnemonic structure */
  Fill_L6470_DaisyChainMnemonic();

  /*Initialize the motor parameters */
  Motor_Param_Reg_Init();


  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  GPIO_Init(GPIOB, GPIO_PIN_0, GPIO_MODE_ANALOG, GPIO_NOPULL, GPIO_SPEED_FREQ_MEDIUM);
  GPIO_Init(GPIOC, GPIO_PIN_1, GPIO_MODE_ANALOG, GPIO_NOPULL, GPIO_SPEED_FREQ_MEDIUM);

  char output_str[100];
  volatile uint32_t adc_value_PB0 = 0;
  volatile uint32_t adc_value_PC1 = 0;
  uint32_t adc_mid_point = 7.5;
  uint32_t max_motor_speed = 10000;

  while(1)
  {

    adc_value_PB0 = (float)get_adc_value(ADC_CHANNEL_8);
    adc_value_PC1 = (float)get_adc_value(ADC_CHANNEL_11);
  


    sprintf(output_str, ">ADC B: %ld | ", adc_value_PB0);
    sprintf(output_str + strlen(output_str), "ADC C: %ld\n", adc_value_PC1);  
    
    HAL_UART_Transmit(&huart2, (uint8_t *)output_str, strlen(output_str), HAL_MAX_DELAY);
    


    if ((adc_value_PB0) > adc_mid_point) {
      L6470_Run(0, L6470_DIR_FWD_ID, 15000);
    } else {
      L6470_Run(0, L6470_DIR_REV_ID, 15000);
    }

    if ((adc_value_PC1) > adc_mid_point) {
      L6470_Run(1, L6470_DIR_FWD_ID, 15000);
    } else {
      L6470_Run(1, L6470_DIR_REV_ID, 15000);
    }



  }


  // Simple test to verify motor moves
  // L6470_Run(1, L6470_DIR_FWD_ID, 5000);
  // L6470_Run(0, L6470_DIR_FWD_ID, 5000);
  // HAL_Delay(3000);
  // L6470_HardStop(1);
  // L6470_HardStop(0);


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
