#pragma once

#include "../../utils/types.h"
#include "../../utils/networking/socket.h"

struct ByteWriter {
	RawNetworkBytes& bytes;

	template<typename T>
	inline void write(const T& _value) {
		static_assert(std::is_trivially_copyable_v<T>,
			"Type must be trivially copyable");

		std::memcpy(&bytes.data[bytes.count], &_value, sizeof(T));
		bytes.count += sizeof(T);
	}

	inline void writeString(const std::string& _value) {
		size_t len = _value.length();
		const char* ptr = _value.c_str();

		std::memcpy(&bytes.data[bytes.count], ptr, len);
		bytes.count += len;
	}

	inline void memcpy(void* _src, size_t _size) {
		assert(_src != nullptr);
		assert(_size > 0);

		std::memcpy(&bytes.data[bytes.count], _src, _size);
		bytes.count += _size;
	}
};

struct ByteReader {
	const RawNetworkBytes& bytes;
	size_t cursor = 0;

	template<typename T>
	inline T read() {
		static_assert(std::is_trivially_copyable_v<T>,
			"Type must be trivially copyable");

		T value;
		std::memcpy(&value, &bytes.data[cursor], sizeof(T));
		cursor += sizeof(T);
		return value;
	}

	inline std::string readString() {
		return (char*)(bytes.data+cursor);
	}

	// safer subBytes and remainingBytes
	inline RawNetworkBytes subBytes(size_t _offset, size_t _size) {
		if (_offset > bytes.count) { // defensive
			RawNetworkBytes empty{};
			empty.count = 0;
			return empty;
		}
		size_t safe_size = std::min(_size, bytes.count - _offset);
		RawNetworkBytes sub;
		sub.count = safe_size;
		if (safe_size) {
			std::memcpy(&sub.data[0], &bytes.data[_offset], safe_size);
		}
		return sub;
	}

	inline RawNetworkBytes subBytes(size_t _size) {
		return subBytes(cursor, _size);
	}

	inline RawNetworkBytes remainingBytes() {
		if (cursor > bytes.count) {
			RawNetworkBytes empty{};
			empty.count = 0;
			return empty;
		}
		return subBytes(cursor, bytes.count - cursor);
	}

	inline void memcpy(void* _dest, size_t _size) {
		assert(_dest != nullptr);
		assert(_size > 0);

		std::memcpy(_dest, &bytes.data[cursor], _size);
		cursor += _size;
	}
};