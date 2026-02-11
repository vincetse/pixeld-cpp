cmake_minimum_required(VERSION 3.10)
project("pixeld++" VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

# --- 1. Coverage Flags (Build 1) ---
if(CMAKE_BUILD_TYPE STREQUAL "Coverage")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -O0")
endif()

# --- 2. Dynamic/Debug Flags (Build 2) ---
if(DYNAMIC_DEBUG_BUILD)
    target_compile_options("pixeld++" PRIVATE -O0 -g)
    # Ensure it stays dynamic (default behavior, but explicit for clarity)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -rdynamic")
endif()

# --- 3. Static/Optimized Flags (Build 3) ---
if(STATIC_RELEASE_BUILD)
    target_compile_options("pixeld++" PRIVATE -O3)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -static-libgcc -static-libstdc++")
endif()

# Targets
add_executable("pixeld++" "src/main.cpp")
target_include_directories("pixeld++" PRIVATE "src")
target_link_libraries("pixeld++" PRIVATE Threads::Threads)

add_executable(app_test "test/app_test.cpp")
target_compile_definitions(app_test PRIVATE UNIT_TEST)
target_include_directories(app_test PRIVATE "src" "test")
target_link_libraries(app_test PRIVATE Threads::Threads)
