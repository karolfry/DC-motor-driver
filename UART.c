/*
 * UART.c
 *
 *  Created on: 4 mar 2018
 *      Author: karol
 */
#include<avr/io.h>
#include<util/delay.h>
#include <avr/interrupt.h>
#include "UART.h"


//#define LED_PIN (1<<PD4)
//#define LED_ON PORTD &= ~(LED_PIN)
//#define LED_OFF PORTD |= (LED_PIN)

char UART_TxBuf[UART_TX_BUF_SIZE]; // bufor nadawczy
uint8_t UART_TxHead=0; //g³owa
uint8_t UART_TxTail=0;//ogon

char UART_RxBuf[UART_RX_BUF_SIZE];
uint8_t UART_RxHead=0;
uint8_t UART_RXTail;

void USART_Init(uint16_t baud)
{
	//ustawienie prêdkoci
	UBRRH = (uint8_t)(baud>>8);
	UBRRL = (uint8_t)baud;

	//za³¹czenie nadajnika i odbiornika
	UCSRB = (1<<RXEN)|(1<<TXEN);
	//ustawienie formatu ramki: 8bitów, 1bit stopu
	UCSRC = (1<<URSEL)|(3<<UCSZ0);
	//przerwanie od nadania i odbioru
	UCSRB |= (1<<RXCIE);

}

// zwyk³¹e wys³anie znaku - bez przerwañ i buforów
//void USART_Transmit(unsigned char data){
//
//	while(!(UCSRA & (1<<UDRE)));
//
//	UDR = data;
//}


char uart_getc(void)
{
	if (UART_RxHead == UART_RXTail) return 0;

	UART_RXTail = (UART_RXTail + 1 ) & UART_RX_BUF_MASK;
	return UART_RxBuf[UART_RXTail];
}


void uart_putc (char data)
{

	uint8_t tmp_head;

	tmp_head = (UART_TxHead +1) & UART_TX_BUF_MASK;

	while(tmp_head == UART_TxTail){}

	UART_TxBuf[tmp_head] = data;
	UART_TxHead = tmp_head;

	UCSRB |= (1<<UDRIE);

}

void uart_putstr(char* data)
{
uint8_t n=0;
while(data[n] != 0)
	{
	uart_putc(data[n]);
	n++;
	}
}

void uart_putnline()
{
	uart_putc(10);
	uart_putc(13);
}

void uart_putInt(uint16_t liczba)
{
	uint8_t n =1,n1,temp;
	if(liczba>9)n++;
	if(liczba>99)n++;
	if(liczba>999)n++;
	n1=n;
	char wynik[n];

	while(n)
	{
	temp = liczba % 10;
	liczba = liczba/10;
	temp +=48;
	wynik[n-1]=temp;
	n--;
	}
	for(int k=0; k<n1;k++)
	{
		uart_putc(wynik[k]);
	}

}

void UDRE_interrupt(void)
{

		if(UART_TxHead != UART_TxTail)
		{
			UART_TxTail = (UART_TxTail + 1) & UART_TX_BUF_MASK;

			UDR = UART_TxBuf[UART_TxTail];

		}
		else UCSRB &=~(1<<UDRIE);

}


void uart_CleanScr(void)
{
	uart_putc(27);   // this is the escape
	uart_putc('[');
	uart_putc('2');
	uart_putc('J');
	uart_putc(27);
	uart_putc('[');
	uart_putc('H');

}

void uart_SetCursor(uint8_t x, uint8_t y)
{
	//puts("\033[0;0H"); // set cursor to 0,0
	uart_putc(27);
	uart_putc('[');
	uart_putInt(x);
	uart_putc(';');
	uart_putInt(y);
	uart_putc('H');
}

void uart_CursorOff()
{
	//puts("\033[0;0H"); // set cursor to 0,0 ?25h
	uart_putc(27);
	uart_putc('[');
	uart_putc('?');
	uart_putc('2');
	uart_putc('5');
	uart_putc('l');
}



void uart_ScrDisplay(void)
{
	uart_CleanScr();

	for(uint8_t x=0;x<10;x++)uart_putc('*');
	uart_putstr("DC MOTOR DRIVER v1.0 by K@ROL");
	for(uint8_t x=0;x<11;x++)uart_putc('*');

	uart_putnline();
	uart_putnline();
	uart_putstr("ADC CH1:");
	uart_SetCursor(3,19);
	uart_putstr("ADC CH2:");
	uart_SetCursor(3,38);
	uart_putstr("ADC VAL:");
	uart_SetCursor(5,0);
	uart_putstr("POWER:");
	uart_SetCursor(5,19);
	uart_putstr("DIR:");
	uart_SetCursor(7,0);
	uart_putstr("BAT VOL:");
	uart_SetCursor(10,0);
	uart_putstr("STATUS:");
	uart_SetCursor(12,0);
	for(uint8_t x=0;x<50;x++)uart_putc('*');
}

