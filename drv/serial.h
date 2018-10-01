#ifndef DRV_SERIAL_H
#define DRV_SERIAL_H

// #define BAUD2UBRR(baud) F_CPU / 16 / baud - 1
#define BAUD_115200   8

void serial_init(uint16_t);
void serial_send(const uint8_t);

#endif // DRV_SERIAL_H
