/*
 * input_reading.h
 *
 *  Created on: Sep 30, 2021
 *      Author: Admin
 */

#ifndef INC_INPUT_READING_H_
#define INC_INPUT_READING_H_

#include "main.h"
#include "timer.h"

//	define and static parameter
///////////////////////////////////////////////
#define NUM_OF_BUTTONS		3

#define PRESSED			    GPIO_PIN_RESET
#define RELEASED			GPIO_PIN_SET
#define HOLDED              2

int buttonBuffer[NUM_OF_BUTTONS];

//	Here is function
/////////////////////////////////
void readButton_preprocess(void);
void readButton(void);
int  getButton(int index);

#endif /* INC_INPUT_READING_H_ */
