#include "render-service.h"

#pragma once

template <typename T, typename ... Args>
T& RenderService::spawn(Args... _args) {
    T* element = new T(_args...);
    m_toAdd.push_back(element);
    return *element;
}