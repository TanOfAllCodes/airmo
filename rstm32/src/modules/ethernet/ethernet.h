#ifndef ETHERNET_H
#define ETHERNET_H

#include <stdint.h>
#include <stdbool.h>

/*
 * Minimal bare-metal Ethernet driver for STM32H753.
 *
 * Manages the ETH MAC peripheral + LAN8742A PHY (RMII).
 * Provides a lightweight TCP client — connect to a known server,
 * then send fixed-size frames.
 *
 * This is NOT a full IP stack. It implements just enough:
 *   ARP (respond + cache one entry)
 *   IPv4 (no fragmentation)
 *   TCP  (single connection, minimal state machine)
 *
 * If a full lwIP integration is desired later, this module can be
 * replaced by an lwIP netif driver reusing the same ETH init code.
 */

// ── Network configuration (static IP, point-to-point link) ──────────────────

struct EthConfig {
    uint8_t  mac[6];
    uint8_t  ip[4];
    uint8_t  gateway[4];
    uint8_t  netmask[4];
    uint8_t  server_ip[4];
    uint16_t server_port;
};

// ── Connection state ────────────────────────────────────────────────────────

enum class EthState : uint8_t {
    UNINITIALIZED,
    LINK_DOWN,
    LINK_UP,
    CONNECTING,
    CONNECTED,
    ERROR
};

// ── Driver API ──────────────────────────────────────────────────────────────

class Ethernet {
public:
    // Bring up MAC + PHY, configure DMA descriptors
    static void init(const EthConfig &cfg);

    // Poll link state, drive TCP state machine (call periodically)
    static void poll();

    // Attempt TCP connection to server_ip:server_port
    static bool connect();

    // Send `len` bytes over the established TCP connection
    static bool send(const uint8_t *data, uint16_t len);

    // Tear down the TCP connection gracefully
    static void disconnect();

    // Current state
    static EthState state();

    // Is a TCP connection active and healthy?
    static bool isConnected();

private:
    static EthConfig  cfg_;
    static EthState   state_;

    // Low-level MAC helpers
    static void initMAC();
    static void initDMA();
    static void initPHY();
    static bool phyLinkUp();

    // Minimal protocol handlers
    static void handleRx();
    static void handleARP(const uint8_t *frame, uint16_t len);
    static void handleIPv4(const uint8_t *frame, uint16_t len);
    static void handleTCP(const uint8_t *packet, uint16_t len);

    // Raw frame send
    static bool sendFrame(const uint8_t *data, uint16_t len);
};

#endif // ETHERNET_H
