cmake_minimum_required(VERSION 3.16)

project(Window VERSION 1.0.0 LANGUAGES CXX)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(INCLUDES_FILES
    include/window.h
    include/autoWindow.h
    include/windowUtils.h
    include/inputs.h
)

set(SOURCES_FILES)

if(WIN32)
    list(APPEND INCLUDES_FILES
        include/Win32/Win32_window.h
        include/Win32/Win32_window_polymorph.h
    )
    list(APPEND SOURCES_FILES
        src/Win32_window.cpp
    )
endif()
if(UNIX)

endif()

add_library(Window STATIC ${INCLUDES_FILES} ${SOURCES_FILES})
target_include_directories(Window PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Elth/window>
)

find_package(Vulkan REQUIRED)
find_package(signals REQUIRED CONFIG)
find_package(QueuedThread REQUIRED CONFIG)

target_link_libraries(Window PRIVATE Elth::signals Elth::QueuedThread Vulkan::Vulkan)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Elth/window)

install(TARGETS Window EXPORT WindowTargets)

install(EXPORT WindowTargets
    FILE WindowTargets.cmake
    NAMESPACE Elth::
    DESTINATION lib/cmake/Window
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/windowConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/windowConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/window
)
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/windowConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/windowConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/window
)

# ============ Tests ============

enable_testing()
add_subdirectory(tests)

add_custom_target(check ALL
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    DEPENDS Window tests_all
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
