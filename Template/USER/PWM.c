#include "PWM.h"

void PWM_Init()
{

    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE); // TIM1 在 APB2 总线上

    // 初始化GPIO
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8;        // 引脚 PA8
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;      // 复用
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // 速度100MHz
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     // 推挽复用输出
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;  // 不启用上下拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // PA8 复用到 TIM1 输出通道
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_TIM1);

    // PB13 配置到CH1N
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_TIM1);

    // 初始化TIM1
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_BDTRInitTypeDef TIM_BDTRInitStructure;

    TIM_TimeBaseInitStructure.TIM_Period      = 8400 - 1;
    TIM_TimeBaseInitStructure.TIM_Prescaler   = 1 - 1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);
    // PWM
    TIM_OCInitStructure.TIM_OCMode       = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCNPolarity  = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState  = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    TIM_OCInitStructure.TIM_OCIdleState  = TIM_OCIdleState_Reset;
    TIM_OCInitStructure.TIM_Pulse        = 1050 - 1;
    TIM_OC1Init(TIM1, &TIM_OCInitStructure);

    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);

    // 死区部分

    TIM_BDTRInitStructure.TIM_OSSIState       = TIM_OSSRState_Enable;
    TIM_BDTRInitStructure.TIM_LOCKLevel       = TIM_LOCKLevel_OFF;
    TIM_BDTRInitStructure.TIM_DeadTime        = 50;
    TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Disable;
    TIM_BDTRInitStructure.TIM_Break           = TIM_Break_Disable;
    TIM_BDTRConfig(TIM1, &TIM_BDTRInitStructure);

    TIM_Cmd(TIM1,ENABLE);
}