cmake_minimum_required(VERSION 3.11 FATAL_ERROR)

project(testing VERSION 0.1 DESCRIPTION "diferent tests for the library" LANGUAGES CXX)

foreach(app  tip24 serialization)
add_executable(${app} ${app}.cpp)
target_link_libraries(${app} PRIVATE qblock)
add_test(NAME ${app} COMMAND ${app})

if (WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(
        TARGET ${app} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_RUNTIME_DLLS:${app}> $<TARGET_FILE_DIR:${app}>
        COMMAND_EXPAND_LISTS
    )
endif ()
endforeach()

