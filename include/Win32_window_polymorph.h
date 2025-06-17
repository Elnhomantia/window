#ifndef WIN32_WINDOW_POLYMORPH_H
#define WIN32_WINDOW_POLYMORPH_H

#include <window.h>
#include <Win32_window.h>

#include <cstdint>
#include <windows.h>

class Win32_window_polymorph : public Window, private Win32_window
{
public:

    Win32_window_polymorph(const char* title, const unsigned int width, const unsigned int height)
        : Win32_window(title, "Win32_window_polymorph", width, height) {}
    ~Win32_window_polymorph() { Win32_window::~Win32_window(); }

    inline void setWindowFlag(WindowFlag flag) override { Win32_window::setWindowFlag(flag); }
    inline void update() override { Win32_window::update(); }
    inline void close() override { Win32_window::close(); }
    inline bool isRunning() const override { return Win32_window::isRunning(); }
    inline uint32_t* getPixelBuffer() override { return Win32_window::getPixelBuffer(); }
    inline void setDimentions(const WindowDimentions & dimentions) override { Win32_window::setDimentions(dimentions); }

};

#endif //WIN32_WINDOW_POLYMORPH_H
