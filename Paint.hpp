#pragma once
#include "DeviceContextBase.hpp"

class Paint final : public DeviceContextBase
{
public:
	explicit Paint(const HWND window);
	~Paint() final;

public:
	void fill(const HBRUSH brush);

private:
	[[nodiscard]] static HDC begin_paint(const HWND window, PAINTSTRUCT* const paint_struct);

private:
	PAINTSTRUCT _paint_struct;
	const HWND _window;

public:
	Paint(const Paint&) = delete;
	Paint& operator=(const Paint&) = delete;
};

