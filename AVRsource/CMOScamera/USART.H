/***************************************************************************
 * Atmel AVR USART Library for GCC
 * Version: 1.0
 * 
 * Works with AVR MCUs equiped with USART hardware (ATmega series). 
 * Does not work with older UART's or USI of ATtiny series.
 * Tested with ATmega8.
 *
 * Uses USART Receive Complete Interrupt. Disabling Global Interrupts
 * after usart initialization disables data receive.
 *
 * Jaakko Ala-Paavola 2003/06/28
 * http://www.iki.fi/jap email:jap@iki.fi
 */

#ifndef USART_h__
#define USART_h__

#include "CommonDefine.h"


/* Initializes USART device. Use USART_BAUDRATE macro for argument or
   consult datasheet for correct value. */
void usart_init(uchar baud_divider);

/* Transmit one character. No buffering, waits until previous character
   transmitted. */
void usart_putc(char data);

/* Transmit string. Returns after whole string transmitted. */
void usart_puts(char *data);

/* Receive one character. Blocking operation, if no new data in buffer. */
char usart_getc(void);

/* Returns number of unread character in ring buffer. */
uchar usart_unread_data(void);

/* 헥사값을 아스키코드로 바꾼다. */
uchar hex2asc(uchar num); 

uchar asc2hex(uchar asc);

/* 1바이트 헥사값을 2바이트의 아스키코드로 변환해서 pc로 송신한다. */
void usart_puthex(uchar num); 

/*16bit 인 int형 변수를 4bit씩 나누고 아스키 코드로 바꿔서 pc로 전송*/
void usart_puthex_long(uint num);

#endif // USART_h__
