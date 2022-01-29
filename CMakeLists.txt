cmake_minimum_required(VERSION 3.15)

# Point CMake to the VCPKG submodule and set static linkage for it
set(CMAKE_TOOLCHAIN_FILE "vcpkg/scripts/buildsystems/vcpkg.cmake")
set(VCPKG_TARGET_TRIPLET x64-windows-static)

#set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)
project(Adam C CXX)

find_package(SFML COMPONENTS audio window system graphics CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(ImGui-SFML CONFIG REQUIRED)

# SFML vcpkg port apparently links the debug libraries in Release, fix that by manually finding and linking the bad ones
find_package(OpenAL CONFIG REQUIRED)
find_package(Ogg CONFIG REQUIRED)
find_package(Vorbis CONFIG REQUIRED)
find_package(flac CONFIG REQUIRED)

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
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${CMAKE_PROJECT_NAME})
target_precompile_headers(${CMAKE_PROJECT_NAME} PRIVATE Adam/pch.h)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE sfml-network sfml-graphics sfml-window sfml-system)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE imgui::imgui)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ImGui-SFML::ImGui-SFML)

# link the audio libraries manually, the port has a bug if you just do sfml-audio
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/vcpkg_installed/x64-windows-static/lib)
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/vcpkg_installed/x64-windows-static/debug/lib)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE debug sfml-audio-s-d.lib optimized sfml-audio-s.lib)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE OpenAL::OpenAL)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Ogg::ogg)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE FLAC::FLAC FLAC::FLAC++)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Vorbis::vorbis Vorbis::vorbisenc Vorbis::vorbisfile)


target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC "/Adam/")