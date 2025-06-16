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
	explicit Window(const HINSTANCE hInstance, const RECT& windowRect);
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
	virtual LRESULT callback(const UINT uMsg, const WPARAM wParam, const LPARAM lParam);

private:
	static HWND initialize_window(const HINSTANCE hInstance, const RECT& windowRect);
	static void initialize_window_class(const HINSTANCE hInstance);

	static LRESULT CALLBACK window_callback_proc(const HWND hwnd, const UINT uMsg, const WPARAM wParam, const LPARAM lParam);
	static VOID CALLBACK timers_callback(HWND hwnd, UINT, UINT_PTR, DWORD);

private:
	HWND _handle;

private:
	std::map<uint64_t, std::shared_ptr<Timer>> _timers;
};

