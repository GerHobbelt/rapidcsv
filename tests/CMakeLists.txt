#
# URL:      https://github.com/panchaBhuta/rapidcsv
#
# Copyright (c) 2023-2025 Gautam Dhar
# All rights reserved.
#
# rapidcsv is distributed under the BSD 3-Clause license, see LICENSE for details.
#


# https://stackoverflow.com/questions/14446495/cmake-project-structure-with-unit-tests


# Test macro add_unit_test
macro(add_unit_test testname)
  if(CMAKE_BUILD_TYPE MATCHES "Debug"     OR
     CMAKE_BUILD_TYPE MATCHES "MemCheck"  OR
     CMAKE_BUILD_TYPE MATCHES "Coverage"  OR
     CMAKE_BUILD_TYPE MATCHES "CpuProf"   OR
     CMAKE_BUILD_TYPE MATCHES "MemProf"   OR
     CMAKE_BUILD_TYPE MATCHES "CppCheck")
  #if(BUILD_TESTING)

    FILE(GLOB sourcesExe  ${testname}.cpp ${ARGN})
    # ARGN here contains list of additional cpp files(includes the extension) if any
    add_executable(${testname} ${sourcesExe})
    add_test(NAME ${testname} COMMAND "${PROJECT_BINARY_DIR}/tests/${testname}")

    if(CMAKE_BUILD_TYPE MATCHES "MemCheck"  OR
       CMAKE_BUILD_TYPE MATCHES "CpuProf"   OR
       CMAKE_BUILD_TYPE MATCHES "MemProf")
      # https://stackoverflow.com/questions/44102050/valgrind-leading-to-numeric-issues-with-long-doubles
      #
      target_compile_definitions(${testname} PRIVATE USE_VALGRIND)
    endif()
    #target_compile_definitions(${testname}
    #            gives error as COMPILE_LANG_AND_ID is empty
    #            $<$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang>:COMPILING_CXX_WITH_CLANG>
    #            $<$<COMPILE_LANG_AND_ID:CXX,Intel>:COMPILING_CXX_WITH_INTEL>)

    #target_compile_options(${testname} PRIVATE OPTION_CONVERTER_DEBUG_LOG=ON)    # for debugging, donot delete
    target_link_libraries(${testname} PRIVATE rapidcsv)  # donot specify PUBLIC here,
    # executable ${testname} has to have transitive association to all its dependencies(aka library chain)
    # refer   https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html#target-usage-requirements
    # refer   https://cmake.org/cmake/help/latest/command/target_link_libraries.html#libraries-for-a-target-and-or-its-dependents

    if(CMAKE_BUILD_TYPE MATCHES "Coverage")
      #refer   https://github.com/bilke/cmake-modules/blob/master/CodeCoverage.cmake
      # compile with coverage options
      target_compile_options(${testname} PRIVATE --coverage -O0)
      target_link_options(${testname} PRIVATE --coverage)

      # How to use gcovr for code coverage?
      # If you have multiple compilers installed or if you are using Clang,
      # you will likely need to tell gcovr which gcov executable to use.
      # By default, gcovr just uses the program named gcov .
      # This is fine for the default GCC compiler, e.g. gcc or g++ .
      # Otherwise, you must use the --gcov-executable to tell gcovr what to use.

      #  doesn't  work as expected, instead directly calling gcovr in make.sh
      # add coverage target
#      add_custom_target(coverage_${testname}
        # gather data
#        COMMAND ${LCOV_COMMAND} --directory . --capture --output-file coverage.info
        # generate report
#        COMMAND ${GENHTML_COMMAND} --demangle-cpp -o coverage coverage.info
#        WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

    endif()

    set_target_properties(${testname} PROPERTIES  LINKER_LANGUAGE CXX)
  endif()
endmacro(add_unit_test)

