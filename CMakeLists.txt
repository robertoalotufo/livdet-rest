cmake_minimum_required(VERSION 3.1)
project(livdet-cpr)

add_subdirectory(docopt.cpp docopt_build)
add_subdirectory(cpr cpr_build)
add_subdirectory(json11 json11_build)

add_executable(livdet-client livdet-client.cpp)
set_property(TARGET livdet-client PROPERTY CXX_STANDARD 11)

include_directories(${CPR_INCLUDE_DIRS})
target_link_libraries(livdet-client ${CPR_LIBRARIES} docopt_s json11)
