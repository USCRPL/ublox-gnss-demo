cmake_minimum_required(VERSION 3.19)
cmake_policy(VERSION 3.19)

# Initialize Mbed OS build system. 
set(MBED_APP_JSON_PATH mbed_app.json)
include(mbed-os/tools/cmake/app.cmake)
add_subdirectory(mbed-os)

project(ublox-gnss-demo)

add_subdirectory(ublox-gnss-driver)

add_executable(GNSSDemo GNSSDemo.cpp GNSSConstructor.cpp BarGraph.cpp)
mbed_set_post_build(GNSSDemo)
target_link_libraries(GNSSDemo ublox-gnss)

mbed_finalize_build()