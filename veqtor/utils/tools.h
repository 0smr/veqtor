#pragma once

#include <QDomDocument>
#include <QString>
#include <QMap>
#include <QList>

#include <regex>
#include <string>

namespace veqtor::utils {
class tools {
public:
    template<typename _key, typename _value>
    inline static auto filter(const QMap<_key, _value> &map, const QList<_key> &exclude = {});

    static std::vector<std::string> globalMatch(const std::string &str, const std::regex &reg);
    static std::vector<double> stodVec(const std::vector<std::string> &svec);
    static QVector<double> stodVec(const QVector<QString> &svec);
    static QString toValidFilePath(const QString &path);
};

template<typename _key, typename _value>
auto tools::filter(const QMap<_key, _value> &map, const QList<_key> &exclude) {
    QMap<_key, _value> out(map);
    for(const auto &key: exclude) out.remove(key);
    return out;
}
}
