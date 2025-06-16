#pragma once
#include <cstdint>
#include <Windows.h>
#include <exception>

#include "Config.hpp"
#include "DeviceContextBase.hpp"

class Tile
{
public:
	explicit Tile() : Tile(uint32_t{}, uint32_t{}) {}
	explicit Tile(const uint32_t x, const uint32_t y) : _x(x), _y(y) {}
	Tile(const Tile& p) = default;
	virtual ~Tile() = default;

public:
	Tile& operator=(const Tile&) = default;

	bool operator==(const Tile& other) const {
		return other._x == _x && other._y == _y;
	}

	bool operator!=(const Tile& other) const {
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

