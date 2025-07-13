#ifndef AUTOWINDOW_H
#define AUTOWINDOW_H

#include <window.h>

template<WindowConcept T>
using CheckedWindow = T;

#if defined(_WIN32)
    #include <Win32/Win32_window.h>
    using AutoWindow = CheckedWindow<Win32_window>;
#elif defined(__linux__)
    #error "Unsupported platform for now"
#else
    #error "Unsupported platform"
#endif

class WindowFactory
{
public:
    static Window* getInstance(const char* title, const unsigned int width, const unsigned int height)
    {
        #if defined(_WIN32)
            #include <Win32/Win32_window_polymorph.h>
            return new Win32_window_polymorph(title, width, height);
        #elif defined(__linux__)
            #error "Unsupported platform for now"
        #else
            #error "Unsupported platform"
        #endif
    }
};

#endif //AUTOWINDOW_H
