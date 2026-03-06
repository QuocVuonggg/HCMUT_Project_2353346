/*
 * timer.h
 *
 * Created on: Oct 11, 2025
 * Author: Tien Vinh
 */

#ifndef INC_TIMER_H_
#define INC_TIMER_H_

#define TIMER_CYCLE  50
extern int timer1_flag;
extern int timer2_flag;
extern int timer3_flag;
extern int timer4_flag;

void setTimer1(int duration);
void setTimer2(int duration);
void setTimer3(int duration);
void setTimer4(int duration);

void timer_run();

#endif /* INC_TIMER_H_ */
