foreach(app serialization)
  add_executable(${app} ${app}.cpp)
  target_link_libraries(${app} PRIVATE qblock)
  add_test(NAME ${app} COMMAND ${app})

  if(WIN32 AND BUILD_SHARED_LIBS)
    add_custom_command(
      TARGET ${app}
      POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
              $<TARGET_RUNTIME_DLLS:${app}> $<TARGET_FILE_DIR:${app}>
      COMMAND_EXPAND_LISTS)
  endif(WIN32 AND BUILD_SHARED_LIBS)
endforeach()
