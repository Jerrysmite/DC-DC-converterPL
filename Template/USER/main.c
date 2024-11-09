#include "main.h"

int main(void)
{
    u32 t = 0;
    uart_init(115200);
    delay_init(168);
    LCD_Init();

    while (1) {
        printf("t:%d\r\n", t);
        delay_ms(500);
        t++;
    }
}
