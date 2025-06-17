#include <autoWindow.h>
#include <window.h>
#include <Win32_window_polymorph.h>

#include <iostream>


using namespace std;

int main()
{
    cout << "Hello World!" << endl;
    AutoWindow window("TEST", 500, 500);
    Window* window2 = new Win32_window_polymorph("TEST2", 500, 500);
    window.setDimentions(WindowDimentions(100, 100, 200, 200, 600, 600));
    window.setWindowFlag(SHOW_NORMAL);
    window2->setWindowFlag(SHOW_NORMAL);

    while(window.isRunning() || window2->isRunning()){
        if(window.isRunning())
        {
            window.update();
        }
        if(window2->isRunning())
        {
            window2->update();
        }
    }

    delete window2;
    return 0;
}
