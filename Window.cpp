#include "Window.hpp"
#include <string_view>
#include "Config.hpp"
#include <exception>
#include "Paint.hpp"
#include <string>

static constexpr std::wstring_view WINDOW_CLASS_NAME = L"SNAKE";
static constexpr std::wstring_view WINDOW_NAME = L"Snake";

static constexpr uint64_t INIT_PER_WINDOW_TIMER_ID = 1;

Window::Window(const HINSTANCE instance, const RECT& rect, const std::wstring& name) :
	_handle{ initialize_window(instance, rect, name) },
	_timers{},
	_timer_id_counter{ INIT_PER_WINDOW_TIMER_ID }
{
	const ULONG_PTR set_ptr_result = SetWindowLongPtrW(_handle, GWLP_USERDATA, reinterpret_cast<ULONG_PTR>(this));
	static constexpr ULONG_PTR POSSIBLE_SET_WINDOW_LONG_PTR_FAIL = NULL;
	if (set_ptr_result == POSSIBLE_SET_WINDOW_LONG_PTR_FAIL && GetLastError() != ERROR_SUCCESS)
	{
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
	catch (...)
	{
		OutputDebugStringW(L"Exception in ~Window()!");
	}
}

void Window::show()
{
	[[maybe_unused]] const BOOL was_visible = ShowWindow(_handle, SW_SHOW);
}

void Window::message_loop()
{
	static constexpr BOOL GET_MESSAGE_GOT_QUIT = 0;
	static constexpr DWORD NO_FILTER = 0;
	MSG msg{ };
	while (GetMessageW(&msg, _handle, NO_FILTER, NO_FILTER) > GET_MESSAGE_GOT_QUIT)
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
}

std::shared_ptr<Timer> Window::set_timer(const std::chrono::milliseconds interval, std::function<void(Window&, const std::shared_ptr<Timer>&)> callback)
{
	const uint64_t timer_id = _timer_id_counter++;
	const auto timer = std::shared_ptr<Timer>(new Timer{ _handle, timer_id, interval, callback });
	_timers.emplace(timer->id(), timer);
	return timer;
}

std::shared_ptr<Timer> Window::get_timer(const uint64_t timer_id)
{
	const auto& val = _timers.find(timer_id);
	if (val == _timers.end())
	{
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

LRESULT Window::callback(const UINT msg, const WPARAM w_param, const LPARAM l_param)
{

	switch (msg)
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
	case WM_TIMER:
	{
		const uint64_t timer_id = w_param;
		if (has_timer(timer_id))
		{
			const auto timer = get_timer(timer_id);
			timer->_callback(*this, timer);
			return 0;
		}
	}
	break;
	}
	return DefWindowProcW(_handle, msg, w_param, l_param);
}

HWND Window::initialize_window(const HINSTANCE hInstance, const RECT& window_rect, const std::wstring& name)
{
	register_window_class(hInstance);

	static constexpr DWORD WINDOW_STYLE = WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME;
	static constexpr HMENU NO_MENU = NULL;

	static constexpr BOOL HAS_MENU = NO_MENU != NULL;
	RECT adjusted_rect = window_rect;
	if (!AdjustWindowRect(&adjusted_rect, WINDOW_STYLE, HAS_MENU)) {
		throw std::exception("Failed to Adjustwindow_rect()!");
	}

	static constexpr LPVOID NO_CREATE_PARAM = NULL;
	static constexpr HWND NO_PARENT = NULL;
	const HWND handle = CreateWindowW(
		WINDOW_CLASS_NAME.data(),
		name.c_str(),
		WINDOW_STYLE,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		adjusted_rect.bottom,
		adjusted_rect.right,
		NO_PARENT,
		NO_MENU,
		hInstance,
		NO_CREATE_PARAM
	);

	if (handle == NULL) {
		throw std::exception("CreateWindowExW failed!");
	}

	return handle;
}

void Window::register_window_class(const HINSTANCE hInstance)
{
	static bool registered = false;
	if (registered)
	{
		return;
	}

	WNDCLASSW class_to_regsiter = { };

	class_to_regsiter.lpfnWndProc = window_callback_proc;
	class_to_regsiter.hInstance = hInstance;
	class_to_regsiter.lpszClassName = WINDOW_CLASS_NAME.data();

	static constexpr ATOM REGISTER_FAILED = 0;
	if (RegisterClassW(&class_to_regsiter) == REGISTER_FAILED)
	{
		throw std::exception("Failed to register window class!");
	}

	registered = true;
}

LRESULT Window::window_callback_proc(const HWND _window_handle, const UINT msg, const WPARAM w_param, const LPARAM l_param)
{
	Window* const win_this = reinterpret_cast<Window*>(GetWindowLongPtrW(_window_handle, GWLP_USERDATA));
	if (win_this == nullptr)
	{
		return DefWindowProcW(_window_handle, msg, w_param, l_param);
	}

	return win_this->callback(msg, w_param, l_param);
}
