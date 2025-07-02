#ifndef WIN32_WINDOW_POLYMORPH_H
#define WIN32_WINDOW_POLYMORPH_H

#include <window.h>
#include <Win32_window.h>

#include <windows.h>

class Win32_window_polymorph : public Window, private Win32_window
{
public:

    Win32_window_polymorph(const char* title, const unsigned int width, const unsigned int height)
        : Win32_window(title, "Win32_window_polymorph", width, height) {}
    ~Win32_window_polymorph() = default;

    inline void setWindowFlag(WindowFlag flag) override { Win32_window::setWindowFlag(flag); }
    inline void update() override { Win32_window::update(); }
    inline void render() override { Win32_window::render(); }
    inline void exec() override { Win32_window::exec(); }
    inline void close() override { Win32_window::close(); }
    inline bool isRunning() const override { return Win32_window::isRunning(); }
    inline void setDimentions(const WindowDimentions & dimentions) override { Win32_window::setDimentions(dimentions); }
    inline const WindowDimentions& getDimentions() const override { return Win32_window::getDimentions(); }

    inline bool registerMouse() override { return Win32_window::registerMouse(); }
    inline bool registerKeyboard() override { return Win32_window::registerKeyboard(); }

    inline void setFullscreen() override { Win32_window::setFullscreen(); }
    inline void setBorderless(const bool borderless) override { Win32_window::setBorderless(borderless); }
    inline void setResizable(bool resizable) override { Win32_window::setResizable(resizable); }

    inline void showCursor(bool show) override { Win32_window::showCursor(show); }
    inline void clipCursor(bool clip) override { Win32_window::clipCursor(clip); }

    inline VkSurfaceKHR createVulkanSurface(VkInstance instance) const override { return Win32_window::createVulkanSurface(instance); }
};

#endif //WIN32_WINDOW_POLYMORPH_H
