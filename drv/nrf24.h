#ifndef DRV_NRF24_H
#define DRV_NRF24_H

#define NRF24_RX_DR  0x40
#define NRF24_TX_DS  0x20
#define NRF24_MAX_RT 0x10

void nrf24_init(void);
void nrf24_setup(void);
void nrf24_set_address(uint8_t *addr);

uint8_t nrf24_fetch_status(void);

int nrf24_payload_read(void *);
void nrf24_payload_write(void *);

void nrf24_debug(void);

#endif // DRV_NRF24_H
