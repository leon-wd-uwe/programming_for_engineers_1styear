cmake_minimum_required(VERSION 3.21)
project(PowerAnalyser C)

set(CMAKE_C_STANDARD 99)

add_executable(PowerAnalyser main.c waveform.c io.c)
target_link_libraries(PowerAnalyser m)