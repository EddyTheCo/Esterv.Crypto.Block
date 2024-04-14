
foreach(app  serialization)
	add_executable(${app} ${app}.cpp)
	target_link_libraries(${app} PRIVATE qblock)
	add_test(NAME ${app} COMMAND ${app})
endforeach()

