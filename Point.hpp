#pragma once
#include <cstdint>
#include <Windows.h>
#include "Config.hpp"
#include <exception>

class Point
{
public:
	explicit Point() : Point(uint32_t{}, uint32_t{}) {}
	explicit Point(const uint32_t x, const uint32_t y) : _x(x), _y(y) {}
	Point(const Point& p) = default;
	virtual ~Point() = default;

public:
	Point& operator=(const Point&) = default;

	bool operator==(const Point& other) const {
		return other._x == _x && other._y == _y;
	}

	bool operator!=(const Point& other) const {
		return !(*this == other);
	}

public:
	uint32_t x() const {
		return _x;
	}

	uint32_t y() const {
		return _y;
	}

	void set_x(uint32_t x) { _x = x; }
	void set_y(uint32_t y) { _y = y; }

	void fill(const HDC hdc, const HBRUSH brush) const {
		static constexpr float32_t ratio = static_cast<float32_t>(Config::BOARD_RESOLUTION) / Config::BOARD_SIZE;
		static constexpr uint32_t NEXT_TILE = 1;
		const RECT r{
			static_cast<LONG>(_x * ratio),
			static_cast<LONG>(_y * ratio),
			static_cast<LONG>((_x + NEXT_TILE) * ratio),
			static_cast<LONG>((_y + NEXT_TILE) * ratio)
		};

		const int fill_rect_result = FillRect(hdc, &r, brush);
		static constexpr int FILL_RECT_FAIL = 0;
		if (fill_rect_result == FILL_RECT_FAIL) {
			throw std::exception("Failed to FillRect!");
		}
	}

private:
	uint32_t _x, _y;
};

