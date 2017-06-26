cmake_minimum_required(VERSION 3.0)
project(vibration)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-std=c++11")

link_directories(/usr/local/lib  ${PROJECT_SOURCE_DOR}/src/jsoncpp/libs/linux-gcc-4.2.1 ${PROJECT_SOURCE_DIR}/src/mosquitto-1.4.5/lib )

include_directories(/usr/local/include ${PROJECT_SOURCE_DIR}/src/mosquitto-1.4.5/lib/cpp  
 ${PROJECT_SOURCE_DIR}/src/jsoncpp/dist/ 
${PROJECT_SOURCE_DIR}/include/)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)


add_subdirectory(src)
