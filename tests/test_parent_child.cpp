#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QSignalSpy>
#include <QTest>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "Column.h"
#include "Row.h"
#include "Item.h"

using namespace qontrol;

class TestParentChild : public QObject {
    Q_OBJECT

private:
    bool m_widgetDestroyed = false;

    void resetDestroyedFlag() { m_widgetDestroyed = false; }
    void markDestroyed() { m_widgetDestroyed = true; }

private slots:
    // === BASIC PUSH/DELETE TESTS ===

    void testColumnPushWidgetDeleted() {
        // Test: Widget pushed to Column should be deleted when Column is deleted
        resetDestroyedFlag();
        auto *widget = new QLabel("test");
        connect(widget, &QObject::destroyed, this, &TestParentChild::markDestroyed);

        auto *column = new Column();
        column->push(widget);

        delete column;

        QVERIFY2(m_widgetDestroyed, "FAIL: Widget pushed to Column was NOT deleted when Column was deleted");
    }

    void testRowPushWidgetDeleted() {
        // Test: Widget pushed to Row should be deleted when Row is deleted
        resetDestroyedFlag();
        auto *widget = new QLabel("test");
        connect(widget, &QObject::destroyed, this, &TestParentChild::markDestroyed);

        auto *row = new Row();
        row->push(widget);

        delete row;

        QVERIFY2(m_widgetDestroyed, "FAIL: Widget pushed to Row was NOT deleted when Row was deleted");
    }

    void testColumnPushOptionalWidgetDeleted() {
        // Test: Optional widget pushed to Column should be deleted
        resetDestroyedFlag();
        auto *widget = new QLabel("test");
        connect(widget, &QObject::destroyed, this, &TestParentChild::markDestroyed);

        auto *column = new Column();
        column->push(std::make_optional(widget));

        delete column;

        QVERIFY2(m_widgetDestroyed, "FAIL: Optional widget pushed to Column was NOT deleted");
    }

    void testRowPushOptionalWidgetDeleted() {
        // Test: Optional widget pushed to Row should be deleted
        resetDestroyedFlag();
        auto *widget = new QLabel("test");
        connect(widget, &QObject::destroyed, this, &TestParentChild::markDestroyed);

        auto *row = new Row();
        row->push(std::make_optional(widget));

        delete row;

        QVERIFY2(m_widgetDestroyed, "FAIL: Optional widget pushed to Row was NOT deleted");
    }

    // === NESTED ROW/COLUMN TESTS ===

    void testNestedRowInColumnDeleted() {
        // Test: Row nested in Column should be deleted when Column is deleted
        resetDestroyedFlag();
        auto *row = new Row();
        connect(row, &QObject::destroyed, this, &TestParentChild::markDestroyed);

        auto *column = new Column();
        column->push(row);

        delete column;

        QVERIFY2(m_widgetDestroyed, "FAIL: Row nested in Column was NOT deleted");
    }

    void testNestedColumnInRowDeleted() {
        // Test: Column nested in Row should be deleted when Row is deleted
        resetDestroyedFlag();
        auto *column = new Column();
        connect(column, &QObject::destroyed, this, &TestParentChild::markDestroyed);

        auto *row = new Row();
        row->push(column);

        delete row;

        QVERIFY2(m_widgetDestroyed, "FAIL: Column nested in Row was NOT deleted");
    }

    void testDeeplyNestedWidgetDeleted() {
        // Test: Widget deeply nested (Column -> Row -> Widget) should be deleted
        resetDestroyedFlag();
        auto *widget = new QLabel("deep");
        connect(widget, &QObject::destroyed, this, &TestParentChild::markDestroyed);

        auto *row = new Row();
        row->push(widget);

        auto *column = new Column();
        column->push(row);

        delete column;

        QVERIFY2(m_widgetDestroyed, "FAIL: Deeply nested widget was NOT deleted");
    }

    // === PUSH LAYOUT TESTS ===

