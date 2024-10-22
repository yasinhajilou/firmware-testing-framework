#ifndef GPIO_DRIVER_H
#define GPIO_DRIVER_H

#include <stdint.h>

typedef enum {
    GPIO_LOW = 0,
    GPIO_HIGH = 1
} GPIO_State;

typedef struct {
    uint8_t pin;
    GPIO_State state;
} GPIO_Pin;

void gpio_init(GPIO_Pin* pin);
void gpio_set_state(GPIO_Pin* pin, GPIO_State state);
GPIO_State gpio_get_state(GPIO_Pin* pin);

#endif // GPIO_DRIVER_H

// GPIO driver implementation
void gpio_init(GPIO_Pin* pin) {
    if (pin) {
        pin->state = GPIO_LOW;
    }
}

void gpio_set_state(GPIO_Pin* pin, GPIO_State state) {
    if (pin) {
        pin->state = state;
    }
}

GPIO_State gpio_get_state(GPIO_Pin* pin) {
    return pin ? pin->state : GPIO_LOW;
}