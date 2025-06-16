#pragma once
#include <Windows.h>

class DeviceContextBase
{
public:
	DeviceContextBase(const HDC device_context);
	virtual ~DeviceContextBase() = default;

public:
	void fill_rect(const RECT& rect, const HBRUSH brush);

protected:
	const HDC _device_context;

public:
	DeviceContextBase(const DeviceContextBase&) = delete;
	DeviceContextBase& operator=(const DeviceContextBase&) = delete;
};

