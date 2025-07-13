#ifndef SHARED_ABSTRACTION_H
#define SHARED_ABSTRACTION_H

#include <Elth/signals/signals.h>
#include <Elth/queuedthread/queuedthread.h>
#include <inputs.h>

/**
 * @brief Class used to share objects accross implementation for differents OS.
 * It only exist because both static and polymorphic implementations exist.
 * If needed, should probably be virtually inherited to guaranty only one instance exist.
 */
class SharedAbstraction
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

#endif //SHARED_ABSTRACTION_H