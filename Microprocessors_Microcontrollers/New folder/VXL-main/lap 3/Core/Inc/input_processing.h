/*
 * input_processing.h
 *
 *  Created on: 5 thg 10, 2021
 *      Author: Admin
 */

#ifndef INC_INPUT_PROCESSING_H_
#define INC_INPUT_PROCESSING_H_

#include "main.h"

int mode;
#define MAX_MODE	4
int value;
#define MAX_VALUE	50

int red_time;
int green_time;
int yellow_time;

int flag_set;	// used for button 3
int flag_1s;	// for counting 1s
int flag_500ms;	// for counting 500ms = 0.5s
int flag_reset;	// reset when change mode

void setTime(void);
void preprocess(void);
void resetPin(void);
void fsm_input_processing(void);

#endif /* INC_INPUT_PROCESSING_H_ */
