#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>

#include <net.h>
#include "../drv/nrf24.h"
#include "../drv/ws2812.h"
#include "switch.h"

#define NSTATE_NOT_CONN 0
#define NSTATE_VALID    1

#define INVALID_DEVICE 0xFFFF

#ifdef HAS_RF_REG3V3
#define HW_TYPE 0xF2
#else
#define HW_TYPE 0xF1
#endif

extern volatile uint8_t sys_status;

static struct ws2812_color net_error_c = { 0x00, 0x20, 0x00 };
static struct ws2812_color net_error_a = { 0x00, 0x00, 0x20 };
static struct ws2812_color net_error_b = { 0x20, 0x00, 0x00 };
static struct ws2812_color net_error_d = { 0x20, 0x20, 0x00 };
static struct ws2812_color net_error_e = { 0x20, 0x20, 0x20 };

static uint8_t net_state = NSTATE_NOT_CONN;
static uint8_t net_buffer[32] = {0};

static uint8_t net_delay = 0;
static uint8_t net_retries = 0;

static uint16_t net_device = 0;

static void net_hello(void)
{
    struct net_hello *payload = (struct net_hello *)net_buffer;
    payload->header.type = MSG_HELLO;
    payload->header.device = INVALID_DEVICE;

    payload->hw_type = HW_TYPE;
    payload->version = ELARA_REV;

    nrf24_payload_write(net_buffer);
    printf("hello sended \r\n");
}

static void net_ping(void)
{
    struct net_ping *message = (struct net_ping *)net_buffer;

    printf("net_ping %02x, %02x \r\n", net_delay, net_retries);
    if ((net_delay++) <= 3)
        return;

    if (net_retries > 3) {
        nrf24_set_address(NULL);
        net_state = NSTATE_NOT_CONN;
        net_delay = 0;
        net_retries = 0;
        return;
    }

    message->header.type = MSG_PING;
    message->header.device = net_device;
    message->state = switch_current_state();

    nrf24_payload_write(net_buffer);

    net_delay = 0;
    net_retries++;
}

static void net_r_acquired(struct net_header *header)
{
    struct net_acquire *message = (struct net_acquire *)header;

    net_device = message->header.device;
    nrf24_set_address(message->addr);
    net_state = NSTATE_VALID;

    ws2812_mode(&net_error_e, WS2812_MODE_FADE);
    printf("r_acquired device %04x, set addr %02x \r\n", net_device, message->addr[4]);
    _delay_us(500);
}

static void net_r_pong(struct net_header *header)
{
    ws2812_mode(&net_error_d, WS2812_MODE_FADE);
    printf("pong for device %04x \r\n", header->device);
    net_retries = 0;
    _delay_us(500);
}

static void net_r_switch(struct net_header *header)
{
    struct net_switch *message = (struct net_switch *)header;
    switch_update_state(message->state);
    net_delay = 0xff;
}

static int net_receive(void)
{
    struct net_header *header = (struct net_header *)net_buffer;
    int status;

    status = nrf24_payload_read(net_buffer);
    if (status < 0) {
        printf("unable to read payload. \r\n");
        return status;
    }

    switch (header->type) {
    case MSG_ACQUIRED:
        net_r_acquired(header);
        break;
    case MSG_PONG:
        net_r_pong(header);
        break;
    case MSG_SWITCH:
        net_r_switch(header);
        break;
    }

    return 0;
}

void net_task(void)
{
    printf("net_taks call\r\n");

    switch (net_state) {
    case NSTATE_NOT_CONN:
        net_hello();
        break;
    case NSTATE_VALID:
        net_ping();
        break;
    }
}

void net_handle(void)
{
    uint8_t status = nrf24_fetch_status();

    if (status & NRF24_RX_DR) {
        net_receive();
    }
    else if (status & NRF24_MAX_RT) {
        ws2812_mode(&net_error_c, WS2812_MODE_FADE);
    }
    else if (status & NRF24_TX_DS) {
         ws2812_mode(&net_error_b, WS2812_MODE_FADE);
    }
}


