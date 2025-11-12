#pragma once
#include <vector>
#include <algorithm>
#include "Driver.h"
#include "Report.h"

class Repository {
private:
    std::vector<Driver> drivers;
    std::vector<Report> reports;

public:
    void loadDrivers(const std::string& filename);
    void loadReports(const std::string& filename);
    void saveDrivers(const std::string& filename);
    void saveReports(const std::string& filename);

    const std::vector<Driver>& getDrivers() const { return drivers; }
    const std::vector<Report>& getReports() const { return reports; }

    Driver* findDriver(const std::string& name);
    void updateDriver(const Driver& driver);
    void addReport(Report report);
    std::vector<Report> getReportsInRadius(const Location& center, double radius);
    bool validateReport(const std::string& reportDesc, const std::string& validatorName);
};