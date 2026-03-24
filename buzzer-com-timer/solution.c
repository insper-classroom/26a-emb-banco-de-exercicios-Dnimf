#include <stdio.h>

#include "hardware/adc.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "pico/stdlib.h"

const int BUZZER_PIN = 6;
volatile int f_buzzer = 0;

bool timer_0_callback(repeating_timer_t* rt) {
    if (f_buzzer) {
        gpio_put(BUZZER_PIN, 1);
    } else {
        gpio_put(BUZZER_PIN, 0);
    }

    f_buzzer = !f_buzzer;

    return true;  // keep repeating
}

int main() {
    stdio_init_all();
    gpio_init(BUZZER_PIN);
    gpio_set_dir(BUZZER_PIN, GPIO_OUT);

    repeating_timer_t timer_0;

    adc_init();
    adc_gpio_init(28);
    adc_selec
    
    t_input(2);
    int freq_0 = 0;
    int freq_0_old = 0;

    while (1) {
        int freq_0 = adc_read();
        sleep_ms(200);

        if (freq_0 != freq_0_old && freq_0 < 6000) {
            freq_0_old = freq_0;

            if (freq_0 > 20) {
                float f = (4095*1000000.0) / (4095-freq_0);
                printf("%f \n", f);
                printf("Freq %d\n", (int)(10000000000.0/f));

                if (!add_repeating_timer_us((int)f, timer_0_callback, NULL,
                                            &timer_0)) {
                    printf("Failed to add timer\n");
                }
            } else {
                printf("cancel\n");
                cancel_repeating_timer(&timer_0);
            }
        }
    }

    return 0;
}
