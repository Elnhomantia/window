#ifndef WINDOWUTILS_H
#define WINDOWUTILS_H

struct WindowDimentions
{
    WindowDimentions() = default;
    WindowDimentions(unsigned int minX, unsigned int minY,
                     unsigned int sizeX, unsigned int sizeY,
                     unsigned int maxX, unsigned int maxY)
    {
        this->minX = minX;
        this->minY = minY;
        this->sizeX = sizeX;
        this->sizeY = sizeY;
        this->maxX = maxX;
        this->maxY = maxY;
    }
    unsigned int minX = 50, minY = 50;
    unsigned int sizeX = 100, sizeY = 100; //width, height
    unsigned int maxX = 200, maxY = 200;
};

enum WindowFlag
{
    HIDE,
    SHOW,
    SHOW_NORMAL,
    SHOW_MINIMIZED,
    SHOW_MAXIMIZED,
    MAXIMIZE,
    SHOW_NOACTIVATE,
    SHOW_NA,
    RESTORE,
    SHOW_DEFAULT
};

#endif //WINDOWUTILS_H
