cmake_minimum_required(VERSION 3.30)
project(PRACTIC_Waze_ing)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


set(CMAKE_CXX_STANDARD 20)

# Qt Setup
set(CMAKE_PREFIX_PATH "D:/QT/6.5.3/mingw_64")
find_package(Qt6 REQUIRED COMPONENTS Widgets)


add_executable(PRACTIC_Waze_ing main.cpp
            Driver.cpp Driver.h
            Report.cpp Report.h
            Repository.cpp Repository.h
            Service.cpp Service.h
            DriverWindow.cpp DriverWindow.h
            MapWindow.cpp MapWindow.h

)

target_link_libraries(PRACTIC_Waze_ing Qt6::Widgets)