# Test macro add_unit_test
macro(add_unit_test_properties testname)
  if(CMAKE_BUILD_TYPE MATCHES "Debug"     OR
     CMAKE_BUILD_TYPE MATCHES "MemCheck"  OR
     CMAKE_BUILD_TYPE MATCHES "Coverage"  OR
     CMAKE_BUILD_TYPE MATCHES "CpuProf"   OR
     CMAKE_BUILD_TYPE MATCHES "MemProf"   OR
     CMAKE_BUILD_TYPE MATCHES "CppCheck")
  #if(BUILD_TESTING)

    FILE(GLOB sourcesExe  ${testname}.cpp ${ARGN})
    # ARGN here contains list of additional cpp files(includes the extension) if any
    add_executable(${testname} ${sourcesExe})
    add_test(NAME ${testname} COMMAND "${PROJECT_BINARY_DIR}/tests/${testname}" "${PROJECT_SOURCE_DIR}/tests/properties")

    if(CMAKE_BUILD_TYPE MATCHES "MemCheck"  OR
       CMAKE_BUILD_TYPE MATCHES "CpuProf"   OR
       CMAKE_BUILD_TYPE MATCHES "MemProf")
      # https://stackoverflow.com/questions/44102050/valgrind-leading-to-numeric-issues-with-long-doubles
      #
      target_compile_definitions(${testname} PRIVATE USE_VALGRIND)
    endif()
    #target_compile_definitions(${testname}
    #            gives error as COMPILE_LANG_AND_ID is empty
    #            $<$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang>:COMPILING_CXX_WITH_CLANG>
    #            $<$<COMPILE_LANG_AND_ID:CXX,Intel>:COMPILING_CXX_WITH_INTEL>)

    target_link_libraries(${testname} PRIVATE rapidcsv)  # donot specify PUBLIC here,
    # executable ${testname} has to have transitive association to all its dependencies(aka library chain)
    # refer   https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html#target-usage-requirements
    # refer   https://cmake.org/cmake/help/latest/command/target_link_libraries.html#libraries-for-a-target-and-or-its-dependents

    if(CMAKE_BUILD_TYPE MATCHES "Coverage")
      #refer   https://github.com/bilke/cmake-modules/blob/master/CodeCoverage.cmake
      # compile with coverage options
      target_compile_options(${testname} PRIVATE --coverage -O0)
      target_link_options(${testname} PRIVATE --coverage)

      # How to use gcovr for code coverage?
      # If you have multiple compilers installed or if you are using Clang,
      # you will likely need to tell gcovr which gcov executable to use.
      # By default, gcovr just uses the program named gcov .
      # This is fine for the default GCC compiler, e.g. gcc or g++ .
      # Otherwise, you must use the --gcov-executable to tell gcovr what to use.

      #  doesn't  work as expected, instead directly calling gcovr in make.sh
      # add coverage target
#      add_custom_target(coverage_${testname}
        # gather data
#        COMMAND ${LCOV_COMMAND} --directory . --capture --output-file coverage.info
        # generate report
#        COMMAND ${GENHTML_COMMAND} --demangle-cpp -o coverage coverage.info
#        WORKING_DIRECTORY ${CMAKE_BINARY_DIR})

    endif()

    set_target_properties(${testname} PROPERTIES  LINKER_LANGUAGE CXX)
  endif()
endmacro(add_unit_test_properties)

# Test macro add_perf_test
macro(add_perf_test testname)
  if(CMAKE_BUILD_TYPE MATCHES "Release")
    FILE(GLOB sourcesExe  ${testname}.cpp ${ARGN})
    # ARGN here contains list of additional cpp files(includes the extension) if any
    add_executable(${testname} ${sourcesExe})
    add_test(NAME ${testname} COMMAND "${PROJECT_BINARY_DIR}/tests/${testname}")

    #target_compile_definitions(${testname}
    #            $<$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang>:COMPILING_CXX_WITH_CLANG>
    #            $<$<COMPILE_LANG_AND_ID:CXX,Intel>:COMPILING_CXX_WITH_INTEL>)

    target_link_libraries(${testname} rapidcsv)   # donot specify PUBLIC here,
    # executable ${testname} has to have transitive association to all its dependencies(aka library chain)
    # refer   https://cmake.org/cmake/help/latest/manual/cmake-buildsystem.7.html#target-usage-requirements
    # refer   https://cmake.org/cmake/help/latest/command/target_link_libraries.html#libraries-for-a-target-and-or-its-dependents
  endif()
endmacro(add_perf_test)




# Unit tests
add_unit_test(testPlatform)

