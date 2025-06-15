#pragma once

#include <cstdint>

class Config final
{
public:
	static constexpr uint32_t BOARD_SIZE = 25;
	static constexpr uint32_t BOARD_RESOLUTION = 500;


public:
	Config() = delete;
	~Config() = delete;
};

