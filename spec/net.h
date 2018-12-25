#ifndef NET_SPEC_H
#define NET_SPEC_H

#define MSG_HELLO 0x01
#define MSG_ACQUIRED 0x02

struct net_header {
    uint8_t type;
};

struct net_hello {
    struct net_header header;
    uint8_t device;
};

struct net_acquire {
    struct net_header header;
    uint8_t addr[5];
};

#endif // NET_SPEC_H