add_unit_test(test001)
add_unit_test(test002)
add_unit_test(test003)
add_unit_test(test004)
add_unit_test(test005)
add_unit_test(test006)
add_unit_test(test007)
add_unit_test(test008)
add_unit_test(test009)
add_unit_test(test010)
add_unit_test(test011)
add_unit_test(test012)
add_unit_test(test013)
add_unit_test(test014)
add_unit_test(test015)
add_unit_test(test016)
add_unit_test(test017)
add_unit_test(test018)
add_unit_test(test019)
add_unit_test(test020)
add_unit_test(test021)
add_unit_test(test022)
add_unit_test(test023)
add_unit_test(test024)
add_unit_test(test025)
add_unit_test(test026)
add_unit_test(test027)
add_unit_test(test028)
add_unit_test(test029)
add_unit_test(test030)
add_unit_test(test031)
add_unit_test(test032)
add_unit_test(test033)
add_unit_test(test034)
add_unit_test(test035)
add_unit_test(test036)
  #[========================================[
    if(CMAKE_BUILD_TYPE MATCHES "Debug")
      if(USE_CHRONO_LIB)
        # as FetchContent is included previously for (!USE_CHRONO_LIB)
        include( FetchContent )
      endif()
      set(BIGFLOAT bigfloat)
      FetchContent_Declare( ${BIGFLOAT}
          GIT_REPOSITORY https://github.com/Mariotti94/BigFloat.git
          GIT_TAG        4d2867c  # adjust tag/branch/commit as needed
        )
      FetchContent_MakeAvailable(${BIGFLOAT}) # as the BigFloat does not support CMake

      FILE(GLOB sources tests/test036*.cpp ${CMAKE_BINARY_DIR}/_deps/${BIGFLOAT}-src/BigFloat.*)
      add_executable(test036 ${sources})
      add_test(NAME test036 COMMAND "${PROJECT_BINARY_DIR}/test036")
      target_include_directories(test036 PRIVATE ${CMAKE_BINARY_DIR}/_deps/${BIGFLOAT}-src/ )
      target_link_libraries(test036 PUBLIC rapidcsv ${DATELIB})
    endif()
  #]========================================]
add_unit_test(test037)
add_unit_test(test038)
add_unit_test(test039)
add_unit_test(test040 test040b.cpp)
add_unit_test(test041)
add_unit_test(test042)
add_unit_test(test043)
add_unit_test(test044)
add_unit_test(test045)
add_unit_test(test046)
add_unit_test(test047)
add_unit_test(test048)
add_unit_test(test049)
add_unit_test(test050)
add_unit_test(test051)
add_unit_test(test052)
add_unit_test(test053)
add_unit_test(test054)
add_unit_test(test055)
add_unit_test(test056)
  if(HAS_CODECVT)
    add_unit_test(test057)
    add_unit_test(test058)
    add_unit_test(test059)
    add_unit_test(test060)
  endif()
add_unit_test(test061)
add_unit_test(test062)
add_unit_test(test063)
add_unit_test(test064)
#add_unit_test(test065)  // labels at Nth row and column are no longer supported, since v3.3.0
add_unit_test(test066)
add_unit_test(test067)
add_unit_test(test068)
add_unit_test(test069)
add_unit_test(test070)
add_unit_test(test071)
add_unit_test(test072)
#add_unit_test(test073)  // labels at Nth row and column are no longer supported, since v3.3.0
add_unit_test(test074)
add_unit_test(test075)
add_unit_test(test076)
add_unit_test(test077)
add_unit_test(test078)
add_unit_test(test079)
add_unit_test(test080)
add_unit_test(test081)
add_unit_test(test082)
add_unit_test(test083)
add_unit_test(test084)
  if(HAS_CODECVT)
    add_unit_test(test085)
  endif()
add_unit_test(test086)
add_unit_test(test087)
add_unit_test(test088)
add_unit_test(test089)
add_unit_test(test090)
add_unit_test(test091)
add_unit_test(test092)
#add_unit_test(test093)  // labels at Nth row and column are no longer supported, since v3.3.0
add_unit_test(test094)
add_unit_test(test095)
add_unit_test(test096)
add_unit_test(test097)
add_unit_test(test098)
add_unit_test(test099)
add_unit_test(test100)
if(HAS_CODECVT)
  add_unit_test(test101)
endif()


add_unit_test(testView001)
add_unit_test(testView003)
add_unit_test(testView005)
add_unit_test(testView012)
add_unit_test(testView014)
add_unit_test(testView016)
add_unit_test(testView023)
add_unit_test(testView025)
add_unit_test(testView027)
add_unit_test(testView040 testView040b.cpp)
add_unit_test(testView042)
add_unit_test(testView070)
add_unit_test(testView071)

add_unit_test(testViewB001)
add_unit_test(testViewfNaN001)
add_unit_test(testViewYmd001)
add_unit_test(testViewYmd003)
add_unit_test(testViewYmd005)


add_unit_test(PropertiesTest)
add_unit_test_properties(PropertiesParserTest)


# perf tests
add_perf_test(ptest001)
add_perf_test(ptest002)



#[===========================================[
add_unit_test(test036)
add_unit_test(test091)
add_unit_test(testViewYmd001)
add_unit_test(testViewYmd003)
add_unit_test(test099)
#]===========================================]


