#pragma once

#include <Windows.h>
#include "DeviceContextBase.hpp"

class DeviceContext final : public DeviceContextBase
{
public:
	DeviceContext(const HWND window);
	~DeviceContext();

private:
	static HDC get_dc(const HWND window);

private:
	const HWND _window;

public:
	DeviceContext(const DeviceContext& other) = delete;
	DeviceContext& operator=(const DeviceContext& other) = delete;
};

