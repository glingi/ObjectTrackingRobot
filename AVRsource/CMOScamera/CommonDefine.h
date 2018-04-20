
#ifndef CommonDefine_h__
#define CommonDefine_h__

////////////////////////////////////////////////////////////
// 사용자 자료형 정의
////////////////////////////////////////////////////////////

typedef unsigned char uchar ;
typedef unsigned int uint;

////////////////////////////////////////////////////////////
// USART 
////////////////////////////////////////////////////////////

#define SYSCLK 16 // 16Mhz
// Size of receive ring buffer. Must be at least 2.
#define USART_BUFFER_SIZE 10

/* Baudrate settings. Refer to datasheet for baud rate error.
   Note also maximun baud rate.
   br = baudrate, fosc = clock frequency in megahertzs */
#define USART_BAUDRATE(br, fosc) (fosc*125000/br-1)

////////////////////////////////////////////////////////////
// TIMER/COUNTER 
////////////////////////////////////////////////////////////

#define PWM1_ON			(DDRB  |=   1 << PB3) //TIMER,COUNTER 0
//#define	PWM1_OFF		(DDRB  &= ~(1 << PB3))
#define ENABLE_RIGHT	(PORTC |=   1 << PC0 )
#define DISABLE_RIGHT	(PORTC &= ~(1 << PC0))
#define RIGHT_DIR_FOR	(PORTC |=	1 << PC1 )
#define RIGHT_DIR_BACK	(PORTC &= ~(1 << PC1))
 
#define PWM2_ON			(DDRD  |=   1 << PD7 ) //TIMER,COUNTER2
//#define	PWM2_OFF		(DDRD  &= ~(1 << PD7)) 
#define ENABLE_LEFT		(PORTC |=   1 << PC2 )
#define DISABLE_LEFT	(PORTC &= ~(1 << PC2))
#define LEFT_DIR_FOR	(PORTC |=   1 << PC3 )
#define LEFT_DIR_BACK	(PORTC &= ~(1 << PC3))

//역회전 OFFSET값
#define OFFSET	0X0A

//모터 방향
enum {
	CW,
	CCW,
	STAY
};

//////////////////////////////////////////////////////////////////////////
// MAIN.C
//////////////////////////////////////////////////////////////////////////

/************************************************************************/
/*			입력 프레임 좌표값
/*
/*        (0,0)     (320,0)
/*           ----------                                                                      
/*           |        |
/*           |        |
/*			 ----------
/*		  (0,240)	(320,240)
/************************************************************************/

#define IMAGE_WIDTH				320
#define IMAGE_HEIGHT			240
#define midx			(IMAGE_WIDTH/2)
#define midy			(IMAGE_HEIGHT/2)

//중심범위 설정 변수
#define range 15

//x,y축 서보모터의 회전 속도 조절 변수
//증가 -> 느려짐 //  감소 -> 빨라짐
#define x_axis_factor			2.7
#define y_axis_factor			2.5	

//DC 모터의 속도 조절 변수
//DC모터의 속도는 입력전압에 비례하므로 입력 전압을 일정하게 해줘야함.
#define motor_factor			10		
#endif // CommonDefine_h__



////////////////////////////////////////////////////////////
// ultrasonic sensor(사용안함)
////////////////////////////////////////////////////////////
// 
// #define TRIGGER_HIGH	PORTC |= (1<<PC4)
// #define TRIGGER_LOW		PORTC &= ~(1<<PC4)
// #define IsECHOup		bit_is_set(PINC,5)
// #define IsECHOdown		bit_is_clear(PINC,5)
// 
// #define sound_velocity  34000 //340m/s =34000cm/s
// #define TRUE	1
// #define FALSE	0
