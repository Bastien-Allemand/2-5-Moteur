#pragma once

#include "message.h"

#include <utils/types.h>
#include <unordered_map>

using MessageFactory = MessageData* (*)();

class MessageRegistry {
public:
    static MessageRegistry& get();
	static void release();

    void registerType(MessageID _id, MessageFactory _factory);

    MessageData* create(MessageID _id) const;
private:
    MessageRegistry() = default;
    ~MessageRegistry() = default;

private:
    static MessageRegistry* m_inst;

    std::unordered_map<MessageID, MessageFactory> m_factories;
};