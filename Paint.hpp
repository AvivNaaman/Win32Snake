#pragma once
#include "DeviceContextBase.hpp"

class Paint final : public DeviceContextBase
{
public:
	explicit Paint(const HWND _window);
	~Paint() final;

public:
	void fill(const HBRUSH brush);

private:
	static HDC begin_paint(const HWND window, PAINTSTRUCT* const paint_struct);

private:
	PAINTSTRUCT _paint_struct;
	HWND _window;
};

