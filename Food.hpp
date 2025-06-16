#pragma once
#include "Tile.hpp"
#include "SnakeBody.hpp"
#include <optional>

class Food final
{
public:
	explicit Food();
	~Food() = default;

public:
	void next(const SnakeBody& avoid);
	[[nodiscard]] const Tile& tile() const;

private:
	Tile _tile;

public:
	Food(const Food&) = delete;
	Food& operator=(const Food&) = delete;
};

