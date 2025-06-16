#pragma once

#include <Windows.h>
#include <memory>
#include "Timer.hpp"
#include <map>
#include <functional>
#include "DeviceContext.hpp"

class Window
{
public:
	explicit Window(const HINSTANCE hInstance, const RECT& rect, const std::wstring& name);
	virtual ~Window();


public:
	void show();
	void message_loop();

public:
	std::shared_ptr<Timer> set_timer(const std::chrono::milliseconds interval, std::function<void (Window&, const std::shared_ptr<Timer>&)>);
	std::shared_ptr<Timer> get_timer(const uint64_t timer_id);
	bool has_timer(const uint64_t timer_id) const;

	DeviceContext get_dc();

protected:
	virtual LRESULT callback(const UINT msg, const WPARAM w_param, const LPARAM l_param);

private:
	static HWND initialize_window(const HINSTANCE instance, const RECT& rect, const std::wstring& name);
	static void register_window_class(const HINSTANCE instance);

	static LRESULT CALLBACK window_callback_proc(const HWND hwnd, const UINT uMsg, const WPARAM wParam, const LPARAM lParam);

private:
	HWND _handle;

private:
	uint64_t _timer_id_counter;
	std::map<uint64_t, std::shared_ptr<Timer>> _timers;
};

