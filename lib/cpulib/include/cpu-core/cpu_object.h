#pragma once

struct cpu_object
{
	int index;
	int sortedIndex;
	bool dead;

	cpu_object();
	virtual ~cpu_object() = default;
};
