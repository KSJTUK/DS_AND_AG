#include <iostream>
#include <Windows.h>

#define DENIED_SYNC_STDIO std::cout.sync_with_stdio(false)

int main(int argc, char* argv[])
{
	DENIED_SYNC_STDIO;

	std::cout << "Hello World" << std::endl;
}