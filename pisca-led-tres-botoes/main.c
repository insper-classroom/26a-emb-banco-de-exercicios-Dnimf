#include "pico/stdlib.h"
#include <stdio.h>
#include "hardware/gpio.h"

const int PIN_LED_Y =  6;
const int PIN_LED_B = 10;
const int PIN_LED_G = 14;

const int PIN_BUTTON_Y = 28;
const int PIN_BUTTON_B = 22;
const int PIN_BUTTON_G = 18;

volatile int flag_y;
volatile int flag_b;
volatile int flag_g_fall;
volatile int flag_g_rise;


void btn_callback(uint gpio, uint32_t events){
  if(events == 0x04){
    if(gpio == PIN_BUTTON_Y){
      flag_y = 1;
    } else if(gpio == PIN_BUTTON_G){
      flag_g_fall = 1;
    }
  }
  else if(events == 0x08){
    if(gpio == PIN_BUTTON_B){
        flag_b = 1;
    }else if(gpio == PIN_BUTTON_G){
      flag_g_rise = 1;
    }
  }
}

int main() {
  stdio_init_all();
  gpio_init(PIN_LED_Y);
  gpio_init(PIN_LED_B);
  gpio_init(PIN_LED_G);
  
  gpio_init(PIN_BUTTON_Y);
  gpio_init(PIN_BUTTON_B);
  gpio_init(PIN_BUTTON_G);

  gpio_set_dir(PIN_LED_Y, GPIO_OUT);
  gpio_set_dir(PIN_LED_B, GPIO_OUT);
  gpio_set_dir(PIN_LED_G, GPIO_OUT);

  gpio_set_dir(PIN_BUTTON_Y, GPIO_IN);
  gpio_set_dir(PIN_BUTTON_B, GPIO_IN);
  gpio_set_dir(PIN_BUTTON_G, GPIO_IN);

  gpio_pull_up(PIN_BUTTON_Y);
  gpio_pull_up(PIN_BUTTON_B);
  gpio_pull_up(PIN_BUTTON_G);

  gpio_set_irq_enabled_with_callback(PIN_BUTTON_Y, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &btn_callback);
  gpio_set_irq_enabled(PIN_BUTTON_B, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);
  gpio_set_irq_enabled(PIN_BUTTON_G, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true);

  int ativou_y = 0;
  int ativou_b = 0;
  int ativou_g = 0;

  int led = 0;
  while (true) {
      if(flag_y){
        ativou_y = !ativou_y;
        if(!ativou_y){
          gpio_put(PIN_LED_Y, 0);
        }
        flag_y = 0;
      }
      if(flag_b){
        ativou_b = !ativou_b;
        if(!ativou_b){
          gpio_put(PIN_LED_B, 0);
        }
        flag_b = 0;
      }
      if(flag_g_fall){
        ativou_g = 1;
        flag_g_fall = 0;
      }
      if(flag_g_rise){
        ativou_g = 0;
        gpio_put(PIN_LED_G, 0);
        flag_g_rise = 0;
      }
    
      if(ativou_y){
        gpio_put(PIN_LED_Y, led);
      }

      if(ativou_b){
        gpio_put(PIN_LED_B, led);
      }

      if(ativou_g){
        gpio_put(PIN_LED_G, led);
      }
      led = !led;
      sleep_ms(200);
  }
}