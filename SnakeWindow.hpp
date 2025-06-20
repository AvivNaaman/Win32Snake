#pragma once
#include "Window.hpp"
#include "Direction.hpp"
#include "SnakeBody.hpp"
#include "Food.hpp"

class SnakeWindow : public Window
{
public:
	SnakeWindow(const HINSTANCE instance);
	~SnakeWindow() final = default;

public:
	void run();

protected:
	[[nodiscard]] LRESULT callback(const UINT uMsg, const WPARAM wParam, const LPARAM lParam) final;

private:
	void on_timer(const std::shared_ptr<Timer>& timer);
	void game_over();

private:
	static void on_timer_cb(Window& this_window, const std::shared_ptr<Timer>& timer);
	[[nodiscard]] static Direction vkey_to_direction(const WPARAM vkey);

private:
	Direction _last_direction;
	Direction _pending_direction;
	Food _food;
	SnakeBody _snake_body;

public:
	SnakeWindow(const SnakeWindow&) = delete;
	SnakeWindow& operator=(const SnakeWindow&) = delete;
};

