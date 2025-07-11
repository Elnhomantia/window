set(TEST_SOURCES
	main.cpp
)

set(tests_executables)

foreach(test_file IN LISTS TEST_SOURCES)
    get_filename_component(file_name ${test_file} NAME_WE)
    add_executable(${file_name} ${test_file})
    target_include_directories(${file_name} PRIVATE ${CMAKE_SOURCE_DIR}/include)
    target_link_libraries(${file_name} PRIVATE Window Elth::signals Elth::QueuedThread Vulkan::Vulkan)
    add_test(NAME ${file_name} COMMAND ${file_name})
    list(APPEND tests_executables ${file_name})
endforeach()

add_custom_target(tests_all DEPENDS ${tests_executables})