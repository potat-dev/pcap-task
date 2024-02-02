cmake_minimum_required(VERSION 3.14)

set (CMAKE_CXX_STANDARD 17)

project(packet-classifier)

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(packet-classifier ${SOURCES})

target_include_directories(packet-classifier PRIVATE ${PROJECT_SOURCE_DIR}/include)

include(FetchContent)

# fetch and link Pcap++

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

target_link_libraries(packet-classifier PRIVATE Common++ Packet++ Pcap++)

# fetch and link CLI11

FetchContent_Declare(
    cli11_proj
    QUIET
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
    GIT_TAG v2.3.2
)

FetchContent_MakeAvailable(cli11_proj)

target_link_libraries(packet-classifier PRIVATE CLI11::CLI11)
