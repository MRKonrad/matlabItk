project(itkBSplineReg)

cmake_minimum_required(VERSION 2.6)
set(CMAKE_MACOSX_RPATH 1) # what is this?

set(OutputDir "~/Documents/MATLAB/mexFuncs")

find_package(MATLAB REQUIRED)

include_directories(${Matlab_INCLUDE_DIRS})
link_directories(${Matlab_INCLUDE_DIRS})

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

file(GLOB_RECURSE util_files util/*.c* util/*.h*)
file(GLOB_RECURSE code_files code/*.c* code/*.h* )

message(STATUS ${util_files})

matlab_add_mex(NAME itkBSplineReg SRC wrapper.cpp ${util_files} ${code_files})
target_link_libraries(itkBSplineReg ${ITK_LIBRARIES})
target_link_libraries(itkBSplineReg ${Matlab_LIBRARIES})
target_link_libraries(itkBSplineReg ${Matlab_BINARIES_DIR}/libmx.dylib)
install (TARGETS itkBSplineReg DESTINATION ${OutputDir})
