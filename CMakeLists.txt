#Required: sudo apt-get install cmake gcc build-essential

#Every CMakeLists should start with this line (version nubmer can change)
cmake_minimum_required(VERSION 3.00)

#This is also required soon after cmake_minimum_required
project(spinning_cube)

#This adds an executable target to be build with the specified src files 
add_executable(spinning_cube src/main.cpp)