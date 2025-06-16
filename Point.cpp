#include "Point.hpp"

void Point::fill(DeviceContextBase& dc, const HBRUSH brush) const
{
	static constexpr float32_t ratio = static_cast<float32_t>(Config::BOARD_RESOLUTION) / Config::BOARD_SIZE;
	static constexpr uint32_t NEXT_TILE = 1;
	const RECT r{
		static_cast<LONG>(_x * ratio),
		static_cast<LONG>(_y * ratio),
		static_cast<LONG>((_x + NEXT_TILE) * ratio),
		static_cast<LONG>((_y + NEXT_TILE) * ratio)
	};

	dc.fill_rect(r, brush);
}
