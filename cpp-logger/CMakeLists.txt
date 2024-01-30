cmake_minimum_required(VERSION 3.14)

set (CMAKE_CXX_STANDARD 17)

project(pcap-logger)

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(pcap-logger ${SOURCES})

target_include_directories(pcap-logger PRIVATE ${PROJECT_SOURCE_DIR}/include)

include(FetchContent)

FetchContent_Declare(
  pcapplusplus
  GIT_REPOSITORY https://github.com/seladb/PcapPlusPlus.git
  GIT_TAG        master
)

FetchContent_GetProperties(pcapplusplus)
if(NOT pcapplusplus_POPULATED)
  FetchContent_Populate(pcapplusplus)
  add_subdirectory(${pcapplusplus_SOURCE_DIR} ${pcapplusplus_BINARY_DIR})
endif()


target_link_libraries(pcap-logger PRIVATE Common++ Packet++ Pcap++)
