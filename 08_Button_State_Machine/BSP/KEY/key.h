//
// Created by PC on 2025/9/21.
//

#ifndef __BSP_KEY_H
#define __BSP_KEY_H

#include <stm32h7xx.h>
#include "main.h"
#define KEY_CYCLES_TIME 10  //10ms

#define KEY1 0x01<<0
#define KEY2 0x01<<1

typedef enum {
    IDLE = 0,
    DEBOUNCE_DOWN,
    DOWN,
    DEBOUNCE_UP,
    UP,
    DONE,
    WaitDouble,
}key_state_t;

typedef struct {
    key_state_t key_state;
    uint16_t key_num;
    uint16_t key_time;
    uint8_t is_long_pressed;
    uint8_t is_double_clicked;
}key_t;
typedef struct {
    GPIO_TypeDef *KEY_GPIO_PORT;
    uint16_t KEY_GPIO_PIN;
    GPIO_PinState DOWN_STATE;
}key_config_t;
void key_scan(void);

extern key_t key;

#endif //__BSP_KEY_H