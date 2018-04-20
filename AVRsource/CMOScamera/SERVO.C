/***************************************************************************
	COMPARE MATCH OUTPUT : PD5(OC1A) 19번핀
 */


#include "servo.h"

void servo_init(void) {

  set_servo_x_pos(0);
  set_servo_y_pos(0);
  
  ICR1 = 20000;  // 20 ms Period, (8*20,000)/16Mhz = 0.01 = 10ms 
				 // if you draw a timing diagram you can know the reason period is 20ms.
    
  TCCR1A = 0xA0; //1010 0000b ,,  OC1b disconnected;
				  //COMA1A1(8) / COM1A0(7) = 10
				  //Clear OC1a on compare match when upcounting.
				  //Set OC1a on compare match when downcounting.
				  //34 = 00 for compatibility
				  // Mode 8 PWM, Phase and Frequency Correct change ,TOP : ICR1
  TCCR1B = 0x12; //0001 0010b,, Mode 8 Preescaler 8;
  
} 

void set_servo_x_pos(int pos){  
	 //HS311 1500~1900usec -980~985
	 //OCR1B 증가 : 정면에서 봤을때 왼쪽        오른쪽
	 //      감소 : 오른쪽                      왼쪽
	if(pos <= -900) pos = -900;
	if (pos >= 900) pos = 900;  
	OCR1B =  pos + 1525; 
}

void set_servo_y_pos(int pos){
	
	 //HES288
	 // 증가 : 정면에서 봤을때 아래   왼쪽
	 // 감소 : 위                    오른쪽
	if(pos <= -700) pos = -700;
	if (pos >= 900) pos = 900; // 
	OCR1A = pos+1400; // 1420
}	

int get_servo_x_pos(void){
	
	return OCR1B-1525;
}

int get_servo_y_pos(void){
	
	return OCR1A-1400;
}
