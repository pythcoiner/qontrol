find_package(Qt6 REQUIRED COMPONENTS Test)

enable_testing()

add_executable(test_parent_child test_parent_child.cpp)

target_link_libraries(test_parent_child PRIVATE
    qontrol
    Qt6::Test
    Qt6::Widgets
    Qt6::Core
    Qt6::Gui
)

add_test(NAME TestParentChild COMMAND test_parent_child)
