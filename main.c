/*
 * main.c
 *
 *  Created on: 4 mar 2018
 *      Author: karol
 */


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "UART.h"
#include "ADC.h"
#include "driver.h"



//// GPIO
#define LED_PIN (1<<PB7)
#define LED_ON PORTB &= ~(LED_PIN)
#define LED_OFF PORTB |= (LED_PIN)

uint8_t pomiar = 0;
uint8_t ADC_mesure_ch1 = 0, ADC_mesure_ch2=0,  ADC_mesure_ch3 = 0 ;
uint8_t ADC_valid_flag=0;
uint8_t ADC_Dir=0;
uint8_t UART_em_stop = 0;
//uint8_t ADC_mesure_start_flag = 0; // flaga rozpoczêcia pomiarów dla ADC
uint8_t ADC_mesure_cmp_flag = 0;  // flaga zakoñczenia pomiaru dla wszystkich kana³ów przez ADC

static uint8_t ADC_value = 127;

uint8_t work_flag = 0;

int main (void)
{
////////GPIO////////////////////////////////////////////////////////
	DDRB |= LED_PIN;
	LED_OFF;

	DDRC |= REL_1_PIN | REL_2_PIN;
	REL_1_OFF;
	REL_2_OFF;

////////USART///////////////////////////////////////////////////////
	USART_Init(__UBRR);


///////////ADC
    ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); // preskaler
    ADMUX |= (1<<REFS0); // nap odniesienia
    ADMUX |= (1<<ADLAR); // pom 8-bit
    ADMUX |= (1<<MUX2)|(1<<MUX0); //kana³ 5
    //ADCSRA |=(1<<ADFR);//free runing
    ADCSRA |=(1<<ADIE);//przerwanie
    ADCSRA |=(1<<ADEN); // w³ przetwornika
///////////
	sei();


 uart_ScrDisplay();
 uart_CursorOff();

	ADCSRA |= (1<<ADSC);

///////// TIMER 0 - CTC MODE - system time base //

	TCCR0 |= (1<<CS02)| (1<<CS00); // - prescaler - F_CPU/1024
	TIMSK |= (1<<TOIE0); // - overflow interrupt


/////////////////////////////////////////////////

///////// TIMER 1 - PWM mode on PB1 (OC1A)
	TCCR1A |= (1<<WGM10); //PWM mode 8 bit
	TCCR1A |= (1<<COM1A1)|(1<<COM1A0);  // Clear OC1A/OC1B on Compare Match when up-counting
	TCCR1B |= (1<<CS10);

	DDRB |= (1<<PB1);
	OCR1A = 255;

	while(1)
		{

		LED_ON;

		_delay_ms(100);
		LED_OFF;

		_delay_ms(100);

		////// uart working indicator
		uart_SetCursor(10,9);
		if(work_flag==1)
		{
			uart_putc(47);
			work_flag = 0;
		}
		else
		{
			uart_putc(92);
			work_flag = 1;
		}
		////// uart adc mesurment display
		if(ADC_mesure_cmp_flag==1)
		{
			ADC_mesure_cmp_flag =0;

			uart_SetCursor(3,10);
			if(ADC_mesure_ch1 < 10)uart_putc('  ');
			if(ADC_mesure_ch1 < 100)uart_putc(' ');
			uart_putInt(ADC_mesure_ch1);

			uart_SetCursor(3,28);
			if(ADC_mesure_ch2 < 10)uart_putc('  ');
			if(ADC_mesure_ch2 < 100)uart_putc(' ');
			uart_putInt(ADC_mesure_ch2);

			uart_SetCursor(7,10);
			if(ADC_mesure_ch3 < 10)uart_putc('  ');
			if(ADC_mesure_ch3 < 100)uart_putc(' ');
			uart_putInt(ADC_mesure_ch3);

			uart_SetCursor(3,47);


			if(ADC_valid_flag) uart_putc('T');
			else uart_putc('F');

//			if(ADC_valid_flag && !UART_em_stop) // Jeœli pomiar prawid³owy i nie przysz³o awaryjne wy³aczenie z komunikacji UART wysteruj PWM oraz przeka¿niki
//			{
//
//					if(ADC_mesure_ch2<=127)
//					{
//						OCR1A= (127-ADC_mesure_ch2)*2;
//						DIR_forward();
//						ADC_Dir =1;
//					}
//					else
//					{
//						OCR1A = (ADC_mesure_ch2 - 128)*2;
//						DIR_backward();
//						ADC_Dir =2;
//					}
//			}else
//					{ 		// jeœli pomiar nieprawid³owy to NEUTRAL i zero na PWM
//
//					OCR1A = 0; //Dioda wy³¹czana stanem H - sprawdzic jak bedzie ze sterowniem MOSFET !!!!!!!!!!!!!
//					DIR_neutral();
//					ADC_Dir =0;
//					}

			uart_SetCursor(5,10);
			if(OCR1A < 10)uart_putc('  ');
			if(OCR1A < 100)uart_putc(' ');
			uart_putInt(OCR1A);

			uart_SetCursor(5,28);
			if(ADC_Dir == 1) uart_putc('F');
			else if (ADC_Dir == 2)uart_putc('B');
			else if (ADC_Dir == 0)uart_putc('N');

			ADCSRA |= (1<<ADSC);
		}

		}


}

