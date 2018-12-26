#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>

#include <net.h>
#include "../drv/nrf24.h"
#include "../drv/ws2812.h"

#define NSTATE_NOT_CONN 0
#define NSTATE_VALID    1

extern volatile uint8_t sys_status;

static struct ws2812_color net_error_c = { 0x00, 0x20, 0x00 };
static struct ws2812_color net_error_a = { 0x00, 0x00, 0x20 };
static struct ws2812_color net_error_b = { 0x20, 0x00, 0x00 };

static uint8_t net_state = NSTATE_NOT_CONN;
static uint8_t net_buffer[32] = {0};

static void net_hello(void)
{
    struct net_hello *payload = (struct net_hello *)net_buffer;
    payload->header.type = MSG_HELLO;
    payload->device = 0xF0;

    nrf24_payload_write(net_buffer);
}

static int net_receive(void)
{
    struct net_header *head = (struct net_header *)net_buffer;
    int status;

    status = nrf24_payload_read(net_buffer);
    if (status < 0) {
        printf("unable to read payload. \r\n");
        return status;
    }
 ws2812_mode(&net_error_a, WS2812_MODE_FADE);
    // printf("-> %d \r\n", net_buffer[0]);
    // _delay_us(4000);

    /*
    switch (head->type) {
    case MSG_ACQUIRED:
        printf("acquired device! \r\n");
        break;

    default:
        printf("invalid message. %x \r\n", head->type);
    }
    */

    return 0;
}

void net_task(void)
{
    if (net_state == NSTATE_NOT_CONN) {
        net_hello();
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


