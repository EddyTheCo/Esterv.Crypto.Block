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
  EstervBlock
  VERSION ${VERSION}
  DESCRIPTION "Library with block types"
  LANGUAGES CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
include(GNUInstallDirs)

FetchContent_Declare(
  EstervBigInt 
  GIT_REPOSITORY https://github.com/EddyTheCo/BigInt.git
  GIT_TAG fixes
  FIND_PACKAGE_ARGS 0.1 CONFIG)
FetchContent_MakeAvailable(EstervBigInt)

find_package(Qt6 COMPONENTS Core)
if(Qt6_FOUND)
  qt_standard_project_setup()

  add_library(
    block
#    src/block.cpp
    src/carray.cpp
#    src/signatures.cpp
#    src/payloads.cpp
#    src/unlocks.cpp
#    src/essences.cpp
#    src/inputs.cpp
#    src/outputs.cpp
    src/unlock_conditions.cpp
    src/features.cpp
    src/addresses.cpp
#    src/output_metadata_response.cpp
#    src/token_scheme.cpp
    include/esterv/crypto/block/carray.hpp
    include/esterv/crypto/block/addresses.hpp
#    include/block/block.hpp
#    include/block/essences.hpp
    include/esterv/crypto/block/features.hpp
#    include/block/inputs.hpp
#    include/block/outputs.hpp
#    include/block/output_metadata_response.hpp
#    include/block/payloads.hpp
#    include/block/signatures.hpp
#    include/block/token_scheme.hpp
    include/esterv/crypto/block/unlock_conditions.hpp
#    include/block/unlocks.hpp
)

  add_library(${PROJECT_NAME}::block ALIAS block)
  set_target_properties(block PROPERTIES VERSION ${VERSION} SOVERSION
                                                             ${VERSION_MAJOR})
  target_link_libraries(block PUBLIC Qt6::Core)
  target_compile_definitions(block PRIVATE WINDOWS_QBLOCK)

  target_include_directories(
    block PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                  "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/Esterv/>")

  target_link_libraries(block PUBLIC bigint)

  install(
    TARGETS block
    EXPORT ${PROJECT_NAME}-config
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT ${PROJECT_NAME}
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT ${PROJECT_NAME})

  install(
    DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Esterv
    COMPONENT ${PROJECT_NAME}-dev)

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
