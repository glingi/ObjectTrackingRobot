 #include "main.h"
 
 void port_init()
 {
	 //not used
	 DDRA = 0X00;
	 //DC motor control
	 DDRC = _BV(PC0) | _BV(PC1) | _BV(PC2) | _BV(PC3);
	 //Servo motor : PWM(PD5,PD4) and DC motor PWM (PD7)
	 DDRD = _BV(PD4) | (1 << PD5) | (1 << PD7) ;
	 //DC motor PWM OUTPUT PORT
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
							if(get_servo_x_pos() >= 0) { 
								//로봇이 봤을때 물체는 오른쪽 그래서 오른쪽 모터 속도를 줄여서 회전이 가능하게함
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
