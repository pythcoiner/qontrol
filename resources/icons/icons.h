#pragma once

#include "minus.h"
#include "plus.h"
#include <qpixmap.h>

// NOLINTNEXTLINE(readability-const-return-type)
inline auto createPixmap(const uchar *buf, uint len) -> const QPixmap {
    QPixmap pixmap;
    pixmap.loadFromData(buf, len);
    return pixmap;
}

const QPixmap MINUS() {
    return createPixmap(minus_png, minus_png_len);
}

const QPixmap PLUS() {
    return createPixmap(plus_png, plus_png_len);
}
