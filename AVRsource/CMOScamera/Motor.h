#ifndef INCFILE1_H_
#define INCFILE1_H_

#include "CommonDefine.h"

/*
	PB3(OC0) = PWM1(5번 )
	PC0 = ENABLE 1 (6번 )
	PC1 = DIR 1	   (7번 ) 
	
	PD7(OC2) = PWM2
	PC2 = ENABLE 2
	PC3 = DIR 2
	
	OCR0 -> 로봇뒤에서 봤을때 오른쪽모터
	OCR2 -> 왼쪽모터
*/

//PWM 제어
//COUNTER0 ->COMPARE MATCH OUPUT PORT = PB3(OC0) (PIN4)
//COUNTER2 ->COMPARE MATCH OUPUT PORT = PD7(OC2) (PIN21) 


volatile uchar left_dir;
volatile uchar right_dir;

void motorinit()
{
	// setup_timer0

	TCCR0 = 0b01100001; 
	//Bit 6,3 –WGM0[1:0]: Waveform Generation Mode : phase correct PWM mode 
	//Bit 5:4 – COM0[1:0]: Compare Match Output Mode
	//Clear OC1a on compare match when upcounting.
	//Set OC1a on compare match when downcounting.
	//Bit 2:0 – CS02:0: Clock Select
	//NO prescaling
	//OCR0 = 128;
	//TIMSK |= 0b00000010;
	// Bit 1 – OCIE0: Timer/Counter0 Output Compare Match Interrupt Enable
	
	//SETUP_8BIT TIMER2
	TCCR2 = 0b01100001; 
	TIMSK = 0X00;
}

void set_dcmotor_dir(uchar right, uchar left)
{
	switch(left){
		case CW :
			left_dir = CW;
			ENABLE_LEFT;
			LEFT_DIR_FOR;
			break;
		case CCW:
			left_dir = CCW;
			ENABLE_LEFT;
			LEFT_DIR_BACK;
			break;
		case STAY:
			left_dir = STAY;
			DISABLE_LEFT;
			break;
		default:	
			break;
	}
	
	switch(right){
		case CW :
			right_dir = CW;
			ENABLE_RIGHT;
			RIGHT_DIR_FOR;
			break;
		case CCW:
			right_dir = CCW;
			ENABLE_RIGHT;
			RIGHT_DIR_BACK;
			break;
		case STAY:
			right_dir = STAY;
			DISABLE_RIGHT;
			break;
		default:	
			break;
	}			
}
	
void set_dcmotor_speed(uchar right, uchar left)
{
	uchar max_velo = 0X50; //모터전원이 13V시에
	
	//갑작스런 움직임을 방지하기위해
	if( right < max_velo) right =max_velo;
	if( left < max_velo) left =max_velo; 

	switch(right_dir)
	{
		case CW:
			OCR0 = right;
			break;
		case CCW:
		    //앞 뒤 회전 속도가 달라서 OFFSET항을 추가함
			OCR0 = 255-right+OFFSET;
			break;
		case STAY:
			OCR0 = 255;
			break;
		default:
			break;
	}
	
	switch(left_dir)
	{
		case CW:
			OCR2 = left;
			break;
		case CCW:
			OCR2 = 255-left+OFFSET ;
			break;
		case STAY:
			OCR2 = 255;
			break;
		default:
			break;
	}
}

void go(uchar right, uchar left)
{
	set_dcmotor_dir(CW,CW);
	set_dcmotor_speed(right,left);
}

void back(uchar right, uchar left)
{
	set_dcmotor_dir(CCW,CCW);
	set_dcmotor_speed(right,left);
}

void stop()
{
	set_dcmotor_dir(STAY,STAY);
	set_dcmotor_speed(STAY,STAY);
}

#endif /* INCFILE1_H_ */