#pragma once

class Service {
public:
	Service();
	virtual ~Service();

	virtual void OnStartup();
	virtual void OnUpdate();
	virtual void OnShutdown();
	
protected:
	bool m_initialized;
	
private:

};