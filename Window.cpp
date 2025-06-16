#include "Window.hpp"
#include <string_view>
#include "Config.hpp"
#include <exception>
#include "Paint.hpp"

static constexpr std::wstring_view WINDOW_CLASS_NAME = L"SNAKE";
static constexpr std::wstring_view WINDOW_NAME = L"Snake";

Window::Window(const HINSTANCE hInstance, const RECT& windowRect) : _handle(initialize_window(hInstance, windowRect))
{
	const ULONG_PTR set_ptr_result = SetWindowLongPtrW(_handle, GWLP_USERDATA, reinterpret_cast<ULONG_PTR>(this));
	if (set_ptr_result == NULL && GetLastError() != ERROR_SUCCESS) {
		if (!CloseWindow(_handle))
		{
			OutputDebugStringW(L"Failed to CloseWindow() on SetWindowLongPtrW error!");
		}
		throw std::exception("Failed to SetWindowLongPtrW() on CreateWindowExW()!");
	}
}

Window::~Window()
{
	try
	{
		if (!CloseWindow(_handle))
		{
			OutputDebugStringW(L"Failed to CloseWindow()!");
		}
	}
	catch (...) {
		OutputDebugStringW(L"Exception in ~Window()!");
	}
}

void Window::show()
{
	[[maybe_unused]] const BOOL was_visible = ShowWindow(_handle, SW_SHOW);
}

void Window::message_loop()
{
	static constexpr BOOL GET_MESSAGE_GOT_QUIT = false;
	static constexpr DWORD NO_FILTER = 0;
	MSG msg = { };
	while (GetMessageW(&msg, _handle, NO_FILTER, NO_FILTER) != GET_MESSAGE_GOT_QUIT)
	{
		static constexpr BOOL TRNASLATE_MESSAGE_W_FAILED = 0;
		if (TranslateMessage(&msg) == TRNASLATE_MESSAGE_W_FAILED) {
			OutputDebugStringW(L"[TRACE] TranslteMessage failed");
		}

		DispatchMessageW(&msg);
	}
}

std::shared_ptr<Timer> Window::set_timer(const std::chrono::milliseconds interval, std::function<void(Window&, const std::shared_ptr<Timer>&)> function)
{
	const auto timer = std::shared_ptr<Timer>(new Timer(_handle, interval, function, timers_callback));
	_timers.emplace(timer->id(), timer);
	return timer;
}

std::shared_ptr<Timer> Window::get_timer(const uint64_t timer_id) {
	const auto& val = _timers.find(timer_id);
	if (val == _timers.end()) {
		throw std::exception("Could not locate timer with specified id!");
	}
	return val->second;
}

bool Window::has_timer(const uint64_t timer_id) const
{
	return _timers.find(timer_id) != _timers.end();
}

DeviceContext Window::get_dc()
{
	return DeviceContext(_handle);
}

LRESULT Window::callback(const UINT uMsg, const WPARAM wParam, const LPARAM lParam)
{

	switch (uMsg)
	{

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
	{
		Paint p{ _handle };
		p.fill(Config::BACKGROUND_BRUSH);
	}
	return 0;
	}
	return DefWindowProcW(_handle, uMsg, wParam, lParam);
}

HWND Window::initialize_window(const HINSTANCE hInstance, const RECT& window_rect)
{
	initialize_window_class(hInstance);

	static constexpr DWORD WINDOW_STYLE = WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME;
	RECT final_window_rect = window_rect;
	if (!AdjustWindowRect(&final_window_rect, WINDOW_STYLE, false)) {
		throw std::exception("Failed to AdjustWindowRect()!");
	}

	const HWND handle = CreateWindowExW(
		0,
		WINDOW_CLASS_NAME.data(),
		WINDOW_NAME.data(),
		WINDOW_STYLE,
		CW_USEDEFAULT, CW_USEDEFAULT, final_window_rect.bottom, final_window_rect.right,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (handle == NULL) {
		throw std::exception("CreateWindowExW failed!");
	}

	return handle;
}

void Window::initialize_window_class(const HINSTANCE hInstance)
{
	static bool initialized = false;
	if (initialized) {
		return;
	}

	WNDCLASSW wc = { };

	wc.lpfnWndProc = window_callback_proc;
	wc.hInstance = hInstance;
	wc.lpszClassName = WINDOW_CLASS_NAME.data();

	RegisterClassW(&wc);

	initialized = true;

}

LRESULT Window::window_callback_proc(const HWND hwnd, const UINT uMsg, const WPARAM wParam, const LPARAM lParam)
{
	Window* const win_this = reinterpret_cast<Window*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
	if (win_this == nullptr)
	{
		return DefWindowProcW(hwnd, uMsg, wParam, lParam);
	}

	return win_this->callback(uMsg, wParam, lParam);
}

VOID Window::timers_callback(HWND hwnd, UINT a, UINT_PTR timer_id, DWORD c)
{
	Window* const window = reinterpret_cast<Window*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
	if (window->has_timer(timer_id))
	{
		const auto timer = window->get_timer(timer_id);
		timer->_callback(*window, timer);
	}
}
