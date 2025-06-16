#pragma once

#include <cstdint>
#include <Windows.h>
#include <chrono>
#include <string_view>

namespace Colors {
	static constexpr COLORREF RGB_RED = 0x000000FF;
	static constexpr COLORREF RGB_GREEN = 0x0000FF00;
}

class Config final
{
public:
	static constexpr uint32_t BOARD_SIZE = 25;
	static constexpr std::chrono::milliseconds TEMPO{ 250 };

	static constexpr std::wstring_view GAME_WINDOW_NAME = L"Snake";

	static constexpr uint32_t BOARD_RESOLUTION = 500;
	static inline const HBRUSH BACKGROUND_BRUSH = reinterpret_cast<HBRUSH>(COLOR_ACTIVECAPTION + 1);

	static constexpr COLORREF SNAKE_BODY_COLOR = Colors::RGB_GREEN;
	static constexpr COLORREF FOOD_COLOR = Colors::RGB_RED;

public:
	Config() = delete;
	~Config() = delete;
};

