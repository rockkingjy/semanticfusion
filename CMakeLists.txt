cmake_minimum_required(VERSION 3.0.2)
if(POLICY CMP0007)
  cmake_policy(SET CMP0007 NEW)
endif()

project(SemanticFusion)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}")

find_package(CUDA REQUIRED)
find_package(Glog REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Caffe REQUIRED)
find_package(OpenNI2 REQUIRED)
find_package(realsense2 REQUIRED)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/elasticfusionpublic/Core/src")
#add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/caffe_semanticfusion")

file(GLOB_RECURSE srcs src/*.cpp)
file(GLOB_RECURSE cuda src/*.cu)

set(CUDA_ARCH_BIN "30 35 50 52 61" CACHE STRING "Specify 'real' GPU arch to build binaries for, BIN(PTX) format is supported. Example: 1.3 2.1(1.3) or 13 21(13)")
include(CudaComputeTargetFlags.cmake)                  
APPEND_TARGET_ARCH_FLAGS()

set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS} "--ftz=true;--prec-div=false;--prec-sqrt=false") 
set(CUDA_HOST_COMPILER gcc-5)

set(CAFFE_PATH /media/elab/sdd/mycodes/caffe)
#set(ELASTICFUSION_PATH /media/elab/sdd/mycodes/ElasticFusion/Core/src)#/media/elab/sdd/mycodes/semanticfusion/elasticfusionpublic/Core/src)
set(CMAKE_CXX_FLAGS "-O3 -msse2 -msse3 -Wall -std=c++11 -I${CAFFE_PATH}/build/include/ -I${CAFFE_PATH}/include/")# -I${ELASTICFUSION_PATH} -I/usr/local/include/eigen3/")

cuda_add_library(semantic_fusion_cuda_library SHARED ${cuda} )

add_executable(${CMAKE_PROJECT_NAME}
               ${srcs}
)

target_link_libraries(${CMAKE_PROJECT_NAME}
                      ${CUDA_LIBRARIES}
                      ${GLOG_LIBRARY}
                      ${OpenCV_LIBS}
                      ${OPENNI2_LIBRARY}
                      semantic_fusion_cuda_library
                      GL
                      GLEW
                      jpeg
                      pangolin
                      efusion
                      caffe
                      realsense2::realsense2
)

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
                           ${EIGEN_INCLUDE_DIRS}
                           ${CUDA_INCLUDE_DIRS}
                           ${GLOG_INCLUDE_DIR}
                           ${OPENNI2_INCLUDE_DIR}
)
