#include "Service.h"
#include <cmath>

bool Service::addReport(const std::string& desc, const std::string& reporter,
                       const Location& loc) {
    if (desc.empty()) return false;
    
    auto* driver = repo.findDriver(reporter);
    if (!driver) return false;

    double dist = std::sqrt(std::pow(loc.latitude - driver->getLocation().latitude, 2) +
                          std::pow(loc.longitude - driver->getLocation().longitude, 2));
    if (dist > 20.0) return false;

    repo.addReport(Report(desc, reporter, loc));
    notifyObservers();
    return true;
}

bool Service::validateReport(const std::string& reportDesc, const std::string& validatorName) {
    bool result = repo.validateReport(reportDesc, validatorName);
    if (result) notifyObservers();
    return result;
}

std::vector<Report> Service::getReportsInRadius(const Location& center, double radius) {
    return repo.getReportsInRadius(center, radius);
}

bool Service::updateDriverLocation(const std::string& name, double dLat, double dLon) {
    auto* driver = repo.findDriver(name);
    if (!driver) return false;

    Location newLoc = driver->getLocation();
    newLoc.latitude += dLat;
    newLoc.longitude += dLon;
    
    Driver updatedDriver = *driver;
    updatedDriver.setLocation(newLoc);
    repo.updateDriver(updatedDriver);
    
    notifyObservers();
    return true;
}

void Service::loadData(const std::string& driversFile, const std::string& reportsFile) {
    repo.loadDrivers(driversFile);
    repo.loadReports(reportsFile);
}

void Service::saveData(const std::string& driversFile, const std::string& reportsFile) {
    repo.saveDrivers(driversFile);
    repo.saveReports(reportsFile);
}