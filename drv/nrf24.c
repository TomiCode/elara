#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "../spec/nrf24.h"

void nrf24_address_init(void);

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

/* Put data on the SPI interface and return received data. */
uint8_t nrf24_spi_sb(uint8_t frame)
{
  SPDR = frame;
  loop_until_bit_is_set(SPSR, SPIF);
  return SPDR;
}

/* Read data from radio. */
uint8_t nrf24_spi_read(uint8_t command, uint8_t *buffer, uint8_t size)
{
  uint8_t status;

  PORTC &= ~_BV(IO_RF_CSN);
  status = nrf24_spi_sb(command);

  for(; size > 0; size--) {
    *(buffer++) = nrf24_spi_sb(0);
  }

  PORTC |= _BV(IO_RF_CSN);
  return status;
}

uint8_t nrf24_spi_write(uint8_t command, uint8_t *buffer, uint8_t size)
{
  uint8_t status;

  PORTC &= ~_BV(IO_RF_CSN);
  status = nrf24_spi_sb(command);

  for(; size > 0; size--) {
    nrf24_spi_sb(*(buffer++));
  }

  PORTC |= _BV(IO_RF_CSN);
  return status;
}

void nrf24_setup(void)
{
  uint8_t rf_config;
  if (nrf24_spi_read(R_REGISTER(CONFIG), &rf_config, sizeof(uint8_t)) != 0x0e) {
    printf("Error while nRF init.\r\n");
  }

  rf_config |= MASK_RX_DR | EN_CRC | PWR_UP;
  printf("nRF status: %02x\r\n", nrf24_spi_write(W_REGISTER(CONFIG), &rf_config, sizeof(uint8_t)));
  
  _delay_ms(20);
  nrf24_address_init();
}

void nrf24_print_hex(uint8_t *buffer, uint8_t size)
{
  for(; size > 0; size--)
    printf("%02x", *(buffer++));
  printf("\r\n");
}

void nrf24_address_init(void)
{
  uint8_t addr[5] = {0};
  printf("nRF address status: %02x\r\n", nrf24_spi_read(RX_ADDR_P0, (uint8_t*)&addr, sizeof(uint8_t) * 5));

  printf("RX_ADDR_P0: ");
  nrf24_print_hex((uint8_t*)&addr, sizeof(uint8_t) * 5);

  nrf24_spi_read(TX_ADDR, (uint8_t*)&addr, sizeof(uint8_t) * 5);
  printf("RX_ADDR_P1: ");
  nrf24_print_hex((uint8_t*)&addr, sizeof(uint8_t) * 5);
}

/* Radio interrupt handler. */
ISR(INT1_vect)
{

}
