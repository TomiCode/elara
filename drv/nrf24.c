#include <avr/io.h>
#include <avr/interrupt.h>

#include "nrf24.h"
#include "../hw_pins.h"

/* Initialize the config register values. */
volatile struct radio_config config = {0};

/* Initialize our SPI registers and radio data. */
void nrf24_init(void)
{
  /* Set SCK and MOSI as outputs. */
  DDRB |= _BV(PB5) | _BV(PB3);
  /* Set radio SS and Enable as outputs. */
  DDRC |= _BV(PIN_RF_SEL) | _BV(PIN_RF_EN);
  /* Enable SPI communication with 250 KHz clock. */
  SPCR = _BV(SPE) | _BV(MSTR) | _BV(SPR1);
}

void nrf24_setup(uint32_t base_addr)
{

}

/* Radio IRQ handler. */
ISR(INT1_vect)
{

}
