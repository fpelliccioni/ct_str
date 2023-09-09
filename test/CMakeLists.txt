# Copyright (c) 2023 Fernando Pelliccioni.
# Distributed under the Boost Software License, Version 1.0.
# See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

file(GLOB_RECURSE ALL_TEST_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
list(REMOVE_ITEM ALL_TEST_FILES main.cpp)

enable_testing()

foreach(test_file ${ALL_TEST_FILES})
    message(STATUS "Adding test ${test_file}")
    get_filename_component(test_name ${test_file} NAME_WE)
    set(executable_name "ctutil_tests_${test_name}")
    add_executable(${executable_name} ${test_file})
    target_link_libraries(${executable_name} PUBLIC ctutils)
    add_test(NAME ${test_name} COMMAND ${executable_name})
endforeach()
