#include <stdio.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>

#include "../spec/hardware.h"

/* Initialize other system hardware. */
void system_init(void)
{
  DDRB = 0;
  DDRC = _BV(IO_RELAY);
  DDRD = _BV(IO_LEDSTAT);
}

/* Setup and start watchdog timer. */
void system_wdt(void)
{
  /* Enable watchdog interrupt and set prescaler to 1s. */
  WDTCSR = _BV(WDCE) | _BV(WDE);
  WDTCSR = _BV(WDIE) | _BV(WDP2) | _BV(WDP1);
  wdt_reset();
}

/* Watchdog interrupt handler. */
ISR(WDT_vect)
{
  wdt_reset();
}

/* Front button interrupt handler. */
ISR(INT0_vect)
{

}

