#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

#ifdef new
#undef new
#endif

#ifdef min
#undef min
#endif

#ifdef max
#undef max
#endif