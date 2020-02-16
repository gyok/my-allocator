cmake_minimum_required(VERSION 2.8.3)

project(myAllocator CXX)

set(HEADER_CHUNK Chunk.h)
set(SOURCE_MYALLOCATOR MyAllocator.cpp)
set(HEADER_MYALLOCATOR MyAllocator.h)

add_library(chunk STATIC ${HEADER_CHUNK})
add_library(allocator STATIC ${SOURCE_MYALLOCATOR} ${HEADER_MYALLOCATOR})

set_target_properties(chunk PROPERTIES LINKER_LANGUAGE CXX)

add_executable(${PROJECT_NAME} "main.cpp")

target_link_libraries(${PROJECT_NAME} chunk allocator)

