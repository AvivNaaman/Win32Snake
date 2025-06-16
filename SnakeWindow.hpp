#pragma once
#include "Window.hpp"
#include "Direction.hpp"
#include "Point.hpp"
#include "SnakeBody.hpp"

class SnakeWindow : private Window
{
public:
	SnakeWindow(HINSTANCE instance);
	~SnakeWindow() final = default;

public:
	void run();

protected:
	LRESULT callback(const UINT uMsg, const WPARAM wParam, const LPARAM lParam) final;

private:
	void next_food();
	void on_timer(const std::shared_ptr<Timer>& timer);

private:
	static void on_timer_cb(Window& this_window, const std::shared_ptr<Timer>& timer);

private:
	Direction _last_direction;
	Direction _pending_direction;
	Point _food;
	SnakeBody _snake_body;
};

