 #include "main.h"
 
 void port_init()
 {
	 //not used
	 DDRA = 0X00;
	 //모터제어
	 DDRC = _BV(PC0) | _BV(PC1) | _BV(PC2) | _BV(PC3);
	 //서보모터의 PWM(PD5,PD4) 과 DC모터의 PWM (PD7)
	 DDRD = _BV(PD4) | (1 << PD5) | (1 << PD7) ;
	 //DC모터 PWM OUTPUT PORT
	 DDRB = (1 << PB3);
	
	 PORTA = 0X00;
	 PORTB = 0X00;
	 PORTC = 0X00;
	 PORTD = 0X00;
 }
 
  
 int main(void) 
 {
//////////////////////////////////////////////////////////////////////////
// DECLARE VARIABLE 
//////////////////////////////////////////////////////////////////////////		
	struct{
		uint x,y,z;
	}point;
	 
	uchar chData ,key = ' ';
	
	uchar nIndex = 0, nDataCount = 0;
	uchar szTemp[20]; //= (char *)malloc(20);

  	uchar help[] = "WELCOME TO AVR WORLD\n";

	uchar left_motor,right_motor,SS;
	uchar min_size ;
	uint mid_size,max_size;
//////////////////////////////////////////////////////////////////////////
// VARIABLE INITIATION
//////////////////////////////////////////////////////////////////////////	

	point.x = point.y =point.z =0;
	memset(szTemp,0,sizeof(szTemp));
	
	SS = 0XA0; //START SPEED;
	
	min_size = 20;
	mid_size = 100;
	max_size = 400;
	
//////////////////////////////////////////////////////////////////////////
// FUNCTION INITIATION
//////////////////////////////////////////////////////////////////////////	
	
	usart_init(USART_BAUDRATE(115200,SYSCLK));
	port_init();
	motorinit();
	servo_init();
	
	sei();

	usart_puts(help);
	
//////////////////////////////////////////////////////////////////////////
// Main Source
//////////////////////////////////////////////////////////////////////////
	
	while(1){
		if( usart_unread_data() ){
			chData = usart_getc();
			
			if(chData == 'B' || chData == 'X' || chData == 'Y' || chData == 'S'|| chData == 'E'){
				szTemp[nIndex] = 0 ;
				nIndex =0;

				if(nDataCount == 0) {	
					nDataCount++;
				} 		
				else if(nDataCount ==1){	 
					point.x = atoi(szTemp);  //X range 0~320
					nDataCount++;			
				}	
				else if(nDataCount == 2) {
					point.y = atoi(szTemp);  //Y range 0~240
					nDataCount++;
				}
				
				else if (nDataCount == 3){

					point.z = atoi(szTemp);
					nDataCount++;			
					
				}				
				else if (nDataCount == 4){
					
					if( point.z > min_size ){	
												
//////////////////////////////////////////////////////////////////////////
// Servo motor control              
//////////////////////////////////////////////////////////////////////////			

						//LEFT (x 0~320)
						if( ( midx + range ) < point.x){
							point.x = (point.x - (midx + range) )/ x_axis_factor;
							set_servo_x_pos(get_servo_x_pos() + point.x);
						}						
						//RIGHT
						else if(point.x <  (midx - range ) ){
							point.x = (( midx+ range) - point.x ) / x_axis_factor;
							set_servo_x_pos(get_servo_x_pos() - point.x);
						}else {}
							
						//DOWN
						if( (midy + range )< point.y  ){
							point.y = (point.y - (midy + range) ) / y_axis_factor;
							set_servo_y_pos(get_servo_y_pos() + point.y);
						}						
						//UP
						else if( point.y < (midy - range)){
							point.y = (( midy + range ) - point.y ) / y_axis_factor;
							set_servo_y_pos(get_servo_y_pos() - point.y);
						}else{;}

//////////////////////////////////////////////////////////////////////////
// DC motor control
//////////////////////////////////////////////////////////////////////////								
						if(point.z < mid_size)
						{
							
							right_motor = left_motor = 0X95;
							if(get_servo_x_pos() >= 0) {  //로봇이 봤을때 물체는 오른쪽 그래서 오른쪽 모터 속도를 줄여서 회전이 가능하게함
								right_motor = right_motor +  (get_servo_x_pos() / motor_factor ) ;
							}								
							else {// 그반대
								left_motor = left_motor -  ( get_servo_x_pos()  / motor_factor ) ;
							}							
							set_dcmotor_dir(CW,CW);
							set_dcmotor_speed(right_motor,left_motor);
 						
						}										
						else if(point.z > max_size){
							set_dcmotor_dir(CCW,CCW);
							set_dcmotor_speed(0xA0,0XA0);
						}							
						else stop();
							

 					}else{	stop();	}
					
					nDataCount=0;
				}					
								
			} else 	szTemp[nIndex++]= chData;
		}//  if(usart_unread_data()) end
	}	//while end
	return 0;
}


