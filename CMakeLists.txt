cmake_minimum_required(VERSION 3.17)
set(LIB qontrol)
project(${LIB} VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets Gui Core Test)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

set(SOURCES
    src/MainWindow.cpp
    src/Controller.cpp
    src/widgets/Collapsible.cpp
    src/widgets/Slider.cpp
    src/widgets/ToggleSwitch.cpp
    src/widgets/ComboBox.cpp
    src/widgets/ComboLine.cpp
    src/widgets/InputLine.cpp
    src/widgets/Input.cpp
    src/Row.cpp
    src/Column.cpp
    src/Panel.cpp
    src/Screen.cpp
)

qt6_add_library(${LIB} STATIC ${SOURCES})

target_include_directories(${LIB} PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

target_link_libraries(${LIB} PRIVATE Qt6::Gui Qt6::Core Qt6::Widgets Qt::Test)

set_target_properties(${BIN} PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)
