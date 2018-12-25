#ifndef WS2812_H
#define WS2812_H

#define WS2812_MODE_NONE    0x00
#define WS2812_MODE_FADE    0x01
#define WS2812_MODE_SET     0x02

struct ws2812_color {
    uint8_t green;
    uint8_t red;
    uint8_t blue;
};

void ws2812_init(void);

/*
 * Device work queue, the MCU should not sleep, if we need to update (fade) colors.
 */
int ws2812_has_work(void);
void ws2812_schedule(void);

/*
 * WS2812 LED control.
 */
void ws2812_mode(struct ws2812_color *color, uint8_t mode);

#endif
