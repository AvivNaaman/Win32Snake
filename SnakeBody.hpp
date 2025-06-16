#pragma once

#include "Direction.hpp"
#include "Tile.hpp"
#include <deque>

class SnakeBody final
{
public:
	SnakeBody();
	~SnakeBody() = default;

public:
	void step(const Direction direction);
	Tile pop_tail();

	[[nodiscard]] const Tile& head() const;
	[[nodiscard]] const Tile& tail() const;

	[[nodiscard]] bool head_collides_body() const;
	[[nodiscard]] bool has_x(const uint32_t value) const;
	[[nodiscard]] bool has_y(const uint32_t value) const;

private:
	std::deque<Tile> _body;

public:
	SnakeBody(const SnakeBody&) = delete;
	SnakeBody& operator=(const SnakeBody&) = delete;
};

