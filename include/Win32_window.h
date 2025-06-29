#ifndef WIN32_WINDOW_H
#define WIN32_WINDOW_H

#include <inputs.h>
#include <windowUtils.h>
#include <cstdint>
#include <windows.h>
#include <signals/signal.h>
#include <vulkan/vulkan.h>

class Win32_window : virtual public inputEventAbstraction
{

public:

    Win32_window(const char* title, const unsigned int width, const unsigned int height);
    ~Win32_window();

    void setWindowFlag(WindowFlag flag);
    void update();
    void render();
    void exec();
    void close();
    bool isRunning() const;
    uint32_t* getPixelBuffer();
    void setDimentions(const WindowDimentions& dimentions);
    const WindowDimentions& getDimentions() const;

    void setFullscreen();
    void setBorderless(const bool borderless);
    void setResizable(const bool resizable);

    inline bool registerMouse() { return registerInputDevice(0x01, 0x02); }
    inline bool registerKeyboard() { return registerInputDevice(0x01, 0x06); }
    /* For whatever reason, the window SDK don't have some kernel lib I need for this
    inline bool registerGamepad() { return registerInputDevice(0x01, 0x05); }
    inline bool registerJoystick(){ return registerInputDevice(0x01, 0x04); }
    */

    void showCursor(bool show);
    void clipCursor(bool clip);

    /**
     * @brief createVulkanSurface
     * @param instance
     * @return
     * Needed extentions : VK_KHR_surface, VK_KHR_win32_surface
     */
    VkSurfaceKHR createVulkanSurface(VkInstance instance) const;

protected:
    Win32_window(const char* title, const char* className, const unsigned int width, const unsigned int height);

private:
    HWND hwnd = nullptr;
    HDC hdc = nullptr;
    HDC memDC = nullptr;
    HBITMAP bitmap = nullptr;

    uint32_t* pixelBuffer;
    bool _isRunning;

    bool _showCursor = true;
    bool _clipCursor = false;

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    LRESULT handleMessage(UINT msg, WPARAM wParam, LPARAM lParam);
    bool handleInput(UINT msg, WPARAM wParam, LPARAM lParam);
    bool handleKeybord(RAWINPUT* raw);
    bool handleMouse(RAWINPUT* raw);

    WindowDimentions dimentions;

    bool registerInputDevice(USHORT usUsagePage , USHORT usUsage);

};

#endif //WIN32_WINDOW_H
