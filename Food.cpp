#include "Food.hpp"
#include <random>

Food::Food() : _tile()
{
	next(SnakeBody());
}

void Food::next(const SnakeBody& avoid)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<uint32_t> distrib(0, Config::BOARD_SIZE - 1);

	uint32_t new_x = 0;
	do
	{
		new_x = distrib(gen);
	} while (avoid.has_x(new_x));

	uint32_t new_y = 0;
	do
	{
		new_y = distrib(gen);
	} while (avoid.has_x(new_x));

	_tile = Tile(new_x, new_y);
}

const Tile& Food::tile() const
{
	return _tile;
}
