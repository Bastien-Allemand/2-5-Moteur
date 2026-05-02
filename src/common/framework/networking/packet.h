#pragma once

// Local dependencies
#include "owner.h"
#include "bytestream.h"
#include "../../utils/types.h"
#include "../../utils/networking/socket.h"
#include "../../utils/networking/address.h"

struct PacketDescriptor {
	uint magic_number;
	uint crc;
	OwnerID sender;
	byte age;

};

struct Packet {
	PacketDescriptor header;
	RawNetworkBytes body;

	inline void toBytes(RawNetworkBytes& _bytes) {
		ByteWriter writer{ _bytes };
		
		writer.write<uint>(header.magic_number);
		writer.write<uint>(header.magic_number);
		writer.write<uint>(header.crc);
		writer.write<uint>(header.sender);
		writer.write<byte>(header.age);

		writer.memcpy(body.data, body.count);
	}
	inline void fromBytes(const RawNetworkBytes& _bytes) {
		ByteReader reader{ _bytes };
		
		header.magic_number = reader.read<uint>();
		header.magic_number = reader.read<uint>();
		header.crc = reader.read<uint>();
		header.sender = reader.read<uint>();
		header.age = reader.read<byte>();

		body = reader.remainingBytes();
	}
};