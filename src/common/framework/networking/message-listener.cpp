#include "pch.h"
#include "message-listener.h"

MessageListenerBase::MessageListenerBase(MessageID _id) {
	m_id = _id;
}

MessageID MessageListenerBase::getMessageID() const {
	return m_id;
}