//////////////////////////////////////////////////////////////////////////
// SERVO MOTOR TEST SOURCE
//////////////////////////////////////////////////////////////////////////
/*
		if(usart_unread_data() )
		{
			chData = usart_getc();
			if(chData == 'A') {//DOWN
				set_servo_x_pos( get_servo_x_pos() + nIndex);
				set_servo_y_pos( get_servo_y_pos() + nIndex);	
			}				
			else if(chData == 'B'){// UP
				set_servo_x_pos( get_servo_x_pos() -nIndex);
				set_servo_y_pos( get_servo_y_pos() -nIndex);
			}				
			else if(chData == 'C'){
				set_servo_x_pos( 0 );
				set_servo_y_pos( 0 );
			}				
			else if(chData == '[')
				{
					nIndex++;
					usart_puthex(nIndex);
					usart_puts("\n");
				}		
			else if(chData == ']')
				{
					nIndex--;
					if(nIndex <=0) nIndex =0;
					usart_puthex(nIndex);
					usart_puts("\n");
				}			
		}		
*/
//////////////////////////////////////////////////////////////////////////
// DC MOTOR TEST SOURCE
//////////////////////////////////////////////////////////////////////////
/*
	if(usart_unread_data()){
		key = usart_getc();
		switch(key){
			case 'R':
				usart_puts("turn_right\r");
				turn_right(SS);
				break;
			case 'L':
				usart_puts("turn_left\r");
				turn_left(SS);
				break;
			case 'B':
				usart_puts("back\r");
				back(SS,SS);
				break;
			
			case 'G':
				usart_puts("go\r");
				go(SS,SS);
				break;
				
			case 'U':

				usart_puts("current speed : \r");
				SS = SS -2;
				usart_puthex(SS);
				usart_puts("\r");
				break;
			case 'D':
				usart_puts("current speed : \r");
				SS = SS +2;
				usart_puthex(SS);
				usart_puts("\r");
				break;
				
			case 'S':
				usart_puts("stop\r");
				stop();
				break;
		
			default:
				break;
		} // SWITCH END
		
	} //IF END
	
*/
//////////////////////////////////////////////////////////////////////////
// TIMER0 / TIMER2 TEST SOURCE2
//////////////////////////////////////////////////////////////////////////

/*
		if(usart_unread_data() )
		{
			chData = usart_getc();
			if(chData == 'D') //DOWN
			{
				set_dcmotor_speed( OCR0 + 1 , OCR2 +1);
				usart_puthex(OCR0);
				usart_puts("\n");
			}				
			else if(chData == 'U')// UP
			{	
				set_dcmotor_speed( OCR0 - 1, OCR2 -1);
				usart_puthex(OCR0);
				usart_puts("\n");
			}				
		}	
		
*/


//////////////////////////////////////////////////////////////////////////
// UltraSonic Sensor TEST not used
//////////////////////////////////////////////////////////////////////////
/*
			ultrasoinc_timer_set_up();
					
			pluse_transmission();
	//		d1 = (get_echo() * 0.0000005 * sound_velocity ) / 2;		<- timer1 
	//		생각한 거와 다름.. 0.000004에 6배..
												
			distance  = ( ( get_echo() * 0.000024 ) * sound_velocity ) / 2; // timer 0
			avg_distance = ( distance + d2 + d3 + d4 + d5 + d6 + d7 )/7;
			d2=distance;
			d3=d2;
			d4=d3;
			d5=d4;
			d6=d5;
			d7=d6;
					
			sprintf(string,"%d cm\r",avg_distance);		
			usart_puts(string);
			
			
			motorinit();
			set_dcmotor_speed(SPEED,SPEED);
			
			_delay_ms(100);
			if(avg_distance < 15){
				set_dcmotor_direction(CCW,CCW);
				usart_puts("BACK\r");
				
						
			}else if( 15 <= avg_distance && avg_distance <= 50){
						
				usart_puts("STOP\r");	
				set_dcmotor_direction(STAY,STAY);						
			}else if(avg_distance > 50){
				
				set_dcmotor_direction(CW,CW);
				usart_puts("GO\r");
						
			}


			//_delay_ms(100);
			
*/			