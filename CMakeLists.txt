cmake_minimum_required(VERSION 3.28)
# Project name
project(cpp_server)

# link source files
add_executable(${PROJECT_NAME} src/main.cpp)

# link threads
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC Threads::Threads)

# link header files
target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/external
)