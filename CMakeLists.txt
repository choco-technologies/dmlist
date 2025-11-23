# =====================================================================
#               DMOD Linked List Library
# =====================================================================
cmake_minimum_required(VERSION 3.10)

# ======================================================================
#               DMOD List
# ======================================================================
project(dmlist
    VERSION 1.0
    DESCRIPTION "DMOD Linked List Library"
    LANGUAGES C CXX)

# ======================================================================
#               Coverage Configuration
# ======================================================================
option(ENABLE_COVERAGE "Enable code coverage" OFF)

if(ENABLE_COVERAGE)
    if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
        message(STATUS "Code coverage enabled")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage -fprofile-arcs -ftest-coverage")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -fprofile-arcs -ftest-coverage")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} --coverage")
    else()
        message(WARNING "Code coverage is only supported with GCC or Clang")
        set(ENABLE_COVERAGE OFF)
    endif()
endif()

# ======================================================================
#               Fetch DMOD repository
# ======================================================================
include(FetchContent)

# Only fetch and build dmod if it's not already available as a target
if(NOT TARGET dmod_inc)
    # ======================================================================
    #               DMOD Configuration
    # ======================================================================
    set(DMOD_MODE "DMOD_SYSTEM" CACHE STRING "DMOD build mode" FORCE)
    set(DMOD_BUILD_TESTS OFF CACHE BOOL "Build tests" FORCE)
    set(DMOD_BUILD_EXAMPLES OFF CACHE BOOL "Build examples" FORCE)
    set(DMOD_BUILD_TOOLS OFF CACHE BOOL "Build tools" FORCE)
    set(DMOD_BUILD_TEMPLATES OFF CACHE BOOL "Build templates" FORCE)
    
    FetchContent_Declare(
        dmod
        GIT_REPOSITORY https://github.com/choco-technologies/dmod.git
        GIT_TAG        develop
    )
    
    # Pass coverage flags to DMOD if enabled - set before FetchContent_MakeAvailable
    if(ENABLE_COVERAGE)
        set(DMOD_ENABLE_COVERAGE ON CACHE BOOL "Enable DMOD coverage")
        # Ensure DMOD uses the same coverage flags
        set(CMAKE_C_FLAGS_INIT "${CMAKE_C_FLAGS}")
        set(CMAKE_CXX_FLAGS_INIT "${CMAKE_CXX_FLAGS}")
    endif()

    FetchContent_MakeAvailable(dmod)
    set(DMOD_DIR ${dmod_SOURCE_DIR} CACHE PATH "DMOD source directory" FORCE)
else()
    message(STATUS "dmod target already exists, skipping FetchContent")
endif()

include(${DMOD_DIR}/paths.cmake)

# ======================================================================
#               DMOD List Library
# ======================================================================
set(MODULE_NAME dmlist)
add_library(${MODULE_NAME} STATIC
    src/dmlist.c
)

target_compile_definitions(${MODULE_NAME} 
    PRIVATE 
        DMLIST_VERSION_STRING="== dmlist ver. ${PROJECT_VERSION} ==\\n"
)

target_include_directories(${MODULE_NAME} 
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)   

target_link_libraries(${MODULE_NAME} 
    PUBLIC    
        dmod_inc
    )

create_library_makefile(${MODULE_NAME})

# ======================================================================
#               Tests
# ======================================================================
option(DMLIST_BUILD_TESTS "Build tests" OFF)

if(DMLIST_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
