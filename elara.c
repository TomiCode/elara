#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "drv/acs724.h"
#include "drv/serial.h"
#include "drv/nrf24.h"
#include "drv/system.h"
#include "drv/ws2812.h"

#include "spec/hardware.h"

volatile uint8_t sys_status = 0;

uint8_t rf_test_payload[32] = {0};

int main(void)
{
  /* Initialize hardware subsystem. */
  system_init();

  /* Initialize drivers. */
  serial_init();
  acs724_init();
  nrf24_init();
  ws2812_init();

  /* Start system watchdog. */
  system_wdt();

  /* Setup nRF24 radio registers. */
  nrf24_setup();

  /* Enable sleep modes. */
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);

  // ws2812_test(1);
  /* Enable global interrupts. */
  sei();

  printf("elara v0.01 \r\n");
  _delay_ms(5);

  ws2812_color_t state_on = {0x2f, 0x00, 0x00};
  ws2812_color_t state_off = {0x00, 0x2f, 0x00};

  for(;;) {
    sleep_mode();

    PORTD |= _BV(IO_LEDSTAT);
    _delay_ms(10);
    PORTD &= ~_BV(IO_LEDSTAT);

    if (sys_status == 0xFF) {
      nrf24_test(rf_test_payload);
      sys_status = 0x00;
    }
    if (sys_status == 0x22) {
      if(relay_state()) {
        set_relay_off();
        ws2812_test(2);
        // ws2812_color_fadeout(state_on, 100, 100);
      }
      else {
        set_relay_on();
        ws2812_test(1);
        // ws2812_color_fadeout(state_off, 100, 100);
      }
      sys_status = 0x00;
    }

    acs724_start();
  }

  return 0;
}

