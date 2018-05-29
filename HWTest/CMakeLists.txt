cmake_minimum_required(VERSION 2.8)

project(HWTest)

#project path

set(root_dir ".")

#set debug & release build option

set(CMAKE_C_FLAGS_DEBUG "$ENV{CFLAGS} -DINFO_OUT -O0 -Wall -g -ggdb")

set(CMAKE_C_FLAGS_RELEASE "$ENV{CFLAGS} -O3 -Wall")

#build executable

set(target_out "hwtest")

include_directories(${root_dir}/Include "../Common/Include" "../XCORR/Include" "../IIR_FILTER/Include" "../AudioIO/Include")

link_directories("/usr/local/lib")

file(GLOB src_c ${root_dir}/Src/*.c "../Common/Src/*.c" "../XCORR/Src/*.c" "../IIR_FILTER/Src/*.c" "../AudioIO/Src/*.c") 

add_library(${target_out} ${src_c})

TARGET_LINK_LIBRARIES(${target_out} m libfftw3.a) #link to math libarary

