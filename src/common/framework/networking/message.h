#pragma once

#include "../../utils/identification/hash.h"
#include "bytestream.h"
#include "packet.h"

using MessageID = uint;

struct MessageDescriptor {
    MessageID id;
};

struct Message {
    MessageDescriptor header;
    RawNetworkBytes body;

    inline void toPacket(Packet& _packet) {
        ByteWriter writer{ _packet.body };

		writer.write<MessageID>(header.id);

        if (body.count < 1)
            return; 
        
		writer.memcpy(body.data, body.count);
    }
    inline void fromPacket(const Packet& _packet) {
        ByteReader reader{ _packet.body };

        header.id = reader.read<MessageID>();

		body = reader.remainingBytes();
	}
};

struct MessageData {
    virtual ~MessageData() = default;
    
    virtual bool isValid() const = 0;
    
    virtual void serialize(ByteWriter& _writer) = 0;
    virtual void deserialize(ByteReader& _reader) = 0;
};

template<typename T>
constexpr  MessageID getMessageID() {
    return fnv1a(typeid(T).name(), 2166136261u);
}

#define REGISTER_MESSAGE(TYPE)                          \
    namespace {                                         \
        struct TYPE##Registrar                          \
        {                                               \
            TYPE##Registrar()                           \
            {                                           \
                MessageID id = getMessageID<TYPE>();    \
                MessageRegistry::get().registerType(    \
                    id,                                 \
                    []() -> MessageData* {              \
                        return new TYPE();              \
                    }                                   \
                );                                      \
            }                                           \
        };                                              \
        static TYPE##Registrar _reg_##TYPE;             \
    }