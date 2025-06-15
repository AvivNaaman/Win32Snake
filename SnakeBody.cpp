#include "SnakeBody.hpp"
#include "Config.hpp"

SnakeBody::SnakeBody() : _body()
{
	static const Point INIT_POINT_IN_MIDDLE{
		Config::BOARD_SIZE / 2,
		Config::BOARD_SIZE / 2,
	};

	_body.push_back(INIT_POINT_IN_MIDDLE);
}

void SnakeBody::step(const Direction direction)
{
	const Point& current_head = head();
	Point next = head();

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

Point SnakeBody::pop_tail()
{
	const Point pt = _body.back();
	_body.pop_back();
	return pt;
}

const Point& SnakeBody::head() const
{
	return _body.front();
}

const Point& SnakeBody::tail() const
{
	return _body.back();
}

bool SnakeBody::head_collides() const
{
	const Point& snake_head = head();
	for (const Point& point : _body) {
		if (&snake_head == &point) {
			continue;
		}
		if (snake_head == point) {
			return true;
		}
	}
	return false;
}
