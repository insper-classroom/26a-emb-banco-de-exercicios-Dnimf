/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <string.h> 
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "hardware/irq.h"

const int PIN_LED = 5;
const int PIN_BUTTON = 28; 

volatile int flag_fall;
volatile int flag_rise;
volatile int flag_timer;
void btn_callback(uint gpio, uint32_t events)
{
    if(events == 0x04){
        flag_fall = 1;
    }
    else if( events == 0x08){
        flag_rise = 1;
    }
}

bool timer_callback(repeating_timer_t *rt)
{
    flag_timer = 1;
    return true;
}

int main() {
    stdio_init_all();
    gpio_init(PIN_LED);
    gpio_init(PIN_BUTTON);
    
    gpio_set_dir(PIN_LED, GPIO_OUT);
    gpio_set_dir(PIN_BUTTON, GPIO_IN);
    gpio_pull_up(PIN_BUTTON);

    gpio_set_irq_enabled_with_callback(PIN_BUTTON, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &btn_callback);

    repeating_timer_t timer;
    int inicio;
    int fim;
    int prepara;
    int delta;
    int rodando;
    int led_status = 0;
    while (true) {
        if(flag_fall){
            if(rodando){
                gpio_put(PIN_LED, 0);
                cancel_repeating_timer(&timer);
                rodando = 0;
            }
            inicio = to_ms_since_boot(get_absolute_time());
            flag_fall = 0;
        }

        if(flag_rise){
            fim = to_ms_since_boot(get_absolute_time());
            // printf("fim %d", fim);
            prepara = 1;
            flag_rise = 0;
        }
        if(prepara){
            delta = fim - inicio;
            if (!add_repeating_timer_ms(delta,
                                        timer_callback,
                                        NULL,
                                        &timer))
            {
                printf("Failed to add timer\n");
            }
            led_status = 1;
            gpio_put(PIN_LED, led_status);
            prepara = 0;
            rodando = 1;
        }
        if(flag_timer){
            led_status = !led_status;
            gpio_put(PIN_LED, led_status);
            flag_timer = 0;
        }

    }

    return 0;
}