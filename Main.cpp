#include <Windows.h>
#include <string>
#include <queue>
#include <random>
enum class Direction
{
	Up = 0,
	Down,
	Left,
	Right
};

static Direction currentDirection = Direction::Right; // Initial direction
static Direction pendingDirection = Direction::Right; // Next direction


static constexpr size_t BOARD_SIZE = 25;
static constexpr size_t BOARD_RESOLUTION = 500;

static constexpr COLORREF rgbRed = 0x000000FF;
static constexpr COLORREF rgbGreen = 0x0000FF00;
static constexpr COLORREF rgbWhite = 0x00FFFFFF;

struct Point
{
	int x;
	int y;
	void draw(const HDC hdc, const COLORREF color) const {
		const auto brush = CreateSolidBrush(color);
		fill(hdc, brush);
	}
	void clear(const HDC hdc) const {
		fill(hdc, (HBRUSH)(COLOR_ACTIVECAPTION + 1));
	}
	bool operator==(const Point& other) const {
		return other.x == x && other.y == y;
	}

private:
	void fill(const HDC hdc, const HBRUSH brush) const {
		static constexpr size_t ratio = (BOARD_RESOLUTION / BOARD_SIZE);
		RECT r{
			static_cast<DWORD>(x * ratio),
			static_cast<DWORD>(y * ratio),
			static_cast<DWORD>((x + 1) * ratio),
			static_cast<DWORD>((y + 1) * ratio)
		};
		FillRect(hdc, &r, brush);
	}
};
static std::queue<Point> snakeBody; // Queue to hold the snake's body segments
static Point food; // Position of the food

void nextFood() {
	std::random_device rd;  // a seed source for the random number engine
	std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<> distrib(0, BOARD_SIZE - 1);

	food = Point{
		distrib(gen),
		distrib(gen)
	};
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		// All painting occurs here, between BeginPaint and EndPaint.

		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_ACTIVECAPTION + 1));

		EndPaint(hwnd, &ps);
	}
	return 0;
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_UP:
			pendingDirection = Direction::Up;
			break;
		case VK_DOWN:
			pendingDirection = Direction::Down;
			break;
		case VK_LEFT:
			pendingDirection = Direction::Left;
			break;
		case VK_RIGHT:
			pendingDirection = Direction::Right;
			break;
		}
	}
	return 0;

	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void SnakeTimerproc(
	HWND unnamedParam1,
	UINT unnamedParam2,
	UINT_PTR unnamedParam3,
	DWORD unnamedParam4
) {
	OutputDebugStringW(L"Timerproc!");


	if (!(pendingDirection == Direction::Up && currentDirection == Direction::Down ||
		pendingDirection == Direction::Down && currentDirection == Direction::Up ||
		pendingDirection == Direction::Left && currentDirection == Direction::Right ||
		pendingDirection == Direction::Right && currentDirection == Direction::Left)) {
		// Ignore the key press if it is the opposite direction
		currentDirection = pendingDirection;
		OutputDebugStringW(L"Direction changed, now it is: ");
		std::wstring directionString = currentDirection == Direction::Up ? L"Up\n" :
			currentDirection == Direction::Down ? L"Down\n" :
			currentDirection == Direction::Left ? L"Left\n" :
			L"Right\n";
		OutputDebugStringW(directionString.c_str());
	}




	const Point snake_head = snakeBody.back();

	Point next = snake_head;

	if (snake_head.x == 0 && currentDirection == Direction::Left) {
		next.x = BOARD_SIZE - 1;
	}
	else if (snake_head.y == 0 && currentDirection == Direction::Up) {
		next.y = BOARD_SIZE - 1;
	}
	else if (snake_head.x == BOARD_SIZE - 1 && currentDirection == Direction::Right) {
		next.x = 0;
	}
	else if (snake_head.y == BOARD_SIZE-1 && currentDirection == Direction::Down) {
		next.y = 0;
	}
	else {

		switch (currentDirection) {
		case Direction::Down:
			next.y += 1;
			break;
		case Direction::Up:
			next.y -= 1;
			break;
		case Direction::Left:
			next.x -= 1;
			break;
		case Direction::Right:
			next.x += 1;
			break;
		}
	}

	const HDC hdc = GetDC(unnamedParam1);

	std::queue<Point> snakeBodyCopy = snakeBody;
	for (; !snakeBodyCopy.empty(); snakeBodyCopy.pop()) {
		const Point &p = snakeBodyCopy.front();
		if (p == next) {
			KillTimer(unnamedParam1, unnamedParam3);
		}
	}

	if (next == food) {
		food.clear(hdc);
		nextFood();
	}
	else {
		const Point toPop = snakeBody.front();
		snakeBody.pop();
		toPop.clear(hdc);
	}
	food.draw(hdc, rgbRed);

	snakeBody.push(next);
	next.draw(hdc, rgbGreen);

	ReleaseDC(unnamedParam1, hdc);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
	// Register the window class.
	const wchar_t CLASS_NAME[] = L"Sample Window Class";

	WNDCLASS wc = { };

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;

	RegisterClassW(&wc);

	static constexpr DWORD WINDOW_STYLE = WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME;

	RECT windowRect = {
		0, 0, BOARD_RESOLUTION, BOARD_RESOLUTION,
	};
	AdjustWindowRect(&windowRect, WINDOW_STYLE, false);
	HWND hwnd = CreateWindowExW(
		0,                              // Optional window styles.
		CLASS_NAME,                     // Window class
		L"Snake",    // Window text
		WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME,            // Window style

		// Size and position
		CW_USEDEFAULT, CW_USEDEFAULT, windowRect.bottom, windowRect.right,
		NULL,       // Parent window    
		NULL,       // Menu
		hInstance,  // Instance handle
		NULL        // Additional application data
	);

	if (hwnd == NULL)
	{
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);

	snakeBody.push(Point{
		BOARD_SIZE / 2,
		BOARD_SIZE / 2
		});
	nextFood();

	const ULONG_PTR timerId = SetTimer(hwnd, 0, 500, SnakeTimerproc);

	// Run the message loop.

	MSG msg = { };
	while (GetMessageW(&msg, hwnd, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}