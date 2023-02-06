#include "tools.h"

namespace veqtor::utils {
std::vector<std::string> tools::globalMatch(const std::string &str, const std::regex &reg) {
    std::vector<std::string> matches;
    std::copy_if(std::sregex_token_iterator{str.begin(), str.end(), reg},
                 std::sregex_token_iterator{},
                 std::back_inserter(matches),
                 [](const std::string& str) -> bool { return str.length(); });
    return matches;
}

std::vector<double> tools::stodVec(const std::vector<std::string> &svec) {
    std::vector<double> dvec;
    std::transform(svec.begin(), svec.end(), std::back_inserter(dvec), [](const std::string& str) {
        // TODO:: use a faster method.
        try { return std::stod(str); } catch(...) { return 0.0; };
    });
    return dvec;
}

QVector<double> tools::stodVec(const QVector<QString> &svec) {
    QVector<double> dvec;
    std::transform(svec.begin(), svec.end(), std::back_inserter(dvec), [](const QString& str) {
        return str.toDouble();
    });
    return dvec;
}
}
