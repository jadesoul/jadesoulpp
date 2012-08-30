cmake_minimum_required(VERSION 2.6)
project (application)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

include_directories(../include)

add_executable(application jadesoul.cpp)
add_custom_command(
	TARGET application
	POST_BUILD
	COMMAND python ${CMAKE_SOURCE_DIR}/scripts/move_target.py ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} application
	COMMENT "Postbuild action : move target application"
)

add_executable(file_copy file_copy.cpp)
add_custom_command(
	TARGET file_copy
	POST_BUILD
	COMMAND python ${CMAKE_SOURCE_DIR}/scripts/move_target.py ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} file_copy
	COMMENT "Postbuild action : move target file_copy"
)