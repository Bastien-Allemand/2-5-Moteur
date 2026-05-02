#include "pch.h"
#include <framework/application.h>

#include "services/client-service.h"
#include "services/render-service.h"

int main() {
	// ip bastien	: 10.10.137.44
	// ip ethan		: 10.10.137.42
	
#ifdef _DEBUG
	_CrtMemState memStateInit;
	_CrtMemCheckpoint(&memStateInit);
#endif
	{
		Application& app = APPLICATION;
		app.registerService<ClientService>();
		app.registerService<RenderService>();
	
		int result = app.run();
	}
#ifdef _DEBUG
	_CrtMemState memState, memStateDiff;
	_CrtMemCheckpoint(&memState);
	if ( _CrtMemDifference(&memStateDiff, &memStateInit, &memState) )
		MessageBoxA(nullptr, "Memory leaks", "ALERT", 0);
	_CrtDumpMemoryLeaks();
#endif

	return 0;
}