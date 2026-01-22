# Qontrol

A modern C++ Qt6 widget framework for building structured UI applications with
automatic memory management.

## Features

- **Layout Containers** - `Row` and `Column` with chainable API
- **Screen Management** - MVC-style screens with lifecycle hooks
- **Custom Widgets** - Sliders, inputs, toggle switches, collapsible sections
- **Memory Safety** - Automatic Qt parent-child ownership
- **Modern C++** - C++17, smart patterns, no manual memory management needed

## Requirements

- CMake 3.17+
- Qt 6.x
- C++ compiler

## Installation

### As a Subdirectory

```cmake
add_subdirectory(qontrol)
target_link_libraries(your_app PRIVATE qontrol)
```

### Manual Build

```bash
cmake -B build
cmake --build build
```

## Quick Start

```cpp
#include <Column.h>
#include <Row.h>
#include <QLabel>
#include <QPushButton>

// Create a simple layout
auto *column = new qontrol::Column();
column->push(new QLabel("Hello"))
      ->push(new QPushButton("Click me"))
      ->pushSpacer(10)
      ->push((new qontrol::Row())
          ->push(new QPushButton("OK"))
          ->pushSpacer()
          ->push(new QPushButton("Cancel")));
```

## Architecture

```
Application
├── Controller (singleton, manages panels)
├── Window (QMainWindow wrapper)
└── Panel (screen container)
    └── Screen (your UI)
        ├── Row (horizontal layout)
        ├── Column (vertical layout)
        └── Widgets
```

## Components

```
+--------------+----------------------------------+
| Component    | Description                      |
+--------------+----------------------------------+
| Row          | Horizontal layout container      |
| Column       | Vertical layout container        |
| Screen       | Base class for UI screens        |
| Controller   | Application controller singleton |
| Panel        | Screen wrapper for navigation    |
| Modal        | Dialog wrapper                   |
+--------------+----------------------------------+
```

### Widgets

```
+--------------+-------------------------------------------+
| Widget       | Description                               |
+--------------+-------------------------------------------+
| Slider       | Configurable slider with ticks and labels |
| InputLine    | Label + input field + unit                |
| ComboLine    | Label + dropdown + unit                   |
| ToggleSwitch | iOS-style toggle                          |
| Collapsible  | Expandable/collapsible section            |
| Input        | Text input with type validation           |
| ComboBox     | Dropdown selector                         |
+--------------+-------------------------------------------+
```

## Documentation

- [Usage Guide](USAGE.md) - Detailed API documentation and examples
- [Memory Leak Detection](docs/memory-leak-detection.md) - Testing for memory leaks

## Examples

See the [examples/](examples/) directory for runnable code samples:

- `01_basic_layout.cpp` - Row and Column basics
- `03_widgets.cpp` - Custom widgets (InputLine, Slider, Collapsible, etc.)
- `04_modal.cpp` - Modal dialogs

Build examples:
```bash
cmake -B build -DBUILD_EXAMPLES=ON
cmake --build build
./build/examples/example_basic_layout
```

### Real-World Example

[**bed**](https://github.com/pythcoiner/bed) - Bitcoin Encrypted Descriptor tool
built with qontrol. Demonstrates:
- Custom Controller implementation
- Screen lifecycle (init/view/doConnect)
- Signal/slot patterns with `qontrol::UNIQUE`
- Modal dialogs
- Dynamic widget updates

## Testing

```bash
cmake -B build
cmake --build build
QT_QPA_PLATFORM=offscreen ./build/tests/test_parent_child
```

### With AddressSanitizer

```bash
cmake -B build -DENABLE_ASAN=ON
cmake --build build
QT_QPA_PLATFORM=offscreen ./build/tests/test_parent_child
```

## License

MIT
