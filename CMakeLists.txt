cmake_minimum_required(VERSION 3.16)
project(MatrixMultiplier LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Qt meta-object features automatically for this target.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

add_executable(matrix_multiplier
    main.cpp
)

target_link_libraries(matrix_multiplier PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
)
