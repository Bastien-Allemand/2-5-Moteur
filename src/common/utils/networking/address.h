#pragma once

// Local dependencies
#include "../types.h"

// Forward declarations
struct sockaddr;
struct sockaddr_storage;

class Address {
public:
    Address();
    Address(cstr ip, ushort port);
    Address(ushort port); // Bind-any (server)

    Address(const Address& other);
    Address& operator=(const Address& other);

    ~Address();

    cstr ip() const;
    ushort port() const;

    const sockaddr* native() const;
    sockaddr* native();
    uint size() const;
    
    bool operator==(const Address& other) const;
    bool operator!=(const Address& other) const;

private:
    sockaddr_storage* m_storage;
};