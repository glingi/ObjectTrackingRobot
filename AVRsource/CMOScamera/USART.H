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

/* ���簪�� �ƽ�Ű�ڵ�� �ٲ۴�. */
uchar hex2asc(uchar num); 

uchar asc2hex(uchar asc);

/* 1����Ʈ ���簪�� 2����Ʈ�� �ƽ�Ű�ڵ�� ��ȯ�ؼ� pc�� �۽��Ѵ�. */
void usart_puthex(uchar num); 

/*16bit �� int�� ������ 4bit�� ������ �ƽ�Ű �ڵ�� �ٲ㼭 pc�� ����*/
void usart_puthex_long(uint num);

#endif // USART_h__