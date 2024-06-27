#ifndef H_MAIN
#define H_MAIN 1

#include <stdio.h>
#include <stdlib.h>
#include "stm32f4xx.h"

//Settings
#define LIM_MIN 429
#define LIM_MAX 3276

//prototype function
void RCC_Init(void);
void key_led_Init(void);
void ADC1_Init(void);
void DAC_Init(void);
void TIM2_Init(void);

#endif