    void testColumnPushLayoutChildrenDeleted() {
        // Test: When pushing a layout to Column, widgets in that layout should be deleted
        resetDestroyedFlag();
        auto *widget = new QLabel("in layout");
        connect(widget, &QObject::destroyed, this, &TestParentChild::markDestroyed);

        auto *innerLayout = new QVBoxLayout();
        innerLayout->addWidget(widget);

        auto *column = new Column();
        column->push(innerLayout);

        delete column;

        QVERIFY2(m_widgetDestroyed, "FAIL: Widget in layout pushed to Column was NOT deleted");
    }

    void testRowPushLayoutChildrenDeleted() {
        // Test: When pushing a layout to Row, widgets in that layout should be deleted
        resetDestroyedFlag();
        auto *widget = new QLabel("in layout");
        connect(widget, &QObject::destroyed, this, &TestParentChild::markDestroyed);

        auto *innerLayout = new QHBoxLayout();
        innerLayout->addWidget(widget);

        auto *row = new Row();
        row->push(innerLayout);

        delete row;

        QVERIFY2(m_widgetDestroyed, "FAIL: Widget in layout pushed to Row was NOT deleted");
    }

    // === MERGE TESTS ===

    void testColumnMergeColumnWidgetsDeleted() {
        // Test: Widgets from merged Column should be deleted when target Column is deleted
        resetDestroyedFlag();
        auto *widget = new QLabel("merged");
        connect(widget, &QObject::destroyed, this, &TestParentChild::markDestroyed);

        auto *sourceColumn = new Column();
        sourceColumn->push(widget);

        auto *targetColumn = new Column();
        targetColumn->merge(sourceColumn);

        // Delete source column first (widgets should have been moved to target)
        delete sourceColumn;

        // Widget should still exist (owned by target now)
        // Note: This tests if merge properly transfers ownership
        bool destroyedAfterSourceDelete = m_widgetDestroyed;

        delete targetColumn;

        QVERIFY2(m_widgetDestroyed,
            "FAIL: Widget from merged Column was NOT deleted when target Column was deleted");

        // Additional check: widget should NOT have been deleted when source was deleted
        // because it should now belong to target
        if (destroyedAfterSourceDelete) {
            qWarning() << "WARNING: Widget was deleted when source Column was deleted, "
                       << "suggesting merge doesn't transfer ownership properly";
        }
    }

    void testColumnMergeRowWidgetsDeleted() {
        // Test: Widgets from merged Row should be deleted when Column is deleted
        resetDestroyedFlag();
        auto *widget = new QLabel("merged from row");
        connect(widget, &QObject::destroyed, this, &TestParentChild::markDestroyed);

        auto *sourceRow = new Row();
        sourceRow->push(widget);

        auto *targetColumn = new Column();
        targetColumn->merge(sourceRow);

        delete sourceRow;
        bool destroyedAfterSourceDelete = m_widgetDestroyed;

        delete targetColumn;

        QVERIFY2(m_widgetDestroyed,
            "FAIL: Widget from merged Row was NOT deleted when Column was deleted");

        if (destroyedAfterSourceDelete) {
            qWarning() << "WARNING: Widget was deleted when source Row was deleted";
        }
    }

    void testRowMergeRowWidgetsDeleted() {
        // Test: Widgets from merged Row should be deleted when target Row is deleted
        resetDestroyedFlag();
        auto *widget = new QLabel("merged");
        connect(widget, &QObject::destroyed, this, &TestParentChild::markDestroyed);

        auto *sourceRow = new Row();
        sourceRow->push(widget);

        auto *targetRow = new Row();
        targetRow->merge(sourceRow);

        delete sourceRow;
        bool destroyedAfterSourceDelete = m_widgetDestroyed;

        delete targetRow;

        QVERIFY2(m_widgetDestroyed,
            "FAIL: Widget from merged Row was NOT deleted when target Row was deleted");

        if (destroyedAfterSourceDelete) {
            qWarning() << "WARNING: Widget was deleted when source Row was deleted";
        }
    }

