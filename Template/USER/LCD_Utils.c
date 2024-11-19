#include "LCD_Utils.h"

//  静态显示框架
void LCD_Display_Init(void)
{
    POINT_COLOR = BLACK;
    LCD_ShowString(30, 80, 400, 16, 16, (u8 *)"STM32F407ZGT6 BUCK @NitrenePL");
    LCD_ShowString(30, 130, 200, 16, 16, (u8 *)"Measured Volt:");
    LCD_ShowString(30, 200, 400, 16, 16, (u8 *)"Please Change Volt or Current.");
    LCD_ShowString(30, 240, 40, 16, 16, (u8 *)"MODE:");
    LCD_ShowString(30, 260, 200, 16, 16, (u8 *)"(K1 to Change MODE)");


    POINT_COLOR = BLUE;
    LCD_ShowChar(80, 300, '-', 16, 0x80);
    LCD_ShowString(76, 320, 16, 16, 16, (u8 *)"K2");

    POINT_COLOR = RED;
    LCD_ShowChar(200, 300, '+', 16, 0x80);
    LCD_ShowString(196, 320, 16, 16, 16, (u8 *)"K0");
}

//  显示不断更新的ADC1_Volt值
void LCD_Show_Measured_Volt(double ADC1_Volt)
{
    POINT_COLOR = RED;
    LCD_ShowxNum(200, 130, (int)ADC1_Volt, 2, 16, 0X80);
    LCD_ShowChar(216, 130, '.', 16, 0X80);
    LCD_ShowxNum(220, 130, (int)((ADC1_Volt - (int)ADC1_Volt) * 1000), 3, 16, 0X80);
    LCD_ShowChar(248, 130, 'V', 16, 0X80);
}

//  显示当前模式，0为电压环， 1为电流环
void LCD_Show_Mode(u8 Mode)
{
    if (Mode == 0) {
        POINT_COLOR = BROWN;
        LCD_ShowString(80, 240, 200, 16, 16, (u8 *)"Voltage Closed Loop");
    }
    if (Mode == 1) {
        POINT_COLOR = BRRED;
        LCD_ShowString(80, 240, 200, 16, 16, (u8 *)"Current Closed Loop");
    }
}

//  显示电压环时设定电压值
void LCD_Show_Set_Volt(float Uo)
{
    // 对电压值进行四舍五入
    int roundedUo = (int)(Uo * 10 + 0.5f);

    // 计算整数和小数部分
    int integerPart = roundedUo / 10;
    int decimalPart = roundedUo % 10;

    POINT_COLOR = BROWN;
    LCD_ShowxNum(120, 310, integerPart, 2, 16, 0X80);
    LCD_ShowChar(136, 310, '.', 16, 0X80);
    LCD_ShowxNum(140, 310, decimalPart, 1, 16, 0X80);
    LCD_ShowChar(150, 310, 'V', 16, 0X80);
}

//  显示输出状态, 0不输出， 1输出
void LCD_Show_Output_State(u8 State)
{
    if (State == 0) {
        LCD_Fill(90, 350, 190, 384, GRAY);
        POINT_COLOR = RED;
        LCD_ShowString(100, 360, 100, 16, 16, (u8 *)"OUTPUT KUP");
    }
    if (State == 1) {
        LCD_Fill(90, 350, 190, 384, YELLOW);
        POINT_COLOR = MAGENTA;
        LCD_ShowString(100, 360, 100, 16, 16, (u8 *)"OUTPUT KUP");
    }
}