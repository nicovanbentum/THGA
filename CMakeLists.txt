cmake_minimum_required(VERSION 3.15)

# Point CMake to the VCPKG submodule and set static linkage for it
set(CMAKE_TOOLCHAIN_FILE "vcpkg/scripts/buildsystems/vcpkg.cmake")
set(VCPKG_TARGET_TRIPLET x64-windows-static)

#set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)
project(Adam C CXX)

find_package(SFML COMPONENTS system window graphics audio CONFIG REQUIRED)
find_package(ImGui-SFML CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

# Build a list of all cpp source files
file(GLOB_RECURSE HFiles Adam/*.h)
file(GLOB_RECURSE HppFiles Adam/*.hpp)
file(GLOB_RECURSE CppFiles Adam/*.cpp)

# Rule to build the actual exectuable and enable c++17
add_executable(${CMAKE_PROJECT_NAME} ${CppFiles} ${HppFiles} ${HFiles})
target_compile_features(${CMAKE_PROJECT_NAME} PUBLIC cxx_std_17)

# Enable MSVC parallel build process
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")

# MSVC stuff for root working directory, static runtime and enabling precompiled headers
set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/Adam")
set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
#set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:")
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${CMAKE_PROJECT_NAME})
target_precompile_headers(${CMAKE_PROJECT_NAME} PRIVATE Adam/pch.h)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE imgui::imgui)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ImGui-SFML::ImGui-SFML)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE FLAC OpenAL OpenGL Vorbis)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE sfml-system sfml-network sfml-graphics sfml-window sfml-audio)

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC "/Adam/")