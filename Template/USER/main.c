#include "main.h"

int main(void)
{
    //  中断采用2:2分组
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    //  初始化ADC1 及 采样时钟
    ADC1_Init();
    ADC1_TIM10_Init(1000 - 1, 168 - 1); // 1kHz 采样率 待定

    uart_init(115200);
    delay_init(168);

    LCD_Init();

    while (1) {
    }
}
