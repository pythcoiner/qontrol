# Examples CMakeLists.txt
# Build examples with: cmake -B build -DBUILD_EXAMPLES=ON

cmake_minimum_required(VERSION 3.17)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Example 01: Basic Layout
add_executable(example_basic_layout 01_basic_layout.cpp)
target_link_libraries(example_basic_layout PRIVATE qontrol Qt6::Widgets)

# Example 03: Widgets
add_executable(example_widgets 03_widgets.cpp)
target_link_libraries(example_widgets PRIVATE qontrol Qt6::Widgets)

# Example 04: Modal
add_executable(example_modal 04_modal.cpp)
target_link_libraries(example_modal PRIVATE qontrol Qt6::Widgets)
