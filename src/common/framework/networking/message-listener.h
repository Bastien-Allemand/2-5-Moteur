#pragma once

#include "message.h"

/// <summary>
/// A callback function type for message listeners.
/// </summary>
/// <param name="M">(typename) The message data type.</param>
/// <param name="T">(typename) The class type of the listener.</param>
template <typename M, typename T>
using MessageListenerCallback = void(T::*)(M*, OwnerID);

class MessageListenerBase {
public:
	MessageListenerBase(MessageID _id);
	virtual ~MessageListenerBase() = default;

	virtual void call(MessageData* _data, OwnerID _owner) = 0;

	MessageID getMessageID() const;

protected:
	MessageID m_id;
};

/// <summary>
/// A message listener that calls a specific callback when a message of a certain type is received.
/// </summary>
/// <param name="M">(typename) The message data type.</param>
/// <param name="T">(typename) The class type of the listener.</param>
template <typename M, typename T>
class MessageListener : public MessageListenerBase {
public:
	MessageListener(MessageID _id, MessageListenerCallback<M, T> _callback, T* _inst);
	~MessageListener() override = default;

	void call(MessageData* _data, OwnerID _owner) override;

private:
	MessageListenerCallback<M, T> m_callback;
	T* m_inst;
};
#include "message-listener.inl"