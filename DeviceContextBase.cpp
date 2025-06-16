#include "DeviceContextBase.hpp"
#include <exception>

DeviceContextBase::DeviceContextBase(const HDC device_context) :
	_device_context(device_context)
{
}

void DeviceContextBase::fill_rect(const RECT& rect, const HBRUSH brush)
{
	const int fill_rect_result = FillRect(_device_context, &rect, brush);
	static constexpr int FILL_RECT_FAIL = 0;
	if (fill_rect_result == FILL_RECT_FAIL) {
		throw std::exception("Failed to FillRect()!");
	}
}
