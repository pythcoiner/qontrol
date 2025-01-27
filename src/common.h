#pragma once

#include <QJsonObject>

namespace qontrol {

const int QONTROL_MARGIN =30;
const int QONTROL_LEFT_MARGIN = QONTROL_MARGIN;
const int QONTROL_RIGHT_MARGIN = QONTROL_MARGIN;
const int QONTROL_TOP_MARGIN = QONTROL_MARGIN;
const int QONTROL_BOTTOM_MARGIN = QONTROL_MARGIN;

const int QONTROL_LABEL_WIDTH = 120;
const int QONTROL_INPUT_WIDTH = 200;
const int QONTROL_UNIT_WIDTH = 30;

const int QONTROL_V_SPACER = 10;
const int QONTROL_H_SPACER = 5;

using Json = std::unique_ptr<QJsonObject>;
using SharedJson = std::shared_ptr<const QJsonObject>;

inline auto newJson() -> Json {
    return std::make_unique<QJsonObject>();
};
inline auto newJson(const QJsonObject& obj) -> Json {
    return std::make_unique<QJsonObject>(obj);
};
inline auto newJson(const SharedJson& ptr) -> Json {
    return std::make_unique<QJsonObject>(*ptr);
};

inline auto newSharedJson() -> SharedJson {
    return std::make_shared<QJsonObject>();
};
inline auto newSharedJson(QJsonObject obj) -> SharedJson {
    return std::make_shared<QJsonObject>(obj);
};
inline auto newJson(Json ptr) -> SharedJson {
    return std::make_unique<QJsonObject>(std::move(*ptr));
};

const Qt::ConnectionType UNIQUE = Qt::ConnectionType::UniqueConnection;

} // namespace qontrol
