#ifndef SWITCH_HANDLER
#define SWITCH_HANDLER

#include "stdint.h"
#include "stdbool.h"
#include "xnucleoihm02a1_interface.h"
#include "GPIO.h"
#include "motor_control.h"

void handle(uint8_t pin);
bool handle_on_axis(AXIS_DIRECTION axis);

#endif