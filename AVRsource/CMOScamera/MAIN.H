
#ifndef MAIN_h__
#define MAIN_h__

#define __OPTIMIZE__
#define F_CPU 16000000

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "CommonDefine.h"
#include "Motor.h"
#include "usart.h"
#include "SERVO.H"

void port_init();

#endif // MAIN_h__


