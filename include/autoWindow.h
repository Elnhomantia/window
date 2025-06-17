#ifndef AUTOWINDOW_H
#define AUTOWINDOW_H

#if defined(_WIN32)
    #include <Win32_window.h>
    using AutoWindow = Win32_window;
#elif defined(__linux__)
    #error "Unsupported platform for now"
#else
    #error "Unsupported platform"
#endif

#endif //AUTOWINDOW_H
