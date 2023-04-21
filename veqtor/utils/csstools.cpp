#include "csstools.h"

#include <QRegularExpression>

namespace veqtor::utils {
cssTools::cssTools() {}

QHash<QString, QString> cssTools::cssStyleParser(const QString &style) {
    QHash<QString, QString> map;

    static QRegularExpression regex(R"(([a-zA-Z0-9_-]+)\s*:\s*(.*?)\s*(;|$))");
    auto matchList = regex.globalMatch(style);
    while(matchList.hasNext()) {
        auto match = matchList.next();
        map[match.captured(1)] = match.captured(2);
    }

    return map;
}

QHash<QString, QString> cssTools::cssStyleParser(const std::string &style) {
    return cssStyleParser(QString::fromStdString(style));
}
}
