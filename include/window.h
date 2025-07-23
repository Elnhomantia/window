#ifndef WINDOW_H
#define WINDOW_H

#include <windowUtils.h>
#include <inputs.h>
#include <sharedAbstration.h>
#include <vulkan/vulkan.h>
#include <span>

/**
 * @brief Interface contract. Any implementation should verify this concept.
 * List all needed functions and verify inherance with @ref SharedAbstraction.
 * 
 * @tparam T Implementation class.
 */
template<typename T>
concept WindowConcept = 
    std::derived_from<T, SharedAbstraction> &&
    requires(T WindowManager, const T& WindowManagerConst,
     const WindowDimentions& dim,
     bool b, VkInstance instance) {
        {WindowManager.show()}              -> std::same_as<void>;
        {WindowManager.hide()}              -> std::same_as<void>;
        {WindowManager.minimize()}          -> std::same_as<void>;
        {WindowManager.maximize()}          -> std::same_as<void>;
        {WindowManager.restore()}           -> std::same_as<void>;
        {WindowManager.focus()}             -> std::same_as<void>;

        {WindowManager.update()}            -> std::same_as<void>;
        {WindowManager.render()}            -> std::same_as<void>;
        {WindowManager.exec()}              -> std::same_as<void>;
        {WindowManager.close()}             -> std::same_as<void>;
        {WindowManager.isRunning()}         -> std::convertible_to<bool>;
        {WindowManager.setDimentions(dim)}  -> std::same_as<void>;
        {WindowManager.getDimentions()}     -> std::convertible_to<const WindowDimentions&>;

        {WindowManager.setFullscreen()}     -> std::same_as<void>;
        {WindowManager.setBorderless(b)}    -> std::same_as<void>;
        {WindowManager.setResizable(b)}     -> std::same_as<void>;

        {WindowManager.registerMouse()}     -> std::convertible_to<bool>;
        {WindowManager.registerKeyboard()}  -> std::convertible_to<bool>;

        {WindowManager.showCursor(b)} -> std::same_as<void>;
        {WindowManager.clipCursor(b)} -> std::same_as<void>;

        {WindowManagerConst.createVulkanSurface(instance)} -> std::convertible_to<VkSurfaceKHR>;
};

/**
 * @brief Window polymorphic interface.
 * A window is not thread safe and should always be ran in the same thread it was created.
 * You should never need to downcast this.
 */
class Window : virtual public SharedAbstraction
{
public:

    virtual ~Window() = default;

    /**
     * @brief Show the window.
     */
    virtual void show() = 0;
    /**
     * @brief Hide the window.
     */
    virtual void hide() = 0;
    /**
     * @brief Minimize the window into task bar.
     */
    virtual void minimize() = 0;
    /**
     * @brief Maximize the window.
     */
    virtual void maximize() = 0;
    /**
     * @brief Restore the window from before minimize or maximize
     */
    virtual void restore() = 0;
    /**
     * @brief Set focus on window.
     */
    virtual void focus() = 0;

    /**
     * @brief Event loop handler. This does not update the content of the window.
     */
    virtual void update() = 0;
    /**
     * @brief Force pixel buffer update.
     */
    virtual void render() = 0;
    /**
     * @brief Basic event loop for a window. 
     * This will lock its thread whil the window is running.
     * Don't call in your main thread. The window should be created where you call @ref exec.
     */
    virtual void exec() = 0;
    /**
     * @brief Close the window definitively.
     */
    virtual void close() = 0;
    /**
     * @brief Get if the window is still running.
     * 
     * @return true The window is running.
     * @return false The window is not running.
     */
    virtual bool isRunning() const = 0;
    /**
     * @brief Set the Dimentions object.
     * 
     * @param dimentions The new Dimentions object.
     */
    virtual void setDimentions(const WindowDimentions & dimentions) = 0;
    /**
     * @brief Get the Dimentions object.
     * 
     * @return Dimentions object of the window.
     */
    virtual const WindowDimentions& getDimentions() const = 0;

    /**
     * @brief Mouses events are emited only if called.
     * See @ref MouseEvent @ref SharedAbstraction::resizeEvent for event structure and calls.
     * 
     * @return true Success
     * @return false Failure
     */
    virtual bool registerMouse() = 0;
    /**
     * @brief Keyboards events are emited only if called.
     * See @ref KeyEvent @ref SharedAbstraction::resizeEvent for event structure and calls.
     * 
     * @return true Success.
     * @return false Failure.
     */
    virtual bool registerKeyboard() = 0;

    /**
     * @brief Change window dimention to occupy all the current screen.
     */
    virtual void setFullscreen() = 0;
    /**
     * @brief Change window to borderless.
     * 
     * @param borderless true to borderless, false for "normal".
     */
    virtual void setBorderless(const bool borderless) = 0;
    /**
     * @brief Enable or disable user resize of the window.
     * Only affect users interractions. 
     * @bug You should probably set resizable to false too, else a white line stay as handle on Windows.
     * 
     * @param resizable true to be able to resize, false to lock.
     */
    virtual void setResizable(bool resizable) = 0;

    /**
     * @brief Hide or show cursor.
     * 
     * @param show true to show, false to hide.
     */
    virtual void showCursor(bool show) = 0;
    /**
     * @brief Clip or free cursor from the window. 
     * If clipped the cursor won't be able to go ouside the window when the focus is on the window.
     * 
     * @param clip true to clip, false to free.
     */
    virtual void clipCursor(bool clip) = 0;

    /**
     * @brief Create a Vulkan Surface object.
     * 
     * @param instance A vulkan instance.
     * @return A vulkan surface.
     */
    virtual VkSurfaceKHR createVulkanSurface(VkInstance instance) const = 0;

    virtual std::array<const char*, 2> getRequiredVulkanExtensions() const = 0;
};

static_assert(WindowConcept<Window>, "Window interface does not satisfy WindowConcept.");

#endif //WINDOW_H
