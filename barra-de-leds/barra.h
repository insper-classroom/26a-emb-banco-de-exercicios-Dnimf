#ifndef BARRA_H
#define BARRA_H

#include "pico/stdlib.h"
#include <stdio.h>
#include "hardware/gpio.h"

#define PIN_BARRA_1 2
#define PIN_BARRA_2 3
#define PIN_BARRA_3 4
#define PIN_BARRA_4 5
#define PIN_BARRA_5 6

void bar_init();
void bar_display(int val);
#endif