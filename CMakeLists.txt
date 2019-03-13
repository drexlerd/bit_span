cmake_minimum_required(VERSION 3.0.2 FATAL_ERROR)
project (bit_span)

# Check if this project is build standalone
#
# We do this in case we want to use this repo as a GIT submodule,
# because then we only need the source files themselves
if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    set(STANDALONE 1)
endif()

if(STANDALONE)
    # I am top-level project.
    set(CMAKE_MODULE_PATH
        ${CMAKE_MODULE_PATH}
        "${CMAKE_SOURCE_DIR}/submodules/build_system/cmakemodules")

    # init build system
    include(tdc_init)

    # downloadable dependencies
    include(depend_glog)

    # soft dependencies
    include(softdepend_gtest)

    # submodules
    include(git_submodule_subdirectories)
    git_submodule_subdirectory(submodules/build_system)
endif()

# Main target
add_library(bit_span INTERFACE)
target_link_libraries(bit_span INTERFACE ${GFLAGS_LIBRARIES} ${GLOG_LIBRARIES})
target_include_directories(bit_span INTERFACE include)

if(STANDALONE)
    # Unit tests
    add_subdirectory(test)

    # Disclaimer
    MESSAGE(STATUS "Built Type: " ${CMAKE_BUILD_TYPE} )

    # finish
    include(tdc_finish)
endif()

