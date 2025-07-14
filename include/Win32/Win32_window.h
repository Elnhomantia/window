#ifndef WIN32_WINDOW_H
#define WIN32_WINDOW_H

#include <inputs.h>
#include <windowUtils.h>
#include <sharedAbstration.h>
#include <windows.h>
#include <vulkan/vulkan.h>

/**
 * @brief Static implementation of a window for Windows.
 * See @ref Window for documentation on public methods.
 */
class Win32_window : virtual public SharedAbstraction
{

public:
    /**
     * @brief Construct a new Win32_window object
     * 
     * @param title
     * @param width
     * @param height 
     */
    Win32_window(const char* title, const unsigned int width, const unsigned int height);
    ~Win32_window();

    /**
     * @copydoc Window::show
     */
    void show();
    /**
     * @copydoc Window::hide
     */
    void hide();
    /**
     * @copydoc Window::minimize
     */
    void minimize();
    /**
     * @copydoc Window::maximize
     */
    void maximize();
    /**
     * @copydoc Window::restore
     */
    void restore();
    /**
     * @copydoc Window::focus
     */
    void focus();

    /**
    * @copydoc Window::update
    */
    void update();
    /**
    * @copydoc Window::render
    */
    void render();
    /**
    * @copydoc Window::exec
    */
    void exec();
    /**
    * @copydoc Window::close
    */
    void close();
    /**
    * @copydoc Window::isRunning
    */
    bool isRunning() const;
    /**
    * @copydoc Window::setDimentions
    */
    void setDimentions(const WindowDimentions& dimentions);
    /**
    * @copydoc Window::getDimentions
    */
    const WindowDimentions& getDimentions() const;

    /**
    * @copydoc Window::setFullscreen
    */
    void setFullscreen();
    /**
    * @copydoc Window::setBorderless
    */
    void setBorderless(const bool borderless);
    /**
    * @copydoc Window::setResizable
    */
    void setResizable(const bool resizable);

    /**
    * @copydoc Window::registerMouse
    */
    inline bool registerMouse() { return registerInputDevice(0x01, 0x02); }
    /**
    * @copydoc Window::registerKeyboard
    */
    inline bool registerKeyboard() { return registerInputDevice(0x01, 0x06); }
    /* For whatever reason, the window SDK don't have some kernel lib I need for this
    inline bool registerGamepad() { return registerInputDevice(0x01, 0x05); }
    inline bool registerJoystick(){ return registerInputDevice(0x01, 0x04); }
    */

    /**
    * @copydoc Window::showCursor
    */
    void showCursor(bool show);
    /**
    * @copydoc Window::clipCursor
    */
    void clipCursor(bool clip);

    /**
    * @copydoc Window::createVulkanSurface
    */
    VkSurfaceKHR createVulkanSurface(VkInstance instance) const;

private:
    HWND hwnd = nullptr;
    HDC hdc = nullptr;

    bool _isRunning;

    bool _showCursor = true;
    bool _clipCursor = false;

    /**
     * @brief Win32 event loop.
     * 
     * @param hwnd 
     * @param msg 
     * @param wParam 
     * @param lParam 
     * @return LRESULT 
     */
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    /**
     * @brief Handle all windows events.
     * 
     * @param msg 
     * @param wParam 
     * @param lParam 
     * @return LRESULT 
     */
    LRESULT handleMessage(UINT msg, WPARAM wParam, LPARAM lParam);

    /**
     * @brief Handle all inputs (using raw inputs).
     * 
     * @param wParam
     * @param lParam
     * @return true Success.
     * @return false Failure.
     */
    bool handleInput(WPARAM wParam, LPARAM lParam);
    /**
     * @brief Handle all keyboards inputs.
     * 
     * @param raw 
     * @return true Success.
     * @return false Failure.
     */
    bool handleKeybord(RAWINPUT* raw);
    /**
     * @brief Handle all mouses inputs.
     * 
     * @param raw 
     * @return true Success.
     * @return false Failure.
     */
    bool handleMouse(RAWINPUT* raw);

    WindowDimentions dimentions;

    bool registerInputDevice(USHORT usUsagePage , USHORT usUsage);

};

#endif //WIN32_WINDOW_H
