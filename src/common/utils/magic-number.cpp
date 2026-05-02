#include "pch.h"
#include "magic-number.h"

const uint getMagicNumber() {
	uint x = MAGIC_NUMBER;

	x ^= x >> 16;
	x *= 0x7feb352d;
	x ^= x >> 15;
	x *= 0x846ca68b;
	x ^= x >> 16;

	x += 0x9e3779b9;
	x ^= x << 7;
	x *= 0x85ebca6b;
	x ^= x >> 13;
	x *= 0xc2b2ae35;
	x ^= x >> 16;

	return x;
}