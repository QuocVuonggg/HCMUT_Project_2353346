/*
 * timer.c
 *
 * Created on: Oct 11, 2025
 * Author: Tien Vinh
 */
#include "timer.h"

int timer1_counter = 0;
int timer1_flag = 0;

int timer2_counter = 0;
int timer2_flag = 0;

int timer3_counter = 0;
int timer3_flag = 0;

int timer4_counter = 0; // Thêm biến đếm cho timer 4
int timer4_flag = 0;    // Thêm cờ cho timer 4


void setTimer1(int duration) {
    timer1_counter = duration / TIMER_CYCLE;
    timer1_flag = 0;
}

void setTimer2(int duration) {
    timer2_counter = duration / TIMER_CYCLE;
    timer2_flag = 0;
}

void setTimer3(int duration) {
    timer3_counter = duration / TIMER_CYCLE;
    timer3_flag = 0;
}

// Thêm hàm set cho timer 4
void setTimer4(int duration) {
    timer4_counter = duration / TIMER_CYCLE;
    timer4_flag = 0;
}


void timer_run() {
    if (timer1_counter > 0) {
        timer1_counter--;
        if (timer1_counter <= 0) {
            timer1_flag = 1; // Bật cờ khi đếm xong
        }
    }

    if (timer2_counter > 0) {
        timer2_counter--;
        if (timer2_counter <= 0) {
            timer2_flag = 1;
        }
    }

    if (timer3_counter > 0) {
        timer3_counter--;
        if (timer3_counter <= 0) {
            timer3_flag = 1;
        }
    }

    // Thêm logic chạy cho timer 4
    if (timer4_counter > 0) {
        timer4_counter--;
        if (timer4_counter <= 0) {
            timer4_flag = 1;
        }
    }
}
