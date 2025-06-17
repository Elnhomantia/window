#ifndef WINDOW_H
#define WINDOW_H

#include <cstdint>
#include <windowUtils.h>


class Window
{
public:

    virtual ~Window() = default;

    virtual void setWindowFlag(WindowFlag flag) = 0;
    virtual void update() = 0;
    virtual void close() = 0;
    virtual bool isRunning() const = 0;
    virtual uint32_t* getPixelBuffer() = 0;
    virtual void setDimentions(const WindowDimentions & dimentions) = 0;
};

#endif //WINDOW_H
