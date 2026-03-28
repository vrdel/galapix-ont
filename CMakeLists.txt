cmake_minimum_required(VERSION 3.20)

project(galapix LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

option(BUILD_TESTS "Build tests" OFF)
option(BUILD_EXTRA_APPS "Build extra apps" OFF)

set(GALAPIX_REMOVED_TESTS
  curl_test
  kra_test
  pnm_test
  rar_test
  seven_zip_test
  xcf_test
  zip_test
)

include(GNUInstallDirs)
include(CTest)
find_package(Boost REQUIRED COMPONENTS filesystem thread)
find_package(GLEW REQUIRED)
find_package(JPEG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(PNG REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(SQLite3 REQUIRED)
find_package(Threads REQUIRED)

pkg_check_modules(LIBEXIF REQUIRED IMPORTED_TARGET libexif)
pkg_check_modules(MAGICK REQUIRED IMPORTED_TARGET Magick++)
pkg_check_modules(SDL REQUIRED IMPORTED_TARGET sdl)

file(GLOB UTIL_SOURCES CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/src/util/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/plugins/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/lisp/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/math/*.cpp"
)

file(GLOB CORE_SOURCES CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/src/database/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/display/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/galapix/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/job/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/jobs/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/sqlite/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/tools/*.cpp"
)

list(REMOVE_ITEM CORE_SOURCES
  "${CMAKE_CURRENT_SOURCE_DIR}/src/galapix/galapix.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/galapix/viewer.cpp"
)

add_library(galapix_util STATIC ${UTIL_SOURCES})
add_library(galapix_core STATIC ${CORE_SOURCES})

foreach(target galapix_util galapix_core)
  target_include_directories(${target} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/src"
  )

  target_link_libraries(${target} PUBLIC
    Boost::filesystem
    Boost::thread
    GLEW::GLEW
    JPEG::JPEG
    OpenGL::GL
    OpenGL::GLU
    PNG::PNG
    SQLite::SQLite3
    Threads::Threads
    PkgConfig::LIBEXIF
    PkgConfig::MAGICK
    PkgConfig::SDL
  )
endforeach()

target_link_libraries(galapix_core PUBLIC galapix_util)

add_executable(galapix_sdl
  src/galapix/galapix.cpp
  src/galapix/viewer.cpp
  src/sdl/sdl_framebuffer.cpp
  src/sdl/sdl_viewer.cpp
)

target_compile_definitions(galapix_sdl PRIVATE GALAPIX_SDL)
target_include_directories(galapix_sdl PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}/src"
)
target_link_libraries(galapix_sdl PRIVATE galapix_core)
set_target_properties(galapix_sdl PROPERTIES OUTPUT_NAME "galapix.sdl")

if(BUILD_TESTS)
  enable_testing()

  file(GLOB TEST_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/test/*_test.cpp"
  )

  foreach(test_source IN LISTS TEST_SOURCES)
    get_filename_component(test_name "${test_source}" NAME_WE)

    if(test_name IN_LIST GALAPIX_REMOVED_TESTS)
      continue()
    endif()

    add_executable("${test_name}" "${test_source}")
    target_include_directories("${test_name}" PRIVATE
      "${CMAKE_CURRENT_SOURCE_DIR}/src"
    )
    target_link_libraries("${test_name}" PRIVATE galapix_core)
    add_test(NAME "${test_name}" COMMAND "${test_name}")
  endforeach()
endif()

if(BUILD_EXTRA_APPS AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/extra")
  file(GLOB EXTRA_SOURCES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/extra/*.cpp"
  )

  foreach(extra_source IN LISTS EXTRA_SOURCES)
    get_filename_component(extra_name "${extra_source}" NAME_WE)
    add_executable("${extra_name}" "${extra_source}")
    target_include_directories("${extra_name}" PRIVATE
      "${CMAKE_CURRENT_SOURCE_DIR}/src"
    )
    target_link_libraries("${extra_name}" PRIVATE galapix_core)
  endforeach()
endif()

install(TARGETS galapix_sdl RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}")
