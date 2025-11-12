#pragma once
#include <string>
#include <vector>
#include "Driver.h"

class Report {
private:
    std::string description;
    std::string reporter;
    Location location;
    bool validated;
    std::vector<std::string> validators;

public:
    Report() : description(""), reporter(""), location{0, 0}, validated(false) {}

    Report(const std::string& desc, const std::string& rep, Location loc, bool val = false)
        : description(desc), reporter(rep), location(loc), validated(val) {}

    std::string getDescription() const { return description; }
    std::string getReporter() const { return reporter; }
    Location getLocation() const { return location; }
    bool isValidated() const { return validated; }
    const std::vector<std::string>& getValidators() const { return validators; }

    void setValidated(bool val) { validated = val; }
    bool addValidator(const std::string& validatorName);

    friend std::istream& operator>>(std::istream& is, Report& r);
    friend std::ostream& operator<<(std::ostream& os, const Report& r);
};