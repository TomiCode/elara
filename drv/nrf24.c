#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <util/delay.h>

#include "nrf24.h"
#include "../spec/nrf24.h"

extern volatile uint8_t sys_status;

static uint8_t rf_addresses[][5] = {
  { 0xE7, 0xE7, 0xE7, 0xE7, 0xE7 },
  { 0xC2, 0xC2, 0xC2, 0xC2, 0xC2 }
};

/* Initialize our SPI registers and radio data. */
void nrf24_init(void)
{
  /* Set SCK and MOSI as outputs. */
  DDRB |= _BV(IO_SCK) | _BV(IO_MOSI) | _BV(IO_SS);
  /* Set radio SS and Enable as outputs. */
  DDRC |= _BV(IO_RF_CE) | _BV(IO_RF_CSN);
  /* Feature of the new mainboard. */
#ifdef HAS_RF_REG3V3
  DRRC |= _BV(IO_3V3REG);
#endif
  /* Enable SPI communication with 250 KHz clock. */
  SPCR = _BV(SPE) | _BV(MSTR) | _BV(SPR0);
  /* Set Chip Select high. */
  PORTC |= _BV(IO_RF_CSN);
  /* Trigger INT1 on falling edge. */
  EICRA |= _BV(ISC11);
  /* Enable INT1 interrupt. */
  EIMSK |= _BV(INT1);
}

static inline void enable_3v3_regulator(void)
{
  PORTC |= _BV(IO_3V3REG);
}

static inline void set_csn_low(void)
{
   PORTC &= ~_BV(IO_RF_CSN);
}

static inline void set_csn_high(void)
{
  PORTC |= _BV(IO_RF_CSN);
}

static inline void set_ce_low(void)
{
   PORTC &= ~_BV(IO_RF_CE);
}

static inline void set_ce_high(void)
{
   PORTC |= _BV(IO_RF_CE);
}

/* Put a byte on the MOSI line and return MISO content. */
static inline uint8_t spi_send_frame(uint8_t frame)
{
  SPDR = frame;
  loop_until_bit_is_set(SPSR, SPIF);
  return SPDR;
}

static uint8_t spi_command(uint8_t command)
{
  uint8_t status;
  set_csn_low();
  status = spi_send_frame(command);
  set_csn_high();
  return status;
}

static uint8_t spi_read_buffer(uint8_t command, uint8_t *buffer, uint8_t size)
{
  uint8_t status;

  set_csn_low();
  status = spi_send_frame(command);
  while((size--) > 0) {
    *(buffer++) = spi_send_frame(0x00);
  }
  set_csn_high();

  return status;
}

static uint8_t spi_write_buffer(uint8_t command, uint8_t *buffer, uint8_t size)
{
  uint8_t status;

  set_csn_low();
  status = spi_send_frame(command);
  while((size--) > 0) {
    spi_send_frame(*(buffer++));
  }
  set_csn_high();

  return status;
}


static uint8_t nrf24_register_read(uint8_t address)
{
  uint8_t value;

  set_csn_low();
  spi_send_frame(R_REGISTER(address));
  value = spi_send_frame(0x00);
  set_csn_high();

  return value;
}

static uint8_t nrf24_register_write(uint8_t address, uint8_t value)
{
  uint8_t status;

  set_csn_low();
  status = spi_send_frame(W_REGISTER(address));
  spi_send_frame(value);
  set_csn_high();

  return status;
}

void nrf24_setup(void)
{
#ifdef HAS_RF_REG3V3
  enable_3v3_regulator();
#endif
  _delay_ms(100);

  nrf24_register_write(CONFIG, _BV(MASK_TX_DS) | _BV(MASK_MAX_RT) | _BV(EN_CRC) | _BV(PWR_UP));
  _delay_ms(5);

  nrf24_register_write(FEATURE, 0x00);
  nrf24_register_write(DYNPD, 0x00);
  nrf24_register_write(RX_PW_P0, 32);
  nrf24_register_write(RX_PW_P1, 32);

  // spi_write_buffer(W_REGISTER(RX_ADDR_P0), rf_addresses[1], 5);
  // spi_write_buffer(W_REGISTER(TX_ADDR), rf_addresses[1], 5);
  // spi_write_buffer(W_REGISTER(RX_ADDR_P1), rf_addresses[0], 5);

  spi_command(FLUSH_RX);
  spi_command(FLUSH_TX);

  nrf24_register_write(RF_CH, 0x34);
  nrf24_register_write(CONFIG, (nrf24_register_read(CONFIG)) | _BV(PRIX_RX));
  set_ce_high();
  _delay_us(130);
}

void nrf24_debug(void)
{
    uint8_t config, status;
    uint8_t a, b, c;
    uint8_t addr[5] = {0};

    config = nrf24_register_read(CONFIG);
    status = nrf24_register_read(STATUS);

    printf("nrf24l01 STATUS: %02x, CONFIG: %02x \r\n", status, config);

    spi_read_buffer(RX_ADDR_P0, addr, 5);
    printf("RX_ADDR_P0: %02x %02x %02x %02x %02x \r\n", addr[0], addr[1], addr[2], addr[3], addr[4]);

    spi_read_buffer(RX_ADDR_P1, addr, 5);
    printf("RX_ADDR_P1: %02x %02x %02x %02x %02x \r\n", addr[0], addr[1], addr[2], addr[3], addr[4]);

    spi_read_buffer(TX_ADDR, addr, 5);
    printf("TX_ADDR: %02x %02x %02x %02x %02x \r\n", addr[0], addr[1], addr[2], addr[3], addr[4]);

    status = nrf24_register_read(FIFO_STATUS);
    printf("FIFO_STATUS: %02x \r\n", status);

    a = nrf24_register_read(RX_PW_P0);
    b = nrf24_register_read(RX_PW_P1);
    printf("RX_PW_P0: %02x, RX_PW_P1: %02x \r\n", a, b);

    status = nrf24_register_read(RF_CH);
    printf("RF_CH: %02x \r\n", status);
    _delay_ms(5);
}

uint8_t nrf24_test(uint8_t *buffer)
{
  nrf24_register_write(STATUS, 0x70);

  // if ((nrf24_register_read(FIFO_STATUS) & 0x01) == 0x01)
  //  return 0;

  ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
  {
    spi_read_buffer(R_RX_PAYLOAD, buffer, 32);
    printf("payload: %02x \r\n", *buffer);
    _delay_ms(5);
  }
  return 1;
}

/* Radio interrupt handler. */
ISR(INT1_vect)
{
  sys_status = 0xFF;
}
