project(vibration)

set(SRC_LIST main.cpp 
Communicator.cpp
FileUtil.cpp
I2Cdev.cpp
ADXL345.cpp)
add_executable(main ${SRC_LIST})

target_link_libraries(main pthread mosquittopp json bcm2835)
