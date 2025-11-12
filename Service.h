#pragma once
#include "Repository.h"
#include <functional>

class Service {
private:
    Repository& repo;
    std::vector<std::function<void()>> observers;

public:
    explicit Service(Repository& repo) : repo(repo) {}

    void addObserver(std::function<void()> observer) {
        observers.push_back(observer);
    }

    void notifyObservers() {
        for (auto& obs : observers)
            obs();
    }

    const std::vector<Driver>& getDrivers() const { return repo.getDrivers(); }
    Driver* findDriver(const std::string& name) { return repo.findDriver(name); }

    bool addReport(const std::string& desc, const std::string& reporter,const Location& loc);
    bool validateReport(const std::string& reportDesc, const std::string& validatorName);
    std::vector<Report> getReportsInRadius(const Location& center, double radius);
    bool updateDriverLocation(const std::string& name, double dLat, double dLon);

    void loadData(const std::string& driversFile, const std::string& reportsFile);
    void saveData(const std::string& driversFile, const std::string& reportsFile);
};