cmake_minimum_required(VERSION 3.24 FATAL_ERROR)
project(cmake-fetchcontent)
set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
  rapidcsv
  GIT_REPOSITORY "https://github.com/panchaBhuta/rapidcsv_FilterSort.git"
  GIT_TAG        "v2.01.fs-8.75"
)
FetchContent_MakeAvailable(rapidcsv)

add_executable(exprog src/exprog.cpp)
target_link_libraries(exprog PUBLIC rapidcsv)
install(TARGETS exprog DESTINATION bin)

add_custom_target(uninstall
  COMMAND "${CMAKE_COMMAND}" -E remove "${CMAKE_INSTALL_PREFIX}/bin/exprog"
)
