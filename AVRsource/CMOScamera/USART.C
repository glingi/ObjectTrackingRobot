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

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include "usart.h"

char usart_buffer[USART_BUFFER_SIZE];
volatile uchar usart_buffer_pos_first = 0, usart_buffer_pos_last = 0;
volatile uchar usart_buffer_overflow = 0;

void usart_init(uchar baud_divider) {

  // Baud rate selection
  UBRRH = 0x00;       
  UBRRL = baud_divider;
  // USART setup
  UCSRA = 0x02;        // 0000 0010
                       // U2X enabled :  Double the USART Transmission Speed
  UCSRC = 0x86;        // 0000 0110
                       // Access UCSRC, Asyncronous 8N1
  UCSRB = 0x98;        // 1001 1000
                       // Receiver enabled, Transmitter enabled
                       // RX Complete interrupt enabled
  sei();               // Enable interrupts globally
}

void usart_putc(char data) {

    while (!(UCSRA & 0x20)); // Wait untill USART data register is empty
    // Transmit data
    UDR = data;
}

void usart_puts(char *data) /* 문자열을 pc로 송신한다. */ 
{ 	
	int len, count;
  
	len = strlen(data);
  	for (count = 0; count < len; count++) 
    usart_putc(*(data+count)); // 배열을 지정하는 포인터의 1 증가는 그 포인터가 가리키는 자료형만큼의 크기와 같다.
}

uchar hex2asc(uchar num) /* 헥사값을 아스키코드로 바꾼다. */
{
    if(num>=10) return(num+'A'-10); else return(num+'0');
    /*num이 10보다 크면 알파벳이다. num=0x0A이면 0x0A+0x41-0x0A=0x41 이는 아스키 코드로 'A'를 나타낸다. 
    알파벳이 아닌 숫자의 경우 num=3이면 3+0x30=0x33 이는 아스키 코드로 '3'을 나타낸다*/
}


uchar asc2hex(uchar asc) /* 아스키코드를 헥사값으로 바꾼다. */
{
    if ((asc >= '0') && (asc <='9')) return (asc - '0'); 
    /*예를들어 asc가 '2'라면 16진수값은0x30 '0'의 16진수값은 0x32이므로 0x32-0x30 16진수 0x02가 된다.*/
    else if ((asc >= 'A') && (asc <='F')) return ((asc - 'A')+10); 
    /*예를들어 asc가 'C'라면 16진수값은0x43 'A'의 16진수값은 0x41이므로 0x43-0x41+0x0A은 십육진수 0x0C가 된다.*/
    else if ((asc >= 'a') && (asc <='f')) return ((asc - 'a')+10);
    else return(0xff); /*        */
}



void usart_puthex(uchar num) /* 1바이트 헥사값을 2바이트의 아스키코드로 변환해서 pc로 송신한다. */
{
    usart_putc(hex2asc(num>>4)); usart_putc(hex2asc(num&0x0f));
    /*num=0x8A라면  오른쪽으로 4쉬프트한 0x08 을 숫자 8에 해당하는 아스키 코드 '8'로 바꿔서 pc로 송신하고
      num의 하위 4비트인 0x0A를	받게된다*/
}

void usart_puthex_long(uint num)/*16bit 인 int형 변수를 4bit씩 나누고 아스키 코드로 바꿔서 pc로 전송*/
{
    uchar i, j;/*i는 num의 하위 8bit, j는 상위 8bit*/
    i = num;
    j = num >> 8;
    usart_putc(hex2asc(j>>4)); usart_putc(hex2asc(j&0x0f));/*j의 상위 4bit 전송// 하위 4bit 전송*/
    usart_putc(hex2asc(i>>4)); usart_putc(hex2asc(i&0x0f)); 
}  

char usart_getc(void) {

  // Wait untill unread data in ring buffer
  if (!usart_buffer_overflow)
    while(usart_buffer_pos_first == usart_buffer_pos_last);
  		usart_buffer_overflow = 0;

  // Increase first pointer
  if (++usart_buffer_pos_first >= USART_BUFFER_SIZE) 
    usart_buffer_pos_first = 0;

  // Get data from the buffer
  return usart_buffer[usart_buffer_pos_first];
}



uchar usart_unread_data(void) {

  if (usart_buffer_overflow) 
    return USART_BUFFER_SIZE;

  if (usart_buffer_pos_last > usart_buffer_pos_first)
    return usart_buffer_pos_last - usart_buffer_pos_first;

  if (usart_buffer_pos_last < usart_buffer_pos_first)
    return USART_BUFFER_SIZE-usart_buffer_pos_first 
      + usart_buffer_pos_last;

  return 0;
}



SIGNAL(SIG_UART_RECV) {

  // Increase last buffer 
  if (++usart_buffer_pos_last >= USART_BUFFER_SIZE)
    usart_buffer_pos_last = 0;

  if (usart_buffer_pos_first == usart_buffer_pos_last) 
    usart_buffer_overflow++;

  // Put data to the buffer
  usart_buffer[usart_buffer_pos_last] = UDR;
}
