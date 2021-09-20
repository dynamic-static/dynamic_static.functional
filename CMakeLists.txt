
# ==========================================
#   Copyright (c) 2021 dynamic_static
#       Licensed under the MIT license
#     http://opensource.org/licenses/MIT
# ==========================================

cmake_minimum_required(VERSION 3.17 FATAL_ERROR)
project(dynamic_static.functional)
set(CMAKE_CXX_STANDARD 17)

################################################################################
# Dependencies
set(external "${CMAKE_CURRENT_LIST_DIR}/external/")
string(REPLACE "\\" "/" DYNAMIC_STATIC "$ENV{DYNAMIC_STATIC}")
include("${external}/dynamic_static.build.cmake")

################################################################################
# dynamic_static.functional
set(includeDirectory "${CMAKE_CURRENT_LIST_DIR}/include/")
set(includePath "${includeDirectory}/dynamic_static/functional/")
set(sourcePath "${CMAKE_CURRENT_LIST_DIR}/source/dynamic_static/functional/")
dst_add_interface_library(
    target
        dynamic_static.functional
    includeDirectories
        "${includeDirectory}"
    includeFiles
        "${includePath}/action.hpp"
        "${includePath}/delegate.hpp"
        "${includePath}/event.hpp"
        "${includePath}/subscribable.hpp"
        "${includeDirectory}/dynamic_static/functional.hpp"
)

################################################################################
# dynamic_static.functional.test
include("${external}/dynamic_static.random.cmake")
dst_add_target_test(
    target
        dynamic_static.functional
    linkLibraries
        dynamic_static.random
    sourceFiles
        "${CMAKE_CURRENT_LIST_DIR}/tests/delegate.tests.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/tests/event.tests.cpp"
        "${CMAKE_CURRENT_LIST_DIR}/tests/subscribable.tests.cpp"
)
