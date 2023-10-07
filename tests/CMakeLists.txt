#
# URL:      https://github.com/panchaBhuta/rapidcsv
#
# Copyright (c) 2023-2023 Gautam Dhar
# All rights reserved.
#
# rapidcsv is distributed under the BSD 3-Clause license, see LICENSE for details.
#


# https://stackoverflow.com/questions/14446495/cmake-project-structure-with-unit-tests


# Test macro add_unit_test
macro(add_unit_test testname)
  if(CMAKE_BUILD_TYPE MATCHES "Debug")
  #if(BUILD_TESTING)
    FILE(GLOB sources ${testname}*.cpp)
    add_executable(${testname} ${sources})
    add_test(NAME ${testname} COMMAND "${PROJECT_BINARY_DIR}/tests/${testname}")
    target_link_libraries(${testname} PUBLIC rapidcsv)
    set_target_properties(${testname} PROPERTIES  LINKER_LANGUAGE CXX)
  endif()
endmacro(add_unit_test)

# Test macro add_perf_test
macro(add_perf_test testname)
  if(CMAKE_BUILD_TYPE MATCHES "Release")
    FILE(GLOB sources ${testname}*.cpp)
    add_executable(${testname} ${sources})
    add_test(NAME ${testname} COMMAND "${PROJECT_BINARY_DIR}/tests/${testname}")
    target_link_libraries(${testname} PUBLIC rapidcsv)
  endif()
endmacro(add_perf_test)




#[===========================================[
# Enable codecvt tests if supported
if (CMAKE_CXX_STANDARD VERSION_LESS "17")
  include(CheckIncludeFileCXX)
  check_include_file_cxx(codecvt HAS_CODECVT)
  if(HAS_CODECVT)
    target_compile_definitions(rapidcsv INTERFACE HAS_CODECVT)
  endif()
else()
  message(STATUS "Skip check for C++ include codecvt - C++${CMAKE_CXX_STANDARD}")
endif()
#]===========================================]


# Unit tests
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
add_unit_test(test040)
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
#[==================================[
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
add_unit_test(test065)
add_unit_test(test066)
add_unit_test(test067)
add_unit_test(test068)
add_unit_test(test069)
add_unit_test(test070)
add_unit_test(test071)
add_unit_test(test072)
add_unit_test(test073)
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
add_unit_test(test093)
add_unit_test(test094)
add_unit_test(test095)

add_unit_test(testView001)
add_unit_test(testView003)
add_unit_test(testView005)
add_unit_test(testView012)
add_unit_test(testView014)
add_unit_test(testView016)
add_unit_test(testView023)
add_unit_test(testView025)
add_unit_test(testView027)
add_unit_test(testView040)
add_unit_test(testView042)
add_unit_test(testView070)
add_unit_test(testView071)

add_unit_test(testViewB001)
add_unit_test(testViewYmd001)
add_unit_test(testViewYmd003)
add_unit_test(testViewYmd005)




# perf tests
add_perf_test(ptest001)
add_perf_test(ptest002)
#]==================================]

