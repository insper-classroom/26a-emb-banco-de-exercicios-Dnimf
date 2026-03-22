#include "barra.h"
void bar_init(){
    stdio_init_all();
    gpio_init(PIN_BARRA_1);
    gpio_init(PIN_BARRA_2);
    gpio_init(PIN_BARRA_3);
    gpio_init(PIN_BARRA_4);
    gpio_init(PIN_BARRA_5);

    gpio_set_dir(PIN_BARRA_1, GPIO_OUT);
    gpio_set_dir(PIN_BARRA_2, GPIO_OUT);
    gpio_set_dir(PIN_BARRA_3, GPIO_OUT);
    gpio_set_dir(PIN_BARRA_4, GPIO_OUT);
    gpio_set_dir(PIN_BARRA_5, GPIO_OUT);
}
void bar_display(int val){
    if(val==0){
        gpio_put(PIN_BARRA_1,1);
    }
    if(val==1){
        gpio_put(PIN_BARRA_2,1);
    }
    if(val==2){
        gpio_put(PIN_BARRA_3,1);
    }
    if(val==3){
        gpio_put(PIN_BARRA_3,1);
    }
    if(val==4){
        gpio_put(PIN_BARRA_4,1);
    }
    if(val == 5){
        gpio_put(PIN_BARRA_1,0);
        gpio_put(PIN_BARRA_2,0);
        gpio_put(PIN_BARRA_3,0);
        gpio_put(PIN_BARRA_4,0);
        gpio_put(PIN_BARRA_5,0);
        
    }
}