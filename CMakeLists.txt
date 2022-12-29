cmake_minimum_required(VERSION 3.11 FATAL_ERROR)
include(local_conf.cmake OPTIONAL)
project(QtIotaBlock VERSION 0.1 DESCRIPTION "QT   library  with block types on IOTA network " LANGUAGES CXX)


set(default_build_type "Release")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
	message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
	set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE
		STRING "Choose the type of build." FORCE)
	# Set the possible values of build type for cmake-gui
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
		"Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_CXX_EXTENSIONS OFF)


add_library(qblock qblock.cpp src/carray.cpp  src/qsignatures.cpp src/qpayloads.cpp src/qunlocks.cpp src/qessences.cpp src/qinputs.cpp src/qoutputs.cpp
    src/qnative_tokens.cpp src/qunlock_conditions.cpp src/qfeatures.cpp src/qaddresses.cpp src/qoutput_metadata_response.cpp)

find_package(Qt6 REQUIRED COMPONENTS Core)
target_link_libraries(qblock PUBLIC  Qt6::Core )

target_include_directories(qblock PUBLIC  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	"$<INSTALL_INTERFACE:include>")

install(TARGETS qblock EXPORT qblockTargets DESTINATION lib)
install(DIRECTORY include/ DESTINATION include/)

install(EXPORT qblockTargets
	FILE qblockTargets.cmake
	DESTINATION lib/cmake/qblock
	)
include(CMakePackageConfigHelpers)
# generate the config file that is includes the exports
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
	"${CMAKE_CURRENT_BINARY_DIR}/qblockConfig.cmake"
	INSTALL_DESTINATION "lib/cmake/qblock"
	NO_SET_AND_CHECK_MACRO
	NO_CHECK_REQUIRED_COMPONENTS_MACRO
	)
write_basic_package_version_file(
	"${CMAKE_CURRENT_BINARY_DIR}/qblockConfigVersion.cmake"
	VERSION "0.1.1.0"
	COMPATIBILITY AnyNewerVersion
	)
install(FILES
	${CMAKE_CURRENT_BINARY_DIR}/qblockConfig.cmake
	${CMAKE_CURRENT_BINARY_DIR}/qblockConfigVersion.cmake
	DESTINATION lib/cmake/qblock
	)
export(EXPORT qblockTargets
	FILE "${CMAKE_CURRENT_BINARY_DIR}/qblockTargets.cmake"
	)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
endif()
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
    add_subdirectory(tests)
endif()
