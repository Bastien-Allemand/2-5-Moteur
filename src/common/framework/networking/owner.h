#pragma once

#include "../../utils/types.h"
#include "../../utils/networking/address.h"

#include <string>

using OwnerID = uint;

struct Owner {
	std::string username;
	bool connected;
	float timeSinceLastPing;
	Address address;
	void* userData;
};