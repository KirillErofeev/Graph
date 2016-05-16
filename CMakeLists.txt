cmake_minimum_required(VERSION 3.5)
project(Graphs)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(ATISTREAMSDKROOT /home/love/Programs/OpenCL/AMDAPPSDK-3.0)

set(clLib /home/love/Programs/OpenCL/AMDAPPSDK-3.0/lib/x86_64/sdk/*.*)

add_library(hts STATIC ${htsLib})

add_library(cl SHARED IMPORTED)
set_property(TARGET cl PROPERTY
        IMPORTED_LOCATION /home/love/Programs/OpenCL/AMDAPPSDK-3.0/lib/x86_64/sdk/libOpenCL.so)

include_directories(${ATISTREAMSDKROOT}/include)

set(SOURCE_FILES main.cpp tools.cpp ClInit.cpp initializer.cpp test.cpp tools.cpp TridigionalEquation.cpp Kernels.cpp Kernels.hpp)

set(SOURCE_FILES
    graph.cpp
    graph.hpp
    main.cpp
    tools.cpp
    tools.hpp random.hpp random.cpp)

add_executable(Graphs ${SOURCE_FILES})