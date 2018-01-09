cmake_minimum_required(VERSION 3.5.1)
project (nrps)

#Set output path for executable
set(RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

#Set library output path
set(LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/library})

#Include the headers
include_directories(${PROJECT_SOURCE_DIR}/include)

#Set compiler flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -std=c++17")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -std=c++17 -o3")

file(GLOB SRC_FILES "src/*.cpp")
add_executable(nrps ${SRC_FILES})

#Use Boost
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.58.0 REQUIRED filesystem)
include_directories(${BOOST_INCLUDE_DIRS})
target_link_libraries(nrps ${Boost_LIBRARIES})
#Add source directory
#add_subdirectory(${CMAKE_SOURCE_DIR}/src)

