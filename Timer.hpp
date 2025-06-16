#pragma once
#include <Windows.h>

#include <chrono>
#include <memory>
#include <functional>

class Timer
{
	friend class Window;

protected:
	Timer(const HWND _window_handle, const uint64_t timer_id, const std::chrono::milliseconds interval, std::function<void(Window&, const std::shared_ptr<Timer>&)> callback);

public:
	virtual ~Timer();

public:
	void kill();
	bool alive() const;
	uint64_t id() const;

private:
	static uint64_t set_timer(const HWND _window_handle, const uint64_t timer_id, const std::chrono::milliseconds interval);

private:
	const HWND _window_handle;
	uint64_t _id;

protected:
	const std::function<void(Window&, const std::shared_ptr<Timer>&)> _callback;
};

