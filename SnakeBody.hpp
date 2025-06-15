#pragma once

#include "Direction.hpp"
#include "Point.hpp"
#include <deque>

class SnakeBody final
{
public:
	SnakeBody();
	~SnakeBody() = default;

public:
	void step(const Direction direction);
	Point pop_tail();

	const Point& head() const;
	const Point& tail() const;

	bool head_collides() const;

private:
	std::deque<Point> _body;

public:
	SnakeBody(const SnakeBody&) = delete;
	SnakeBody& operator=(const SnakeBody&) = delete;
};

