#ifndef WINDOWUTILS_H
#define WINDOWUTILS_H

/**
 * @brief Window dimention object. This include min, actual and max dimentions.
 */
struct WindowDimentions
{
    unsigned int minX = 50, minY = 50;
    unsigned int sizeX = 100, sizeY = 100; //width, height
    unsigned int maxX = 200, maxY = 200;
};

#endif //WINDOWUTILS_H
