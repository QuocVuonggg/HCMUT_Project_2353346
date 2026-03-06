/*
 * input_reading.c
 *
 *  Created on: Oct 11, 2025
 *      Author: Tien Vinh
 */
#include "main.h"
#include "input_reading.h"


// the buffer that the final result is stored after debouncing
static GPIO_PinState buttonBuffer[NO_OF_BUTTONS];

// we define two buffers for debouncing
static GPIO_PinState debounceButtonBuffer1[NO_OF_BUTTONS];
static GPIO_PinState debounceButtonBuffer2[NO_OF_BUTTONS];

static GPIO_TypeDef* button_port[NO_OF_BUTTONS] = {BUT0_GPIO_Port, BUT1_GPIO_Port, BUT2_GPIO_Port};
static uint16_t button_pin[NO_OF_BUTTONS] = {BUT0_Pin, BUT1_Pin, BUT2_Pin};


void button_reading(void) {
    for (int i = 0; i < NO_OF_BUTTONS; i++) {
        debounceButtonBuffer2[i] = debounceButtonBuffer1[i];
        debounceButtonBuffer1[i] = HAL_GPIO_ReadPin(button_port[i], button_pin[i]);

        if (debounceButtonBuffer1[i] == debounceButtonBuffer2[i]) {
            buttonBuffer[i] = debounceButtonBuffer1[i];
        }
    }
}
unsigned char is_button_pressed(unsigned char index) {
    if (index >= NO_OF_BUTTONS) return 0;
    return (buttonBuffer[index] == GPIO_PIN_RESET);
}

unsigned char is_button_pressed_1s(unsigned char index) {
    if (index >= NO_OF_BUTTONS) return 0xff;
    return 0;
}


