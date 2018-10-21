#ifndef DRV_NRF24_H
#define DRV_NRF24_H

#define SYS_STATUS_NRF24_IRQ 0x11

void nrf24_init(void);
void nrf24_setup(void);

uint8_t nrf24_test(uint8_t *buffer);

#endif // DRV_NRF24_H
