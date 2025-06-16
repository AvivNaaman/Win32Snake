#include "SnakeWindow.hpp"
#include "DeviceContext.hpp"
#include <random>
#include <string>

SnakeWindow::SnakeWindow(const HINSTANCE instance) :
	Window{
		instance,
		RECT{
			0, 0, Config::BOARD_RESOLUTION, Config::BOARD_RESOLUTION,
		},
		std::wstring(Config::GAME_WINDOW_NAME)
},
_last_direction(Direction::Right),
_pending_direction(Direction::Right),
_snake_body(),
_food()
{
}

void SnakeWindow::run()
{
	set_timer(Config::TEMPO, SnakeWindow::on_timer_cb);
	show();

	message_loop();
}

LRESULT SnakeWindow::callback(const UINT msg, const WPARAM w_param, const LPARAM l_param) {
	if (msg == WM_KEYDOWN)
	{
		if (VK_LEFT <= w_param && w_param <= VK_DOWN) {
			_pending_direction = vkey_to_direction(w_param);
		}
	}
	return Window::callback(msg, w_param, l_param);
}


void SnakeWindow::on_timer(const std::shared_ptr<Timer>& timer)
{
	if (!(_pending_direction == Direction::Up && _last_direction == Direction::Down ||
		_pending_direction == Direction::Down && _last_direction == Direction::Up ||
		_pending_direction == Direction::Left && _last_direction == Direction::Right ||
		_pending_direction == Direction::Right && _last_direction == Direction::Left)) {
		_last_direction = _pending_direction;
	}

	_snake_body.step(_last_direction);

	DeviceContext dc = get_dc();

	if (_snake_body.head_collides_body()) {
		timer->kill();
		game_over();
		return;
	}

	if (_snake_body.head() == _food.tile()) {
		_food.tile().fill(dc, Config::BACKGROUND_BRUSH);
		_food.next(_snake_body);
	}
	else {
		const Tile tail = _snake_body.pop_tail();
		tail.fill(dc, Config::BACKGROUND_BRUSH);
	}
	_food.tile().fill(dc, CreateSolidBrush(Config::FOOD_COLOR));

	_snake_body.head().fill(dc, CreateSolidBrush(Config::SNAKE_BODY_COLOR));
}

void SnakeWindow::game_over()
{
	static constexpr uint32_t SCORE_FACTOR = 10;
	const uint32_t score = SCORE_FACTOR * _snake_body.size();
	static constexpr std::wstring_view GAME_OVER_CAPTION = L"Game over!";
	std::wstring game_over_message = L"Game over!\nYou scored " + std::to_wstring(score) + L" points!";
	message_box(GAME_OVER_CAPTION.data(), game_over_message);
}

Direction SnakeWindow::vkey_to_direction(const WPARAM vkey)
{
	switch (vkey)
	{
	case VK_UP:
		return Direction::Up;
	case VK_DOWN:
		return Direction::Down;
	case VK_LEFT:
		return Direction::Left;
	case VK_RIGHT:
		return Direction::Right;
	}
	throw std::exception("Key is not an arrow VKEY!");
}


void SnakeWindow::on_timer_cb(Window& this_window, const std::shared_ptr<Timer>& timer)
{
	dynamic_cast<SnakeWindow&>(this_window).on_timer(timer);
}