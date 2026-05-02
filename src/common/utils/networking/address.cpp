#include "pch.h"
#include "address.h"

#include "../plateform/win-safe.h"

Address::Address() {
    m_storage = new sockaddr_storage{};
}

Address::Address(cstr ip, ushort port) : Address() {
    sockaddr_in* addr = reinterpret_cast<sockaddr_in*>(m_storage);
    addr->sin_family = AF_INET;
    addr->sin_port = htons(port);
    inet_pton(AF_INET, ip, &addr->sin_addr);
}

Address::Address(ushort port) : Address() {
    sockaddr_in* addr = reinterpret_cast<sockaddr_in*>(m_storage);
    addr->sin_family = AF_INET;
    addr->sin_port = htons(port);
    addr->sin_addr.s_addr = INADDR_ANY;
}

Address::Address(const Address& other) : Address() {
    memcpy(m_storage, other.m_storage, sizeof(sockaddr_storage));
}

Address& Address::operator=(const Address& other) {
    if (this != &other) {
        memcpy(m_storage, other.m_storage, sizeof(sockaddr_storage));
    }
    return *this;
}

Address::~Address() {
    delete m_storage;
}

const sockaddr* Address::native() const {
    return reinterpret_cast<const sockaddr*>(m_storage);
}

sockaddr* Address::native() {
    return reinterpret_cast<sockaddr*>(m_storage);
}

uint Address::size() const {
    return sizeof(sockaddr_in);
}
bool Address::operator==(const Address& other) const {
    bool sameIP = ip() == other.ip();
    bool samePort = port() == other.port();
    return sameIP && samePort;
}
bool Address::operator!=(const Address& other) const {
    bool differentIP = ip() != other.ip();
    bool differentPort = port() != other.port();
    return differentIP && differentPort;
}

cstr Address::ip() const {
    static char buffer[INET_ADDRSTRLEN];
    auto* addr = reinterpret_cast<const sockaddr_in*>(m_storage);
    inet_ntop(AF_INET, &addr->sin_addr, buffer, sizeof(buffer));
    return buffer;
}

ushort Address::port() const {
    auto* addr = reinterpret_cast<const sockaddr_in*>(m_storage);
    return ntohs(addr->sin_port);
}