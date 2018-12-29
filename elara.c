#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "drv/acs724.h"
#include "drv/serial.h"
#include "drv/nrf24.h"
#include "drv/system.h"
#include "drv/ws2812.h"

#include "sys/net.h"
#include "sys/switch.h"

#include <hardware.h>
#include <system.h>

volatile uint8_t sys_status = 0;

static struct ws2812_color error_c = { 0x00, 0x20, 0x00 };

int main(void)
{
  /* Initialize hardware subsystem. */
  system_init();

  /* Initialize drivers. */
  serial_init();
  acs724_init();
  nrf24_init();
  ws2812_init();

  /* Setup nRF24 radio registers. */
  nrf24_setup();

  /* Enable sleep modes. */
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);

  /* Enable global interrupts. */
  sei();

  printf("elara v0.01 \r\n");
  _delay_ms(50);

  for(;;) {
      if (ws2812_has_work())
          ws2812_schedule();
      else
        sleep_mode();

    PORTD |= _BV(IO_LEDSTAT);
    _delay_us(200);

    if (sys_status & STAT_WDT) {
        sys_status &= ~STAT_WDT;
        net_task();
    }

    if (sys_status & STAT_BTN) {
        sys_status &= ~STAT_BTN;
        switch_handle();
    }

    if (sys_status & STAT_RADIO) {
        sys_status &= ~STAT_RADIO;
        net_handle();
    }

    if (sys_status & STAT_TX_ERR) {
        sys_status &= ~STAT_TX_ERR;
        ws2812_mode(&error_c, WS2812_MODE_FADE);
    }

    PORTD &= ~_BV(IO_LEDSTAT);
  }

  return 0;
}

