cmake_minimum_required(VERSION 3.16)

project(Window LANGUAGES CXX)

include(FetchContent)
set(FETCHCONTENT_BASE_DIR ${CMAKE_SOURCE_DIR}/lib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(INCLUDES_FILES
    include/Win32_window.h
    include/Win32_window_polymorph.h
    include/window.h
    include/autoWindow.h
    include/windowUtils.h
    include/inputs.h
)

set(SOURCES_FILES
    src/Win32_window.cpp
)

add_executable(Window main.cpp ${INCLUDES_FILES} ${SOURCES_FILES})
target_include_directories(Window PRIVATE include)

FetchContent_Declare(
    signals
    GIT_REPOSITORY https://github.com/Elnhomantia/signals
    GIT_TAG v1.1.2
)
FetchContent_MakeAvailable(signals)
target_link_libraries(Window PRIVATE signals)

FetchContent_Declare(
    QueuedThread
    GIT_REPOSITORY https://github.com/Elnhomantia/QueuedThread
    GIT_BRANCH master
)
FetchContent_MakeAvailable(QueuedThread)
target_link_libraries(Window PRIVATE QueuedThread)

find_package(Vulkan REQUIRED)
target_link_libraries(Window PRIVATE Vulkan::Vulkan)

include(GNUInstallDirs)
install(TARGETS Window
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
