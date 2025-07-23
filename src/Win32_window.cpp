#include <Win32/Win32_window.h>
#include <iostream>
#include <array>
#include <hidsdi.h>
#include <hidusage.h>
#include <vulkan/vulkan_win32.h>

#define D_SHOW      (WM_APP + 1)
#define D_HIDE      (WM_APP + 2)
#define D_MINIMIZE  (WM_APP + 3)
#define D_MAXIMIZE  (WM_APP + 4)
#define D_RESTORE   (WM_APP + 5)
#define D_FOCUS     (WM_APP + 6)

Win32_window::Win32_window(const char* title,  const unsigned int width, const unsigned int height)
{
    this->dimentions = {0, 0, width, height, width, height};
    
    static std::once_flag registerClassFlag;
    std::call_once(registerClassFlag, [&]() {
        WNDCLASS wc = {};
        wc.lpfnWndProc   = WindowProc;
        wc.hInstance     = GetModuleHandle(nullptr);
        wc.lpszClassName = "Win32_window";
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wc.hbrBackground = nullptr;

        if(!RegisterClass(&wc))
        {
            DWORD err = GetLastError();
            std::cout << "RegisterClass failed : " << err <<std::endl;
            exit(EXIT_FAILURE);
        }
    });

    WNDCLASS wc;
    if (!(GetClassInfo(GetModuleHandle(nullptr), "Win32_window", &wc))) {
        std::cout << "Unregistered class : " << "Win32_window or child class" <<std::endl;
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

    _isRunning = true;
}

Win32_window::~Win32_window()
{
    if(_isRunning)
    {
        this->close();
    }
}

void Win32_window::show()
{
    PostMessage(hwnd, D_SHOW, 0, 0);
}
void Win32_window::hide()
{
    PostMessage(hwnd, D_HIDE, 0, 0);
}
void Win32_window::minimize()
{
    PostMessage(hwnd, D_MINIMIZE, 0, 0);
}
void Win32_window::maximize()
{
    PostMessage(hwnd, D_MAXIMIZE, 0, 0);
}
void Win32_window::restore()
{
    PostMessage(hwnd, D_RESTORE, 0, 0);
}
void Win32_window::focus()
{
    PostMessage(hwnd, D_FOCUS, 0, 0);
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
    if(_isRunning)
    {
        WaitMessage();
    }
}

void Win32_window::render()
{
    InvalidateRect(hwnd, nullptr, FALSE);
    UpdateWindow(hwnd);
}

void Win32_window::exec()
{
    while(_isRunning)
    {
        update();
    }
}

void Win32_window::close()
{
    SendMessage(hwnd, WM_CLOSE, 0, 0);
}

bool Win32_window::isRunning() const
{
    return _isRunning;
}

void Win32_window::setDimentions(const WindowDimentions &dimentions)
{
    this->dimentions = dimentions;
    RECT rect = {
        0, 0,
        static_cast<LONG>(dimentions.sizeX),
        static_cast<LONG>(dimentions.sizeY)
    };

    DWORD style = GetWindowLongPtr(hwnd, GWL_STYLE);
    DWORD exStyle = GetWindowLongPtr(hwnd, GWL_EXSTYLE);

    AdjustWindowRectEx(&rect, style, FALSE, exStyle);

    int totalWidth = rect.right - rect.left;
    int totalHeight = rect.bottom - rect.top;

    SetWindowPos(hwnd, nullptr,
                 0, 0, totalWidth, totalHeight,
                 SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
}

const WindowDimentions &Win32_window::getDimentions() const
{
    return dimentions;
}

void Win32_window::setFullscreen()
{
    this->setBorderless(true);

    MONITORINFO mi = { sizeof(mi) };
    GetMonitorInfo(MonitorFromWindow(hwnd, MONITOR_DEFAULTTOPRIMARY), &mi);
    SetWindowPos(hwnd, HWND_TOP,
                mi.rcMonitor.left, mi.rcMonitor.top,
                mi.rcMonitor.right - mi.rcMonitor.left,
                mi.rcMonitor.bottom - mi.rcMonitor.top,
                 SWP_NOOWNERZORDER | SWP_FRAMECHANGED);

    this->setDimentions(WindowDimentions{
        0, 0,
        static_cast<unsigned int>(mi.rcMonitor.right - mi.rcMonitor.left),
        static_cast<unsigned int>(mi.rcMonitor.bottom - mi.rcMonitor.top),
        static_cast<unsigned int>(mi.rcMonitor.right - mi.rcMonitor.left),
        static_cast<unsigned int>(mi.rcMonitor.bottom - mi.rcMonitor.top)
    });
}

void Win32_window::setBorderless(const bool borderless)
{
    LONG style = GetWindowLongPtr(hwnd, GWL_STYLE);
    if(borderless)
        style &= ~(WS_CAPTION | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU);
    else
        style |= WS_CAPTION | WS_THICKFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU;

    SetWindowLongPtr(hwnd, GWL_STYLE, style);
    SetWindowPos(hwnd, nullptr, 0, 0, 0, 0,
                 SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
}

void Win32_window::setResizable(const bool resizable)
{
    long style = GetWindowLongPtr(hwnd, GWL_STYLE);
    if(resizable)
        style |= WS_THICKFRAME | WS_MAXIMIZEBOX;
    else
        style &= ~(WS_THICKFRAME | WS_MAXIMIZEBOX);

    SetWindowLong(hwnd, GWL_STYLE, style);
    SetWindowPos(hwnd, nullptr, 0, 0, 0, 0,
                 SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
}

void Win32_window::showCursor(bool show)
{
    _showCursor = show;
    ShowCursor(show);
}

void Win32_window::clipCursor(bool clip)
{
    _clipCursor = clip;
    if(clip)
    {
        RECT rect;
        GetClientRect(hwnd, &rect);

        POINT ul = { rect.left, rect.top };
        POINT lr = { rect.right, rect.bottom };
        ClientToScreen(hwnd, &ul);
        ClientToScreen(hwnd, &lr);

        RECT clipRect = { ul.x, ul.y, lr.x, lr.y };
        ClipCursor(&clipRect);
    }
    else
    {
        ClipCursor(NULL);
    }
}

VkSurfaceKHR Win32_window::createVulkanSurface(VkInstance instance) const
{
    VkSurfaceKHR surface = VK_NULL_HANDLE;

    VkWin32SurfaceCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
    createInfo.hwnd = hwnd;
    createInfo.hinstance = GetModuleHandle(nullptr);

    VkResult result = vkCreateWin32SurfaceKHR(instance, &createInfo, nullptr, &surface);
    if(result != VK_SUCCESS)
    {
        throw std::runtime_error("Failed to create Win32 Vulkan surface.");
    }

    return surface;
}

std::array<const char*, 2> Win32_window::getRequiredVulkanExtensions() const {
    return {
        VK_KHR_SURFACE_EXTENSION_NAME,
        VK_KHR_WIN32_SURFACE_EXTENSION_NAME
    };
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
    case D_SHOW:
        ShowWindow(hwnd, SW_SHOWNORMAL);
        break;
    case D_HIDE:
        ShowWindow(hwnd, SW_HIDE);
        break;
    case D_MINIMIZE:
        ShowWindow(hwnd, SW_MINIMIZE);
        break;
    case D_MAXIMIZE:
        ShowWindow(hwnd, SW_MAXIMIZE);
        break;
    case D_RESTORE:
        ShowWindow(hwnd, SW_RESTORE);
        break;
    case D_FOCUS:
        SetForegroundWindow(hwnd);
        SetFocus(hwnd);
        break;

    case WM_CLOSE:
        _isRunning = false;
        closeEvent.emit();
        DestroyWindow(hwnd);
        return 0;
    case WM_DESTROY:
        ReleaseDC(hwnd, hdc);
        PostQuitMessage(0);
        return 0;

    case WM_ENTERSIZEMOVE:
        resizeStatus.emit(ResizeStep::START);
        break;
    case WM_EXITSIZEMOVE:
        resizeStatus.emit(ResizeStep::FINISH);
        break;
    case WM_SIZE:
    {
        switch (wParam) {
            case SIZE_MINIMIZED:
                showCursor(true);
                clipCursor(false);
                break;

            case SIZE_RESTORED:
            case SIZE_MAXIMIZED:
                this->dimentions.sizeX = LOWORD(lParam);
                this->dimentions.sizeY = HIWORD(lParam);
                clipCursor(_clipCursor);
                showCursor(_showCursor);
                InvalidateRect(hwnd, nullptr, FALSE);
                break;
        }
        return 0;
    }

    case WM_GETMINMAXINFO:
    {
        MINMAXINFO* mmi = reinterpret_cast<MINMAXINFO*>(lParam);
        mmi->ptMinTrackSize.x = this->dimentions.minX;
        mmi->ptMinTrackSize.y = this->dimentions.minY;
        mmi->ptMaxTrackSize.x = this->dimentions.maxX;
        mmi->ptMaxTrackSize.y = this->dimentions.maxY;
        return 0;
    }

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        BeginPaint(hwnd, &ps);
        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_INPUT:
    {
        handleInput(wParam, lParam);
        break;
    }

    case WM_KILLFOCUS:
    case WM_ACTIVATE:
        if (wParam == WA_INACTIVE) {
            ClipCursor(NULL);
            ShowCursor(TRUE);
        }
        break;

    case WM_SETFOCUS:
        showCursor(_showCursor);
        clipCursor(_clipCursor);

    default:
        break;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

bool Win32_window::handleInput(WPARAM wParam, LPARAM lParam)
{
    UINT dwSize = 0;
    GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT,
                    nullptr, &dwSize, sizeof(RAWINPUTHEADER));

    std::vector<BYTE> buffer(dwSize);
    if(GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT,
                        buffer.data(), &dwSize, sizeof(RAWINPUTHEADER)) != dwSize)
    {
        return false;
    }

    RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(buffer.data());

    switch (raw->header.dwType) {
    case RIM_TYPEKEYBOARD:
        return handleKeybord(raw);
    case RIM_TYPEMOUSE:
        return handleMouse(raw);

    default:
        return false;
    }
}

bool Win32_window::handleKeybord(RAWINPUT* raw)
{
    const RAWKEYBOARD& rk = raw->data.keyboard;
    InputEvent::KeyState state;

    switch (rk.Message) {
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
        state = InputEvent::KeyState::KEY_DOWN;
        break;

    case WM_KEYUP:
    case WM_SYSKEYUP:
        state = InputEvent::KeyState::KEY_UP;
        break;

    default:
        return false;
    }

    KeyEvent event(
        static_cast<InputEvent::DeviceTypeId>(reinterpret_cast<uintptr_t>(raw->header.hDevice)),
        rk.VKey,
        state
    );

    inputEventThread.schedule([this, event = std::move(event)]() mutable{
        inputEvent.emit(&event);
    });

    return true;
}

bool Win32_window::handleMouse(RAWINPUT* raw)
{
    const RAWMOUSE& rm = raw->data.mouse;
    const InputEvent::DeviceTypeId deviceId =
        static_cast<InputEvent::DeviceTypeId>(reinterpret_cast<uintptr_t>(raw->header.hDevice));

    int absX = 0, absY = 0;
    int relX = 0, relY = 0;
    int scrollDelta = 0;

    relX = rm.lLastX;
    relY = rm.lLastY;

    POINT p;
    if (GetCursorPos(&p)) {
        ScreenToClient(hwnd, &p);
        absX = p.x;
        absY = p.y;
    }


    if (rm.usButtonFlags & RI_MOUSE_WHEEL)
    {

        scrollDelta = static_cast<SHORT>(rm.usButtonData);
    }

    auto getKeyState = [&](USHORT downFlag, USHORT upFlag) -> InputEvent::KeyState {
        if (rm.usButtonFlags & downFlag)
            return InputEvent::KeyState::KEY_DOWN;
        else if (rm.usButtonFlags & upFlag)
            return InputEvent::KeyState::KEY_UP;
        else
            return InputEvent::KeyState::KEY_NONE;
    };

    const InputEvent::KeyState leftState = getKeyState(RI_MOUSE_LEFT_BUTTON_DOWN, RI_MOUSE_LEFT_BUTTON_UP);
    const InputEvent::KeyState rightState = getKeyState(RI_MOUSE_RIGHT_BUTTON_DOWN, RI_MOUSE_RIGHT_BUTTON_UP);
    const InputEvent::KeyState middleState = getKeyState(RI_MOUSE_MIDDLE_BUTTON_DOWN, RI_MOUSE_MIDDLE_BUTTON_UP);
    const InputEvent::KeyState x1State = getKeyState(RI_MOUSE_BUTTON_4_DOWN, RI_MOUSE_BUTTON_4_UP);
    const InputEvent::KeyState x2State = getKeyState(RI_MOUSE_BUTTON_5_DOWN, RI_MOUSE_BUTTON_5_UP);

    MouseEvent event(deviceId, absX, absY, relX, relY, scrollDelta,
               leftState, rightState, middleState, x1State, x2State);

    inputEventThread.schedule([this, event = std::move(event)]() mutable{
        inputEvent.emit(&event);
    });

    return true;
}


bool Win32_window::registerInputDevice(USHORT usUsagePage, USHORT usUsage)
{
    RAWINPUTDEVICE rid;
    rid.usUsagePage = usUsagePage;
    rid.usUsage = usUsage;
    rid.dwFlags = RIDEV_INPUTSINK;
    rid.hwndTarget = hwnd;

    return RegisterRawInputDevices(&rid, 1, sizeof(rid));
}

