/*
Write an application which simulates the communication between drivers, as follows:
1. The information about all registered drivers is in a text file.
Each driver has a name (string) a status (baby, grown-up, knight), a current location (latitude and longitude)
and a score. This file is manually created and it is read when the application starts.
2. Another file contains road information (reports). Each Report has a description,
the reporter-the name of the driver who reported it and the exact location (latitude and longitude)and whether it was validated or not.
These are read when the application starts.
3. When the application is launched, a new window is created for each driver, having as title the driver's name.
The driver's current location, score and status are also shown. The window will show all reports for the driver's region of interest
(a radius of 10 units from the driver's current location). The windows will be coloured differently,
according to the driver's status (Hint: function setStyleSheet). (1.25p)
4. Any driver can add a new report, by inputting the description and exact location.
The reporter will automatically be the name of the driver who added the report.
This operation fails if the description is empty, or if the report's location is more than 20 units away from the driver's location).(1p)
5. Drivers can validate reports, only if these were not validated yet.
If a report is validated by 2 drivers other than the reporter, the report becomes valid and the reporter's score increases by 1.
When the score reaches 10, a baby driver becomes grown-up and when the score reaches 15 a driver becomes a knight.
When a user status changes the window colour will change accordingly. (1.25p)
6. Drivers can search reports in a specified radius, by using a QSlider.
Its default value is 10 and as it is moved, the user's window will show all reports within the given radius. (0.75p)
7. A driver can update his/her current location. This is achieved by clicking one of four buttons (north, east, south, west)
and the location's longitude/latitude updates accordingly (with 1 unit).
In this case, the driver's list of reports is also updated according to the value of the QSlider and the new location. (1p)
8. A new window will show the "map", with all wazers.
This is a rectangle and each driver's location and name will be shown within the rectangle. This is shown when the application starts. (1p)
9. When a modification is made by any driver, all other drivers will see it, automatically.
The map will also be updated automatically. (2p)
10. When the application closes, the files containing driver and report will be updated.
*/

#include <QApplication>
#include <QMessageBox>
#include "Service.h"
#include "DriverWindow.h"
#include "MapWindow.h"
#include <memory>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    try {
        // Initialize repository and service
        auto repo = std::make_unique<Repository>();
        repo->loadDrivers("drivers.txt");
        repo->loadReports("reports.txt");
        auto service = std::make_unique<Service>(*repo);

        // Create and show map window
        auto mapWindow = std::make_unique<MapWindow>(service.get());
        mapWindow->show();

        // Create and show driver windows
        std::vector<std::unique_ptr<DriverWindow>> driverWindows;
        for (const auto& driver : service->getDrivers()) {
            auto window = std::make_unique<DriverWindow>(service.get(), driver);
            window->show();
            driverWindows.push_back(std::move(window));
        }

        // Run application
        int result = app.exec();

        // Save data before exit
        service->saveData("drivers.txt", "reports.txt");

        return result;

    } catch (const std::exception& e) {
        QMessageBox::critical(nullptr, "Error",
            QString("Application error: %1").arg(e.what()));
        return 1;
    }
}