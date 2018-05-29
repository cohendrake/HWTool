cmake_minimum_required(VERSION 2.8)

project(Test)

#project path

set(root_dir ".")

#set debug & release build option

set(CMAKE_C_FLAGS_DEBUG "$ENV{CFLAGS} -DINFO_OUT -O0 -Wall -g -ggdb")

set(CMAKE_C_FLAGS_RELEASE "$ENV{CFLAGS} -O3 -Wall")

#subproject
add_subdirectory("../HWTest" "../HWTest")

#build executable

set(target_out "test")

include_directories("../HWTest/Include")

link_directories("/usr/local/lib")

file(GLOB src_c ${root_dir}/Src/*.c)

add_executable(${target_out} ${src_c})

TARGET_LINK_LIBRARIES(${target_out} hwtest libfftw3.a m) #link to math libarary

