#include "SnakeBody.hpp"
#include "Config.hpp"
#include <algorithm>

SnakeBody::SnakeBody() : _body()
{
	static const Tile INIT_POINT_IN_MIDDLE{
		Config::BOARD_SIZE / 2,
		Config::BOARD_SIZE / 2,
	};

	_body.push_back(INIT_POINT_IN_MIDDLE);
}

void SnakeBody::step(const Direction direction)
{
	const Tile& current_head = head();
	Tile next = head();

	if (current_head.x() == 0 && direction == Direction::Left) {
		next.set_x(Config::BOARD_SIZE - 1);
	}
	else if (current_head.y() == 0 && direction == Direction::Up) {
		next.set_y(Config::BOARD_SIZE - 1);
	}
	else if (current_head.x() == Config::BOARD_SIZE - 1 && direction == Direction::Right) {
		next.set_x(0);
	}
	else if (current_head.y() == Config::BOARD_SIZE - 1 && direction == Direction::Down) {
		next.set_y(0);
	}
	else {
		switch (direction) {
		case Direction::Down:
			next.set_y(next.y() + 1);
			break;
		case Direction::Up:
			next.set_y(next.y() - 1);
			break;
		case Direction::Right:
			next.set_x(next.x() + 1);
			break;
		case Direction::Left:
			next.set_x(next.x() - 1);
			break;
		}
	}

	_body.push_front(std::move(next));
}

Tile SnakeBody::pop_tail()
{
	const Tile pt = _body.back();
	_body.pop_back();
	return pt;
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
	for (const Tile& point : _body) {
		if (&snake_head == &point) {
			continue;
		}
		if (snake_head == point) {
			return true;
		}
	}
	return false;
}

bool SnakeBody::has_x(const uint32_t value) const
{
	return std::any_of(_body.begin(), _body.end(), [value](const Tile& t) {return t.x() == value; });
}

bool SnakeBody::has_y(const uint32_t value) const
{
	return std::any_of(_body.begin(), _body.end(), [value](const Tile& t) {return t.y() == value; });
}
