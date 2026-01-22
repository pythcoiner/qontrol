/**
 * Example 03: Custom Widgets
 *
 * Demonstrates InputLine, ComboLine, Slider, ToggleSwitch, and Collapsible.
 */

#include <Column.h>
#include <Row.h>
#include <widgets/Collapsible.h>
#include <widgets/ComboLine.h>
#include <widgets/InputLine.h>
#include <widgets/Slider.h>
#include <widgets/ToggleSwitch.h>
#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QWidget>

auto main(int argc, char *argv[]) -> int {
    QApplication app(argc, argv);

    auto *window = new QWidget();
    window->setWindowTitle("Widgets Example");
    window->setFixedSize(500, 600);

    // InputLine: label + input + unit
    auto *widthInput = (new qontrol::widgets::InputLine("width"))
        ->label("Width")
        ->input(qontrol::widgets::InputType::Int)
        ->unit("px");

    auto *heightInput = (new qontrol::widgets::InputLine("height"))
        ->label("Height")
        ->input(qontrol::widgets::InputType::Int)
        ->unit("px");

    // ComboLine: label + dropdown
    auto *formatCombo = (new qontrol::widgets::ComboLine("format"))
        ->label("Format")
        ->combo("format_enum");
    formatCombo->combo()->addItem("PNG");
    formatCombo->combo()->addItem("JPEG");
    formatCombo->combo()->addItem("WebP");

    // Slider with percentage
    auto *opacitySlider = qontrol::widgets::Slider::percent("Opacity", 80);
    opacitySlider->setValue(100);

    // Slider with custom ticks
    QList<int> qualityTicks = {0, 25, 50, 75, 100};
    auto *qualitySlider = qontrol::widgets::Slider::simple(
        "Quality", 80, qualityTicks, "%", nullptr);

    // ToggleSwitch
    auto *toggle = new qontrol::widgets::ToggleSwitch();
    auto *toggleRow = (new qontrol::Row())
        ->push(new QLabel("Enable feature"))
        ->pushSpacer()
        ->push(toggle);

    // Collapsible section
    auto *advanced = new qontrol::widgets::Collapsible("Advanced Settings");
    advanced->pushInner(new QLabel("DPI:"));
    advanced->pushInner((new qontrol::widgets::InputLine("dpi"))
        ->input(qontrol::widgets::InputType::Int)
        ->unit("dpi"));
    advanced->pushInner(new QLabel("Compression:"));
    advanced->pushInner((new qontrol::widgets::InputLine("compression"))
        ->input(qontrol::widgets::InputType::Int)
        ->unit("%"));
    advanced->setCollapsed(false);

    // Build main layout
    auto *layout = (new qontrol::Column())
        ->pushSpacer()
        ->push(new QLabel("Image Settings"))
        ->pushSpacer(20)
        ->push(widthInput)
        ->pushSpacer(10)
        ->push(heightInput)
        ->pushSpacer(10)
        ->push(formatCombo)
        ->pushSpacer(20)
        ->push(opacitySlider)
        ->pushSpacer(10)
        ->push(qualitySlider)
        ->pushSpacer(20)
        ->push(toggleRow)
        ->pushSpacer(20)
        ->push(advanced)
        ->pushSpacer();

    auto *centered = (new qontrol::Row())
        ->pushSpacer()
        ->push(layout)
        ->pushSpacer();

    window->setLayout(centered->layout());
    window->show();

    return QApplication::exec();
}
