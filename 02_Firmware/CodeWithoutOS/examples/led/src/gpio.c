
#include "gpio.h"
#include "f1c100s_gpio.h" 
#include "stdio.h"
 


void led_init()
{
    gpio_init(GPIOE, PIN11, GPIO_MODE_OUTPUT, GPIO_PULL_UP, GPIO_DRV_3);
    gpio_init(GPIOE, PIN12, GPIO_MODE_OUTPUT, GPIO_PULL_UP, GPIO_DRV_3);

}

void led_switch(int i, int state)
{
    if(i == LED1){
        if(state == 0){
            gpio_clear(GPIOE, PIN11);        
        }else{
            gpio_set(GPIOE, PIN11);
        }
    }else if(i == LED2)
    {
        if(state == 0){
            gpio_clear(GPIOE, PIN12);        
        }else{
            gpio_set(GPIOE, PIN12);
        }        
    }
    
}

 