cmake_minimum_required(VERSION 3.24 FATAL_ERROR)
project(cmake-addsubdirectory)
set(CMAKE_CXX_STANDARD 20)


add_subdirectory(rapidcsv_FilterSort)

add_executable(exprog2 src/exprog2.cpp)
target_link_libraries(exprog2 rapidcsv) # donot specify PUBLIC here,
    # executable 'exprog2' has to have transitive association to all its dependencies(aka library chain)
    # refer   https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html#target-usage-requirements
    # refer   https://cmake.org/cmake/help/latest/command/target_link_libraries.html#libraries-for-a-target-and-or-its-dependents
    # refer   https://cmake.org/cmake/help/latest/command/target_link_libraries.html#libraries-for-a-target-and-or-its-dependents
install(TARGETS exprog2 DESTINATION bin)

add_custom_target(uninstall
  COMMAND "${CMAKE_COMMAND}" -E remove "${CMAKE_INSTALL_PREFIX}/bin/exprog2"
)
