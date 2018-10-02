#ifndef DRV_NRF24_H
#define DRV_NRF24_H

struct radio_config {
  uint32_t address;
  uint8_t power;
};

void nrf24_init(void);

#endif // DRV_NRF24_H
