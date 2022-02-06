
# ==========================================
#   Copyright (c) 2021 dynamic_static
#       Licensed under the MIT license
#     http://opensource.org/licenses/MIT
# ==========================================

include(FetchContent)

if(EXISTS "${DYNAMIC_STATIC}/dynamic_static.random/")
    FetchContent_Declare(dynamic_static.random SOURCE_DIR "${DYNAMIC_STATIC}/dynamic_static.random/")
else()
    FetchContent_Declare(
        dynamic_static.random
        GIT_REPOSITORY "https://github.com/dynamic-static/dynamic_static.random.git"
        GIT_TAG fa1c8d6058e4ae7a16eab815ede5f3e80741e172

        GIT_PROGRESS TRUE
        FETCHCONTENT_UPDATES_DISCONNECTED
    )
endif()
FetchContent_MakeAvailable(dynamic_static.random)
