# Usage Guide

This guide covers the complete API of the qontrol framework.

## Table of Contents

- [Layout Containers](#layout-containers)
  - [Row](#row)
  - [Column](#column)
- [Application Structure](#application-structure)
  - [Controller](#controller)
  - [Window](#window)
  - [Screen](#screen)
  - [Panel](#panel)
  - [Modal](#modal)
- [Widgets](#widgets)
  - [InputLine](#inputline)
  - [ComboLine](#comboline)
  - [Slider](#slider)
  - [ToggleSwitch](#toggleswitch)
  - [Collapsible](#collapsible)
  - [Input](#input)
  - [ComboBox](#combobox)
- [Memory Management](#memory-management)
- [Constants](#constants)

---

## Layout Containers

### Row

Horizontal layout container with a chainable API.

```cpp
#include <Row.h>

// Create a row
auto *row = new qontrol::Row(parent);

// Push widgets (returns Row* for chaining)
row->push(new QLabel("Name:"))
   ->push(new QLineEdit())
   ->push(new QPushButton("Submit"));
```

#### Methods

```
+----------------------------------+---------------+------------------------------------+
| Method                           | Returns       | Description                        |
+----------------------------------+---------------+------------------------------------+
| push(QWidget*)                   | Row*          | Add widget, sets parent auto       |
| push(std::optional<QWidget*>)    | Row*          | Add widget if present              |
| push(QLayout*)                   | Row*          | Add layout, parents all widgets    |
| pushSpacer()                     | Row*          | Add flexible spacer                |
| pushSpacer(int width)            | Row*          | Add fixed-width spacer             |
| pushStretch(int factor)          | Row*          | Add stretch with factor            |
| merge(Row*)                      | Row*          | Copy items from another row        |
| merge(Column*)                   | Row*          | Copy items from column             |
| clear()                          | void          | Remove and delete all items        |
| layout()                         | QHBoxLayout*  | Get underlying layout              |
| toItemList()                     | QList<Item*>  | Get item list                      |
+----------------------------------+---------------+------------------------------------+
```

#### Examples

```cpp
// Row with spacers
auto *buttonRow = (new qontrol::Row())
    ->pushSpacer()
    ->push(new QPushButton("Cancel"))
    ->pushSpacer(10)
    ->push(new QPushButton("OK"))
    ->pushSpacer();

// Conditional widget
std::optional<QWidget*> maybeWidget = getOptionalWidget();
row->push(maybeWidget);  // Only adds if has value

// Merge rows
auto *row1 = (new qontrol::Row())->push(new QLabel("A"));
auto *row2 = (new qontrol::Row())->push(new QLabel("B"));
row1->merge(row2);  // row1 now contains A and B
```

### Column

Vertical layout container with the same chainable API as Row.

```cpp
#include <Column.h>

auto *column = new qontrol::Column(parent);

column->push(new QLabel("Title"))
      ->pushSpacer(20)
      ->push(new QLineEdit())
      ->pushStretch(1);
```

#### Methods

```
+----------------------------------+---------------+------------------------------------+
| Method                           | Returns       | Description                        |
+----------------------------------+---------------+------------------------------------+
| push(QWidget*)                   | Column*       | Add widget, sets parent auto       |
| push(std::optional<QWidget*>)    | Column*       | Add widget if present              |
| push(QLayout*)                   | Column*       | Add layout, parents all widgets    |
| pushSpacer()                     | Column*       | Add flexible spacer                |
| pushSpacer(int height)           | Column*       | Add fixed-height spacer            |
| pushStretch(int factor)          | Column*       | Add stretch with factor            |
| merge(Row*)                      | Column*       | Copy items from row                |
| merge(Column*)                   | Column*       | Copy items from another column     |
| clear()                          | void          | Remove and delete all items        |
| layout()                         | QVBoxLayout*  | Get underlying layout              |
| toItemList()                     | QList<Item*>  | Get item list                      |
+----------------------------------+---------------+------------------------------------+
```

#### Nesting Example

```cpp
auto *form = (new qontrol::Column())
    ->push(new QLabel("User Settings"))
    ->pushSpacer(10)
    ->push((new qontrol::Row())
        ->push(new QLabel("Name:"))
        ->push(new QLineEdit()))
    ->push((new qontrol::Row())
        ->push(new QLabel("Email:"))
        ->push(new QLineEdit()))
    ->pushSpacer()
    ->push((new qontrol::Row())
        ->pushSpacer()
        ->push(new QPushButton("Save"))
        ->push(new QPushButton("Cancel")));
```

---

## Application Structure

### Controller

Singleton that manages the application lifecycle and panel navigation.

```cpp
#include <Controller.h>

class MyController : public qontrol::Controller {
    Q_OBJECT
public:
    void loadPanels() override {
        insertPanel(new qontrol::Panel(new HomeScreen(), "home"));
        insertPanel(new qontrol::Panel(new SettingsScreen(), "settings"));
    }
};

// In main.cpp
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    auto *controller = new MyController();
    qontrol::Controller::init(controller);

    auto *window = new qontrol::Window(nullptr);
    controller->start(window);

    window->show();
    return app.exec();
}
```

#### Static Methods

```
+----------------------+---------------+---------------------------+
| Method               | Returns       | Description               |
+----------------------+---------------+---------------------------+
| init(Controller*)    | void          | Initialize singleton      |
| isInit()             | bool          | Check if initialized      |
| get()                | Controller*   | Get singleton instance    |
| window()             | QWidget*      | Get main window           |
| execModal(QDialog*)  | void          | Show modal dialog         |
+----------------------+---------------+---------------------------+
```

#### Instance Methods

```
+----------------------+------------------------+-----------------------------+
| Method               | Returns                | Description                 |
+----------------------+------------------------+-----------------------------+
| start(Window*)       | void                   | Start application w/ window |
| loadPanels()         | void                   | Override to register panels |
| loadPanel(QString)   | void                   | Navigate to panel by name   |
| insertPanel(Panel*)  | void                   | Register a panel            |
| screen(QString)      | std::optional<Screen*> | Get screen by panel name    |
+----------------------+------------------------+-----------------------------+
```

#### Signals

```
+----------+-----------------------------------+
| Signal   | Description                       |
+----------+-----------------------------------+
| closed() | Emitted when application closes   |
+----------+-----------------------------------+
```

### Window

Main window wrapper around QMainWindow.

```cpp
#include <Window.h>

auto *window = new qontrol::Window(nullptr);
window->setWindowTitle("My App");
window->resize(800, 600);
```

#### Methods

```
+----------------------+------------+--------------------------------+
| Method               | Returns    | Description                    |
+----------------------+------------+--------------------------------+
| loadPanel(QWidget*)  | void       | Set central widget             |
| takePanel()          | QWidget*   | Remove and return central widget|
+----------------------+------------+--------------------------------+
```

### Screen

Base class for UI screens. Implement `init()`, `view()`, and `doConnect()`.

```cpp
#include <Screen.h>

class HomeScreen : public qontrol::Screen {
    Q_OBJECT
public:
    HomeScreen() : Screen() {}

protected:
    void init() override {
        // Initialize member widgets
        m_title = new QLabel("Home");
        m_button = new QPushButton("Click");
    }

    void view() override {
        // Build layout
        auto *layout = new QVBoxLayout(this);
        layout->addWidget(m_title);
        layout->addWidget(m_button);
    }

    void doConnect() override {
        // Setup signal/slot connections
        connect(m_button, &QPushButton::clicked, this, &HomeScreen::onButtonClick);
    }

public slots:
    void onUnload() override {
        // Called when navigating away
    }

private slots:
    void onButtonClick() {
        qDebug() << "Button clicked!";
    }

private:
    QLabel *m_title;
    QPushButton *m_button;
};
```

#### Lifecycle

1. Constructor called
2. `init()` - Create widgets
3. `view()` - Build layout
4. `doConnect()` - Setup connections
5. `onUnload()` - Called when navigating away

### Panel

Container that wraps a Screen for navigation.

```cpp
#include <Panel.h>

auto *screen = new HomeScreen();
auto *panel = new qontrol::Panel(screen, "home");

// Register with controller
controller->insertPanel(panel);

// Navigate to it
controller->loadPanel("home");
```

#### Methods

```
+----------------------+-------------+------------------------+
| Method               | Returns     | Description            |
+----------------------+-------------+------------------------+
| widget()             | Screen*     | Get the screen widget  |
| screen()             | Screen*     | Alias for widget()     |
| name()               | const char* | Get panel name         |
| setScreen(Screen*)   | void        | Replace the screen     |
+----------------------+-------------+------------------------+
```

### Modal

Dialog wrapper for popup windows.

```cpp
#include <Modal.h>

// Simple message modal
auto *modal = new qontrol::Modal("Title", "Message text");
qontrol::Controller::execModal(modal);

// Custom content modal
auto *modal = new qontrol::Modal();
modal->setWindowTitle("Custom Dialog");

auto *content = new qontrol::Column();
content->push(new QLabel("Enter value:"))
       ->push(new QLineEdit());
modal->setMainWidget(content);

qontrol::Controller::execModal(modal);
```

---

## Widgets

### InputLine

A row with label, input field, and optional unit label.

```cpp
#include <widgets/InputLine.h>

auto *input = (new qontrol::widgets::InputLine("width_input"))
    ->label("Width")
    ->input(qontrol::widgets::InputType::Int)
    ->unit("px");

// Get/set value
input->setValue(100);
QString value = input->input()->text();
```

#### Methods

```
+---------------------------+-------------+-------------------------------------+
| Method                    | Returns     | Description                         |
+---------------------------+-------------+-------------------------------------+
| label(QString)            | InputLine*  | Set label text                      |
| input(InputType)          | InputLine*  | Set input type (String, Int, Float) |
| validator(QValidator*)    | InputLine*  | Set input validator                 |
| inputWidth(float)         | InputLine*  | Set input width factor              |
| unit(QString)             | InputLine*  | Set unit label                      |
| setEnabled(bool)          | InputLine*  | Enable/disable                      |
| setNoFill(bool)           | InputLine*  | Disable auto-fill                   |
| input()                   | Input*      | Get input widget                    |
| setValue(int/QString/dbl) | void        | Set input value                     |
+---------------------------+-------------+-------------------------------------+
```

### ComboLine

A row with label, dropdown, and optional unit label.

```cpp
#include <widgets/ComboLine.h>

auto *combo = (new qontrol::widgets::ComboLine("format_combo"))
    ->label("Format")
    ->combo("format_enum")
    ->unit("type");

// Add items manually
combo->combo()->addItem("PNG");
combo->combo()->addItem("JPEG");
combo->combo()->addItem("GIF");

// Set selection
combo->setCurrentIndex(0);
```

#### Methods

```
+----------------------+-------------+---------------------------+
| Method               | Returns     | Description               |
+----------------------+-------------+---------------------------+
| label(QString)       | ComboLine*  | Set label text            |
| combo(QString)       | ComboLine*  | Set enum name             |
| comboWidth(float)    | ComboLine*  | Set combo width factor    |
| unit(QString)        | ComboLine*  | Set unit label            |
| setEnabled(bool)     | ComboLine*  | Enable/disable            |
| setNoFill(bool)      | ComboLine*  | Disable auto-fill         |
| combo()              | ComboBox*   | Get combo widget          |
| setCurrentIndex(int) | void        | Set selection             |
+----------------------+-------------+---------------------------+
```

### Slider

Configurable slider with header, ticks, and input field.

```cpp
#include <widgets/Slider.h>

// Percentage slider (0-100)
auto *opacity = qontrol::widgets::Slider::percent("Opacity", 80);
opacity->setValue(75);

// Slider with custom ticks
QList<int> ticks = {0, 25, 50, 75, 100};
auto *slider = qontrol::widgets::Slider::simple("Value", 100, ticks, "%", parent);

// Axis slider with optional checkbox
auto *axis = qontrol::widgets::Slider::axis("X Axis", 80, ticks, true, true, parent);
```

#### Factory Methods

```
+-------------------------------------------------------+---------------------------+
| Method                                                | Description               |
+-------------------------------------------------------+---------------------------+
| percent(label, labelWidth, parent)                    | 0-100 percentage slider   |
| simple(name, labelWidth, ticks, unit, parent)         | Basic slider with ticks   |
| axis(name, labelWidth, ticks, optional, linear, par)  | Axis control slider       |
| cursor(leftLbl, leftW, rightLbl, rightW, parent)      | Dual-label slider         |
+-------------------------------------------------------+---------------------------+
```

#### Methods

```
+-----------------------+------------+---------------------------+
| Method                | Returns    | Description               |
+-----------------------+------------+---------------------------+
| value()               | int        | Get current value         |
| setValue(int)         | void       | Set value                 |
| slider()              | QSlider*   | Get underlying QSlider    |
| addTicks(QList<int>*) | void       | Add tick marks            |
| updateTicks()         | void       | Refresh tick display      |
+-----------------------+------------+---------------------------+
```

#### Signals

```
+------------------+------------------------------+
| Signal           | Description                  |
+------------------+------------------------------+
| valueUpdated(int)| Emitted when value changes   |
+------------------+------------------------------+
```

### ToggleSwitch

iOS-style toggle switch.

```cpp
#include <widgets/ToggleSwitch.h>

auto *toggle = new qontrol::widgets::ToggleSwitch(parent);
toggle->setChecked(true);

// Custom brush color
toggle->setBrush(QBrush(Qt::blue));

// Connect to state changes
connect(toggle, &QCheckBox::toggled, [](bool checked) {
    qDebug() << "Toggle:" << checked;
});
```

#### Methods

```
+------------------+----------+---------------------------+
| Method           | Returns  | Description               |
+------------------+----------+---------------------------+
| brush()          | QBrush   | Get toggle head color     |
| setBrush(QBrush) | void     | Set toggle head color     |
+------------------+----------+---------------------------+
```

Inherits all `QCheckBox` methods: `isChecked()`, `setChecked()`, `toggled()` signal, etc.

### Collapsible

Expandable/collapsible section container.

```cpp
#include <widgets/Collapsible.h>

auto *section = new qontrol::widgets::Collapsible("Advanced Settings", parent);

section->pushInner(new QLabel("Option 1"))
       ->pushInner(new QCheckBox("Enable feature"))
       ->pushInner(new QSpinBox());

section->setCollapsed(true);  // Start collapsed
```

#### Methods

```
+---------------------+--------------+----------------------------------+
| Method              | Returns      | Description                      |
+---------------------+--------------+----------------------------------+
| pushInner(QWidget*) | Collapsible* | Add widget to collapsible content|
| setCollapsed(bool)  | void         | Set collapsed state              |
+---------------------+--------------+----------------------------------+
```

### Input

Text input with type validation.

```cpp
#include <widgets/Input.h>

// Integer input
auto *intInput = new qontrol::widgets::Input(qontrol::widgets::InputType::Int, parent);

// Float input
auto *floatInput = new qontrol::widgets::Input(qontrol::widgets::InputType::Float, parent);

// String input with name
auto *nameInput = new qontrol::widgets::Input("username", qontrol::widgets::InputType::String, parent);
```

#### InputType Enum

```
+------------------+-------------------------+
| Value            | Description             |
+------------------+-------------------------+
| InputType::String| Text input              |
| InputType::Int   | Integer input           |
| InputType::Float | Floating-point input    |
+------------------+-------------------------+
```

#### Signals

```
+----------------+------------------------------+
| Signal         | Description                  |
+----------------+------------------------------+
| valueChanged() | Emitted when value changes   |
+----------------+------------------------------+
```

### ComboBox

Dropdown selector widget.

```cpp
#include <widgets/ComboBox.h>

auto *combo = new qontrol::widgets::ComboBox("my_enum", parent);
combo->addItem("Option 1");
combo->addItem("Option 2");
combo->addItem("Option 3");

connect(combo, &qontrol::widgets::ComboBox::valueChanged, []() {
    qDebug() << "Selection changed";
});
```

#### Signals

```
+----------------+--------------------------------+
| Signal         | Description                    |
+----------------+--------------------------------+
| valueChanged() | Emitted when selection changes |
+----------------+--------------------------------+
```

---

## Memory Management

Qontrol uses Qt's parent-child ownership model. Widgets pushed to Row/Column automatically become children and are deleted when the container is destroyed.

### Safe Patterns

```cpp
// Parent set automatically by push()
auto *row = new qontrol::Row(parent);
row->push(new QLabel("text"));  // QLabel owned by row

// Explicit parent in constructor
auto *label = new QLabel("text", parent);  // Owned by parent

// Containers own their children
auto *column = new qontrol::Column();
column->push(new QLabel("A"));
column->push(new QLabel("B"));
delete column;  // A and B are deleted automatically
```

### Testing for Leaks

```bash
# Build with AddressSanitizer
cmake -B build -DENABLE_ASAN=ON
cmake --build build

# Run tests
QT_QPA_PLATFORM=offscreen ./build/tests/test_parent_child
```

See [Memory Leak Detection](docs/memory-leak-detection.md) for details.

---

## Constants

Defined in `common.h`:

```
+------------------------+-------+--------------------------------+
| Constant               | Value | Description                    |
+------------------------+-------+--------------------------------+
| QONTROL_MARGIN         | 30    | Default margin                 |
| QONTROL_LEFT_MARGIN    | 30    | Left margin                    |
| QONTROL_RIGHT_MARGIN   | 30    | Right margin                   |
| QONTROL_TOP_MARGIN     | 30    | Top margin                     |
| QONTROL_BOTTOM_MARGIN  | 30    | Bottom margin                  |
| QONTROL_LABEL_WIDTH    | 120   | Default label width            |
| QONTROL_INPUT_WIDTH    | 200   | Default input width            |
| QONTROL_UNIT_WIDTH     | 30    | Default unit label width       |
| QONTROL_V_SPACER       | 10    | Vertical spacer size           |
| QONTROL_H_SPACER       | 5     | Horizontal spacer size         |
| UNIQUE                 | Qt::UniqueConnection | Prevent duplicates|
+------------------------+-------+--------------------------------+
```

---

## Real-World Example

For a complete application example, see [**bed**](https://github.com/pythcoiner/bed) (Bitcoin Encrypted Descriptor), which demonstrates:

- Custom `Controller` subclass with application state
- Multiple `Screen` implementations
- Signal/slot patterns with `qontrol::UNIQUE`
- Dynamic widget updates
- Modal dialogs for errors and file selection
- Drag-and-drop support
