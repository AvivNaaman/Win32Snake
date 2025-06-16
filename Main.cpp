#include <Windows.h>
#include "SnakeWindow.hpp"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int) {
	SnakeWindow w(hInstance);
	w.run();
	return 0;
}