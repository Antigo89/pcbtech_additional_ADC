/*
File    : main.c
Software "Kurs STM32 PCBtech"
Lesson 8: ADC1 3 measurement.
Student: antigo1989@gmail.com
*/

#include "main.h"

//global values
volatile uint32_t counter = 0;

/*********************************main************************************/
int main(void) {
  //System init
  SystemInit();
  RCC_Init();
  __enable_irq();
  //ADC init
  ADC1_Init();
  DAC_Init();
  //Timer
  TIM2_Init();
  //GPIO init
  key_led_Init();
 
  while(1){
    
    __NOP();
  }
}

/***********************interrupts function**************************/
//keys
void EXTI15_10_IRQHandler(void){
  switch(EXTI->PR & (EXTI_PR_PR10|EXTI_PR_PR11|EXTI_PR_PR12)){
    case EXTI_PR_PR10:
      ADC1->JSQR &= ~(ADC_JSQR_JSQ4);
      break;
    case EXTI_PR_PR11:
      ADC1->JSQR &= ~(ADC_JSQR_JSQ4);
      ADC1->JSQR |= (4 << ADC_JSQR_JSQ4_Pos);
      break;
    case EXTI_PR_PR12:
      ADC1->JSQR &= ~(ADC_JSQR_JSQ4);
      ADC1->JSQR |= (6 << ADC_JSQR_JSQ4_Pos);
      break;
  }
  EXTI->PR |= EXTI_PR_PR10|EXTI_PR_PR11|EXTI_PR_PR12;
}

void ADC_IRQHandler(void){
  if(ADC1->SR & ADC_SR_JEOC){
    counter = ADC1->JDR1;
    DAC->DHR12R2 = ADC1->JDR1;
    DAC->SWTRIGR |= DAC_SWTRIGR_SWTRIG2;
    ADC1->SR &= ~(ADC_SR_JEOC);
  }
}

/****************************** function**********************************/
void DAC_Init(void){
  RCC->APB1ENR |= RCC_APB1ENR_DACEN;
  DAC->CR |= DAC_CR_BOFF2|DAC_CR_EN2;
}

void ADC1_Init(void){
  RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
  //PA
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
  GPIOA->MODER |= GPIO_MODER_MODE0|GPIO_MODER_MODE4|GPIO_MODER_MODE5|GPIO_MODER_MODE6; //PA0,PA4,PA5,PA6 - Analog
  //ADC1
  ADC1->SMPR2 |= ADC_SMPR2_SMP0_0|ADC_SMPR2_SMP4_0|ADC_SMPR2_SMP6_0;
  ADC1->JSQR &= ~(ADC_JSQR_JL); //1 measurement
  ADC1->JSQR |= (0 << ADC_JSQR_JSQ4_Pos); //CH0
  ADC1->CR1 &= ~(ADC_CR1_RES); //12bit
  ADC1->CR2 |= ADC_CR2_JEXTEN_1|(0b0011<<ADC_CR2_JEXTSEL_Pos); //EXT Trigger TIM2
  ADC1->CR1 |= ADC_CR1_JEOCIE; //Interrupt enable
  NVIC_EnableIRQ(ADC_IRQn);
  ADC1->CR2 |= ADC_CR2_ADON; //ADC enable
}

void TIM2_Init(void){
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
  TIM2->PSC = 419;
  TIM2->CR1 |= TIM_CR1_URS;
  TIM2->CR1 &= ~(TIM_CR1_CMS|TIM_CR1_DIR);
  TIM2->ARR = 1999;
  TIM2->CR2 |= (0b010<<TIM_CR2_MMS_Pos); //master
  TIM2->CR1 |= TIM_CR1_CEN;
  TIM2->EGR |= TIM_EGR_UG;
}

void key_led_Init(void){
  // Clock BUS Initial
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
  RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
  SYSCFG->EXTICR[2] |= SYSCFG_EXTICR3_EXTI10_PE|SYSCFG_EXTICR3_EXTI11_PE;
  SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI12_PE;
  //GPIO 
  GPIOE->MODER |= GPIO_MODER_MODE13_0|GPIO_MODER_MODE14_0|GPIO_MODER_MODE15_0;
  GPIOE->MODER &= ~(GPIO_MODER_MODE10|GPIO_MODER_MODE11|GPIO_MODER_MODE12);
  //Interrupts keys
  EXTI->PR |= EXTI_PR_PR10|EXTI_PR_PR11|EXTI_PR_PR12;
  EXTI->FTSR |= EXTI_FTSR_TR10|EXTI_FTSR_TR11|EXTI_FTSR_TR12;
  EXTI->IMR |= EXTI_IMR_IM10|EXTI_IMR_IM11|EXTI_IMR_IM12;
  //Interrupt NVIC Enable
  NVIC_EnableIRQ(EXTI15_10_IRQn);
  //LED turn off
  GPIOE->BSRR |= GPIO_BSRR_BS13|GPIO_BSRR_BS14|GPIO_BSRR_BS15;
}
/*************************** End of file ****************************/
