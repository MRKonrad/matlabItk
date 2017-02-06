project(template_matlabItk)

cmake_minimum_required(VERSION 2.6)
set(CMAKE_MACOSX_RPATH 1) # what is this?

set(OutputDir "~/Documents/MATLAB/mexFuncs")
# set(OutputDir ${PROJECT_BINARY_DIR})

find_package(MATLAB REQUIRED)

include_directories(${Matlab_INCLUDE_DIRS})
link_directories(${Matlab_INCLUDE_DIRS})

find_package(ITK REQUIRED)
include(${ITK_USE_FILE})

file(GLOB_RECURSE util_files util/*.c* util/*.h*)
file(GLOB_RECURSE code_files code/*.c* code/*.h* )

message(STATUS ${util_files})

matlab_add_mex(NAME template_matlabItk SRC wrapper.cpp ${util_files} ${code_files})
target_link_libraries(template_matlabItk ${ITK_LIBRARIES})
target_link_libraries(template_matlabItk ${Matlab_LIBRARIES})
target_link_libraries(template_matlabItk ${Matlab_BINARIES_DIR}/libmx.dylib)
install (TARGETS template_matlabItk DESTINATION ${OutputDir})

# might be usefull one day
#
# SET_TARGET_PROPERTIES(hello PROPERTIES PREFIX "" SUFFIX .${Matlab_MEX_EXTENSION})
#
# get_cmake_property(_variableNames VARIABLES)
# foreach (_variableName ${_variableNames})
#     message(STATUS "${_variableName}=${${_variableName}}")
# endforeach()
