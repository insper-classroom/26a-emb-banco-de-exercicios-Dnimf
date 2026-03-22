#include <stdio.h>

#include "hardware/adc.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "pico/stdlib.h"

const int PIN_BUZZER = 10;
volatile int g_timer_0 = 1;
volatile int flag_toca = 0;
bool timer_0_callback(repeating_timer_t *rt) {
    g_timer_0 = 1;
    return true; // keep repeating
}
bool timer_1_callback(repeating_timer_t *rt) {
    flag_toca = 1;
    return true; // keep repeating
}
    int buzz_status = 0;
int main() {
    stdio_init_all();
    gpio_init(PIN_BUZZER);
    gpio_set_dir(PIN_BUZZER, GPIO_OUT);

    repeating_timer_t timer_0;
    repeating_timer_t timer_1;

    if (!add_repeating_timer_ms(50, 
                                timer_0_callback,
                                NULL, 
                                &timer_0)) {
        printf("Failed to add timer\n");
    }
    int cont = 1;
    while (1) {
       if(g_timer_0){
        cancel_repeating_timer(&timer_1);       
        if (!add_repeating_timer_us(cont*5, 
                                    timer_1_callback,
                                    NULL, 
                                    &timer_1)) {
            printf("Failed to add timer\n");
        }
        cont++;
        g_timer_0=0;
       }
       if(flag_toca){
        buzz_status = ! buzz_status;
        gpio_put(PIN_BUZZER, buzz_status);
        flag_toca=0;
       }
    }

    return 0;
}