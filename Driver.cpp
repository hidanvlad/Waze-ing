#include "Driver.h"

std::istream& operator>>(std::istream& is, Driver& d) {
    std::string line;
    if (std::getline(is, line)) {
        std::stringstream ss(line);
        std::string status_str;
        std::string::size_type sz;

        std::getline(ss, d.name, ',');
        std::getline(ss, status_str, ',');

        if (status_str == "baby") d.status = Status::Baby;
        else if (status_str == "grown-up") d.status = Status::GrownUp;
        else d.status = Status::Knight;

        std::string lat_str, lon_str, score_str;
        std::getline(ss, lat_str, ',');
        std::getline(ss, lon_str, ',');
        std::getline(ss, score_str, ',');

        d.location.latitude = std::stod(lat_str);
        d.location.longitude = std::stod(lon_str);
        d.score = std::stoi(score_str);
    }
    return is;
}

std::ostream& operator<<(std::ostream& os, const Driver& d) {
    os << d.name << ","
       << (d.status == Status::Baby ? "baby" :
           d.status == Status::GrownUp ? "grown-up" : "knight") << ","
       << d.location.latitude << ","
       << d.location.longitude << ","
       << d.score;
    return os;
}