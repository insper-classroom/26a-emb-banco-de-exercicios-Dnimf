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


void btn_callback(uint gpio, uint32_t events)
{
    if(events == 0x04){
        
    }
    else if( events == 0x08){

    }
}

int main() {
    stdio_init_all();
    gpio_init(PIN_LED);
    gpio_init(PIN_BUTTON);
    
    gpio_set_dir(PIN_LED, GPIO_OUT);
    gpio_set_dir(PIN_BUTTON, GPIO_IN);

    gpio_pull_up(PIN_BUTTON);

    gpio_set_irq_enabled_with_callback(PIN_BUTTON, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &btn_callback);

    while (true) {

    }

    return 0;
}