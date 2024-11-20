#include "LCD_Utils.h"

//  静态显示框架
void LCD_Display_Init(void)
{
    POINT_COLOR = BLACK;
    LCD_ShowString(30, 80, 400, 16, 16, (u8 *)"STM32F407ZGT6 BUCK @NitrenePL");
    LCD_ShowString(30, 130, 200, 16, 16, (u8 *)"Measured Volt:");
    LCD_ShowString(30, 150, 200, 16, 16, (u8 *)"Measured Current:");
    LCD_ShowString(30, 200, 400, 16, 16, (u8 *)"Please Change Volt or Current.");
    LCD_ShowString(30, 240, 40, 16, 16, (u8 *)"MODE:");
    LCD_ShowString(30, 270, 200, 16, 16, (u8 *)"(K1 to Change MODE)");
    LCD_ShowString(30, 400, 50, 16, 16, (u8 *)"Duty:");
    LCD_ShowChar(120, 400, '%', 16, 0x80);

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

void LCD_Show_Measured_Current(double ADC2_Current)
{
    POINT_COLOR = BLUE;
    LCD_ShowxNum(200, 150, (int)ADC2_Current, 2, 16, 0X80);
    LCD_ShowChar(216, 150, '.', 16, 0X80);
    LCD_ShowxNum(220, 150, (int)((ADC2_Current - (int)ADC2_Current) * 1000), 3, 16, 0X80);
    LCD_ShowChar(248, 150, 'A', 16, 0X80);
}

//  显示当前模式，0为电压环， 1为电流环
void LCD_Show_Mode(u8 Mode)
{
    if (Mode == 0) {
        LCD_Fill(90, 230, 206, 264, YELLOW);
        LCD_Fill(230, 230, 346, 264, WHITE);
        POINT_COLOR = BLUE;
        LCD_ShowString(100, 240, 200, 16, 16, (u8 *)"Voltage Mode");
        LCD_ShowString(240, 240, 200, 16, 16, (u8 *)"Current Mode");

        LCD_Fill(114, 300, 118, 316, RED);
        LCD_Fill(114, 320, 118, 336, WHITE);
    }
    if (Mode == 1) {
        LCD_Fill(90, 230, 206, 264, WHITE);
        LCD_Fill(230, 230, 346, 264, YELLOW);
        POINT_COLOR = BLUE;
        LCD_ShowString(100, 240, 200, 16, 16, (u8 *)"Voltage Mode");
        LCD_ShowString(240, 240, 200, 16, 16, (u8 *)"Current Mode");

        LCD_Fill(114, 300, 118, 316, WHITE);
        LCD_Fill(114, 320, 118, 336, RED);
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
    LCD_ShowxNum(120, 300, integerPart, 2, 16, 0X80);
    LCD_ShowChar(136, 300, '.', 16, 0X80);
    LCD_ShowxNum(140, 300, decimalPart, 1, 16, 0X80);
    LCD_ShowChar(150, 300, ' ', 16, 0X80);
    LCD_ShowChar(150, 300, 'V', 16, 0X80);
}

//  显示电流环时设定电流值
void LCD_Show_Set_Current(float Io)
{
    // 对电压值进行四舍五入
    int roundedIo = (int)(Io * 10 + 0.5f);

    // 计算整数和小数部分
    int integerPart = roundedIo / 10;
    int decimalPart = roundedIo % 10;

    POINT_COLOR = BROWN;

    LCD_ShowxNum(120, 320, integerPart, 2, 16, 0X80);
    LCD_ShowChar(136, 320, '.', 16, 0X80);
    LCD_ShowxNum(140, 320, decimalPart, 1, 16, 0X80);
    LCD_ShowChar(150, 320, 'A', 16, 0X80);
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

void LCD_Show_Duty(double duty)
{
    POINT_COLOR = RED;
    LCD_ShowxNum(100, 400, (int)(100.0 * duty), 2, 16, 0x80);
}