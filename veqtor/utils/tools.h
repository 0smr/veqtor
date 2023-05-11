#pragma once

#include <QDomDocument>
#include <QString>
#include <QMap>
#include <QList>
#include <QFile>
#include <QUrl>

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
    static QString contentResolver(const QString &data) {
        bool isPath = data.startsWith("file:") || data.startsWith(":/") ||
                      data.startsWith("qrc:") || data.endsWith(".svg");
        /// If data is file address extract it's data.
        if(data.size() < 256 && isPath) {
            QFile file(toValidFilePath(data));

            if(file.open(QFile::ReadOnly)) {
                QString content = file.readAll();
                file.close();
                return content;
            }
        } else if(data.startsWith("data:image/svg+xml,")) {
            return QByteArray::fromPercentEncoding(data.chopped(19).toLatin1());
        } else if(data.startsWith("data:image/svg+xml;base64,")) {
            return QByteArray::fromBase64(data.chopped(26).toLatin1());
        } else if(data.trimmed().startsWith("<svg")) {
            return data;
        }
        return "";
    }
};

template<typename _key, typename _value>
auto tools::filter(const QMap<_key, _value> &map, const QList<_key> &exclude) {
    QMap<_key, _value> out(map);
    for(const auto &key: exclude) out.remove(key);
    return out;
}
}
