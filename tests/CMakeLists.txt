set(TEST_SOURCES

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

add_executable(manual_test manual_test.cpp)
target_link_libraries(manual_test PRIVATE Window Elth::signals Elth::QueuedThread Vulkan::Vulkan)
target_include_directories(manual_test PRIVATE ${CMAKE_SOURCE_DIR}/include)
add_custom_target(run_manual_test
    COMMAND manual_test
    DEPENDS manual_test
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)