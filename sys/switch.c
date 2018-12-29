#include <stdio.h>
#include <stdlib.h>

#include "../drv/ws2812.h"
#include "../drv/system.h"
#include "switch.h"

extern volatile uint8_t sys_status;

static struct ws2812_color switch_enable_c = {
    0x23, 0x68, 0x03
};

static struct ws2812_color switch_disable_c = {
    0x42, 0x02, 0x68
};

static uint8_t switch_state = SW_STATE_OFF;

void switch_handle(void)
{
    if (switch_state == SW_STATE_OFF)
        switch_update_state(SW_STATE_ON);
    else
        switch_update_state(SW_STATE_OFF);
}

void switch_update_state(uint8_t state)
{
    switch(state) {
    case SW_STATE_ON:
        set_relay_on();
        ws2812_mode(&switch_enable_c, WS2812_MODE_FADE);
        break;
    case SW_STATE_OFF:
        set_relay_off();
        ws2812_mode(&switch_disable_c, WS2812_MODE_FADE);
        break;
    }
    switch_state = state;
}

uint8_t switch_current_state(void)
{
    return switch_state;
}
