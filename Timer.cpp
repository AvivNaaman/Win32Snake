#include "Timer.hpp"


static constexpr UINT_PTR INVALID_TIMER_ID = 0;

Timer::Timer(const HWND window_handle,
	const uint64_t timer_id,
	const std::chrono::milliseconds interval,
	const std::function<void(Window&, const std::shared_ptr<Timer>&)> callback) :
	_window_handle(window_handle),
	_id(set_timer(_window_handle, timer_id, interval)),
	_callback(callback)
{
}

void Timer::kill()
{
	if (!KillTimer(_window_handle, _id))
	{
		throw std::exception("Failed to KillTimer()!");
	}
	_id = INVALID_TIMER_ID;
}

bool Timer::alive() const
{
	return _id != INVALID_TIMER_ID;
}

uint64_t Timer::id() const
{
	return _id;
}

Timer::~Timer()
{
	try
	{
		if (!alive())
		{
			return;
		}
		kill();
	}
	catch (...)
	{
		OutputDebugStringW(L"Exception in ~Timer()!");
	}
}

uint64_t Timer::set_timer(const HWND _window_handle, const uint64_t timer_id, const std::chrono::milliseconds interval)
{
	static constexpr TIMERPROC USE_WINDOW_CALLBACK = nullptr;
	const UINT_PTR timer = SetTimer(_window_handle, timer_id, static_cast<UINT>(interval.count()), USE_WINDOW_CALLBACK);

	static constexpr UINT_PTR SET_TIMER_FAIL = 0;
	if (timer == SET_TIMER_FAIL)
	{
		throw std::exception("SetTimer() failed!");
	}

	return timer;
}
