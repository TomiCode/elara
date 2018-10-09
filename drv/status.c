#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "../spec/hardware.h"

void status_init(void)
{
  DDRD |= IO_LEDSTAT;
}

ISR(WDT_vect)
{
  PORTD ^= IO_LEDSTAT;
}
