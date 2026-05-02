#pragma once

#include <stdint.h>

// Taken from github : https://gist.github.com/ruby0x1/81308642d0325fd386237cfa3b44785c
constexpr uint32_t fnv1a(const char* _str, uint32_t _h = 2166136261u) {
    return *_str ? fnv1a(_str + 1, (_h ^ static_cast<uint8_t>(*_str)) * 16777619u) : _h;
}

uint32_t crc32(const void* _content,size_t _len);