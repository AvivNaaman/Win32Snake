#pragma once
#include <cstdint>
#include <Windows.h>
#include <exception>

#include "Config.hpp"
#include "DeviceContextBase.hpp"

class Tile final
{
public:
	explicit Tile() : Tile({}, {}) {}
	explicit Tile(const uint32_t x, const uint32_t y) : _x(x), _y(y) {}
	Tile(const Tile& p) = default;
	~Tile() = default;

public:
	Tile& operator=(const Tile&) = default;

public:
	[[nodiscard]] bool operator==(const Tile& other) const {
		return other._x == _x && other._y == _y;
	}

	[[nodiscard]] bool operator!=(const Tile& other) const {
		return !(*this == other);
	}

public:
	[[nodiscard]] uint32_t x() const { return _x; }
	[[nodiscard]] uint32_t y() const { return _y; }
	void set_x(uint32_t x) { _x = x; }
	void set_y(uint32_t y) { _y = y; }

public:
	void fill(DeviceContextBase& dc, const HBRUSH brush) const;

private:
	uint32_t _x, _y;
};

