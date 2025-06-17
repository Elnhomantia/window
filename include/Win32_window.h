#ifndef WIN32_WINDOW_H
#define WIN32_WINDOW_H

#include <windowUtils.h>
#include <cstdint>
#include <windows.h>

class Win32_window
{
public:

    Win32_window(const char* title, const unsigned int width, const unsigned int height);
    ~Win32_window();

    void setWindowFlag(WindowFlag flag);
    void update();
    void close();
    bool isRunning() const;
    uint32_t* getPixelBuffer();
    void setDimentions(const WindowDimentions & dimentions);

protected:
    Win32_window(const char* title, const char* className, const unsigned int width, const unsigned int height);

    HWND hwnd = nullptr;
    HDC hdc = nullptr;
    HDC memDC = nullptr;
    HBITMAP bitmap = nullptr;

    uint32_t* pixelBuffer;
    bool _isRunning;

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT handleMessage(UINT msg, WPARAM wParam, LPARAM lParam);

    WindowDimentions dimentions;
};

#endif //WIN32_WINDOW_H
