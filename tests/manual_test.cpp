#include <autoWindow.h>
#include <window.h>
#include <Win32/Win32_window_polymorph.h>

#include <iostream>

int main()
{
    std::cout << "Start" << std::endl;
    auto logInputs = [](InputEvent* event){
        if(event->type == InputEvent::InputType::KEY_TYPE)
        {
            KeyEvent * ke = static_cast<KeyEvent*>(event);
            std::cout << "Keystate : " << static_cast<int>(ke->keyEvent) <<"\n";
            std::cout << "Keycode : " << static_cast<int>(ke->keyCode) <<"\n";
            std::cout << "Key : " << static_cast<char>(ke->keyCode) <<"\n";
            std::cout << "Device Id : " << static_cast<int>(ke->deviceId) <<std::endl;
        }
        else if(event->type == InputEvent::InputType::MOUSE_TYPE)
        {
            MouseEvent * me = static_cast<MouseEvent*>(event);
            std::cout << "relX : " << me->relX <<"\n";
            std::cout << "relY : " << me->relY <<"\n";
            std::cout << "absX : " << me->absX <<"\n";
            std::cout << "absY : " << me->absY <<"\n";
            std::cout << "X1 : " << static_cast<int>(me->X1) <<"\n";
            std::cout << "X2 : " << static_cast<int>(me->X2) <<"\n";
            std::cout << "Left : " << static_cast<int>(me->left) <<"\n";
            std::cout << "Middle : " << static_cast<int>(me->middle) <<"\n";
            std::cout << "Right : " << static_cast<int>(me->right) <<"\n";
            std::cout << "Scroll delta : " << me->scrollDelta <<"\n";
            std::cout << "device Id : " << me->deviceId << std::endl;
        }
    };

    std::thread t1 ([&logInputs](){
        AutoWindow window("Window 1", 500, 500);
        window.setDimentions(WindowDimentions{100, 100, 500, 500, 2000, 2000});
        window.show();
        std::cout << "window 1 : " << sizeof(window) << std::endl;
        //window.registerKeyboard();
        //window.registerMouse();
        //Connection c = window.connect_inputEvent(logInputs);
        window.exec();
    });

    std::thread t2 ([&logInputs](){
        AutoWindow window("Window 2", 500, 500);
        window.show();
        std::cout << "window 2 : " << sizeof(window) << std::endl;
        //window.registerKeyboard();
        //window.registerMouse();
        //Connection c = window.connect_inputEvent(logInputs);
        window.exec();
    });

    std::thread t3 ([&logInputs](){
        Window* window = WindowFactory::getInstance("Window 3", 500, 500);
        window->show();
        //std::cout << "window 3 : " << sizeof(*(static_cast<Win32_window_polymorph*>(window))) << std::endl;
        //window->registerKeyboard();
        //window->registerMouse();
        //Connection c = window->connect_inputEvent(logInputs);
        window->exec();
        delete window;
    });

    t1.join();
    t2.join();
    t3.join();

    return EXIT_SUCCESS;
}
