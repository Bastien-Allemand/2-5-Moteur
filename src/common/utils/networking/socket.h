#pragma once

#include "../types.h"
#include "address.h"

constexpr ushort DEFAULT_PORT = 1888;
constexpr uint   DEFAULT_PACKET_SIZE = 1024;

struct RawNetworkBytes {
    byte data[DEFAULT_PACKET_SIZE] = { 0 };
    size_t count = 0;
};

class UdpSocket {
public:
    // Client style (no bind, has a default remote)
    UdpSocket(cstr _remoteIP, ushort _remotePort = DEFAULT_PORT);

    // Server style (bind only, no default remote)
    explicit UdpSocket(ushort _localPort = DEFAULT_PORT);

    ~UdpSocket();

    bool send(const RawNetworkBytes& _bytes); // Client style, only work if has a default remote
                                                                   bool sendTo(Address& _addr, const RawNetworkBytes& _bytes); // Server style, useful for responses
                                                                   bool receive(RawNetworkBytes& _bytes, Address* _sender = nullptr);

    const Address& remote() const;
    void setRemote(const Address& _addr);

private:
    uint m_socket;
    
    Address m_local;
    Address m_remote;
    
    bool m_hasRemote;
};