#pragma once

#include <vector>

// forward declarations
class Service;

#define APPLICATION Application::get()

class Application {
public:
	static Application& get();

	int run();
	void shutdown();

	template <typename T, typename... Args>
	T& registerService(Args... _args);

	template <typename T>
	T& getService();

private:
	Application();
	~Application();

	std::vector<Service*> m_services;
	bool m_running;

	static Application* m_inst;
};

#include "application.inl"