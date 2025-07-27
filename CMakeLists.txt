cmake_minimum_required(VERSION 3.5)
project(sysfetch C)
set(CMAKE_C_CSTANDARD 89)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -O -W -Wall -pedantic -ansi -std=c89")
file (GLOB MAIN
	"src/main.c",
)
add_executable(sysfetch ${MAIN})
