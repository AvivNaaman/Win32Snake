#include "Food.hpp"
#include <random>

Food::Food() : _tile()
{
	next(SnakeBody());
}

void Food::next(const SnakeBody& avoid)
{
	std::random_device rd;
	std::mt19937 generator(rd());
	std::uniform_int_distribution<uint32_t> board_distribution(0, Config::BOARD_SIZE - 1);

	uint32_t new_x = 0;
	do
	{
		new_x = board_distribution(generator);
	} 
	while (avoid.has_x(new_x));

	uint32_t new_y = 0;
	do
	{
		new_y = board_distribution(generator);
	} 
	while (avoid.has_x(new_x));

	_tile = Tile(new_x, new_y);
}

const Tile& Food::tile() const
{
	return _tile;
}
