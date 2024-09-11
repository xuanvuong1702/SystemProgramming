add_executable(${EXECUTABLE_NAME} "main.c")

target_link_libraries(${EXECUTABLE_NAME} my_string)

add_subdirectory(my_string)
