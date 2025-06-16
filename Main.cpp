#include <Windows.h>
#include "SnakeWindow.hpp"

int WINAPI wWinMain(const HINSTANCE hInstance, HINSTANCE, PWSTR, int) {
	SnakeWindow(hInstance).run();
	return 0;
}