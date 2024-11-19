#include "key.h"

// 初始化按键GPIO（输入）和输出引脚
void KEY_Init(void)
{
    // 初始化 GPIO 结构体
    GPIO_InitTypeDef GPIO_InitStructure;

    // 初始化 NVIC 和 EXTI 结构体
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // 使能 GPIOA、GPIOE 和 GPIOF 时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);

    // 使能 SYSCFG 时钟，用于配置外部中断线
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    // 配置 PA0 引脚为输入模式（WKUP 按键）
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN; // WKUP 按键需要下拉电阻
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 将 PA0 连接到 EXTI0
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);

    // 配置 EXTI0 线为下降沿触发（按键按下时触发中断）
    EXTI_InitStructure.EXTI_Line    = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; // 双边沿触发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    // 配置 NVIC 中断优先级并使能 EXTI0 中断
    NVIC_InitStructure.NVIC_IRQChannel                   = EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 配置 PE2 ~ PE4 引脚为输入模式
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; // PE2 PE3 PE4需要上拉电阻
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    // 将 PE2、PE3、PE4 连接到 EXTI 线
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2); // PE2 对应 EXTI_Line2
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3); // PE3 对应 EXTI_Line3
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource4); // PE4 对应 EXTI_Line4

    // 配置 EXTI2 线（PE2）
    EXTI_InitStructure.EXTI_Line    = EXTI_Line2;
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling; // 下降沿触发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    // 配置 EXTI3 线（PE3）
    EXTI_InitStructure.EXTI_Line = EXTI_Line3;
    EXTI_Init(&EXTI_InitStructure);

    // 配置 EXTI4 线（PE4）
    EXTI_InitStructure.EXTI_Line = EXTI_Line4;
    EXTI_Init(&EXTI_InitStructure);

    // 配置 NVIC，启用 EXTI2 中断
    NVIC_InitStructure.NVIC_IRQChannel                   = EXTI2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 配置 NVIC，启用 EXTI3 中断
    NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
    NVIC_Init(&NVIC_InitStructure);

    // 配置 NVIC，启用 EXTI4 中断
    NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
    NVIC_Init(&NVIC_InitStructure);

    // 配置 PF9 PF10 为输出模式
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; // 推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL; // 无上拉或下拉电阻
    GPIO_Init(GPIOF, &GPIO_InitStructure);
}
