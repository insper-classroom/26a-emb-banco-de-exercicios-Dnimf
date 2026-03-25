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

volatile int flag_alarm_y;
volatile int flag_alarm_b;
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

int64_t alarm_callback_y(alarm_id_t id, void *user_data)
{
    flag_alarm_y = 1;
    return 0;
}
int64_t alarm_callback_b(alarm_id_t id, void *user_data)
{
    flag_alarm_b = 1;
    return 0;
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
    if (!add_repeating_timer_ms((1000 / 5),
                                timer_callback_y,
                                NULL,
                                &timer_y))
    {
        printf("Failed to add timer\n");
    }
    repeating_timer_t timer_b;
    if (!add_repeating_timer_ms((1000 / 2),
                                timer_callback_b,
                                NULL,
                                &timer_b))
    {
        printf("Failed to add timer\n");
    }
    alarm_id_t alarm_1;
    alarm_id_t alarm_2;
    // alarm_id_t alarm_3;
    // alarm_id_t alarm_2;
    int led_state_y = 0;
    int led_state_b = 0;
    int pode_y = 0;
    int pode_b = 0;
    int tem_b_dps = 0;
    int tem_y_dps = 0;

    while (1)
    {
        if (flag_btn_y)
        {
            pode_y = 1;
            alarm_1 = add_alarm_in_ms(1000, alarm_callback_y, NULL, false);
            flag_btn_y = 0;
            tem_b_dps = 1;
        }

        if (flag_btn_b)
        {
            pode_b = 1;
            alarm_1 = add_alarm_in_ms(2000, alarm_callback_b, NULL, false);
            flag_btn_b = 0;
            tem_y_dps = 1;
        }
        if (flag_alarm_y)
        {
            pode_y = 0;

            led_state_y = 0;
            gpio_put(LED_PIN_Y, led_state_y);
            if (tem_b_dps)
            {
                alarm_2 = add_alarm_in_ms(2000, alarm_callback_b, NULL, false);
                pode_b = 1;
                tem_b_dps = 0;
            }
            flag_alarm_y = 0;
        }

        if (flag_alarm_b)
        {
            pode_b = 0;
            led_state_b = 0;
            gpio_put(LED_PIN_B, led_state_b);
            if (tem_y_dps)
            {
                alarm_2 = add_alarm_in_ms(1000, alarm_callback_y, NULL, false);
                pode_y = 1;
                tem_y_dps = 0;
            }
            flag_alarm_b = 0;
        }

        if (flag_led_y & pode_y)
        {
            led_state_y = !led_state_y;
            gpio_put(LED_PIN_Y, led_state_y);
            flag_led_y = 0;
        }

        if (flag_led_b & pode_b)
        {
            led_state_b = !led_state_b;
            gpio_put(LED_PIN_B, led_state_b);
            flag_led_b = 0;
        }
    }

    return 0;
}