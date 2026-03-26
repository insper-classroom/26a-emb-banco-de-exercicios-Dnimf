#include "pico/stdlib.h"
#include <stdio.h>

#include "hardware/gpio.h"

const int LED_PIN_P = 26;
const int LED_PIN_G = 9;
const int LED_PIN_R = 2;

const int BTN_PIN_G = 28;
const int BTN_PIN_B = 27;
const int BTN_PIN_Y = 21;
const int BTN_PIN_W = 17;

volatile int flag_purple;
volatile int flag_red;
volatile int flag_green;

volatile int flag_g;
volatile int flag_b;
volatile int flag_y;
volatile int flag_w;

void btn_callback(uint gpio, uint32_t events)
{
    if(events == 0x04){
        flag_purple = 1;
        if(gpio == BTN_PIN_G){
            flag_g = 1;
        }
        if(gpio == BTN_PIN_B){
            flag_b = 1;
        }
        if(gpio == BTN_PIN_Y){
            flag_y = 1;
        }
        if(gpio == BTN_PIN_W){
            flag_w = 1;
        }
    }
}
int main() {
    stdio_init_all();

    gpio_init(LED_PIN_G);
    gpio_init(LED_PIN_R);
    gpio_init(LED_PIN_P);

    gpio_init(BTN_PIN_G);
    gpio_init(BTN_PIN_B);
    gpio_init(BTN_PIN_Y);
    gpio_init(BTN_PIN_W);


    gpio_set_dir(LED_PIN_G, GPIO_OUT);
    gpio_set_dir(LED_PIN_R, GPIO_OUT);
    gpio_set_dir(LED_PIN_P, GPIO_OUT);

    gpio_set_dir(BTN_PIN_G, GPIO_IN);
    gpio_set_dir(BTN_PIN_B, GPIO_IN);
    gpio_set_dir(BTN_PIN_Y, GPIO_IN);
    gpio_set_dir(BTN_PIN_W, GPIO_IN);
    gpio_pull_up(BTN_PIN_G);
    gpio_pull_up(BTN_PIN_B);
    gpio_pull_up(BTN_PIN_Y);
    gpio_pull_up(BTN_PIN_W);

    gpio_set_irq_enabled_with_callback(BTN_PIN_G, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &btn_callback);
    gpio_set_irq_enabled(BTN_PIN_B, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(BTN_PIN_Y, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(BTN_PIN_W, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);

    int cont =0;
    int  senha[5];
    int  temp[5];
    int definida = 0;
    while (1) {
        // printf("Hello, world!\n");
        if(flag_purple){
            gpio_put(LED_PIN_P,1);
            sleep_ms(100);
            gpio_put(LED_PIN_P,0);
            flag_purple =0;
            // sleep_ms(10)
        }
        if(flag_g){
            if(!definida & (cont<4)){
                senha[cont] = 0;
                cont++;
            }else if(definida && (cont<4)){
                temp[cont] = 0;
                cont++;
            }
            flag_g = 0;
        }
        if(flag_b){
            if(!definida & (cont<4)){
                senha[cont] = 1;
                cont++;
            }else if(definida && (cont<4)){
                temp[cont] = 1;
                cont++;
            }
            flag_b = 0;
        }
        if(flag_y){
            if(!definida & (cont<4)){
                senha[cont] = 2;
                cont++;
            }else if(definida && (cont<4)){
                temp[cont] = 2;
                cont++;
            }
            flag_y = 0;
        }
        if(flag_w){
            if(!definida & (cont<4)){
                senha[cont] = 3;
                cont++;
            }else if(definida && (cont<4)){
                temp[cont] = 3;
                cont++;
            }
            flag_w = 0;
        }
        if(cont == 4){

            int acertou = 1;
            if(definida){
                for(int i = 0; i<4; i++){
                    // printf("%d, %d \n", senha[i], temp[i]);
                    if(senha[i]!=temp[i]){
                        acertou = 0;
                    }
                }
            }
            if(!definida){
                definida = 1;
                // printf("senha definida");
            }
            if(acertou){
                gpio_put(LED_PIN_G,1);
                sleep_ms(300);
                gpio_put(LED_PIN_G,0);
            }

            if(!acertou){
                gpio_put(LED_PIN_R,1);
                sleep_ms(300);
                gpio_put(LED_PIN_R,0);
            }
            cont = 0;
        }
        // sleep_ms(1000);
    }
}
