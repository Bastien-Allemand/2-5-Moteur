#include "pch.h"
#include "hash.h"

uint32_t crc32(const void* _content, size_t _len)
{
	const uint8_t* byteContent = static_cast<const uint8_t*>(_content);
	uint32_t crc = 0xFFFFFFFF;

	for (size_t i = 0; i < _len; i++)
	{
		crc ^= byteContent[i];
		for (int j = 0; j < 8; j++)
		{
			if (crc & 1)
			{
				crc = (crc >> 1) ^ 0xEDB88320;
			}
			else
			{
				crc = crc >> 1;
			}
		}
	}
	return ~crc;
}