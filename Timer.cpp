#include "Timer.hpp"


static constexpr UINT_PTR INVALID_TIMER_ID = 0;

Timer::Timer(const HWND window_handle,
	const std::chrono::milliseconds interval,
	const std::function<void(Window&, const std::shared_ptr<Timer>&)> callback,
	const TIMERPROC window_timers_callback) :
	_window_handle(window_handle),
	_id(set_timer(_window_handle, interval, window_timers_callback)),
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

uint64_t Timer::set_timer(const HWND _window_handle, const std::chrono::milliseconds interval, const TIMERPROC window_timers_callback)
{
	static constexpr ULONG_PTR GENERATE_NEW_ID = NULL;
	static constexpr TIMERPROC USE_WINDOW_CALLBACK = nullptr;
	const UINT_PTR timer = SetTimer(_window_handle, GENERATE_NEW_ID, static_cast<UINT>(interval.count()), window_timers_callback);

	static constexpr UINT_PTR SET_TIMER_FAIL = 0;
	if (timer == SET_TIMER_FAIL)
	{
		throw std::exception("SetTimer() failed!");
	}

	return timer;
}
