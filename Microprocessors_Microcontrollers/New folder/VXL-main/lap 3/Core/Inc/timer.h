/*
 * timer.h
 *
 *  Created on: 5 thg 10, 2021
 *      Author: Admin
 */

#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#include "main.h"

#define TIMER_INTERUPT		10		//milisecond


// used in buttonCounter
////////////
#define TIME_1S				1000	//milisecond
static int COUNT_1S = TIME_1S / TIMER_INTERUPT;

#define TIME_50MS			50
static int COUNT_50MS = TIME_50MS / TIMER_INTERUPT;

#define TIME_500MS			500		// for blinking
static int COUNT_500MS = TIME_500MS / TIMER_INTERUPT;



#endif /* INC_TIMER_H_ */
