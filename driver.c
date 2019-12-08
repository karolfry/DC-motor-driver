/*
 * driver.c
 *
 *  Created on: 5 mar 2018
 *      Author: karol
 */
#include <avr/io.h>
#include "driver.h"


void DIR_forward(void)
{
	REL_1_ON;
	REL_2_OFF;
}
void DIR_backward(void)
{
	REL_1_OFF;
	REL_2_ON;
}
void DIR_neutral(void)
{
	REL_1_OFF;
	REL_2_OFF;
}
