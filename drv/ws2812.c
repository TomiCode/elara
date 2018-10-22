#include <avr/io.h>
#include <util/delay.h>
#include <util/atomic.h>

#include "ws2812.h"
#include "../spec/hardware.h"

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

static inline void ws2812_push(uint8_t byte, uint8_t bits)
{
   while(bits) {
    PORTC |= _BV(IO_FLED);
    cpu_wait();
    if (!(byte & 0x80))
      PORTC &= ~_BV(IO_FLED);
    byte <<= 1;
    cpu_wait();
    PORTC &= ~_BV(IO_FLED);
    cpu_wait();
    bits--;
  }
}

void ws2812_set_color(ws2812_color_t *color)
{
  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {
    ws2812_push(color->green, 8);
    ws2812_push(color->red, 8);
    ws2812_push(color->blue, 8);
  }
}

/* Proably not this type. */
void ws2812_test(uint8_t color)
{
  // (void)color;

  ws2812_color_t test; // = {0x00, 0x2b, 0x2b};
  if (color == 1) {
    test.red = 0x2f;
    test.green = 0x00;
    test.blue = 0x2f;
  }
  else {
    test.red = 0x2f;
    test.green = 0x2f;
    test.blue = 0x00;
  }

  while(test.red > 0 || test.blue > 0) {
    ws2812_set_color(&test);
    _delay_ms(10);
    if (test.blue > 0)
      test.blue--;
    if (test.red > 0)
      test.red--;
    if (test.green > 0)
      test.green--;
  }
  ws2812_set_color(&test);
}

