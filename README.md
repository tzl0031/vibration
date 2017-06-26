# vibration

Please follow this tutorial to install CMake.

https://cmake.org/install/

The project structure:
         
 /bin stores the executables.
 /src stores all the source files.
 /include stores all header files.
 /lib stores all libraries.
 

To build the project

1. Enter the project directory. 
2. $ cmake .

  If the makefile has been successfully generated, it should show something like:
    -- Configuring done//
    -- Generating done//
    -- Build files have been written to: /home/pi/vibration

3. $ make 
  If building has completed, it should show:
  ...
  [100%] Linking CXX executable ../bin/main
  [100%] Built target main
  
4. To run the executable, go to /bin.
  $ sudo ./main
  
  
Please run the following command to make sure there is no previous program running before you start a new one.
  $ source endExperiment 

If you have any question in building or using this project, please contact Tian at tzl0031@auburn.edu.
