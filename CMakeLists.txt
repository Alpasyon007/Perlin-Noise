# CMake Bulid File
#
# CMake Version
cmake_minimum_required(VERSION 3.10)

# Compiler Settings
set(CMAKE_CXX_COMPILER "/usr/bin/g++")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set Variables
set(SOURCE_DIR src)

# Set the project name
project(PerlinNoise)

# Add the executable
add_executable(
	PerlinNoise
		${SOURCE_DIR}/main.cpp
)

# Include target directories
# target_include_directories(
# 	PerlinNoise
# 	PRIVATE
# 		${SOURCE_DIR}/Lexer
# 		${SOURCE_DIR}/Token
# 		${SOURCE_DIR}/Parser
# 		${SOURCE_DIR}/Operation
# 		${SOURCE_DIR}/Debug
# )