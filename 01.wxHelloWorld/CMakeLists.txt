### CMake minimum version required.  I choose to use at least 3.5
cmake_minimum_required(VERSION 3.5 FATAL_ERROR)

### The name of the project
project(wxHelloWorld)

### As we are going to use wxWidgets, we need to locate the package
find_package(wxWidgets REQUIRED core base)

### After we find wxWidgets, we include this file fo CMake to
### recognize and configure the library.
include(${wxWidgets_USE_FILE})

### We create a new executable entry.
add_executable(wxHelloWorld wxHelloWorld.cpp)

### Link the executable to wxWidgets.
target_link_libraries(wxHelloWorld ${wxWidgets_LIBRARIES})
