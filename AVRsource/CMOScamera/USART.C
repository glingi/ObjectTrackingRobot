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

void usart_puts(char *data) /* ���ڿ��� pc�� �۽��Ѵ�. */ 
{ 	
	int len, count;
  
	len = strlen(data);
  	for (count = 0; count < len; count++) 
    usart_putc(*(data+count)); // �迭�� �����ϴ� �������� 1 ������ �� �����Ͱ� ����Ű�� �ڷ�����ŭ�� ũ��� ����.
}

uchar hex2asc(uchar num) /* ���簪�� �ƽ�Ű�ڵ�� �ٲ۴�. */
{
    if(num>=10) return(num+'A'-10); else return(num+'0');
    /*num�� 10���� ũ�� ���ĺ��̴�. num=0x0A�̸� 0x0A+0x41-0x0A=0x41 �̴� �ƽ�Ű �ڵ�� 'A'�� ��Ÿ����. 
    ���ĺ��� �ƴ� ������ ��� num=3�̸� 3+0x30=0x33 �̴� �ƽ�Ű �ڵ�� '3'�� ��Ÿ����*/
}


uchar asc2hex(uchar asc) /* �ƽ�Ű�ڵ带 ���簪���� �ٲ۴�. */
{
    if ((asc >= '0') && (asc <='9')) return (asc - '0'); 
    /*������� asc�� '2'��� 16��������0x30 '0'�� 16�������� 0x32�̹Ƿ� 0x32-0x30 16���� 0x02�� �ȴ�.*/
    else if ((asc >= 'A') && (asc <='F')) return ((asc - 'A')+10); 
    /*������� asc�� 'C'��� 16��������0x43 'A'�� 16�������� 0x41�̹Ƿ� 0x43-0x41+0x0A�� �������� 0x0C�� �ȴ�.*/
    else if ((asc >= 'a') && (asc <='f')) return ((asc - 'a')+10);
    else return(0xff); /*        */
}



void usart_puthex(uchar num) /* 1����Ʈ ���簪�� 2����Ʈ�� �ƽ�Ű�ڵ�� ��ȯ�ؼ� pc�� �۽��Ѵ�. */
{
    usart_putc(hex2asc(num>>4)); usart_putc(hex2asc(num&0x0f));
    /*num=0x8A���  ���������� 4����Ʈ�� 0x08 �� ���� 8�� �ش��ϴ� �ƽ�Ű �ڵ� '8'�� �ٲ㼭 pc�� �۽��ϰ�
      num�� ���� 4��Ʈ�� 0x0A��	�ްԵȴ�*/
}

void usart_puthex_long(uint num)/*16bit �� int�� ������ 4bit�� ������ �ƽ�Ű �ڵ�� �ٲ㼭 pc�� ����*/
{
    uchar i, j;/*i�� num�� ���� 8bit, j�� ���� 8bit*/
    i = num;
    j = num >> 8;
    usart_putc(hex2asc(j>>4)); usart_putc(hex2asc(j&0x0f));/*j�� ���� 4bit ����// ���� 4bit ����*/
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