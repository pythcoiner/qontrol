cmake_minimum_required(VERSION 3.17)
set(LIB_QONTROL qontrol)
project(${LIB_QONTROL} VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Memory leak detection options
option(ENABLE_ASAN "Enable AddressSanitizer for memory leak detection" OFF)
option(ENABLE_LSAN "Enable LeakSanitizer only (lighter weight)" OFF)

if(ENABLE_ASAN)
    message(STATUS "AddressSanitizer enabled")
    add_compile_options(-fsanitize=address -fno-omit-frame-pointer -g)
    add_link_options(-fsanitize=address)
endif()

if(ENABLE_LSAN AND NOT ENABLE_ASAN)
    message(STATUS "LeakSanitizer enabled")
    add_compile_options(-fsanitize=leak -fno-omit-frame-pointer -g)
    add_link_options(-fsanitize=leak)
endif()

find_package(Qt6 REQUIRED COMPONENTS Widgets Gui Core)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

set(SOURCES
    src/Controller.cpp
    src/Window.cpp
    src/widgets/Collapsible.cpp
    src/widgets/Slider.cpp
    src/widgets/ToggleSwitch.cpp
    src/widgets/ComboBox.cpp
    src/widgets/ComboLine.cpp
    src/widgets/InputLine.cpp
    src/widgets/Input.cpp
    src/Item.cpp
    src/Row.cpp
    src/Column.cpp
    src/Panel.cpp
    src/Modal.cpp
    src/Screen.cpp
)

qt6_add_library(${LIB_QONTROL} STATIC ${SOURCES})

target_include_directories(${LIB_QONTROL} PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

target_link_libraries(${LIB_QONTROL} PRIVATE Qt6::Gui Qt6::Core Qt6::Widgets)

set_target_properties(${LIB_QONTROL_QONTROL} PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)

add_subdirectory(tests)

option(BUILD_EXAMPLES "Build example applications" OFF)
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
