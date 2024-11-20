#include "main.h"
#define SAMPLE_TIMES 20

// #define SINGLE
#define AVG

volatile u32 ADC1_Val        = 0;
volatile u32 ADC1_Val_AVG    = 0;
volatile double ADC1_Volt    = 0;
volatile u32 ADC2_Val        = 0;
volatile u32 ADC2_Val_AVG    = 0;
volatile double ADC2_Current = 0;
volatile float duty          = 0;

PID_Structure PID;

volatile float Uo = 6.0;
volatile float Io = 0.1;

volatile u8 key_up = 0;
volatile u8 key_0  = 0;
volatile u8 key_1  = 0;
volatile u8 key_2  = 0;

volatile u8 MODE         = 0; // 切换当前模式, 0为电压环(默认), 1为电流环,由K1控制
volatile u8 Output_State = 0; // 输出的状态，0为不输出(默认), 1为输出

/*
Key map:
  WK_UP
2   1   0
*/

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
    PID_Init(&PID, Uo, 0.05, 1.1, 0, 0.99, 0.1, 0.99, 0.000075);
    TIM6_Init(75 - 1, 84 - 1);

    KEY_Init();

    LCD_Init();
    LCD_Display_Init();
    LCD_Show_Mode(MODE);
    LCD_Show_Output_State(Output_State);
    LCD_Show_Set_Current(Io);
    LCD_Show_Set_Volt(Uo);

    while (1) {
        delay_ms(100);
        LCD_Show_Measured_Volt(ADC1_Volt);
        LCD_Show_Measured_Current(ADC2_Current);

        if (key_0) {
            key_0 = 0;
            if (MODE == 0) {
                //  电压环模式
                Uo += (double)0.1;
                LCD_Show_Set_Volt(Uo);
                PID_ref(&PID, Uo);
            }
            if (MODE == 1) {
                //  电流环模式
                Io += (double)0.1;
                LCD_Show_Set_Current(Io);
                // todo
            }
        }
        if (key_1) {
            key_1 = 0;
            MODE  = !MODE;
            LCD_Show_Mode(MODE);
            // 切换模式时一定要先把输出关闭
            Output_State = 0;
            LCD_Show_Output_State(Output_State);
            delay_ms(10);
            if (MODE == 0) {
                //  电压环模式
                LCD_Show_Set_Volt(Uo);
                PID_ref(&PID, Uo);
            }
            if (MODE == 1) {
                //  电流环模式
                LCD_Show_Set_Current(Io);
                // todo
            }
        }
        if (key_2) {
            key_2 = 0;
            if (MODE == 0) {
                //  电压环模式
                Uo -= (double)0.1;
                if (Uo <= 0.0) Uo = 0.0;
                LCD_Show_Set_Volt(Uo);
                PID_ref(&PID, Uo);
            }
            if (MODE == 1) {
                //  电流环模式
                Io -= (double)0.1;
                if (Io <= 0.0) Io = 0.0;
                LCD_Show_Set_Current(Io);
                // todo
            }
        }
        if (key_up) {
            key_up       = 0;
            Output_State = !Output_State;
            LCD_Show_Output_State(Output_State);
        }
    }
}

volatile u8 ADC1_Times = 0;
volatile u8 ADC2_Times = 0;
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
            ADC1_Volt    = (double)ADC1_Val_AVG * 0.0036457 - 0.005;
            // 截断为一位小数,可选
            // ADC1_Volt = (double)((int)(ADC1_Volt * 10)) / 10.0;
            // 截断为两位小数,可选
            // ADC1_Volt  = (double)((int)(ADC1_Volt * 100)) / 100.0;
            ADC1_Val   = 0;
            ADC1_Times = 0;
        }
#endif
    }
    if (ADC_GetITStatus(ADC2, ADC_IT_EOC) != RESET) {
        ADC_ClearITPendingBit(ADC2, ADC_IT_EOC);
        if (ADC2_Times < SAMPLE_TIMES) {
            ADC2_Val += ADC_GetConversionValue(ADC2);
            ADC2_Times++;
        }
        if (ADC2_Times == SAMPLE_TIMES) {
            ADC2_Val_AVG = ADC2_Val / SAMPLE_TIMES;
            // ADC2_Current    = (double)ADC1_Val_AVG * 0.0036457 - 0.005;
            // 截断为一位小数,可选
            // ADC1_Volt = (double)((int)(ADC1_Volt * 10)) / 10.0;
            // 截断为两位小数,可选
            // ADC1_Volt  = (double)((int)(ADC1_Volt * 100)) / 100.0;
            ADC2_Val   = 0;
            ADC2_Times = 0;
        }
    }
}
void TIM6_DAC_IRQHandler(void)
{
    if (MODE == 0) {

        if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET) {
            TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
            if (Output_State == 1) {
                //  设定为输出模式时开始工作
                duty       = PID_Realize(&PID, ADC1_Volt);
                TIM1->CCR1 = duty * (8400 - 1);
            }
            if (Output_State == 0) {
                //  非输出模式
                TIM1->CCR1 = 1 - 1;
            }
        }
    }
    if (MODE == 1) {
    }
}

void EXTI0_IRQHandler(void)
{
    // 检查是否是 EXTI Line0 触发了中断
    if (EXTI_GetITStatus(EXTI_Line0) != RESET) {
        // 清除 EXTI Line0 中断挂起标志
        EXTI_ClearITPendingBit(EXTI_Line0);

        // 读取按键状态并进行处理
        if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) == Bit_SET) {
            key_up = 0;
        } else {
            key_up = 1;
        }
    }
}

void EXTI2_IRQHandler(void)
{
    // 检查是否是 EXTI Line2 触发了中断
    if (EXTI_GetITStatus(EXTI_Line2) != RESET) {
        // 清除 EXTI Line2 中断挂起标志
        EXTI_ClearITPendingBit(EXTI_Line2);

        // 读取按键状态并进行处理
        if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2) == Bit_SET) {
            key_2 = 1;
        } else {
            key_2 = 0;
        }
    }
}

void EXTI3_IRQHandler(void)
{
    // 检查是否是 EXTI Line3 触发了中断
    if (EXTI_GetITStatus(EXTI_Line3) != RESET) {
        // 清除 EXTI Line3 中断挂起标志
        EXTI_ClearITPendingBit(EXTI_Line3);

        // 读取按键状态并进行处理
        if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3) == Bit_SET) {
            key_1 = 0;
        } else {
            key_1 = 1;
        }
    }
}

void EXTI4_IRQHandler(void)
{
    // 检查是否是 EXTI Line4 触发了中断
    if (EXTI_GetITStatus(EXTI_Line4) != RESET) {
        // 清除 EXTI Line4 中断挂起标志
        EXTI_ClearITPendingBit(EXTI_Line4);

        // 读取按键状态并进行处理
        if (GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4) == Bit_SET) {
            key_0 = 0;
        } else {
            key_0 = 1;
        }
    }
}
