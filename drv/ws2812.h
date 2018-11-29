#ifndef WS2812_H
#define WS2812_H

typedef struct {
  uint8_t green;
  uint8_t red;
  uint8_t blue;
} ws2812_color_t;

void ws2812_init(void);

void ws2812_test(uint8_t);

void ws2812_color_fadeout(ws2812_color_t color, uint8_t step, uint8_t delay);

#endif
