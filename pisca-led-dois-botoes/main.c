#include <stdio.h>

#include "hardware/adc.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "pico/stdlib.h"

const int PIN_LED_G = 5;
const int PIN_LED_Y = 9;
const int PIN_BUTTON_G = 28;
const int PIN_BUTTON_Y = 26;

volatile int flag_b_g;
volatile int flag_b_y;

volatile int flag_l_g;
volatile int flag_l_y;

volatile int flag_alarm_g;
volatile int flag_alarm_y;

void btn_callback(uint gpio, uint32_t events)
{
    if (events == 0x4)
    {
        if (gpio == PIN_BUTTON_G)
        {
            flag_b_g = 1;
        }
        if (gpio == PIN_BUTTON_Y)
        {
            flag_b_y = 1;
        }
    }
}

bool timer_g_callback(repeating_timer_t *rt)
{
    flag_l_g = 1;
    return true;
}
bool timer_y_callback(repeating_timer_t *rt)
{
    flag_l_y = 1;
    return true;
}

int64_t alarm_callback_g(alarm_id_t id, void *user_data)
{
    flag_alarm_g = 1;
    return 0;
}
int64_t alarm_callback_y(alarm_id_t id, void *user_data)
{
    flag_alarm_y = 1;
    return 0;
}

int main()
{
    stdio_init_all();
    gpio_init(PIN_LED_G);
    gpio_init(PIN_LED_Y);

    gpio_init(PIN_BUTTON_G);
    gpio_init(PIN_BUTTON_Y);

    gpio_set_dir(PIN_LED_G, GPIO_OUT);
    gpio_set_dir(PIN_LED_Y, GPIO_OUT);

    gpio_set_dir(PIN_BUTTON_G, GPIO_IN);
    gpio_set_dir(PIN_BUTTON_Y, GPIO_IN);

    gpio_pull_up(PIN_BUTTON_G);
    gpio_pull_up(PIN_BUTTON_Y);

    gpio_set_irq_enabled_with_callback(PIN_BUTTON_G, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &btn_callback);
    gpio_set_irq_enabled(PIN_BUTTON_Y, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);

    repeating_timer_t timer_g;
    repeating_timer_t timer_y;

    alarm_id_t alarm_g;
    alarm_id_t alarm_y;

    int led_g_status = 1;
    int led_y_status = 1;

    int alarm_g_up = 0;
    int alarm_y_up = 0;
    while (true)
    {

        if (flag_b_g)
        {

            if (!add_repeating_timer_ms(200,
                                        timer_g_callback,
                                        NULL,
                                        &timer_g))
            {
                printf("Failed to add timer\n");
            }
            alarm_g = add_alarm_in_ms(1000, alarm_callback_g, NULL, false);
            alarm_g_up = 1;
            gpio_put(PIN_LED_G, led_g_status);
            flag_b_g = 0;
        }

        if (flag_b_y)
        {
            // gpio_put(PIN_LED_Y, 1);
            if (!add_repeating_timer_ms(500,
                                        timer_y_callback,
                                        NULL,
                                        &timer_y))
            {
                printf("Failed to add timer\n");
            }
            alarm_y = add_alarm_in_ms(2000, alarm_callback_y, NULL, false);
            gpio_put(PIN_LED_Y, led_y_status);
            alarm_y_up = 1;
            flag_b_y = 0;
        }
        if (flag_l_g)
        {
            led_g_status = !led_g_status;
            gpio_put(PIN_LED_G, led_g_status);
            flag_l_g = 0;
        }

        if (flag_l_y)
        {
            led_y_status = !led_y_status;
            gpio_put(PIN_LED_Y, led_y_status);
            flag_l_y = 0;
        }

        if (flag_alarm_g)
        {
            led_g_status = 0;
            gpio_put(PIN_LED_G, led_g_status);
            cancel_repeating_timer(&timer_g);
            if (alarm_y_up)
            {
                cancel_repeating_timer(&timer_y);
                cancel_alarm(alarm_y);
                led_y_status = 0;
                gpio_put(PIN_LED_Y, led_y_status);
                alarm_y_up =0;
            }
            alarm_g_up = 0;
            flag_alarm_g = 0;
        }

        if (flag_alarm_y)
        {
            led_y_status = 0;
            gpio_put(PIN_LED_Y, led_y_status);
            cancel_repeating_timer(&timer_y);
            if (alarm_g_up)
            {
                cancel_repeating_timer(&timer_g);
                cancel_alarm(alarm_g);
                led_g_status = 0;
                gpio_put(PIN_LED_G, led_g_status);
                alarm_g_up =0;
            }
            alarm_y_up = 0;
            flag_alarm_y = 0;
        }
    }
}

// #include <stdio.h>

