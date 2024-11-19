#ifndef LCD_UTILS_H
#define LCD_UTILS_H
#include "lcd.h"

void LCD_Display_Init(void);
void LCD_Show_Measured_Volt(double ADC1_Volt);
void LCD_Show_Mode(u8 Mode);
void LCD_Show_Set_Volt(float Uo);
void LCD_Show_Output_State(u8 State);
void LCD_Show_Set_Current(float Io);
void LCD_Show_Measured_Current(double ADC2_Current);

#endif // LCD_UTILS_H