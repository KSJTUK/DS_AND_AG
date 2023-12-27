#include "pch.h"

#define DENIED_SYNC_STDIO std::cout.sync_with_stdio(false)

Timer timer{ };

int main(int argc, char* argv[])
{
	DENIED_SYNC_STDIO;

	//HandleConsole::setConsoleTextColor(ConsoleColor::BLUE);
	//std::cout << "Hello World" << std::endl;

	Console console{ };
	while (true) {
		timer.Update();

		console.Write("Elapsed Time: " + std::to_string(timer.GetElapsedTime()));
		console.Write("Delta Time: " + std::to_string(timer.GetTimeDelta()));
		console.Write("FPS: " + std::to_string(timer.GetFPS()));
		console.Render();
		console.ClearText();
	}
}