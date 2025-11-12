#include "Report.h"
#include <sstream>

bool Report::addValidator(const std::string& validatorName) {
    if (validatorName == reporter) return false;

    for (const auto& v : validators)
        if (v == validatorName) return false;

    validators.push_back(validatorName);
    if (validators.size() >= 2) validated = true;
    return true;
}

std::istream& operator>>(std::istream& is, Report& r) {
    std::string line;
    if (std::getline(is, line)) {
        std::stringstream ss(line);
        std::getline(ss, r.description, ',');
        std::getline(ss, r.reporter, ',');

        std::string lat_str, lon_str, val_str;
        std::getline(ss, lat_str, ',');
        std::getline(ss, lon_str, ',');
        std::getline(ss, val_str, ',');

        r.location.latitude = std::stod(lat_str);
        r.location.longitude = std::stod(lon_str);
        r.validated = (val_str == "true");
    }
    return is;
}

std::ostream& operator<<(std::ostream& os, const Report& r) {
    os << r.description << ","
       << r.reporter << ","
       << r.location.latitude << ","
       << r.location.longitude << ","
       << (r.validated ? "true" : "false");
    return os;
}