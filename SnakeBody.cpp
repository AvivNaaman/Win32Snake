#include "SnakeBody.hpp"
#include "Config.hpp"
#include <algorithm>

SnakeBody::SnakeBody() : _body()
{
	static const Tile MIDDLE{
		Config::BOARD_SIZE / 2,
		Config::BOARD_SIZE / 2,
	};

	_body.push_back(MIDDLE);
}

void SnakeBody::step(const Direction direction)
{
	const Tile& current_head = head();
	Tile next = head();

	static constexpr uint32_t START_EDGE = 0;
	static constexpr uint32_t END_EDGE = Config::BOARD_SIZE - 1;
	if (current_head.x() == START_EDGE && direction == Direction::Left) {
		next.set_x(END_EDGE);
	}
	else if (current_head.y() == START_EDGE && direction == Direction::Up) {
		next.set_y(END_EDGE);
	}
	else if (current_head.x() == END_EDGE && direction == Direction::Right) {
		next.set_x(START_EDGE);
	}
	else if (current_head.y() == END_EDGE && direction == Direction::Down) {
		next.set_y(START_EDGE);
	}
	else {
		static constexpr uint32_t NEXT_TILE = 1;
		static constexpr uint32_t PREV_TILE = 1;
		switch (direction) {
		case Direction::Right:
			next.set_x(next.x() + NEXT_TILE);
			break;
		case Direction::Left:
			next.set_x(next.x() - PREV_TILE);
			break;
		case Direction::Down:
			next.set_y(next.y() + NEXT_TILE);
			break;
		case Direction::Up:
			next.set_y(next.y() - PREV_TILE);
			break;
		}
	}

	_body.push_front(std::move(next));
}

Tile SnakeBody::pop_tail()
{
	const Tile tail = _body.back();
	_body.pop_back();
	return tail;
}

const Tile& SnakeBody::head() const
{
	return _body.front();
}

const Tile& SnakeBody::tail() const
{
	return _body.back();
}

bool SnakeBody::head_collides_body() const
{
	const Tile& snake_head = head();
	for (const Tile& point : _body)
	{
		if (&snake_head == &point)
		{
			continue;
		}
		if (snake_head == point)
		{
			return true;
		}
	}
	return false;
}

bool SnakeBody::has_x(const uint32_t x_value) const
{
	return std::any_of(_body.begin(), _body.end(), [x_value](const Tile& t) { return t.x() == x_value; });
}

bool SnakeBody::has_y(const uint32_t y_value) const
{
	return std::any_of(_body.begin(), _body.end(), [y_value](const Tile& t) { return t.y() == y_value; });
}

uint32_t SnakeBody::size() const
{
	return static_cast<uint32_t>(_body.size());
}