// #include "hardware/adc.h"
// #include "hardware/gpio.h"
// #include "hardware/timer.h"
// #include "pico/stdlib.h"

// const int BTN_PIN_G = 28;
// const int BTN_PIN_Y = 26;

// const int LED_PIN_G = 5;
// const int LED_PIN_Y = 9;
// const int LED_PIN_R = 13;

// volatile int btn_f = 0;
// volatile int g_timer_g = 0;
// volatile int g_timer_y = 0;
// volatile int g_timer_r = 0;
// volatile int g_fired_g = 0;
// volatile int g_fired_y = 0;

// void btn_callback(uint gpio, uint32_t events) {
//     if (events == 0x4) {  // fall edge
//         btn_f = gpio;
//     } else if (events == 0x8) {  // rise edge
//     }
// }

// bool timer_g_callback(repeating_timer_t *rt) {
//     g_timer_g = 1;
//     return true;  // keep repeating
// }

// bool timer_y_callback(repeating_timer_t *rt) {
//     g_timer_y = 1;
//     return true;  // keep repeating
// }

// bool timer_r_callback(repeating_timer_t *rt) {
//     g_timer_r = 1;
//     return true;  // keep repeating
// }

// int64_t alarm_g_callback(alarm_id_t id, void *user_data) {
//     g_fired_g = 1;
//     return 0;
// }

// int64_t alarm_y_callback(alarm_id_t id, void *user_data) {
//     g_fired_y = 1;
//     return 0;
// }

// int main() {
//     stdio_init_all();

//     gpio_init(BTN_PIN_G);
//     gpio_set_dir(BTN_PIN_G, GPIO_IN);
//     gpio_pull_up(BTN_PIN_G);
//     gpio_set_irq_enabled_with_callback(BTN_PIN_G, GPIO_IRQ_EDGE_FALL, true,
//                                        &btn_callback);

//     gpio_init(BTN_PIN_Y);
//     gpio_set_dir(BTN_PIN_Y, GPIO_IN);
//     gpio_pull_up(BTN_PIN_Y);
//     gpio_set_irq_enabled_with_callback(BTN_PIN_Y, GPIO_IRQ_EDGE_FALL, true,
//                                        &btn_callback);

//     gpio_init(LED_PIN_G);
//     gpio_set_dir(LED_PIN_G, GPIO_OUT);

//     gpio_init(LED_PIN_Y);
//     gpio_set_dir(LED_PIN_Y, GPIO_OUT);

//     gpio_init(LED_PIN_R);
//     gpio_set_dir(LED_PIN_R, GPIO_OUT);

//     repeating_timer_t timer_g;
//     if (!add_repeating_timer_ms(100, timer_g_callback, NULL, &timer_g)) {
//         printf("Failed to add timer\n");
//     }

//     repeating_timer_t timer_y;
//     if (!add_repeating_timer_ms(200, timer_y_callback, NULL, &timer_y)) {
//         printf("Failed to add timer\n");
//     }

//     int led_g = 0;
//     int led_y = 0;

//     int alarm_enable_g = 0;
//     int alarm_enable_y = 0;

//     alarm_id_t alarm_g = NULL;
//     alarm_id_t alarm_y = NULL;

//     while (1) {
//         if (g_timer_g && alarm_enable_g) {
//             led_g = !led_g;
//             gpio_put(LED_PIN_G, led_g);
//             g_timer_g = 0;
//         } else if (alarm_enable_g == 0) {
//             gpio_put(LED_PIN_G, 0);
//         }

//         if (g_timer_y && alarm_enable_y) {
//             led_y = !led_y;
//             gpio_put(LED_PIN_Y, led_y);
//             g_timer_y = 0;
//         } else if (alarm_enable_y == 0) {
//             gpio_put(LED_PIN_Y, 0);
//         }

//         if (btn_f == BTN_PIN_G) {
//             if (alarm_enable_g == 0) {
//                 alarm_g = add_alarm_in_ms(1000, alarm_g_callback, NULL, false);
//                 alarm_enable_g = 1;
//             }
//             btn_f = 0;
//         }

//         if (btn_f == BTN_PIN_Y) {
//             if (alarm_enable_y == 0) {
//                 alarm_y = add_alarm_in_ms(2000, alarm_y_callback, NULL, false);
//                 alarm_enable_y = 1;
//             }
//             btn_f = 0;
//         }

//         if (g_fired_g == 1 ||g_fired_y == 1 ) {
//             g_fired_g = 0;
//             alarm_enable_g = 0;

//             g_fired_y = 0;
//             alarm_enable_y = 0;
//             cancel_alarm(alarm_y);
//             cancel_alarm(alarm_g);
//         }

//     }

//     return 0;
// }
