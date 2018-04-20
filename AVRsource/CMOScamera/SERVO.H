/***************************************************************************
 *
 * Servo
 *
 *
 ***************************************************************************/


#ifndef SERVO_h__
#define SERVO_h__

#include <avr/io.h>
#include "CommonDefine.h"

void servo_init(void);
void ports_init(void);
void set_servo_y_pos(int pos);
void set_servo_x_pos(int pos);
int get_servo_y_pos(void);
int get_servo_x_pos(void);
void init_servo_ports(void);

#endif // SERVO_h__
