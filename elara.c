#include <inttypes.h>
#include <avr/boot.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/power.h>
#include <util/delay.h>

#include "drv/acs724.h"
#include "drv/serial.h"
#include "drv/nrf24.h"

int main(void)
{
  /* Wait a while for startup. */
  _delay_ms(5);

  /* Initialize drivers. */
  serial_init();
  nrf24_init();

  /* Enable global interrupts. */
  sei();
  
  nrf24_setup();

  printf("elara v0.01 boot\r\n");

  return 0;
}

/* Button interrupt handler. */
ISR(INT0_vect)
{

}
