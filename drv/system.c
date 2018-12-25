#include <stdio.h>
#include <hardware.h>
#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <system.h>

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
  /* Enable watchdog interrupt and set prescaler to 4s. */
  WDTCSR = _BV(WDCE) | _BV(WDE);
  WDTCSR = _BV(WDIE) | _BV(WDP3);
  wdt_reset();
}

void set_relay_on(void)
{
  PORTC |= _BV(IO_RELAY);
}

void set_relay_off(void)
{
  PORTC &= ~_BV(IO_RELAY);
}

/* Watchdog interrupt handler. */
ISR(WDT_vect)
{
  wdt_reset();
  sys_status |= STAT_WDT;
}

/* Front button interrupt handler. */
ISR(INT0_vect)
{
  sys_status |= STAT_BTN;
}

