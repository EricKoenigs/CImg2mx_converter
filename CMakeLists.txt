cmake_minimum_required(VERSION 2.8)
project(CONVERTER)

find_package(Matlab)
find_package(X11)
find_package(Threads)
find_package(JPEG)
find_package(PNG)

include_directories(${MATLAB_INCLUDE_DIR}
                    ${CONVERTER_SOURCE_DIR}/include)

add_executable(converter converter.cpp)
target_link_libraries(
    converter
    ${MATLAB_MX_LIBRARY} 
    ${X11_X11_LIB}
    ${MATLAB_ENG_LIBRARY}
	${JPEG_LIBRARY}
	${PNG_LIBRARY}
    )
