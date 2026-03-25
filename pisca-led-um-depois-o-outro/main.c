#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "hardware/adc.h"

const int LED_PIN_Y = 10;
const int LED_PIN_B = 14;

const int BTN_PIN_Y = 26;
const int BTN_PIN_B = 19;

volatile int flag_btn_y;
volatile int flag_btn_b;

volatile int flag_led_y;
volatile int flag_led_b;

void btn_callback(uint gpio, uint32_t events)
{
    if (events == 0x04)
    {
        if (gpio == BTN_PIN_Y)
        {
            flag_btn_y = 1;
        }
        else if (gpio == BTN_PIN_B)
        {
            flag_btn_b = 1;
        }
    }
}

bool timer_callback_y(repeating_timer_t *rt)
{
    flag_led_y = 1;
    return true;
}
bool timer_callback_b(repeating_timer_t *rt)
{
    flag_led_b = 1;
    return true;
}
int main()
{
    stdio_init_all();

    gpio_init(LED_PIN_Y);
    gpio_init(LED_PIN_B);

    gpio_init(BTN_PIN_Y);
    gpio_init(BTN_PIN_B);

    gpio_set_dir(LED_PIN_Y, GPIO_OUT);
    gpio_set_dir(LED_PIN_B, GPIO_OUT);

    gpio_set_dir(BTN_PIN_Y, GPIO_IN);
    gpio_set_dir(BTN_PIN_B, GPIO_IN);
    gpio_pull_up(BTN_PIN_Y);
    gpio_pull_up(BTN_PIN_B);

    gpio_set_irq_enabled_with_callback(BTN_PIN_Y, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &btn_callback);
    gpio_set_irq_enabled(BTN_PIN_B, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);

    repeating_timer_t timer_y;
    if (!add_repeating_timer_ms((1000/5),
                                timer_callback_y,
                                NULL,
                                &timer_y))
    {
        printf("Failed to add timer\n");
    }
    repeating_timer_t timer_b;
    if (!add_repeating_timer_ms((1000/2),
                                timer_callback_b,
                                NULL,
                                &timer_b))
    {
        printf("Failed to add timer\n");
    }
    alarm_id_t alarm_1;
    alarm_id_t alarm_2;

    while (1)
    {
        if (flag_btn_y)
        {
        }
    }

    return 0;
}