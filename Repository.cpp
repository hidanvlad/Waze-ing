#include "Repository.h"
#include <fstream>
#include <cmath>

void Repository::loadDrivers(const std::string& filename) {
    std::ifstream file(filename);
    Driver driver;
    while (file >> driver)
        drivers.push_back(driver);
}

void Repository::loadReports(const std::string& filename) {
    std::ifstream file(filename);
    Report report;
    while (file >> report)
        reports.push_back(report);
}

void Repository::saveDrivers(const std::string& filename) {
    std::ofstream file(filename);
    for (const auto& driver : drivers)
        file << driver << '\n';
}

void Repository::saveReports(const std::string& filename) {
    std::ofstream file(filename);
    for (const auto& report : reports)
        file << report << '\n';
}

Driver* Repository::findDriver(const std::string& name) {
    auto it = std::find_if(drivers.begin(), drivers.end(),
        [&name](const Driver& d) { return d.getName() == name; });
    return it != drivers.end() ? &(*it) : nullptr;
}

void Repository::updateDriver(const Driver& driver) {
    auto it = std::find_if(drivers.begin(), drivers.end(),
        [&](const Driver& d) { return d.getName() == driver.getName(); });
    if (it != drivers.end())
        *it = driver;
}

void Repository::addReport(Report report) {
    reports.push_back(report);
}

std::vector<Report> Repository::getReportsInRadius(const Location& center, double radius) {
    std::vector<Report> result;
    for (const auto& report : reports) {
        Location loc = report.getLocation();
        double dist = std::sqrt(std::pow(loc.latitude - center.latitude, 2) +
                              std::pow(loc.longitude - center.longitude, 2));
        if (dist <= radius)
            result.push_back(report);
    }
    return result;
}

bool Repository::validateReport(const std::string& reportDesc, const std::string& validatorName) {
    for (auto& report : reports) {
        if (report.getDescription() == reportDesc) {
            if (report.addValidator(validatorName)) {
                if (report.isValidated()) {
                    auto* reporter = findDriver(report.getReporter());
                    if (reporter) {
                        int newScore = reporter->getScore() + 1;
                        reporter->setScore(newScore);
                        if (newScore >= 15)
                            reporter->setStatus(Status::Knight);
                        else if (newScore >= 10)
                            reporter->setStatus(Status::GrownUp);
                    }
                }
                return true;
            }
        }
    }
    return false;
}