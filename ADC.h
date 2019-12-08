/*
 * ADC.h
 *
 *  Created on: 5 mar 2018
 *      Author: karol
 */

#ifndef ADC_H_
#define ADC_H_

#define valid_prog 10 // Próg detekcji uszkodzenia potencjometru;

uint8_t ADC_chanels_valid_check(int16_t ch_1, int16_t ch_2);

#endif /* ADC_H_ */
