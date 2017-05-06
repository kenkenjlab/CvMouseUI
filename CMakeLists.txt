#### CMake Settings ####

cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
cmake_policy(SET CMP0015 OLD)	#// Do not add relative path if the designated path is not exist
add_definitions(-DUNICODE -D_UNICODE)	#// Set Unicode, not MultiBytes


#### Solution Settings ####

set(PROJ_NAME CvMouseUI)
project(${PROJ_NAME})
find_package(OpenCV 3.1 REQUIRED)


#### Includes and Libraries ####

include_directories(${OpenCV_INCLUDE_DIRS})


#### Projects Settings ####

file(GLOB SOURCE_FILES *.c *.cpp *.h *.hpp)
add_executable(${PROJ_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJ_NAME} ${OpenCV_LIBS})