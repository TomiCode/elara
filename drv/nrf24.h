#ifndef DRV_NRF24_H
#define DRV_NRF24_H

#define SYS_STATUS_NRF24_IRQ 0x11

void nrf24_init(void);
void nrf24_setup(void);
void nrf24_set_address(uint8_t *addr);

int nrf24_payload_read(void *);
void nrf24_payload_write(void *);

void nrf24_debug(void);

#endif // DRV_NRF24_H
