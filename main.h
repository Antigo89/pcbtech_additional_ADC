#ifndef H_MAIN
#define H_MAIN 1

#include <stdio.h>
#include <stdlib.h>
#include "stm32f4xx.h"

//Settings
//4095/3,3*1.0=1241
#define DEF_OUT 1241

//user flags
#define CH0 1
#define CH4 2
#define CH6 3

//prototype function
void RCC_Init(void);
void key_led_Init(void);
void ADC1_Init(void);
void DAC_Init(void);
void TIM2_Init(void);

#endif