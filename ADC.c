/*
 * ADC.c
 *
 *  Created on: 5 mar 2018
 *      Author: karol
 */
#include <avr/io.h>

#include "ADC.h"

uint8_t ADC_chanels_valid_check(int16_t ch_1, int16_t ch_2)
{
	uint8_t valid_flag = 0;
	int16_t dif=255;

	ch_1 = 255 - ch_1; // odwrocenie polaryzacji jednego z kana³ów, aby oba mialy tak¹ sam¹

	if(ch_1>= ch_2)dif = ch_1-ch_2;
	else dif = ch_2-ch_1;

	if(dif <= valid_prog)valid_flag = 1;
	return valid_flag;
}
