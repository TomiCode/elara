#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "drv/acs724.h"
#include "drv/serial.h"
#include "drv/nrf24.h"
#include "drv/system.h"

#include "spec/hardware.h"

int main(void)
{
  /* Wait a while for startup. */
  _delay_ms(5);

  /* Initialize hardware subsystem. */
  system_init();

  /* Initialize drivers. */
  serial_init();
  nrf24_init();

  /* Start system watchdog. */
  system_wdt();

  /* Setup nRF24 radio registers. */
  nrf24_setup();

  /* Enable global interrupts. */
  sei();

  printf("elara v0.01\r\n");
  _delay_ms(50);

  for(;;) {
    set_sleep_mode(0x6);
    sleep_mode();

    PORTD ^= _BV(IO_LEDSTAT);
  }

  return 0;
}

