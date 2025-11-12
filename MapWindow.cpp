#include "MapWindow.h"
#include <QPainter>
#include <QVBoxLayout>

MapWindow::MapWindow(Service* service, QWidget* parent)
    : QWidget(parent), service(service)
{
    setWindowTitle("Driver Map");
    setMinimumSize(600, 400);
    setupUI();

    // Register for updates
    service->addObserver([this]() { updateMap(); });
}

void MapWindow::setupUI() {
    // Set a white background
    setAutoFillBackground(true);
    QPalette pal = palette();
    pal.setColor(QPalette::Window, Qt::white);
    setPalette(pal);
}

void MapWindow::paintEvent(QPaintEvent*) {
    QPainter painter(this);

    // Draw border
    painter.setPen(QPen(Qt::black, 2));
    painter.drawRect(10, 10, width() - 20, height() - 20);

    const auto& drivers = service->getDrivers();
    if (drivers.empty()) return;

    // Find bounds
    double minLat = drivers[0].getLocation().latitude;
    double maxLat = minLat;
    double minLon = drivers[0].getLocation().longitude;
    double maxLon = minLon;

    for (const auto& driver : drivers) {
        const auto& loc = driver.getLocation();
        minLat = std::min(minLat, loc.latitude);
        maxLat = std::max(maxLat, loc.latitude);
        minLon = std::min(minLon, loc.longitude);
        maxLon = std::max(maxLon, loc.longitude);
    }

    // Add padding to bounds
    double padLat = (maxLat - minLat) * 0.1;
    double padLon = (maxLon - minLon) * 0.1;
    minLat -= padLat;
    maxLat += padLat;
    minLon -= padLon;
    maxLon += padLon;

    // Draw drivers
    for (const auto& driver : drivers) {
        const auto& loc = driver.getLocation();

        // Scale coordinates to window
        int x = 20 + (loc.longitude - minLon) * (width() - 40) / (maxLon - minLon);
        int y = 20 + (loc.latitude - minLat) * (height() - 40) / (maxLat - minLat);

        // Draw driver point
        switch (driver.getStatus()) {
            case Status::Baby:
                painter.setBrush(QColor("#f8d7da")); break;
            case Status::GrownUp:
                painter.setBrush(QColor("#fff3cd")); break;
            case Status::Knight:
                painter.setBrush(QColor("#d4edda")); break;
        }
        painter.setPen(Qt::black);
        painter.drawEllipse(QPoint(x, y), 5, 5);

        // Draw name
        painter.drawText(x - 20, y - 10, QString::fromStdString(driver.getName()));
    }
}

void MapWindow::updateMap() {
    update();
}