#include <autoWindow.h>
#include <window.h>
#include <Win32_window_polymorph.h>

#include <iostream>


using namespace std;

int main()
{
    cout << "Hello World!" << endl;
    AutoWindow window("TEST", 500, 500);
    //Window* window2 = WindowFactory::getInstance("TEST2", 1920, 1080);
    window.setDimentions(WindowDimentions{100, 100, 1920, 1080, 2000, 2000});
    window.setWindowFlag(SHOW_NORMAL);
    //window2->setWindowFlag(SHOW_NORMAL);

    //window.clipCursor(true);

    cout << "window 1 : " << sizeof(window) << endl;
    //cout << "window 2 : " << sizeof(*(static_cast<Win32_window_polymorph*>(window2))) << endl;

    window.registerKeyboard();
    window.registerMouse();
    Connection c = window.connect_inputEvent([](InputEvent* event){
        if(event->type == InputEvent::InputType::KEY_TYPE)
        {
            KeyEvent * ke = static_cast<KeyEvent*>(event);
            std::cout << "Keystate : " << static_cast<int>(ke->keyEvent) <<"\n";
            std::cout << "Keycoe : " << static_cast<int>(ke->keyCode) <<"\n";
            std::cout << "Device Id : " << static_cast<int>(ke->deviceId) <<std::endl;
        }
        else if(event->type == InputEvent::InputType::MOUSE_TYPE)
        {
            MouseEvent * me = static_cast<MouseEvent*>(event);
            std::cout << "relX : "<< me->relX <<"\n";
            std::cout << "relY : "<< me->relY <<"\n";
            std::cout << "absX : "<< me->absX <<"\n";
            std::cout << "absY : "<< me->absY <<"\n";
            std::cout << "X1 : "<< static_cast<int>(me->X1) <<"\n";
            std::cout << "X2 : "<< static_cast<int>(me->X2) <<"\n";
            std::cout << "Left : "<< static_cast<int>(me->left) <<"\n";
            std::cout << "Middle : "<< static_cast<int>(me->middle) <<"\n";
            std::cout << "Right : "<< static_cast<int>(me->right) <<"\n";
            std::cout << "Scroll delta : "<< me->scrollDelta <<"\n";
            std::cout << "device Id : "<< me->deviceId <<std::endl;
        }
    });

    window.exec();
    //window2->exec();

    //delete window2;
    return 0;
}
