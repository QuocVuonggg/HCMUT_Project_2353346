/*
 * input_processing.c
 *
 *  Created on: Oct 11, 2025
 *      Author: Tien Vinh
 */
#include "main.h"
#include "input_reading.h"
#include "timer.h"

enum ButtonState {
    BUTTON_RELEASED,
    BUTTON_PRESSED,
    BUTTON_PRESSED_MORE_THAN_1_SECOND
};

enum ButtonState buttonState = BUTTON_RELEASED;

static int timer_for_auto_increase = 0;

void fsm_for_input_processing(void) {
    switch(buttonState) {
        case BUTTON_RELEASED:
            if(is_button_pressed(0)) {
                buttonState = BUTTON_PRESSED;
                // INCREASE VALUE OF PORT A BY ONE UNIT
            }
            break;

        case BUTTON_PRESSED:
            if(!is_button_pressed(0)) {
                buttonState = BUTTON_RELEASED;
            } else {
                if(is_button_pressed_1s(0)) {
                    buttonState = BUTTON_PRESSED_MORE_THAN_1_SECOND;
                }
            }
            break;

        case BUTTON_PRESSED_MORE_THAN_1_SECOND:
            if(!is_button_pressed(0)) {
                buttonState = BUTTON_RELEASED;
            }
            //to do: Add logic for auto-increment every 500ms
            if (timer_for_auto_increase == 0){
            	setTimer1(250);
            }

            break;
    }
}


