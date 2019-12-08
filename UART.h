/*
 * UART.h
 *
 *  Created on: 4 mar 2018
 *      Author: karol
 */

#ifndef UART_H_
#define UART_H_

////UART
#define UART_BAUD 9600
#define __UBRR 8000000/16/UART_BAUD-1


/////////////////////////////Bufor nadawczy UART///////////////////////////////////////

#define UART_TX_BUF_SIZE 256 // rozmiar bufora nadawczego
#define UART_TX_BUF_MASK (UART_TX_BUF_SIZE - 1) // maska dla bufora nadawczego


//////////////////////////////Bufor odbiorczy UART//////////////////////////////////////

#define UART_RX_BUF_SIZE 32
#define UART_RX_BUF_MASK (UART_RX_BUF_SIZE -1)


void USART_Init (uint16_t baud);
void UDRE_interrupt(void);
//void USART_Transmit(unsigned char data);
char uart_getc(void);
void uart_putc (char data);
void uart_putstr(char* data);
void uart_putnline();
void uart_putInt(uint16_t liczba);
void uart_CleanScr(void);
void uart_SetCursor(uint8_t x, uint8_t y);
void uart_CursorOff();
void uart_ScrDisplay(void);


#endif /* UART_H_ */
