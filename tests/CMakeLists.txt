set(TEST_SOURCES
	main.cpp
)



foreach(test_file IN LISTS TEST_SOURCES)
    get_filename_component(file_name ${test_file} NAME_WE)
    add_executable(${file_name} ${test_file})
    target_include_directories(${file_name} PRIVATE ${CMAKE_SOURCE_DIR}/include)
    target_link_libraries(${file_name} PRIVATE Window signals QueuedThread Vulkan::Vulkan)
    add_test(NAME ${file_name} COMMAND ${file_name})
endforeach()
