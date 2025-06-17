cmake_minimum_required(VERSION 3.16)

project(Window LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(INCLUDES_FILES
    include/Win32_window.h
    include/Win32_window_polymorph.h
    include/window.h
    include/autoWindow.h
    include/windowUtils.h
)

set(SOURCES_FILES
    src/Win32_window.cpp
)

add_executable(Window main.cpp ${INCLUDES_FILES} ${SOURCES_FILES})

target_include_directories(Window PRIVATE include)

include(GNUInstallDirs)
install(TARGETS Window
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
