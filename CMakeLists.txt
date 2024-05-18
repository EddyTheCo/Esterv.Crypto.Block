cmake_minimum_required(VERSION 3.24)
include(${CMAKE_CURRENT_BINARY_DIR}/local_conf.cmake OPTIONAL)

include(FetchContent)
FetchContent_Declare(
  ccommon
  GIT_REPOSITORY https://github.com/EddyTheCo/Common.git
  GIT_TAG v0.4.0)
FetchContent_MakeAvailable(ccommon)
version_from_git(LOG OFF TIMESTAMP "%Y%m%d%H%M%S")

project(
  QtIotaBlock
  VERSION ${VERSION}
  DESCRIPTION "Library  with block types on IOTA"
  LANGUAGES CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
include(GNUInstallDirs)

FetchContent_Declare(
  QtbigInt
  GIT_REPOSITORY https://github.com/EddyTheCo/QbigInt.git
  GIT_TAG v0.1.1
  FIND_PACKAGE_ARGS 0.1 CONFIG)
FetchContent_MakeAvailable(QtbigInt)

find_package(Qt6 COMPONENTS Core)
if(Qt6_FOUND)
  qt_standard_project_setup()

  add_library(
    qblock
    src/qblock.cpp
    src/carray.cpp
    src/qsignatures.cpp
    src/qpayloads.cpp
    src/qunlocks.cpp
    src/qessences.cpp
    src/qinputs.cpp
    src/qoutputs.cpp
    src/qnative_tokens.cpp
    src/qunlock_conditions.cpp
    src/qfeatures.cpp
    src/qaddresses.cpp
    src/qoutput_metadata_response.cpp
    src/qtoken_scheme.cpp)

  add_library(${PROJECT_NAME}::qblock ALIAS qblock)
  set_target_properties(qblock PROPERTIES VERSION ${VERSION} SOVERSION
                                                             ${VERSION_MAJOR})
  target_link_libraries(qblock PUBLIC Qt6::Core)
  target_compile_definitions(qblock PRIVATE WINDOWS_QBLOCK)

  target_include_directories(
    qblock PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                  "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Esterv/>")

  target_link_libraries(qblock PUBLIC qbigint)

  install(
    TARGETS qblock
    EXPORT ${PROJECT_NAME}-config
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT block
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT block)

  install(
    DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Esterv
    COMPONENT ${PROJECT_NAME})

  build_exports()
  if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    build_cpack()
    if(BUILD_TESTING)
      include(CTest)
      add_subdirectory(tests)
    endif(BUILD_TESTING)
  endif(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
endif(Qt6_FOUND)
if(BUILD_DOCS)
  build_docs()
endif(BUILD_DOCS)
