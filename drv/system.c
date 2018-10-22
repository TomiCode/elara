#include <stdio.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "../spec/hardware.h"

extern volatile uint8_t sys_status;

/* Initialize other system hardware. */
void system_init(void)
{
  DDRB = 0;
  DDRC = _BV(IO_RELAY);
  DDRD = _BV(IO_LEDSTAT);
  /* Enable interrupt on rising edge from front button input. */
  EICRA |= _BV(ISC00) | _BV(ISC01);
  EIMSK |= _BV(INT0);
}

/* Setup and start watchdog timer. */
void system_wdt(void)
{
  /* Enable watchdog interrupt and set prescaler to 4s. */
  WDTCSR = _BV(WDCE) | _BV(WDE);
  WDTCSR = _BV(WDIE) | _BV(WDP3);
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

