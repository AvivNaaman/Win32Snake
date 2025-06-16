#include <Windows.h>
#include "SnakeWindow.hpp"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
	SnakeWindow w(hInstance);
	w.run();
	return 0;
}