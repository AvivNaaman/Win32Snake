#include "DeviceContext.hpp"
#include <exception>

DeviceContext::DeviceContext(const HWND window) : DeviceContextBase(get_dc(window)), _window(window)
{
}

HDC DeviceContext::get_dc(const HWND window)
{
	const HDC hdc = GetDC(window);
	if (hdc == NULL)
	{
		throw std::exception("Failed to initialize DeviceContext using GetDC(window)");
	}
	return hdc;
}

DeviceContext::~DeviceContext() {
	try
	{
		static constexpr int RELEASE_DC_FAILURE = 0;
		if (ReleaseDC(_window, _device_context) == RELEASE_DC_FAILURE)
		{
			OutputDebugStringW(L"Failed to ReleaseDC(_hdc)");
		}
	}
	catch (...)
	{
		OutputDebugStringW(L"Exception in ~DeviceContext()!");
	}
}
