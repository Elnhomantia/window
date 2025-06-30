#ifndef WINDOW_H
#define WINDOW_H

#include <cstdint>
#include <windowUtils.h>
#include <inputs.h>
#include <vulkan/vulkan.h>

template<typename T>
concept WindowConcept = requires(T WindowManager,
                                 const T& WindowManagerConst,
                                 WindowFlag flag,
                                 const WindowDimentions& dim,
                                 bool b,
                                 VkInstance instance) {
    {WindowManager.setWindowFlag(flag)} -> std::same_as<void>;
    {WindowManager.update()}            -> std::same_as<void>;
    {WindowManager.render()}            -> std::same_as<void>;
    {WindowManager.exec()}              -> std::same_as<void>;
    {WindowManager.close()}             -> std::same_as<void>;
    {WindowManager.isRunning()}         -> std::convertible_to<bool>;
    {WindowManager.getPixelBuffer()}    -> std::convertible_to<uint32_t*>;
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

class Window : virtual public inputEventAbstraction
{
public:

    virtual ~Window() = default;

    virtual void setWindowFlag(WindowFlag flag) = 0;
    virtual void update() = 0;
    virtual void render() = 0;
    virtual void exec() = 0;
    virtual void close() = 0;
    virtual bool isRunning() const = 0;
    virtual uint32_t* getPixelBuffer() = 0;
    virtual void setDimentions(const WindowDimentions & dimentions) = 0;
    virtual const WindowDimentions& getDimentions() const = 0;

    virtual bool registerMouse() = 0;
    virtual bool registerKeyboard() = 0;

    virtual void setFullscreen() = 0;
    virtual void setBorderless(const bool borderless) = 0;
    virtual void setResizable(bool resizable) = 0;

    virtual void showCursor(bool show) = 0;
    virtual void clipCursor(bool clip) = 0;

    virtual VkSurfaceKHR createVulkanSurface(VkInstance instance) const = 0;
};

static_assert(WindowConcept<Window>, "Window interface does not satisfy WindowConcept.");

#endif //WINDOW_H
