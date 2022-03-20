cmake_minimum_required(VERSION 3.3)

project(xrt-load-linux CXX)

add_executable(xrt-load-linux src/main.cpp)

set_property(TARGET xrt-load-linux PROPERTY CXX_STANDARD 14)

target_include_directories(xrt-load-linux PUBLIC /opt/xilinx/xrt/include)
target_link_libraries(xrt-load-linux /opt/xilinx/xrt/lib/libxrt_coreutil.so /opt/xilinx/xrt/lib/libxrt_core.so pthread)