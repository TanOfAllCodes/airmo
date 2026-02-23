#include "ethernet.h"
#include "target.h"

/*
 * Bare-metal Ethernet driver for STM32H753ZI + LAN8742A (RMII).
 *
 * TODO: Implement register-level ETH MAC / DMA / PHY init and the
 *       minimal TCP state machine.  The scaffolding below outlines
 *       each piece that needs to be filled in.
 *
 * Nucleo-144 RMII pin mapping:
 *   PA1  → ETH_REF_CLK      PA2  → ETH_MDIO
 *   PA7  → ETH_CRS_DV       PB13 → ETH_TXD1
 *   PC1  → ETH_MDC          PC4  → ETH_RXD0
 *   PC5  → ETH_RXD1         PG11 → ETH_TX_EN
 *   PG13 → ETH_TXD0
 */

// ── Static storage ──────────────────────────────────────────────────────────
EthConfig Ethernet::cfg_   = {};
EthState  Ethernet::state_ = EthState::UNINITIALIZED;

// ── Public API ──────────────────────────────────────────────────────────────

void Ethernet::init(const EthConfig &cfg) {
    cfg_ = cfg;

    // 1. Enable clocks: GPIOA/B/C/G, ETH MAC, ETH TX, ETH RX
    // 2. Configure RMII GPIO pins (AF11, high speed, no pull)
    initPHY();
    initMAC();
    initDMA();

    state_ = EthState::LINK_DOWN;
}

void Ethernet::poll() {
    if (state_ == EthState::UNINITIALIZED) return;

    // Check physical link
    if (!phyLinkUp()) {
        state_ = EthState::LINK_DOWN;
        return;
    }

    if (state_ == EthState::LINK_DOWN) {
        state_ = EthState::LINK_UP;
    }

    // Process any received frames (ARP replies, TCP ACKs, etc.)
    handleRx();
}

bool Ethernet::connect() {
    if (state_ != EthState::LINK_UP) return false;

    state_ = EthState::CONNECTING;
    // Send TCP SYN to cfg_.server_ip:cfg_.server_port
    // The TCP handshake completes asynchronously via handleTCP()

    return true; // TODO: return false on immediate failure
}

bool Ethernet::send(const uint8_t *data, uint16_t len) {
    if (state_ != EthState::CONNECTED) return false;
    (void)data; (void)len;

    // Build: Ethernet frame → IPv4 header → TCP header (PSH|ACK) → payload
    // Compute checksums, enqueue via DMA

    return false; // TODO
}

void Ethernet::disconnect() {
    // Send TCP FIN, transition to LINK_UP
    state_ = EthState::LINK_UP;
}

EthState Ethernet::state()       { return state_; }
bool     Ethernet::isConnected() { return state_ == EthState::CONNECTED; }

// ── Low-level init ──────────────────────────────────────────────────────────

void Ethernet::initMAC() {
    // Configure ETH_MACCR: speed, duplex, checksum offload
    // Configure ETH_MACFFR: promiscuous off, pass-all off
}

void Ethernet::initDMA() {
    // Set up TX/RX descriptor rings in AXI SRAM
    // Configure ETH_DMABMR: burst length, descriptor skip
    // Enable TX/RX DMA
}

void Ethernet::initPHY() {
    // Reset LAN8742A via MDIO (SMI)
    // Wait for auto-negotiation complete
    // Read link speed + duplex from PHY registers
}

bool Ethernet::phyLinkUp() {
    // Read PHY BSR register bit 2 (link status) via MDIO
    return false; // TODO
}

// ── Receive path ────────────────────────────────────────────────────────────

void Ethernet::handleRx() {
    // Walk RX descriptor ring, for each completed frame:
    //   - EtherType 0x0806 → handleARP()
    //   - EtherType 0x0800 → handleIPv4()
    //   - Release descriptor back to DMA
}

void Ethernet::handleARP(const uint8_t *frame, uint16_t len) {
    (void)frame; (void)len;
    // If ARP request for our IP → send ARP reply
    // If ARP reply → cache sender MAC for server_ip
}

void Ethernet::handleIPv4(const uint8_t *packet, uint16_t len) {
    (void)packet; (void)len;
    // Verify header checksum, check dest IP == our IP
    // Protocol 6 (TCP) → handleTCP()
}

void Ethernet::handleTCP(const uint8_t *packet, uint16_t len) {
    (void)packet; (void)len;
    // Minimal TCP state machine:
    //   CONNECTING + SYN-ACK received → send ACK, state = CONNECTED
    //   CONNECTED  + ACK              → advance send window
    //   FIN received                  → send ACK, state = LINK_UP
}

// ── Transmit path ───────────────────────────────────────────────────────────

bool Ethernet::sendFrame(const uint8_t *data, uint16_t len) {
    (void)data; (void)len;
    // Copy into TX descriptor buffer, set OWN bit, poll DMA
    return false; // TODO
}
