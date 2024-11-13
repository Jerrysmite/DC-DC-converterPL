#include "main.h"
#define SAMPLE_TIMES 10

// #define SINGLE
#define AVG

volatile u32 ADC1_Val     = 0;
volatile u32 ADC1_Val_AVG = 0;

int main(void)
{
    //  中断采用2:2分组
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    //  初始化ADC1 及 采样时钟
    // ADC1_Init();
    // ADC1_TIM10_Init(1000 - 1, 168 - 1); // 1kHz 采样率 待定
    ADC1_IN6_Init();
    ADC1_TIM3_Init(1000 - 1, 84 - 1);

    uart_init(115200);
    delay_init(168);

    LCD_Init();

    while (1) {
    }
}

volatile u8 ADC1_Times = 0;
void ADC_IRQHandler(void)
{
    if (ADC_GetITStatus(ADC1, ADC_IT_EOC) != RESET) {
        ADC_ClearITPendingBit(ADC1, ADC_IT_EOC);
#ifdef SINGLE
        ADC1_Val = ADC_GetConversionValue(ADC1);
#endif
#ifdef AVG
        if (ADC1_Times < SAMPLE_TIMES) {
            ADC1_Val += ADC_GetConversionValue(ADC1);
            ADC1_Times++;
        }
        if (ADC1_Times == SAMPLE_TIMES) {
            // adc2_in7_val_avg = adc2_in7_val / ADC2_SAMPLE_TIMES;
            // adc2_in7_val     = 0;
            ADC1_Val_AVG = ADC1_Val / SAMPLE_TIMES;
            ADC1_Val     = 0;
            ADC1_Times   = 0;
        }
#endif
    }
}