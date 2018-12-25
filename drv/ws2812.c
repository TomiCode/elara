#include <avr/io.h>
#include <util/delay.h>
#include <util/atomic.h>

#include "ws2812.h"
#include <hardware.h>

struct ws2812_element {
    union {
        struct {
            uint8_t lo;
            uint8_t hi;
        } __attribute__((packed));
        uint16_t value;
    } color;

    uint8_t step;
    uint8_t next;

    uint8_t enable  : 1;
    uint8_t stop    : 1;
    uint8_t dir     : 1;
    uint8_t fade    : 1;
};

struct ws2812_state {
    struct ws2812_element red;
    struct ws2812_element green;
    struct ws2812_element blue;
};

static struct ws2812_state state;

/* Initialize our front panel LED. */
void ws2812_init(void)
{
    DDRC |= _BV(IO_FLED);
}

static inline void cpu_wait(void)
{
    asm volatile (
        "nop \n\t"
        "nop \n\t"
    );
}

static inline void ws2812_push(uint8_t byte)
{
    uint8_t count;

    for (count = 0; count < 8; count++) {
        PORTC |= _BV(IO_FLED);
        cpu_wait();

        if (!(byte & 0x80))
            PORTC &= ~_BV(IO_FLED);

        byte <<= 1;
        cpu_wait();

        PORTC &= ~_BV(IO_FLED);
        cpu_wait();
    }
}

int ws2812_has_work(void)
{
    if (state.red.enable || state.green.enable || state.blue.enable)
        return 1;

    return 0;
}

static void ws2812_update(struct ws2812_element *el)
{
    if (!el->enable || el->stop)
        return;

    if (el->dir)
        el->color.value += el->step;
    else
        el->color.value -= el->step;

    if (el->color.hi == el->next) {
        if (el->fade && el->next) {
            el->step = el->color.hi;
            el->next = 0;
            el->stop = 1;
            el->dir = 0;
        }
        else
            el->enable = 0;
    }
}

static inline void ws2812_set_state(struct ws2812_element *el, uint8_t val,
        uint8_t mode)
{
    el->next = val;
    el->color.lo = 0;

    el->enable = 0;
    el->stop = 0;
    el->step = 0;

    if (mode == WS2812_MODE_FADE)
        el->fade = 1;
    else
        el->fade = 0;

    if (el->color.hi > val) {
        el->dir = 0;
        el->step = el->color.hi - val;
    }
    else if (el->color.hi < val){
        el->dir = 1;
        el->step = val - el->color.hi;
    }

    if (el->step)
        el->enable = 1;
}

static inline int ws2812_sync(struct ws2812_element *el)
{
    return (el->enable && el->stop) || !el->enable;
}

void ws2812_schedule(void)
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        ws2812_update(&state.red);
        ws2812_update(&state.blue);
        ws2812_update(&state.green);

        if (ws2812_sync(&state.red) && ws2812_sync(&state.blue) && ws2812_sync(&state.green)) {
            if (state.red.enable)
                state.red.stop = 0;

            if (state.blue.enable)
                state.blue.stop = 0;

            if (state.green.enable)
                state.green.stop = 0;
        }

        ws2812_push(state.green.color.hi);
        ws2812_push(state.red.color.hi);
        ws2812_push(state.blue.color.hi);
    }
    _delay_us(300);
}

void ws2812_mode(struct ws2812_color *color, uint8_t mode)
{
    ws2812_set_state(&state.red, color->red, mode);
    ws2812_set_state(&state.blue, color->blue, mode);
    ws2812_set_state(&state.green, color->green, mode);
}

