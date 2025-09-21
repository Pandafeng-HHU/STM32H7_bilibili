//
// Created by PC on 2025/9/21.
//
#include "key.h"

key_t key;

static key_config_t key_config[] = {
    {.KEY_GPIO_PORT = KEY1_GPIO_Port,.KEY_GPIO_PIN = KEY1_Pin,.DOWN_STATE = GPIO_PIN_RESET,},
    {.KEY_GPIO_PORT = KEY2_GPIO_Port,.KEY_GPIO_PIN = KEY2_Pin,.DOWN_STATE = GPIO_PIN_SET,},
};
#define double_click_time 300
static uint16_t double_click_timer = double_click_time;

static uint16_t key_get(void) {
    uint16_t key_num = 0;

    for (uint8_t i = 0; i < sizeof(key_config) / sizeof(key_config[0]); ++i) {
        uint8_t temp = 0;
        temp = (HAL_GPIO_ReadPin(key_config[i].KEY_GPIO_PORT, key_config[i].KEY_GPIO_PIN) == key_config[i].DOWN_STATE)?1:0;
        key_num |= (temp<<i);
    }
    return key_num;
}


void key_scan(void) {
    uint16_t key_num = key_get();
    switch (key.key_state) {
        case IDLE: {
            if (key_num ==0) {
                key.key_state = IDLE;
            }else {
                key.key_state = DEBOUNCE_DOWN;
                key.key_time = 0;
            }
        }break;
        case DEBOUNCE_DOWN: {
            if (key_num==0) {
                key.key_state = IDLE;
            }else {
                key.key_state = DOWN;
                key.key_num = key_num;
                key.key_time += KEY_CYCLES_TIME;
            }

        }break;
        case DOWN: {
            if (key_num==0) {
                key.key_state = DEBOUNCE_UP;
                key.key_time += KEY_CYCLES_TIME;
            }else {
                key.key_state = DOWN;
                key.key_num |= key_num;
                key.key_time += KEY_CYCLES_TIME;
                if (key.key_time >= 1000) {
                    key.is_long_pressed = 1;
                }
            }break;
            case DEBOUNCE_UP: {
                if (key_num==0) {
                    key.key_state = UP;
                }else {
                    key.key_state = DOWN;
                    key.key_time += KEY_CYCLES_TIME;
                }
            }break;
            case UP: {
                if (key.is_long_pressed ==1) {
                    key.key_state = DONE;
                }else {
                    double_click_timer = double_click_time;
                    key.key_state = WaitDouble;
                }
            }break;
            case DONE: {

            }break;
            case WaitDouble: {
                if (double_click_timer ==0) {
                    key.key_state = DONE;
                }else {
                    double_click_timer -= KEY_CYCLES_TIME;
                    if (key_num !=0) {
                        key.is_double_clicked = 1;
                        key.key_state = DONE;
                    }
                }

            }break;
            default:{}break;
        }
    }
}
