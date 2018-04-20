
/* Defines
 **********************************************************************/

#ifndef ULTRASONICSENSOR_H_
#define ULTRASONICSENSOR_H_

#define __OPTIMIZE__
#define F_CPU 16000000


typedef unsigned char BOOL;

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "USART.H"
#include "CommonDefine.h"

unsigned char get_echo(void);
BOOL pluse_transmission(void);
void ultrasoinc_timer_set_up();

#endif /* ULTRASONICSENSOR_H_ */