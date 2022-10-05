#ifndef _DRIVER_MOTOR_H_
#define _DRIVER_MOTOR_H_



#include "inc.h"



void digital_write_motor_pins_reset();
uint8_t motor_move_left(uint16_t);
uint8_t motor_move_right(uint16_t);
void motor_set_resolution(uint8_t);
void motor_pins_irq_configure();
int is_motor_left_limit_hit();
int is_motor_right_limit_hit();



#endif
