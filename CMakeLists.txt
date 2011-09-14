cmake_minimum_required(VERSION 2.8)
project(CONVERTER)

find_package(Matlab)
find_package(X11)
find_package(Threads)

include_directories(${MATLAB_INCLUDE_DIR})

add_executable(converter converter.cpp)
target_link_libraries(
    converter
    ${MATLAB_MX_LIBRARY} 
    ${X11_X11_LIB}
    ${MATLAB_ENG_LIBRARY}
    )
