cmake_minimum_required(VERSION 3.16)

project(Window LANGUAGES CXX)

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

find_package(Vulkan REQUIRED)

add_executable(Window main.cpp ${INCLUDES_FILES} ${SOURCES_FILES})

add_subdirectory(lib/signals)
target_link_libraries(Window PRIVATE signals)
target_link_libraries(Window PRIVATE Vulkan::Vulkan)

target_include_directories(Window PRIVATE include)
target_include_directories(Window PRIVATE lib)

include(GNUInstallDirs)
install(TARGETS Window
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
