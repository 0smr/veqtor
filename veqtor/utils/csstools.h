#pragma once

#include <QString>
#include <QHash>

#include <regex>

namespace veqtor::utils {
class cssTools {
public:
    cssTools();
    /**
     * @abstract
     * Parse and converts input css style to a key valye hash map
     * @brief cssStyleParser
     * @param style
     * @return a map contains styles as key-value
     */
    static QHash<QString, QString> cssStyleParser(const QString &style);
    static QHash<QString, QString> cssStyleParser(const std::string &style);
};
} // namespace veqtor::utils
