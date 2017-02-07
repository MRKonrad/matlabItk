project(itkRegMatlab)

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

include_directories (${PROJECT_SOURCE_DIR} util code)

matlab_add_mex(NAME itkBSplineReg SRC itkBSplineRegWrapper.cxx ${util_files} ${code_files})
target_link_libraries(itkBSplineReg ${ITK_LIBRARIES})
target_link_libraries(itkBSplineReg ${Matlab_LIBRARIES})
target_link_libraries(itkBSplineReg ${Matlab_BINARIES_DIR}/libmx.dylib)
install (TARGETS itkBSplineReg DESTINATION ${OutputDir})

matlab_add_mex(NAME itkDaemonReg SRC itkDaemonRegWrapper.cxx ${util_files} ${code_files})
target_link_libraries(itkDaemonReg ${ITK_LIBRARIES})
target_link_libraries(itkDaemonReg ${Matlab_LIBRARIES})
target_link_libraries(itkDaemonReg ${Matlab_BINARIES_DIR}/libmx.dylib)
install (TARGETS itkDaemonReg DESTINATION ${OutputDir})

matlab_add_mex(NAME itkFemReg SRC itkFemRegWrapper.cxx ${util_files} ${code_files})
target_link_libraries(itkFemReg ${ITK_LIBRARIES})
target_link_libraries(itkFemReg ${Matlab_LIBRARIES})
target_link_libraries(itkFemReg ${Matlab_BINARIES_DIR}/libmx.dylib)
install (TARGETS itkFemReg DESTINATION ${OutputDir})
