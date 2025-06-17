#include <Win32_window.h>
#include <iostream>

Win32_window::Win32_window(const char* title,  const unsigned int width, const unsigned int height)
    : Win32_window(title, "Win32_window", width, height) {}


Win32_window::Win32_window(const char* title, const char* className, const unsigned int width, const unsigned int height)
{
    this->dimentions = {0, 0, width, height, width, height};
    WNDCLASS wc = {};

    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = GetModuleHandle(nullptr);
    wc.lpszClassName = className;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

    if(!RegisterClass(&wc))
    {
        DWORD err = GetLastError();
        std::cout << "RegisterClass failed : " << err <<std::endl;
        exit(EXIT_FAILURE);
    }

    RECT rect = {0, 0, (LONG)width, (LONG)height};
    AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);
    int totalWidth = rect.right - rect.left;
    int totalHeight = rect.bottom - rect.top;

    hwnd = CreateWindowEx(0, "Win32_window", title,
                          WS_OVERLAPPEDWINDOW | WS_CAPTION,
                          CW_USEDEFAULT, CW_USEDEFAULT, totalWidth, totalHeight,
                          nullptr, nullptr, wc.hInstance, this);

    if(nullptr == hwnd)
    {
        DWORD err = GetLastError();
        std::cout << "CreateWindowEx failed : " << err <<std::endl;
        exit(EXIT_FAILURE);
    }

    hdc = GetDC(hwnd);
    memDC = CreateCompatibleDC(hdc);

    BITMAPINFO bmi = {};

    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = static_cast<LONG>(width);
    bmi.bmiHeader.biHeight = -static_cast<LONG>(height);
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    bitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (void**)&pixelBuffer, nullptr, 0);
    SelectObject(memDC, bitmap);

    _isRunning = true;
}

Win32_window::~Win32_window()
{
    DeleteObject(bitmap);
    DeleteDC(memDC);
    ReleaseDC(hwnd, hdc);
    DestroyWindow(hwnd);
}

void Win32_window::setWindowFlag(WindowFlag flag)
{
    int winFlag;

    switch (flag)
    {
    case HIDE:              winFlag = SW_HIDE; break;
    case SHOW:              winFlag = SW_SHOW; break;
    case SHOW_NORMAL:       winFlag = SW_SHOWNORMAL; break;
    case SHOW_MINIMIZED:    winFlag = SW_SHOWMINIMIZED; break;
    case SHOW_MAXIMIZED:    winFlag = SW_SHOWMAXIMIZED; break;
    case MAXIMIZE:          winFlag = SW_MAXIMIZE; break;
    case SHOW_NOACTIVATE:   winFlag = SW_SHOWNOACTIVATE; break;
    case SHOW_NA:           winFlag = SW_SHOWNA; break;
    case RESTORE:           winFlag = SW_RESTORE; break;
    case SHOW_DEFAULT:      winFlag = SW_SHOWDEFAULT; break;
    }
    ShowWindow(hwnd, winFlag);
}

void Win32_window::update()
{
    MSG msg = {};
    while(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
    {
        if(msg.message == WM_QUIT)
        {
            _isRunning = false;
            break;
        }
        else
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    UpdateWindow(hwnd);
}

void Win32_window::close()
{
    _isRunning = false;
    PostQuitMessage(0);
}

bool Win32_window::isRunning() const
{
    return _isRunning;
}

uint32_t *Win32_window::getPixelBuffer()
{
    return pixelBuffer;
}

void Win32_window::setDimentions(const WindowDimentions &dimentions)
{
    this->dimentions = dimentions;
}

LRESULT Win32_window::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    Win32_window* pThis = nullptr;
    if(msg == WM_CREATE)
    {
        CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
        pThis = reinterpret_cast<Win32_window*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);
        return 0;
    }
    else
    {
        pThis = reinterpret_cast<Win32_window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

        if(nullptr != pThis)
        {
            return pThis->handleMessage(msg, wParam, lParam);
        }
        else
        {
            return DefWindowProc(hwnd, msg, wParam, lParam);
        }
    }
}

LRESULT Win32_window::handleMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_SIZE:
    {
        this->dimentions.sizeX = LOWORD(lParam);
        this->dimentions.sizeY = HIWORD(lParam);
        if(nullptr != bitmap)
        {
            DeleteObject(bitmap);
        }

        BITMAPINFO bmi = {};

        bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmi.bmiHeader.biWidth = static_cast<LONG>(this->dimentions.sizeX);
        bmi.bmiHeader.biHeight = -static_cast<LONG>(this->dimentions.sizeY);
        bmi.bmiHeader.biPlanes = 1;
        bmi.bmiHeader.biBitCount = 32;
        bmi.bmiHeader.biCompression = BI_RGB;

        bitmap = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS, (void**)&pixelBuffer, nullptr, 0);
        SelectObject(memDC, bitmap);

        InvalidateRect(hwnd, nullptr, TRUE);

        return 0;
    }

    case WM_GETMINMAXINFO:
    {
        MINMAXINFO* mmi = reinterpret_cast<MINMAXINFO*>(lParam);
        mmi->ptMinTrackSize.x = this->dimentions.minX;
        mmi->ptMinTrackSize.y = this->dimentions.minY;
        mmi->ptMaxTrackSize.x = this->dimentions.maxX;
        mmi->ptMaxTrackSize.y = this->dimentions.maxY;
    }

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdcPaint = BeginPaint(hwnd, &ps);
        BitBlt(hdcPaint, 0, 0, this->dimentions.sizeX, this->dimentions.sizeY, memDC, 0, 0, SRCCOPY);
        EndPaint(hwnd, &ps);
        return 0;
    }

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
}
