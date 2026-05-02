#include "pch.h"

#include <framework/application.h>

#include "services/game-service.h"
#include "services/server-service.h"

int main() {
	// ip bastien	: 10.10.137.44
	// ip ethan		: 10.10.137.42
	
	Application& app = APPLICATION;
	app.registerService<ServerService>();
	app.registerService<GameService>();

	return app.run();
}