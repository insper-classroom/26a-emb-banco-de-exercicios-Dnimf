
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "hardware/adc.h"

const int PIN_LED_Y = 5;
const int PIN_LED_B = 9;
const int PIN_BUTTON = 28;

volatile int flag_led;
volatile int flag_y;
volatile int flag_b;
volatile int flag_timer;

void btn_callback(uint gpio, uint32_t events)
{
    if (events == 0x04)
    {
        flag_led = 1;
    }
}

bool timer_callback_y(repeating_timer_t *rt)
{
    flag_y = 1;
    return true;
}
bool timer_callback_b(repeating_timer_t *rt)
{
    flag_b = 1;
    return true;
}

int64_t alarm_callback(alarm_id_t id, void *user_data)
{
    flag_timer =1;
    return 0;
}

int main()
{
    stdio_init_all();
    gpio_init(PIN_LED_Y);
    gpio_init(PIN_LED_B);

    gpio_init(PIN_BUTTON);

    gpio_set_dir(PIN_LED_Y, GPIO_OUT);
    gpio_set_dir(PIN_LED_B, GPIO_OUT);

    gpio_set_dir(PIN_BUTTON, GPIO_IN);
    gpio_pull_up(PIN_BUTTON);

    gpio_set_irq_enabled_with_callback(PIN_BUTTON, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &btn_callback);

    repeating_timer_t timer_y;
    repeating_timer_t timer_b;
    alarm_id_t alarm;


    int led_y_status = 0;
    int led_b_status = 0;
    while (true)
    {
        if (flag_led)
        {
            led_y_status = !led_y_status;
            led_b_status = !led_b_status;
            gpio_put(PIN_LED_Y, led_y_status);
            gpio_put(PIN_LED_B, led_b_status);

            if (!add_repeating_timer_ms(500,
                                        timer_callback_y,
                                        NULL,
                                        &timer_y))
            {
                printf("Failed to add timer\n");
            }

            if (!add_repeating_timer_ms(150,
                                        timer_callback_b,
                                        NULL,
                                        &timer_b))
            {
                printf("Failed to add timer\n");
            }
            alarm = add_alarm_in_ms(5000, alarm_callback, NULL, false);
            flag_led = 0;
        }
    
    if(flag_y){
        led_y_status = !led_y_status;
        gpio_put(PIN_LED_Y, led_y_status);
        flag_y = 0;
    }

    if(flag_b){
        led_b_status = !led_b_status;
        gpio_put(PIN_LED_B, led_b_status);
        flag_b = 0;
    }

    if(flag_timer){
        cancel_repeating_timer(&timer_y);
        cancel_repeating_timer(&timer_b);
        led_y_status = 0;
        led_b_status = 0;
        gpio_put(PIN_LED_Y, 0);
        gpio_put(PIN_LED_B, 0);
        flag_y = 0;
        flag_timer = 0;
    }
    }
    return 1;
}
