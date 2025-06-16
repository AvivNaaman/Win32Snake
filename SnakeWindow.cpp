#include "SnakeWindow.hpp"
#include "DeviceContext.hpp"
#include <random>

SnakeWindow::SnakeWindow(HINSTANCE instance) :
	Window(
		instance,
		RECT{
			0, 0, Config::BOARD_RESOLUTION, Config::BOARD_RESOLUTION,
		}),
		_last_direction(Direction::Right),
		_pending_direction(Direction::Right),
		_snake_body(),
		_food()
{
}

void SnakeWindow::run()
{
	next_food();

	set_timer(Config::TEMPO, SnakeWindow::on_timer_cb);

	show();

	message_loop();
}

LRESULT SnakeWindow::callback(const UINT uMsg, const WPARAM wParam, const LPARAM lParam) {
	switch (uMsg) {
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_UP:
			_pending_direction = Direction::Up;
			break;
		case VK_DOWN:
			_pending_direction = Direction::Down;
			break;
		case VK_LEFT:
			_pending_direction = Direction::Left;
			break;
		case VK_RIGHT:
			_pending_direction = Direction::Right;
			break;
		}
	}
	return 0;
	}
	return Window::callback(uMsg, wParam, lParam);
}

void SnakeWindow::next_food()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<uint32_t> distrib(0, Config::BOARD_SIZE - 1);

	_food = Point{
		distrib(gen),
		distrib(gen)
	};
}

void SnakeWindow::on_timer(const std::shared_ptr<Timer>& timer)
{
	OutputDebugStringW(L"Timerproc!");


	if (!(_pending_direction == Direction::Up && _last_direction == Direction::Down ||
		_pending_direction == Direction::Down && _last_direction == Direction::Up ||
		_pending_direction == Direction::Left && _last_direction == Direction::Right ||
		_pending_direction == Direction::Right && _last_direction == Direction::Left)) {
		// Ignore the key press if it is the opposite direction
		_last_direction = _pending_direction;
		OutputDebugStringW(L"Direction changed, now it is: ");
		std::wstring directionString = _last_direction == Direction::Up ? L"Up\n" :
			_last_direction == Direction::Down ? L"Down\n" :
			_last_direction == Direction::Left ? L"Left\n" :
			L"Right\n";
		OutputDebugStringW(directionString.c_str());
	}

	_snake_body.step(_last_direction);

	DeviceContext dc = get_dc();

	if (_snake_body.head_collides()) {
		timer->kill();
		return;
	}

	if (_snake_body.head() == _food) {
		_food.fill(dc, Config::BACKGROUND_BRUSH);
		next_food();
	}
	else {
		const Point tail = _snake_body.pop_tail();
		tail.fill(dc, Config::BACKGROUND_BRUSH);
	}
	_food.fill(dc, CreateSolidBrush(Config::FOOD_COLOR));

	_snake_body.head().fill(dc, CreateSolidBrush(Config::SNAKE_BODY_COLOR));
}


void SnakeWindow::on_timer_cb(Window& this_window, const std::shared_ptr<Timer>& timer)
{
	dynamic_cast<SnakeWindow&>(this_window).on_timer(timer);
}