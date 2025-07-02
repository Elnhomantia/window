#ifndef INPUTS_H
#define INPUTS_H

#include <signal.h>
#include <queuedthread.h>

struct InputEvent
{
    using DeviceTypeId = unsigned int;
    enum class InputType
    {
        KEY_TYPE,
        MOUSE_TYPE,
        GAMEPAD_TYPE,
    };
    enum class KeyState
    {
        KEY_NONE,
        KEY_DOWN,
        KEY_UP,
        KEY_DOUBLE,
    };

    InputType type;
    DeviceTypeId deviceId;

    InputEvent() = default;
    InputEvent(const InputEvent & copy) = default;
    InputEvent(const InputType type, const DeviceTypeId deviceId) : type(type), deviceId(deviceId) {}

};

struct KeyEvent : InputEvent
{
    int keyCode;
    KeyState keyEvent;

    KeyEvent(const DeviceTypeId deviceId, const int keyCode, const KeyState keyEvent)
        : InputEvent(InputEvent::InputType::KEY_TYPE, deviceId), keyCode(keyCode), keyEvent(keyEvent) {}
};

struct MouseEvent : InputEvent
{
    int absX, absY;
    int relX, relY;
    int scrollDelta;
    KeyState left, right, middle;
    KeyState X1, X2;

    MouseEvent(const DeviceTypeId deviceId,
               const int absX, const int absY, const int relX, const int relY, const int scrDelt,
               const KeyState left, const KeyState right, const KeyState middle,
               const KeyState X1, const KeyState X2)
        : InputEvent(InputEvent::InputType::MOUSE_TYPE, deviceId),
        absX(absX), absY(absY), relX(relX), relY(relY), scrollDelta(scrDelt),
        left(left), right(right), middle(middle), X1(X1), X2(X2) {}
};

struct GamepadEvent : InputEvent
{
    std::vector<int> axes;       // valeurs des axes
    std::vector<bool> buttons;   // état des boutons (true=pressé, false=relâché)

    GamepadEvent(const DeviceTypeId deviceId,
                 std::vector<int> axes,
                 std::vector<bool> buttons)
        : InputEvent(InputEvent::InputType::GAMEPAD_TYPE, deviceId),
        axes(std::move(axes)),
        buttons(std::move(buttons))
    {}
};

class EventAbstraction
{
public:
    SIGNAL_CONNECT_FORWARD(inputEvent, InputEvent*)
    SIGNAL_CONNECT_FORWARD(resizeEvent, int, int)
    SIGNAL_CONNECT_FORWARD(closeEvent)

protected:
    /**
     * @brief This event is emited when when an input is registered.
     * (Asynchronous event).
     */
    Signal<InputEvent*> inputEvent;
    /**
     * @brief inputEventThread
     *
     */
    QueuedThread inputEventThread;

    /**
     * @brief This event is emited when the window is resized.
     * (Synchronous event).
     */
    Signal<int, int> resizeEvent;

    /**
     * @brief This event is emited when the window is closed.
     * (Synchronous event).
     */
    Signal<> closeEvent;
};

#endif
