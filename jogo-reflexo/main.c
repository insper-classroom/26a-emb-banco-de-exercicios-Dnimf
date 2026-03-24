#include <stdio.h>

#include "hardware/gpio.h"
#include "pico/stdlib.h"

const int PIN_BUTTON_G = 28;
const int PIN_BUTTON_Y = 26;
const int PIN_BUTTON_R = 20;

const int PIN_LED_G = 5;
const int PIN_LED_Y = 9;
const int PIN_LED_R = 13;
const int lista[] = {1, 0, 2, 1, 0, 1, 2, 1, 0, 1};


volatile int flag_g = 0;
volatile int flag_y = 0;
volatile int flag_r = 0;


void btn_callback(uint gpio, uint32_t events){
    if(events == 0x4){
        if(gpio == PIN_BUTTON_G){
            flag_g =1;
        }
        if(gpio == PIN_BUTTON_Y){
            flag_y=1;
        }
        if(gpio == PIN_BUTTON_R){
            flag_r=1;
        }
    }
}
void liga(int numero){
    if(numero == 0){
        gpio_put(PIN_LED_G, 1);
        sleep_ms(300);
        gpio_put(PIN_LED_G, 0);
    }

    if(numero == 1){
        gpio_put(PIN_LED_Y, 1);
        sleep_ms(300);
        gpio_put(PIN_LED_Y, 0);
    }

    if(numero == 0){
        gpio_put(PIN_LED_Y, 1);
        sleep_ms(300);
        gpio_put(PIN_LED_Y, 0);
    }
}
int main() {
    stdio_init_all();
    gpio_init(PIN_BUTTON_G);
    gpio_init(PIN_BUTTON_Y);
    gpio_init(PIN_BUTTON_R);

    gpio_init(PIN_LED_G);
    gpio_init(PIN_LED_G);
    gpio_init(PIN_LED_G);

    gpio_set_dir(PIN_BUTTON_G,GPIO_IN);
    gpio_set_dir(PIN_BUTTON_Y,GPIO_IN);
    gpio_set_dir(PIN_BUTTON_R,GPIO_IN);
    gpio_pull_up(PIN_BUTTON_G);
    gpio_pull_up(PIN_BUTTON_Y);
    gpio_pull_up(PIN_BUTTON_R);

    gpio_set_dir(PIN_LED_G, GPIO_OUT);
    gpio_set_dir(PIN_LED_Y, GPIO_OUT);
    gpio_set_dir(PIN_LED_R, GPIO_OUT);

    gpio_set_irq_enabled_with_callback(PIN_BUTTON_G, GPIO_IRQ_EDGE_RISE|GPIO_IRQ_EDGE_FALL , true, &btn_callback);
    gpio_set_irq_enabled(PIN_BUTTON_Y, GPIO_IRQ_EDGE_RISE|GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(PIN_BUTTON_R, GPIO_IRQ_EDGE_RISE|GPIO_IRQ_EDGE_FALL , true);
    int flag_start = 0;
    int flag_nivel = 0;
    int nivel = 0;
    int cont = 0;
    while (true) {
        if(flag_g){
            if(!flag_start){
                flag_start = 1;
                flag_nivel =1;
            }else{
                if(cont<nivel){
                    if(lista[cont] == 0){
                        cont++;
                    }
                    else{
                        int pnts = nivel+1;
                        printf("Points %d\n", pnts);
                        // cont = 0;
                        nivel =0;
                        flag_start = 0;
                    }
                }
                else if(cont == nivel){
                    if(lista[cont] == 0){
                        cont = 0;
                        nivel++;
                        flag_nivel = 1;
                    }
                    else{
                        int pnts = nivel+1;
                        printf("Points %d\n", pnts);
                        cont = 0;
                        nivel =0;
                        flag_start = 0;
                    }    
                }
            }
            flag_g=0;
        }
        if(flag_nivel){
            cont = 0;
            if(nivel == 10){
                int pnts = nivel+1;
                printf("Points %d\n", pnts);
                nivel =0;
                flag_start = 0;
            }
            else{
                for(int i = 0; i<=nivel; i++){
                    liga(lista[i]);
                    sleep_ms(300);
                }
            }
            flag_nivel = 0;
        }

        if(flag_y){
            if(flag_start){
                if(cont<nivel){
                    if(lista[cont] == 1){
                        cont++;
                    }
                    else{
                        int pnts = nivel+1;
                        printf("Points %d\n", pnts);
                        cont = 0;
                        nivel =0;
                        flag_start = 0;
                    }
                }
                else if(cont == nivel){
                    if(lista[cont] == 1){
                        // cont = 0;
                        nivel++;
                        flag_nivel = 1;
                    }
                    else{
                        int pnts = nivel+1;
                        printf("Points %d\n", pnts);
                        cont = 0;
                        nivel =0;
                        flag_start = 0;
                    }    
                }
            }
            sleep_ms(100);
            flag_y=0;
        }

        if(flag_r){
            if(flag_start){
                if(cont<nivel){
                    if(lista[cont] == 2){
                        cont++;
                    }
                    else{
                        int pnts = nivel+1;
                        printf("Points %d\n", pnts);
                        cont = 0;
                        nivel =0;
                        flag_start = 0;
                    }
                }
                else if(cont == nivel){
                    if(lista[cont] == 2){

                        nivel++;
                        flag_nivel = 1;
                    }
                    else{
                        int pnts = nivel+1;
                        printf("Points %d\n", pnts);
                        cont = 0;
                        nivel =0;
                        flag_start = 0;
                    }    
                }
            }
            sleep_ms(100);
            flag_r=0;
        }
    }
}
