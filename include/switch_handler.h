#ifndef SWITCH_HANDLER
#define SWITCH_HANDLER

#include "stdint.h"
#include "stdbool.h"
#include "xnucleoihm02a1_interface.h"
#include "GPIO.h"
#include "motor_control.h"


const uint32_t debounce_delay_loops = 50000;

void handle(uint8_t pin);

#endif