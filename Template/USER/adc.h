#ifndef ADC_H
#define ADC_H
#include "stm32f4xx.h"

extern volatile u16 ADC1_Value;
void ADC1_Init(void);
void ADC1_TIM10_Init(u32 arr, u16 psc);

#endif // ADC_H