#include "message-listener.h"

#pragma once

template<typename M, typename T>
inline MessageListener<M, T>::MessageListener(MessageID _id, MessageListenerCallback<M, T> _callback, T* _inst)
	: MessageListenerBase(_id) {
	m_callback = _callback;
	m_inst = _inst;
}

template<typename M, typename T>
inline void MessageListener<M, T>::call(MessageData* _data, OwnerID _owner) {
	M* castedData = static_cast<M*>(_data);
	(m_inst->*m_callback)(castedData, _owner);
}