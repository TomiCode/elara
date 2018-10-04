#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "../spec/nrf24.h"

/* Initialize our SPI registers and radio data. */
void nrf24_init(void)
{
  /* Set SCK and MOSI as outputs. */
  DDRB = _BV(IO_SCK) | _BV(IO_MOSI) | _BV(IO_SS);
  /* Set radio SS and Enable as outputs. */
  DDRC = _BV(IO_RF_CE) | _BV(IO_RF_CSN);
  /* Enable SPI communication with 250 KHz clock. */
  SPCR = _BV(SPE) | _BV(MSTR) | _BV(SPR0);
  /* Set Chip Select high. */
  PORTC = _BV(IO_RF_CSN);
  /* Wait for the radio initialization. */
  _delay_ms(11); // 10.3 ms power on reset time for nRF24L01
}

void nrf24_setup(uint32_t base_addr)
{
  (void)base_addr;
  uint8_t status;
  uint8_t config;

  PORTC &= ~_BV(IO_RF_CSN);
  SPDR = R_REGISTER(CONFIG);
  loop_until_bit_is_set(SPSR, SPIF);
  status = SPDR;
  SPDR = 0x00;
  loop_until_bit_is_set(SPSR, SPIF);
  config = SPDR;
  PORTC |= _BV(IO_RF_CSN);
  printf("rf: %x, %x\r\n", status, config);
}

/* Radio IRQ handler. */
ISR(INT1_vect)
{

}
