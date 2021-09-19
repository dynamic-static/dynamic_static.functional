
# ==========================================
#   Copyright (c) 2021 dynamic_static
#       Licensed under the MIT license
#     http://opensource.org/licenses/MIT
# ==========================================

cmake_minimum_required(VERSION 3.17 FATAL_ERROR)
project(dynamic_static.event)

################################################################################
# Dependencies
set(external "${CMAKE_CURRENT_LIST_DIR}/external/")
string(REPLACE "\\" "/" DYNAMIC_STATIC "$ENV{DYNAMIC_STATIC}")
include("${external}/dynamic_static.build.cmake")

################################################################################
# dynamic_static.event
dst_add_interface_library(
    target dynamic_static.event
    includeDirectories PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include/"
    includeFiles
        "${CMAKE_CURRENT_LIST_DIR}/include/dynamic_static/action.hpp"
        "${CMAKE_CURRENT_LIST_DIR}/include/dynamic_static/delegate.hpp"
        "${CMAKE_CURRENT_LIST_DIR}/include/dynamic_static/event.hpp"
        "${CMAKE_CURRENT_LIST_DIR}/include/dynamic_static/subscribable.hpp"
)

################################################################################
# dynamic_static.event.test
dst_add_target_test(
    target dynamic_static.event
    sourceFiles
        "${CMAKE_CURRENT_LIST_DIR}/tests/action.tests.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/tests/delegate.tests.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/tests/event.tests.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/tests/subscribable.tests.cpp"
)
