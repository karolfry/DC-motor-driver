/*
 * driver.h
 *
 *  Created on: 5 mar 2018
 *      Author: karol
 */

#ifndef DRIVER_H_
#define DRIVER_H_


#define REL_1_PIN (1<<PC3)					//Za³. stanem wysokim
#define REL_1_OFF PORTC &= ~(REL_1_PIN)
#define REL_1_ON PORTC |= (REL_1_PIN)

#define REL_2_PIN (1<<PC2)					//Za³. stanem wysokim
#define REL_2_OFF PORTC &= ~(REL_2_PIN)
#define REL_2_ON PORTC |= (REL_2_PIN)



void DIR_forward(void);
void DIR_backward(void);
void DIR_neutral(void);

#endif /* DRIVER_H_ */
