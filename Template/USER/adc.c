#include "adc.h"

volatile u16 ADC1_Value;

void ADC1_Init(void)
{
    //  ADC1_IN6 => PA6
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;     //  Analog IN
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;       // PA6
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // NoPull
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    ADC_CommonInitTypeDef ADC_CommonInitStructure;
    ADC_InitTypeDef ADC_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

    ADC_CommonInitStructure.ADC_Mode             = ADC_Mode_Independent;          // ADC Independent Mode
    ADC_CommonInitStructure.ADC_Prescaler        = ADC_Prescaler_Div4;            // 84MHz / 4 = 21MHz
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_10Cycles; //
    ADC_CommonInitStructure.ADC_DMAAccessMode    = ADC_DMAAccessMode_Disabled;
    ADC_CommonInit(&ADC_CommonInitStructure);

    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
    ADC_InitStructure.ADC_ScanConvMode       = DISABLE;
    ADC_InitStructure.ADC_DataAlign          = ADC_DataAlign_Right;
    // ADC_InitStructure.ADC_ExternalTrigConv     = ADC_ExternalTrigConv_T8_TRGO;
    // ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
    ADC_InitStructure.ADC_NbrOfConversion      = 1;
    ADC_InitStructure.ADC_Resolution           = ADC_Resolution_12b;
    ADC_Init(ADC1, &ADC_InitStructure);

    ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 1, ADC_SampleTime_112Cycles);

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE); // DMA2时钟使能
    while (DMA_GetCmdStatus(DMA2_Stream0) != DISABLE);
    DMA_InitTypeDef DMA_InitStructure;
    DMA_StructInit(&DMA_InitStructure);
    DMA_InitStructure.DMA_Channel            = DMA_Channel_0;
    DMA_InitStructure.DMA_Mode               = DMA_Mode_Circular;
    DMA_InitStructure.DMA_DIR                = DMA_DIR_PeripheralToMemory;
    DMA_InitStructure.DMA_BufferSize         = 1;
    DMA_InitStructure.DMA_Memory0BaseAddr    = (u32)&ADC1_Value;
    DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1->DR;
    DMA_InitStructure.DMA_MemoryDataSize     = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryInc          = DMA_MemoryInc_Disable;
    DMA_InitStructure.DMA_PeripheralInc      = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_Priority           = DMA_Priority_High;
    DMA_InitStructure.DMA_FIFOMode           = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_MemoryBurst        = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst    = DMA_PeripheralBurst_Single;
    DMA_InitStructure.DMA_FIFOThreshold      = DMA_FIFOThreshold_HalfFull;

    DMA_Cmd(DMA2_Stream0, ENABLE);
    ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
    ADC_DMACmd(ADC1, ENABLE);
    ADC_Cmd(ADC1, ENABLE);
    // ADC_SoftwareStartConv(ADC1);
}

void ADC1_TIM10_Init(u32 arr, u16 psc)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE); // 对于F407, APB2 Timer 时钟频率为168MHz

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Period        = arr;
    TIM_TimeBaseStructure.TIM_Prescaler     = psc;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM10, &TIM_TimeBaseStructure);

    TIM_Cmd(TIM10, ENABLE);
    TIM_ITConfig(TIM10, TIM_IT_Update, ENABLE);
    TIM_ClearITPendingBit(TIM10, TIM_IT_Update); // 清除中断标志位

    //   电压采样优先级最高
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel                   = TIM1_UP_TIM10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;      // 抢占优先级0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;      // 响应优先级0
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE; // IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);
}

void ADC1_TIM3_Init(u32 arr, u16 psc)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); // 对于F407, APB2 Timer 时钟频率为168MHz

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Period        = arr;
    TIM_TimeBaseStructure.TIM_Prescaler     = psc;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_Update);
    TIM_Cmd(TIM3, ENABLE);
}

void ADC1_IN6_Init(void)
{
    //  ADC1_IN6 => PA6
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;     //  Analog IN
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;       // PA6
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // NoPull
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    ADC_CommonInitTypeDef ADC_CommonInitStructure;
    ADC_InitTypeDef ADC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

    ADC_CommonInitStructure.ADC_Mode             = ADC_Mode_Independent;          // ADC Independent Mode
    ADC_CommonInitStructure.ADC_Prescaler        = ADC_Prescaler_Div4;            // 84MHz / 4 = 21MHz
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_10Cycles; //
    ADC_CommonInitStructure.ADC_DMAAccessMode    = ADC_DMAAccessMode_Disabled;
    ADC_CommonInit(&ADC_CommonInitStructure);

    ADC_InitStructure.ADC_ContinuousConvMode   = DISABLE;
    ADC_InitStructure.ADC_ScanConvMode         = DISABLE;
    ADC_InitStructure.ADC_DataAlign            = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_ExternalTrigConv     = ADC_ExternalTrigConv_T3_TRGO;
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;
    ADC_InitStructure.ADC_NbrOfConversion      = 1;
    ADC_InitStructure.ADC_Resolution           = ADC_Resolution_12b;

    ADC_Init(ADC1, &ADC_InitStructure);

    ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 1, ADC_SampleTime_112Cycles);

    ADC_ITConfig(ADC1, ADC_IT_EOC, ENABLE);

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel                   = ADC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    ADC_Cmd(ADC1, ENABLE);
}

void ADC2_IN7_Init(void)
{
    //  ADC2_IN7 => PA7
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;     //  Analog IN
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_7;       // PA6
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // NoPull
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    ADC_CommonInitTypeDef ADC_CommonInitStructure;
    ADC_InitTypeDef ADC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);

    ADC_CommonInitStructure.ADC_Mode             = ADC_Mode_Independent;          // ADC Independent Mode
    ADC_CommonInitStructure.ADC_Prescaler        = ADC_Prescaler_Div4;            // 84MHz / 4 = 21MHz
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_10Cycles; //
    ADC_CommonInitStructure.ADC_DMAAccessMode    = ADC_DMAAccessMode_Disabled;
    ADC_CommonInit(&ADC_CommonInitStructure);

    ADC_InitStructure.ADC_ContinuousConvMode   = DISABLE;
    ADC_InitStructure.ADC_ScanConvMode         = DISABLE;
    ADC_InitStructure.ADC_DataAlign            = ADC_DataAlign_Right;
    ADC_InitStructure.ADC_ExternalTrigConv     = ADC_ExternalTrigConv_T2_TRGO;
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_Rising;
    ADC_InitStructure.ADC_NbrOfConversion      = 1;
    ADC_InitStructure.ADC_Resolution           = ADC_Resolution_12b;

    ADC_Init(ADC2, &ADC_InitStructure);

    ADC_RegularChannelConfig(ADC2, ADC_Channel_7, 1, ADC_SampleTime_112Cycles);
    ADC_ITConfig(ADC2, ADC_IT_EOC, ENABLE);

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel                   = ADC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    ADC_Cmd(ADC2, ENABLE);
}

void ADC2_TIM2_Init(u32 arr, u16 psc)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); // 对于F407, APB2 Timer 时钟频率为168MHz

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Period        = arr;
    TIM_TimeBaseStructure.TIM_Prescaler     = psc;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);
    TIM_Cmd(TIM2, ENABLE);
}