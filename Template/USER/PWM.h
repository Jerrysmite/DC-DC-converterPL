#ifndef PWM_H
#define PWM_H

#include"stm32f4xx.h"
#include"stm32f4xx_tim.h"
#include"stm32f4xx_gpio.h"
#include"stm32f4xx_rcc.h"


void PWM_Init();
void TIM6_Init(u32 arr,u16 psc);


#endif 