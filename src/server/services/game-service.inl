#include <iostream>
#include <ostream>

#include "game-service.h"

#pragma once

template <typename T, typename ... Args>
inline T& GameService::spawn(Args... _args) {
	T* element = new T(_args...);
	m_toAdd.push_back(element);
	
	return *element;
}