ISR(USART_RXC_vect)
{
char received = UDR;//  !!!! Do dopisania obs³uga buforu RX
if(received == ' ') UART_em_stop = 1;
}


ISR(USART_UDRE_vect)
{
	UDRE_interrupt();
}



ISR(ADC_vect)
{
	static uint8_t sel_channel = 0;

	if(sel_channel == 0)
	{
		ADC_mesure_ch3 = ADCH;

		 ADMUX &= ~((1<<MUX3)|(1<<MUX2)|(1<<MUX1)|(1<<MUX0)); //kana³ 0
		 sel_channel=1;
		 ADCSRA |= (1<<ADSC);
	} else if (sel_channel == 1)
			{
			ADC_mesure_ch1 = ADCH;
			ADMUX |= (1<<MUX0); //kana³ 1
			sel_channel=2;
			ADCSRA |= (1<<ADSC);
			}else if (sel_channel == 2)
				{
				ADC_mesure_ch2 = ADCH;
				ADMUX |= (1<<MUX2)|(1<<MUX0); //kana³ 5
				sel_channel=0;
				ADC_mesure_cmp_flag = 1;
				}



//	pomiar = ADCH;


}

ISR(TIMER0_OVF_vect)
{
	static uint8_t count = 0;
	static uint8_t ADC_err_count = 0;
	static uint8_t ADC_err_flag = 0;
	static uint8_t Dir_change_flag = 0;
	static uint16_t PWR_zero_detect_flag = 0;
	static uint8_t Zero_cross_delay = 0;

	static uint8_t prescaler =0;

///////////////
	ADC_valid_flag = ADC_chanels_valid_check(ADC_mesure_ch1, ADC_mesure_ch2); //sprawdzenie symetrii sygna³ów
	if(ADC_valid_flag == 0 && ADC_err_count<150) // jesli blad pomiaru to zliczaj
	{
		ADC_err_count++;
	} else if(ADC_err_count<150) ADC_err_count =0;

	//if(ADC_err_count == 150) ADC_err_flag = 1; // Przy taktowaniu 8Mhz jesli blad pomiaru bedzie sie utrzymywal 5s to wystaw flage bleduk



	if(ADC_err_flag ==0 && UART_em_stop==0) //Jeœli brak b³êdu pomiaru oraz brak wylaczenia awaryjnego z uart to wysteruj PWM oraz kierunek
	{
		//if(prescaler <1)prescaler ++;
		//else
		//{
		//	prescaler=0;

		if(ADC_value != 127 && PWR_zero_detect_flag == 0)
		{
				if(ADC_mesure_ch1 > ADC_value)ADC_value++;
				else if (ADC_mesure_ch1 < ADC_value) ADC_value--;
		} else if (ADC_value == 127)
		{
			PWR_zero_detect_flag = 1;
			Zero_cross_delay ++;
				if(Zero_cross_delay>50)
				{
					if(ADC_mesure_ch1 > ADC_value)ADC_value++;
					else if (ADC_mesure_ch1 < ADC_value) ADC_value--;
					Zero_cross_delay =0;
					PWR_zero_detect_flag = 0;
					//LED_ON;
				}
		}

		if(ADC_value<127)
		{
			OCR1A= (127-ADC_value)*2;
			DIR_forward();
			ADC_Dir =1;
		}
		else if (ADC_value>127)
			{
			OCR1A = (ADC_value - 128)*2;
			DIR_backward();
			ADC_Dir =2;
			}
		else
			{
			OCR1A = 0;
			DIR_neutral();
			ADC_Dir =0;
			}



	}
		else  // jesli b³¹d pomiaru z potencjometru lub awaryjne wylaczenie z UART to zatrzymnaj silnik
		{
		OCR1A = 0; //Dioda wy³¹czana stanem H - sprawdzic jak bedzie ze sterowniem MOSFET !!!!!!!!!!!!!
		DIR_neutral();
		ADC_Dir =0;
		}
//////////////////
//	if(prescaler <2)prescaler ++;
//	else{
//		prescaler =0;
//
//		if(count==1)
//			{
//				LED_ON;
//				count = 0;
//
//			}else
//			{
//				LED_OFF;
//				count = 1;
//			}
//
//	}


}





