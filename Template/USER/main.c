#include "main.h"
#define SAMPLE_TIMES 20

// #define SINGLE
#define AVG

volatile u32 ADC1_Val     = 0;
volatile u32 ADC1_Val_AVG = 0;
volatile double ADC1_Volt = 0;
volatile float duty       = 0;

PID_Structure PID;

volatile float Uo = 6.0;

int main(void)
{
    //  中断采用2:2分组
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    //  初始化ADC1 及 采样时钟
    ADC1_IN6_Init();
    ADC1_TIM3_Init(10 - 1, 84 - 1);

    uart_init(115200);
    delay_init(168);

    PWM_Init();
    PID_Init(&PID, Uo, 0.1, 0.01, 0, 0.99, 0.1, 0.99, 0.0001);

    TIM6_Init(100 - 1, 84 - 1);

    LCD_Init();
    LCD_ShowString(30, 80, 400, 16, 16, "STM32F407ZGT6 BUCK @NitrenePL");

    while (1) {
        delay_ms(500);
        LCD_ShowString(30, 130, 400, 16, 16, "Volt:");
        LCD_ShowxNum(110, 210, (int)ADC1_Volt, 1, 16, 0);
        LCD_ShowxNum(126, 210, (int)((ADC1_Volt - (int)ADC1_Volt) * 1000), 3, 16, 0X80);
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
            ADC1_Val_AVG = ADC1_Val / SAMPLE_TIMES;
            ADC1_Volt    = (double)ADC1_Val_AVG * 0.0036 + 0.001;
            ADC1_Val     = 0;
            ADC1_Times   = 0;
        }
#endif
    }
}
void TIM6_DAC_IRQHandler()
{
    if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET) {
        TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
        duty       = PID_Realize(&PID, ADC1_Volt);
        TIM1->CCR1 = duty * (8400 - 1);
    }
    TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
};
