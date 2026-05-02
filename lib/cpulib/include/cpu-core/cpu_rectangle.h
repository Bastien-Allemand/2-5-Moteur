#pragma once

struct cpu_rectangle
{
public:
	XMFLOAT2 min;
	XMFLOAT2 max;

public:
	cpu_rectangle();

	void Zero();
};
