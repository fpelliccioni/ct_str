# Copyright (c) 2023 Fernando Pelliccioni.
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

file(GLOB_RECURSE ALL_TEST_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
list(REMOVE_ITEM ALL_TEST_FILES build_tests/should_compile.cpp)
file(GLOB FAIL_TEST_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/build_tests/should_fail_*.cpp)
list(REMOVE_ITEM ALL_TEST_FILES ${FAIL_TEST_FILES})

enable_testing()

foreach(test_file ${ALL_TEST_FILES})
    message(STATUS "Adding test ${test_file}")
    get_filename_component(test_name ${test_file} NAME_WLE)
    message(STATUS "Adding test ${test_name}")
    set(executable_name "ctutil_tests_${test_name}")
    add_executable(${executable_name} ${test_file})
    target_link_libraries(${executable_name} PUBLIC ctutils)
    add_test(NAME ${test_name} COMMAND ${executable_name})
endforeach()

# add_executable(tmp ${CMAKE_CURRENT_SOURCE_DIR}/build_tests/should_compile.cpp)
# target_link_libraries(tmp PUBLIC ctutils)
# add_test(NAME tmp COMMAND tmp)


try_compile(COMPILE_SUCCEEDED
            ${CMAKE_BINARY_DIR}/tmp
            ${CMAKE_CURRENT_SOURCE_DIR}/build_tests/should_compile.cpp
            CMAKE_FLAGS
                "-DCMAKE_CXX_STANDARD=20"
                "-DINCLUDE_DIRECTORIES=${CMAKE_CURRENT_SOURCE_DIR}/../include"
)

if(COMPILE_SUCCEEDED)
    message(STATUS "build_tests/should_compile.cpp: Compilation succeeded as expected.")
else()
    message(FATAL_ERROR "build_tests/should_compile.cpp: Compilation failed but was expected to succeed!")
endif()

foreach(fail_test_file ${FAIL_TEST_FILES})
    message(STATUS "Checking ${fail_test_file}")

    try_compile(COMPILE_SUCCEEDED
                ${CMAKE_BINARY_DIR}/tmp
                "${CMAKE_CURRENT_SOURCE_DIR}/${fail_test_file}"
                CMAKE_FLAGS
                    "-DCMAKE_CXX_STANDARD=20"
                    "-DINCLUDE_DIRECTORIES=${CMAKE_CURRENT_SOURCE_DIR}/../include"
    )

    if(NOT COMPILE_SUCCEEDED)
        message(STATUS "${fail_test_file}: Compilation failed as expected.")
    else()
        message(FATAL_ERROR "${fail_test_file}: Compilation succeeded but was expected to fail!")
    endif()
endforeach()
