cmake_minimum_required(VERSION 3.5)
project(sysfetch C)
set(CMAKE_C_CSTANDARD 89)
file (GLOB MAIN
	"src/main.c",
)
add_executable(sysfetch ${MAIN})
