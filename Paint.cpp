#include "Paint.hpp"
#include <exception>

Paint::Paint(const HWND window) : DeviceContextBase(begin_paint(window, &_paint_struct)), _window(window)
{
}

Paint::~Paint() 
{
	try
	{
		if (!EndPaint(_window, &_paint_struct)) 
		{
			OutputDebugStringW(L"Failed to EndPaint!");
		}
	}
	catch (...)
	{
		OutputDebugStringW(L"Excpetion in ~Paint()!");
	}
}

void Paint::fill(const HBRUSH brush)
{
	fill_rect(_paint_struct.rcPaint, brush);
}

HDC Paint::begin_paint(const HWND window, PAINTSTRUCT* const paint_struct)
{
	const HDC hdc = BeginPaint(window, paint_struct);
	if (hdc == nullptr) 
	{
		throw std::exception("Failed to BeginPaint()!");
	}

	return hdc;
}
