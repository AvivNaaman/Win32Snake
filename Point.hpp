#pragma once
#include <cstdint>
#include <Windows.h>
#include <exception>

#include "Config.hpp"
#include "DeviceContextBase.hpp"

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

	void fill(DeviceContextBase& dc, const HBRUSH brush) const;

private:
	uint32_t _x, _y;
};

