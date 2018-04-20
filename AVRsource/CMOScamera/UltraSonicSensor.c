#include "UltraSonicSensor.h"


//////////////////////////////////////////////////////////////////////////
// global 
unsigned int breakcount =0;  //input capture를 이용하는 방법도 있음

/* FUNCTION : 
 *
 * 
 **********************************************************************/
unsigned char get_echo(void)
{			
	breakcount =0;
	while(IsECHOdown){			//wait for echo pin to go high
		breakcount++;
		if(breakcount >60000) { usart_puts("echodown\r");breakcount=0; break;}
	} 
	
	TCNT0 =0;
	while(IsECHOup){			 // wait for echo pin to go low
		breakcount++;
		if(breakcount >60000) { usart_puts("echoup\r");breakcount=0; break;}
	}	

	return TCNT0;
}        

BOOL pluse_transmission(void)
{
	TRIGGER_HIGH;
	_delay_us(10);
	TRIGGER_LOW;
				
	return TRUE;
}

void ultrasoinc_timer_set_up()
{
	cli();
	TCCR0 =  (1<< CS02); //prescaler 64 , normal mode
	//TCCR0 =  (1 << CS21 | 1<< CS20) ; 
	/*
	PRESCALER가 64인경우에는 OVERFLOW가 발생하지 않는다. .. 
	32인 경우에는 두 세번 발생한다.
*/
	sei();
}



