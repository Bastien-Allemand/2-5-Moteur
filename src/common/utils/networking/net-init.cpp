#include "pch.h"
#include "net-init.h"

int netStart() {
	static bool initialized = false;
	static WSADATA data;

	if (initialized)
		return 0;

	if (WSAStartup(MAKEWORD(2, 2), &data) != 0) {
		initialized = true;
		return WSAGetLastError();
	}
}

int netShutdown() {
	if (WSACleanup() == SOCKET_ERROR)
		return WSAGetLastError();
}