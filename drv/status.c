#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "../spec/hardware.h"

/* Initialize the status led output. */
void status_init(void)
{
  DDRD |= _BV(IO_LEDSTAT);
}

/* Hardware watchdog. */
void status_wdt(void)
{
  /* Watchdog change enable. */
  WDTCSR = _BV(4);
  /* Enable Watchdog interrupt and set prescale to 4s. */
  WDTCSR |= _BV(WDIE) | _BV(5);
}

/* Watchdog interrupt handler. */
ISR(WDT_vect)
{
  PORTD ^= _BV(IO_LEDSTAT);
}
