#include <QApplication>
#include <QBoxLayout>
#include <QCoreApplication>
#include <QEvent>
#include <QGridLayout>
#include <QLabel>
#include <QMargins>
#include <QTest>

#include "Column.h"
#include "Grid.h"
#include "Item.h"
#include "Row.h"

using namespace qontrol;

class TestLayoutApi : public QObject {
    Q_OBJECT

private slots:
    // push(widget, stretch, alignment) applies both to the underlying layout.
    void testPushStretchAlignment() {
        auto *col = new Column();
        auto *w = new QLabel("x");
        col->push(w, 3, Qt::AlignHCenter);
        QCOMPARE(col->layout()->stretch(0), 3);
        QVERIFY(col->layout()->itemAt(0)->alignment() == Qt::Alignment(Qt::AlignHCenter));
        delete col;
    }

    // pushAligned() aligns with stretch 0 and avoids overload ambiguity.
    void testPushAligned() {
        auto *row = new Row();
        auto *w = new QLabel("x");
        row->pushAligned(w, Qt::AlignBottom);
        QCOMPARE(row->layout()->stretch(0), 0);
        QVERIFY(row->layout()->itemAt(0)->alignment() == Qt::Alignment(Qt::AlignBottom));
        delete row;
    }

    // spacing()/margins() forward to the layout.
    void testSpacingAndMargins() {
        auto *col = new Column();
        col->spacing(12)->margins(4, 5, 6, 7);
        QCOMPARE(col->layout()->spacing(), 12);
        QCOMPARE(col->layout()->contentsMargins(), QMargins(4, 5, 6, 7));
        delete col;

        auto *row = new Row();
        row->margins(9);
        QCOMPARE(row->layout()->contentsMargins(), QMargins(9, 9, 9, 9));
        delete row;
    }

    // Layout spacing collapses around a hidden widget (unlike a fixed pushSpacer):
    // hiding the middle item removes its width AND a spacing from the layout's
    // size hint.
    void testSpacingCollapsesAroundHidden() {
        auto *row = new Row();
        row->spacing(10);
        auto *a = new QLabel("a");
        auto *b = new QLabel("b");
        auto *c = new QLabel("c");
        a->setFixedSize(20, 20);
        b->setFixedSize(20, 20);
        c->setFixedSize(20, 20);
        row->push(a)->push(b)->push(c);
        const int full = row->layout()->sizeHint().width(); // 3*20 + 2*10
        QCOMPARE(full, 80);
        b->hide();
        row->layout()->invalidate();
        const int hidden = row->layout()->sizeHint().width(); // 2*20 + 1*10
        QCOMPARE(hidden, 50);
        delete row;
    }

    // toItemList() -> merge() reproduces stretch + alignment.
    void testMergePreservesStretchAlignment() {
        auto *src = new Column();
        auto *a = new QLabel("a");
        auto *b = new QLabel("b");
        src->push(a, 2, Qt::AlignRight);
        src->push(b);

        auto *dst = new Column();
        dst->merge(src);

        QCOMPARE(dst->layout()->count(), 2);
        QCOMPARE(dst->layout()->stretch(0), 2);
        QVERIFY(dst->layout()->itemAt(0)->alignment() == Qt::Alignment(Qt::AlignRight));
        QCOMPARE(dst->layout()->stretch(1), 0);

        delete dst; // owns a and b after the merge reparented them
        delete src;
    }

    // into() hosts the builder inside an existing widget.
    void testInto() {
        auto *target = new QWidget();
        auto *col = new Column();
        col->push(new QLabel("x"));
        col->into(target);
        QVERIFY(target->layout() != nullptr);
        QVERIFY(col->parent() == target);
        delete target; // deletes col (and its child)
    }

    // insert() places a widget at the given index.
    void testInsert() {
        auto *col = new Column();
        auto *a = new QLabel("a");
        auto *c = new QLabel("c");
        auto *b = new QLabel("b");
        col->push(a)->push(c);
        col->insert(1, b, 2, Qt::AlignHCenter);
        QCOMPARE(col->layout()->count(), 3);
        QCOMPARE(col->layout()->itemAt(0)->widget(), a);
        QCOMPARE(col->layout()->itemAt(1)->widget(), b);
        QCOMPARE(col->layout()->itemAt(2)->widget(), c);
        QCOMPARE(col->layout()->stretch(1), 2);
        QVERIFY(col->layout()->itemAt(1)->alignment() == Qt::Alignment(Qt::AlignHCenter));
        delete col;
    }

    // take() detaches and returns the widget without deleting it.
    void testTake() {
        auto *col = new Column();
        auto *a = new QLabel("a");
        auto *b = new QLabel("b");
        col->push(a)->push(b);
        QWidget *taken = col->take(a);
        QCOMPARE(taken, a);
        QVERIFY(a->parent() == nullptr);
        QCOMPARE(col->layout()->count(), 1);
        QCOMPARE(col->layout()->indexOf(a), -1);
        delete a; // caller owns it now
        delete col;
    }

    // remove() detaches and deletes the widget.
    void testRemove() {
        auto *col = new Column();
        auto *a = new QLabel("a");
        auto *b = new QLabel("b");
        col->push(a)->push(b);
        col->remove(a);
        QCOMPARE(col->layout()->count(), 1);
        QCOMPARE(col->layout()->indexOf(a), -1);
        QCoreApplication::sendPostedEvents(nullptr, QEvent::DeferredDelete); // flush deleteLater
        delete col;
    }

    // replace() swaps a widget preserving its stretch and alignment.
    void testReplace() {
        auto *col = new Column();
        auto *a = new QLabel("a");
        auto *b = new QLabel("b");
        col->push(a, 2, Qt::AlignRight)->push(b);
        auto *x = new QLabel("x");
        col->replace(a, x);
        QCOMPARE(col->layout()->count(), 2);
        QCOMPARE(col->layout()->itemAt(0)->widget(), x);
        QCOMPARE(col->layout()->stretch(0), 2);
        QVERIFY(col->layout()->itemAt(0)->alignment() == Qt::Alignment(Qt::AlignRight));
        QCOMPARE(col->layout()->indexOf(a), -1);
        QCoreApplication::sendPostedEvents(nullptr, QEvent::DeferredDelete);
        delete col;
    }

    // Grid places widgets by (row, col) with spans.
    void testGrid() {
        auto *grid = new Grid();
        auto *a = new QLabel("a");
        auto *b = new QLabel("b");
        grid->spacing(4)->margins(2)->push(a, 0, 0)->push(b, 0, 1, 1, 2);
        QCOMPARE(grid->layout()->spacing(), 4);
        QCOMPARE(grid->layout()->itemAtPosition(0, 0)->widget(), a);
        QCOMPARE(grid->layout()->itemAtPosition(0, 1)->widget(), b);
        // colSpan 2 means (0,2) is covered by b as well.
        QCOMPARE(grid->layout()->itemAtPosition(0, 2)->widget(), b);
        delete grid;
    }

    // Grid::into() hosts the grid inside a target widget.
    void testGridInto() {
        auto *target = new QWidget();
        auto *grid = new Grid();
        grid->push(new QLabel("x"), 0, 0);
        grid->into(target);
        QVERIFY(target->layout() != nullptr);
        QVERIFY(grid->parent() == target);
        delete target;
    }
};

QTEST_MAIN(TestLayoutApi)
#include "test_layout_api.moc"
