#pragma once

#include <stdexcept>

#include "application.h"

template <typename T, typename... Args>
inline T& Application::registerService(Args... _args) {
	T* service = new T(std::forward<Args>(_args)...);
	m_services.push_back(service);
	return *service;
}
template<typename T>
inline T& Application::getService() {
	for (auto it = m_services.begin(); it != m_services.end(); ++it) {
		if (T* service = dynamic_cast<T*>(*it)) {
			return *service;
		}
	}
	// TO DO : Throw an error !
	throw new std::runtime_error(__FUNCSIG__);
}