    void testRowMergeColumnWidgetsDeleted() {
        // Test: Widgets from merged Column should be deleted when Row is deleted
        resetDestroyedFlag();
        auto *widget = new QLabel("merged from column");
        connect(widget, &QObject::destroyed, this, &TestParentChild::markDestroyed);

        auto *sourceColumn = new Column();
        sourceColumn->push(widget);

        auto *targetRow = new Row();
        targetRow->merge(sourceColumn);

        delete sourceColumn;
        bool destroyedAfterSourceDelete = m_widgetDestroyed;

        delete targetRow;

        QVERIFY2(m_widgetDestroyed,
            "FAIL: Widget from merged Column was NOT deleted when Row was deleted");

        if (destroyedAfterSourceDelete) {
            qWarning() << "WARNING: Widget was deleted when source Column was deleted";
        }
    }

    // === CLEAR TESTS ===

    void testColumnClearDeletesWidgets() {
        // Test: Calling clear() on Column should delete its widgets
        resetDestroyedFlag();
        auto *widget = new QLabel("to be cleared");
        connect(widget, &QObject::destroyed, this, &TestParentChild::markDestroyed);

        auto *column = new Column();
        column->push(widget);
        column->clear();

        QVERIFY2(m_widgetDestroyed, "FAIL: Widget was NOT deleted when Column::clear() was called");

        delete column;
    }

    void testRowClearDeletesWidgets() {
        // Test: Calling clear() on Row should delete its widgets
        resetDestroyedFlag();
        auto *widget = new QLabel("to be cleared");
        connect(widget, &QObject::destroyed, this, &TestParentChild::markDestroyed);

        auto *row = new Row();
        row->push(widget);
        row->clear();

        QVERIFY2(m_widgetDestroyed, "FAIL: Widget was NOT deleted when Row::clear() was called");

        delete row;
    }

    void testColumnClearThenPushNewWidgets() {
        // Test: After clear(), new widgets should be properly managed
        resetDestroyedFlag();
        auto *widget1 = new QLabel("first");
        auto *widget2 = new QLabel("second");
        connect(widget2, &QObject::destroyed, this, &TestParentChild::markDestroyed);

        auto *column = new Column();
        column->push(widget1);
        column->clear();
        column->push(widget2);

        delete column;

        QVERIFY2(m_widgetDestroyed,
            "FAIL: Widget pushed after clear() was NOT deleted when Column was deleted");
    }

    void testRowClearThenPushNewWidgets() {
        // Test: After clear(), new widgets should be properly managed
        resetDestroyedFlag();
        auto *widget1 = new QLabel("first");
        auto *widget2 = new QLabel("second");
        connect(widget2, &QObject::destroyed, this, &TestParentChild::markDestroyed);

        auto *row = new Row();
        row->push(widget1);
        row->clear();
        row->push(widget2);

        delete row;

        QVERIFY2(m_widgetDestroyed,
            "FAIL: Widget pushed after clear() was NOT deleted when Row was deleted");
    }

    // === EDGE CASES ===

    void testWidgetWithExistingParentColumn() {
        // Test: Widget with existing parent pushed to Column
        resetDestroyedFlag();
        auto *originalParent = new QWidget();
        auto *widget = new QLabel("has parent", originalParent);
        connect(widget, &QObject::destroyed, this, &TestParentChild::markDestroyed);

        auto *column = new Column();
        column->push(widget);

        // Delete original parent - widget should NOT be deleted (reparented to column)
        delete originalParent;
        bool destroyedWithOriginalParent = m_widgetDestroyed;

        // Delete column - widget SHOULD be deleted now
        delete column;

        QVERIFY2(m_widgetDestroyed,
            "FAIL: Widget was NOT deleted when Column was deleted");
        QVERIFY2(!destroyedWithOriginalParent,
            "FAIL: Widget was deleted with original parent instead of being reparented to Column");
    }

