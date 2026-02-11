cmake_minimum_required(VERSION 3.10)
project("pixeld++" VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Find Dependencies
find_package(Threads REQUIRED)

# 2. DEFINE THE TARGETS FIRST
add_executable("pixeld++" "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")
add_executable(app_test "${CMAKE_CURRENT_SOURCE_DIR}/test/app_test.cpp")

# 3. NOW CONFIGURE THE DEFINED TARGETS
# --- Coverage Config ---
if(CMAKE_BUILD_TYPE STREQUAL "Coverage")
    target_compile_options(app_test PRIVATE --coverage -O0)
    target_link_libraries(app_test PRIVATE --coverage)
endif()

# --- Static & Optimized Config ---
if(STATIC_RELEASE_BUILD)
    # 1. Apply -O3 optimization
    target_compile_options("pixeld++" PRIVATE -O3)

    # 2. Force true static linking and disable PIE
    # -static: No dynamic linking
    # -no-pie: Disables Position Independent Executable (crucial for 'scratch')
    # -s: Strips symbols (reduces size from ~10MB to ~2MB)
    set_target_properties("pixeld++" PROPERTIES
        LINK_FLAGS "-static -no-pie -s -static-libgcc -static-libstdc++"
    )
endif()

# 4. Common Configuration
target_compile_definitions(app_test PRIVATE UNIT_TEST)

target_include_directories("pixeld++" PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_include_directories(app_test PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src" "${CMAKE_CURRENT_SOURCE_DIR}/test")

target_link_libraries("pixeld++" PRIVATE Threads::Threads)
target_link_libraries(app_test PRIVATE Threads::Threads)
