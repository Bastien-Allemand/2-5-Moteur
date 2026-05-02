#include "pch.h"
#include "message-registry.h"

MessageRegistry* MessageRegistry::m_inst = nullptr;

MessageRegistry& MessageRegistry::get() {
	if (m_inst == nullptr) {
		m_inst = new MessageRegistry();
	}
	return *m_inst;
}

void MessageRegistry::release() {
	delete m_inst;
	m_inst = nullptr;
}

void MessageRegistry::registerType(MessageID _id, MessageFactory _factory) {
	m_factories[_id] = _factory;
}

MessageData* MessageRegistry::create(MessageID _id) const {
	auto it = m_factories.find(_id);
	return (it != m_factories.end()) ? it->second() : nullptr;
}