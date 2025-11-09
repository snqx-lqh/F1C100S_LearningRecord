#include <string.h>
#include <math.h>
#include "system.h"
#include "f1c100s_gpio.h"
#include "f1c100s_uart.h"
#include "gpio.h"
 
int main(void)
{
    system_init();            // Initialize clocks, mmu, cache, uart, ...
    printf("\r\napp [gpio_exit] run. compiled_time %s %s\r\n", __DATE__, __TIME__);
    led_init();

    while (1)
    {
        led_switch(LED1,0);
        delay_soft(200);
        led_switch(LED1,1);
        delay_soft(200);
    }
}



 