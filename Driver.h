#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <sstream>

enum class Status { Baby, GrownUp, Knight };

struct Location {
    double latitude;
    double longitude;
};

class Driver {
private:
    std::string name;
    Status status;
    Location location;
    int score;

public:
    Driver() : name(""), status(Status::Baby), location{0, 0}, score(0) {}

    Driver(const std::string& name, Status status, Location loc, int score)
        : name(name), status(status), location(loc), score(score) {}

    std::string getName() const { return name; }
    Status getStatus() const { return status; }
    Location getLocation() const { return location; }
    int getScore() const { return score; }

    void setStatus(Status newStatus) { status = newStatus; }
    void setLocation(const Location& loc) { location = loc; }
    void setScore(int newScore) { score = newScore; }

    friend std::istream& operator>>(std::istream& is, Driver& d);
    friend std::ostream& operator<<(std::ostream& os, const Driver& d);
};