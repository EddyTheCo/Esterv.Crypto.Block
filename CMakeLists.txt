cmake_minimum_required(VERSION 3.11 FATAL_ERROR)
include(local_conf.cmake OPTIONAL)

include(FetchContent)
FetchContent_Declare(
	ccommon
	GIT_REPOSITORY https://github.com/EddyTheCo/Common.git
	GIT_TAG main
	)
FetchContent_MakeAvailable(ccommon)
version_from_git(
	LOG       OFF
	TIMESTAMP "%Y%m%d%H%M%S"
	)


project(QtIotaBlock VERSION ${VERSION} DESCRIPTION "QT library  with block types on IOTA" LANGUAGES CXX)

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
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
include(GNUInstallDirs)

if (Qt6_FOUND)
	qt_standard_project_setup()
endif(Qt6_FOUND)

add_library(qblock qblock.cpp src/carray.cpp  src/qsignatures.cpp src/qpayloads.cpp src/qunlocks.cpp src/qessences.cpp src/qinputs.cpp src/qoutputs.cpp
	src/qnative_tokens.cpp src/qunlock_conditions.cpp src/qfeatures.cpp src/qaddresses.cpp src/qoutput_metadata_response.cpp src/qtoken_scheme.cpp)
target_compile_definitions(qblock PRIVATE WINDOWS_SHAREDLIB)
if (Qt6_FOUND)
	target_link_libraries(qblock PUBLIC  Qt6::Core )
endif(Qt6_FOUND)

FetchContent_Declare(
	qbigint
	GIT_REPOSITORY https://github.com/EddyTheCo/QbigInt.git
	GIT_TAG v0.0.1
	FIND_PACKAGE_ARGS 0 CONFIG
	)
FetchContent_MakeAvailable(qbigint)


target_include_directories(qblock PUBLIC  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	"$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

target_link_libraries(qblock PUBLIC qbigint)

install(TARGETS qblock EXPORT qblockTargets
	DESTINATION ${CMAKE_INSTALL_LIBDIR}
	)
install(DIRECTORY include/
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
	)

install(EXPORT qblockTargets
	FILE qblockTargets.cmake
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/qblock
	)

include(CMakePackageConfigHelpers)
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
	"${CMAKE_CURRENT_BINARY_DIR}/qblockConfig.cmake"
	INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/qblock
	NO_SET_AND_CHECK_MACRO
	NO_CHECK_REQUIRED_COMPONENTS_MACRO
	)
write_basic_package_version_file(
	"${CMAKE_CURRENT_BINARY_DIR}/qblockConfigVersion.cmake"
	VERSION ${VERSION}
	COMPATIBILITY SameMajorVersion
	)
install(FILES
	${CMAKE_CURRENT_BINARY_DIR}/qblockConfig.cmake
	${CMAKE_CURRENT_BINARY_DIR}/qblockConfigVersion.cmake
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/qblock
	)
export(EXPORT qblockTargets
	FILE "${CMAKE_CURRENT_BINARY_DIR}/qblockTargets.cmake"
	)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
	include(CTest)
	set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-v${SEMVER}-${CMAKE_SYSTEM_NAME}_${CMAKE_SYSTEM_VERSION}-${CMAKE_SYSTEM_PROCESSOR}-${CMAKE_CXX_COMPILER_ID}")
	if(NOT BUILD_SHARED_LIBS)
		set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_FILE_NAME}-static")
	endif(NOT BUILD_SHARED_LIBS)
	include(CPack)
endif()
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
	add_subdirectory(tests)
endif()
if(BUILD_DOCS)
	get_target_property(build_docs cmake_build_docs SOURCES)
	include(${build_docs})
endif()
