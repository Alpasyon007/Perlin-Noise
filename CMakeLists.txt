# CMake Bulid File
#
# CMake Version
cmake_minimum_required(VERSION 3.10)

# Compiler Settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set the project name
project(PerlinNoise)

# Add the executable
add_executable(
	PerlinNoise
	${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/stb/stb_image_write.h
	${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/stb/stb_image_write.cpp

)

# Include target directories
target_include_directories(
	PerlinNoise
	PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/stb
)