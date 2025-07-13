#ifndef AUTOWINDOW_H
#define AUTOWINDOW_H

#include <window.h>

template<WindowConcept T>
using CheckedWindow = T;

/**
 * @typedef AutoWindow
 * @brief Static abstraction over an OS specific implementation that verify the @ref WindowConcept.
 * It chooses based on your compilation plateform OS.
 */

#if defined(_WIN32)
    #include <Win32/Win32_window.h>
    using AutoWindow = CheckedWindow<Win32_window>;
#endif
#if defined(__linux__)
    #error "Unsupported platform for now"
#endif

/**
 * @brief Factory to abstact creation of a polymorphic window.
 */
class WindowFactory
{
public:
    /**
     * @brief Get a polymorphic instance of a window.
     * 
     * @param title Title of the window
     * @param width Width of the window
     * @param height Height of the window
     * @return Window* A pointer to a new instance of an OS specific implementation of window.
     */
    static Window* getInstance(const char* title, const unsigned int width, const unsigned int height)
    {
        #if defined(_WIN32)
            #include <Win32/Win32_window_polymorph.h>
            return new Win32_window_polymorph(title, width, height);
        #endif
        #if defined(__linux__)
            #error "Unsupported platform for now"
        #endif
    }
};

#endif //AUTOWINDOW_H
