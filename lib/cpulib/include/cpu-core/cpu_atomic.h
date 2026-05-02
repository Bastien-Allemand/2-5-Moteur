#pragma once

template <typename T>
struct cpu_atomic
{
public:
	void operator=(const T& v) { val = v; }
	T Add(T v) { return val.fetch_add(v, std::memory_order_relaxed); }

private:
	std::atomic<T> val;
};
