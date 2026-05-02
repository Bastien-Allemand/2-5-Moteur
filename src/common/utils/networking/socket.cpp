#include "pch.h"
#include "socket.h"

#include "../plateform/win-safe.h"

#include <string>
#include <stdexcept>

// Small error-handling utility (To Do : Remove)
void runtime_assert(int value, const char* msg) {
    if (value != INVALID_SOCKET)
        return;

    throw std::runtime_error(std::string(msg) + std::to_string(WSAGetLastError()));
}

////////////////////////////////////////////////////////////////
// Client-style constructor
////////////////////////////////////////////////////////////////

UdpSocket::UdpSocket(cstr remoteIp, ushort remotePort)
    : m_remote(remoteIp, remotePort), m_hasRemote(true) {

    m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    runtime_assert(m_socket, "Socket creation failed: ");
}

////////////////////////////////////////////////////////////////
// Server-style constructor
////////////////////////////////////////////////////////////////

UdpSocket::UdpSocket(ushort localPort)
    : m_local(localPort), m_hasRemote(false) {

    m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    runtime_assert(m_socket, "Socket creation failed: ");

    int res = bind(m_socket, m_local.native(), m_local.size());
    runtime_assert(res, "Bind failed: ");
}

////////////////////////////////////////////////////////////////

UdpSocket::~UdpSocket() {
    if (m_socket == NULL) return;
    int error = closesocket(m_socket);
    runtime_assert(error, "Error closing socket : ");
}

////////////////////////////////////////////////////////////////

bool UdpSocket::send(const RawNetworkBytes& _bytes) {
    if (!m_hasRemote)
        return false;

    return sendTo(m_remote, _bytes);
}

bool UdpSocket::sendTo(Address& _addr, const RawNetworkBytes& _bytes) {
    int sent = ::sendto(
        m_socket,
        reinterpret_cast<const char*>(_bytes.data),
        _bytes.count,
        0,
        _addr.native(),
        _addr.size()
    );

    return sent == static_cast<int>(_bytes.count);
}

bool UdpSocket::receive(RawNetworkBytes& _bytes, Address* _sender) {
    Address tempFrom;
    sockaddr* addr = _sender ? _sender->native() : tempFrom.native();
    uint addrSize = _sender ? _sender->size() : tempFrom.size();

    int length = recvfrom(
        m_socket,
        reinterpret_cast<char*>(_bytes.data),
        DEFAULT_PACKET_SIZE,
        0,
        addr,
        reinterpret_cast<int*>(&addrSize)
    );

    _bytes.count = (length > 0) ? length : 0;

    // Learn remote (For servers replies)
    if (!m_hasRemote && length > 0) {
        m_remote = _sender ? *_sender : tempFrom;
        m_hasRemote = true;
    }

    return length > 0;
}

const Address& UdpSocket::remote() const {
    return m_remote;
}

void UdpSocket::setRemote(const Address& addr) {
    m_remote = addr;
    m_hasRemote = true;
}