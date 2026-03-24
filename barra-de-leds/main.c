#include "pico/stdlib.h"
#include <stdio.h>
#include "hardware/gpio.h"
#include "barra.h"
const int PIN_BUTTON_G = 22;
const int PIN_BUTTON_B = 28;
volatile int flag_cont = 0;
volatile int flag_dir = 0;
void btn_callback(uint gpio, uint32_t events){
    if(events == 0x4){
        if(gpio == PIN_BUTTON_G){
            flag_cont = 1;
        }
        if(gpio == PIN_BUTTON_B){
            flag_dir = 0;
        }

    }
    if(events == GPIO_IRQ_EDGE_RISE){
        if(gpio == PIN_BUTTON_B){
            flag_dir = 1;
        }
    }

}

int main() {
    stdio_init_all();
    bar_init();
    gpio_init(PIN_BUTTON_G);
    gpio_init(PIN_BUTTON_B);

    gpio_set_dir(PIN_BUTTON_G,GPIO_IN);
    gpio_set_dir(PIN_BUTTON_B,GPIO_IN);

    gpio_set_irq_enabled_with_callback(PIN_BUTTON_G, GPIO_IRQ_EDGE_RISE|GPIO_IRQ_EDGE_FALL , true, &btn_callback);
    gpio_set_irq_enabled_with_callback(PIN_BUTTON_B, GPIO_IRQ_EDGE_RISE|GPIO_IRQ_EDGE_FALL , true, &btn_callback);

    gpio_pull_up(PIN_BUTTON_G);
    gpio_pull_up(PIN_BUTTON_B);
    int cont = 0;
    while (true) {
        if(flag_cont){
            if(!flag_dir && cont<5){
                printf("%d \n", cont);
                if(cont<0){
                    cont = 0;
                }
                bar_display(cont);
                cont++;
            }
            if(flag_dir && cont>=0){
                bar_display(5);
                printf("%d \n", cont);
                for(int i = (cont-2); i>=0; i--){
                    printf("   %d \n", i);
                    bar_display(i);
                }
                cont--;
            }
            flag_cont = 0;
        }
    }
}