    void testWidgetWithExistingParentRow() {
        // Test: Widget with existing parent pushed to Row
        resetDestroyedFlag();
        auto *originalParent = new QWidget();
        auto *widget = new QLabel("has parent", originalParent);
        connect(widget, &QObject::destroyed, this, &TestParentChild::markDestroyed);

        auto *row = new Row();
        row->push(widget);

        delete originalParent;
        bool destroyedWithOriginalParent = m_widgetDestroyed;

        delete row;

        QVERIFY2(m_widgetDestroyed,
            "FAIL: Widget was NOT deleted when Row was deleted");
        QVERIFY2(!destroyedWithOriginalParent,
            "FAIL: Widget was deleted with original parent instead of being reparented to Row");
    }

    void testMultipleWidgetsColumn() {
        // Test: Multiple widgets in Column should all be deleted
        int destroyedCount = 0;
        auto countDestroyed = [&destroyedCount]() { destroyedCount++; };

        auto *widget1 = new QLabel("w1");
        auto *widget2 = new QLabel("w2");
        auto *widget3 = new QLabel("w3");

        connect(widget1, &QObject::destroyed, countDestroyed);
        connect(widget2, &QObject::destroyed, countDestroyed);
        connect(widget3, &QObject::destroyed, countDestroyed);

        auto *column = new Column();
        column->push(widget1);
        column->push(widget2);
        column->push(widget3);

        delete column;

        QVERIFY2(destroyedCount == 3,
            QString("FAIL: Only %1/3 widgets were deleted when Column was deleted")
                .arg(destroyedCount).toUtf8().constData());
    }

    void testMultipleWidgetsRow() {
        // Test: Multiple widgets in Row should all be deleted
        int destroyedCount = 0;
        auto countDestroyed = [&destroyedCount]() { destroyedCount++; };

        auto *widget1 = new QLabel("w1");
        auto *widget2 = new QLabel("w2");
        auto *widget3 = new QLabel("w3");

        connect(widget1, &QObject::destroyed, countDestroyed);
        connect(widget2, &QObject::destroyed, countDestroyed);
        connect(widget3, &QObject::destroyed, countDestroyed);

        auto *row = new Row();
        row->push(widget1);
        row->push(widget2);
        row->push(widget3);

        delete row;

        QVERIFY2(destroyedCount == 3,
            QString("FAIL: Only %1/3 widgets were deleted when Row was deleted")
                .arg(destroyedCount).toUtf8().constData());
    }

    // === VERIFY PARENT IS SET CORRECTLY ===

    void testColumnSetsParent() {
        auto *widget = new QLabel("test");
        auto *column = new Column();

        column->push(widget);

        QVERIFY2(widget->parent() == column,
            "FAIL: Column::push() did not set widget's parent to Column");

        delete column;
    }

    void testRowSetsParent() {
        auto *widget = new QLabel("test");
        auto *row = new Row();

        row->push(widget);

        QVERIFY2(widget->parent() == row,
            "FAIL: Row::push() did not set widget's parent to Row");

        delete row;
    }

    void testNestedColumnParent() {
        auto *innerColumn = new Column();
        auto *outerColumn = new Column();

        outerColumn->push(innerColumn);

        QVERIFY2(innerColumn->parent() == outerColumn,
            "FAIL: Nested Column's parent is not set to outer Column");

        delete outerColumn;
    }

    void testNestedRowParent() {
        auto *innerRow = new Row();
        auto *outerRow = new Row();

        outerRow->push(innerRow);

        QVERIFY2(innerRow->parent() == outerRow,
            "FAIL: Nested Row's parent is not set to outer Row");

        delete outerRow;
    }
};

QTEST_MAIN(TestParentChild)
#include "test_parent_child.moc"
