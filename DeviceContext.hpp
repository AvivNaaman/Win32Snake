#pragma once

#include <Windows.h>
#include "DeviceContextBase.hpp"

class DeviceContext final : public DeviceContextBase
{
public:
	DeviceContext(const HWND window);
	~DeviceContext();

private:
	[[nodiscard]] static HDC get_dc(const HWND window);

private:
	const HWND _window;

public:
	DeviceContext(const DeviceContext&) = delete;
	DeviceContext& operator=(const DeviceContext&) = delete;
};

