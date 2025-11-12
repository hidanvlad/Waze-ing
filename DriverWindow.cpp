#include "DriverWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <cmath>

DriverWindow::DriverWindow(Service* service, const Driver& driver, QWidget* parent)
    : QWidget(parent), service(service), currentDriver(driver), currentRadius(10.0)
{
    setupUI();
    service->addObserver([this]() { updateAll(); });
    updateDriverInfo();
    updateReportList();
}

void DriverWindow::setupUI() {
    setWindowTitle(QString::fromStdString(currentDriver.getName()));
    nameLabel = new QLabel(this);
    statusLabel = new QLabel(this);
    scoreLabel = new QLabel(this);
    locationLabel = new QLabel(this);

    reportList = new QListWidget(this);

    radiusSlider = new QSlider(Qt::Horizontal, this);
    radiusSlider->setRange(1, 50);
    radiusSlider->setValue(10);

    descEdit = new QLineEdit(this);
    latEdit = new QLineEdit(this);
    longEdit = new QLineEdit(this);
    addReportBtn = new QPushButton("Add Report", this);
    validateBtn = new QPushButton("Validate Report", this);

    northBtn = new QPushButton("North", this);
    southBtn = new QPushButton("South", this);
    eastBtn = new QPushButton("East", this);
    westBtn = new QPushButton("West", this);

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(nameLabel);
    mainLayout->addWidget(statusLabel);
    mainLayout->addWidget(scoreLabel);
    mainLayout->addWidget(locationLabel);
    mainLayout->addWidget(new QLabel("Reports in radius:"));
    mainLayout->addWidget(reportList);

    QHBoxLayout* sliderLayout = new QHBoxLayout;
    sliderLayout->addWidget(new QLabel("Radius:"));
    sliderLayout->addWidget(radiusSlider);
    mainLayout->addLayout(sliderLayout);

    QHBoxLayout* addLayout = new QHBoxLayout;
    addLayout->addWidget(new QLabel("Description:"));
    addLayout->addWidget(descEdit);
    addLayout->addWidget(new QLabel("Latitude:"));
    addLayout->addWidget(latEdit);
    addLayout->addWidget(new QLabel("Longitude:"));
    addLayout->addWidget(longEdit);
    addLayout->addWidget(addReportBtn);
    mainLayout->addLayout(addLayout);

    mainLayout->addWidget(validateBtn);

    QHBoxLayout* moveLayout = new QHBoxLayout;
    moveLayout->addWidget(northBtn);
    moveLayout->addWidget(southBtn);
    moveLayout->addWidget(eastBtn);
    moveLayout->addWidget(westBtn);
    mainLayout->addLayout(moveLayout);

    setLayout(mainLayout);

    connect(radiusSlider, &QSlider::valueChanged, this, &DriverWindow::onRadiusChanged);
    connect(addReportBtn, &QPushButton::clicked, this, &DriverWindow::onAddReport);
    connect(validateBtn, &QPushButton::clicked, this, &DriverWindow::onValidateReport);
    connect(northBtn, &QPushButton::clicked, this, &DriverWindow::onMoveNorth);
    connect(southBtn, &QPushButton::clicked, this, &DriverWindow::onMoveSouth);
    connect(eastBtn, &QPushButton::clicked, this, &DriverWindow::onMoveEast);
    connect(westBtn, &QPushButton::clicked, this, &DriverWindow::onMoveWest);

    updateWindowColor();
}

void DriverWindow::updateWindowColor() {
    switch (currentDriver.getStatus()) {
        case Status::Baby:
            setStyleSheet("background-color: #f8d7da;");
            break;
        case Status::GrownUp:
            setStyleSheet("background-color: #fff3cd;");
            break;
        case Status::Knight:
            setStyleSheet("background-color: #d4edda;");
            break;
    }
}

void DriverWindow::updateDriverInfo() {
    nameLabel->setText("Name: " + QString::fromStdString(currentDriver.getName()));
    statusLabel->setText("Status: " + QString::fromStdString(
        currentDriver.getStatus() == Status::Baby ? "Baby" :
        currentDriver.getStatus() == Status::GrownUp ? "GrownUp" : "Knight"));
    scoreLabel->setText("Score: " + QString::number(currentDriver.getScore()));

    const auto& loc = currentDriver.getLocation();
    locationLabel->setText(QString("Location: (%1, %2)")
        .arg(loc.latitude)
        .arg(loc.longitude));

    updateWindowColor();
}

void DriverWindow::updateReportList() {
    reportList->clear();
    auto reports = service->getReportsInRadius(currentDriver.getLocation(), currentRadius);
    for (const auto& report : reports) {
        const auto& loc = report.getLocation();
        QString item = QString("%1 [%2] (%3, %4)%5")
            .arg(QString::fromStdString(report.getDescription()))
            .arg(QString::fromStdString(report.getReporter()))
            .arg(loc.latitude)
            .arg(loc.longitude)
            .arg(report.isValidated() ? " [VALID]" : "");
        reportList->addItem(item);
    }
}

void DriverWindow::updateAll() {
    auto* driver = service->findDriver(currentDriver.getName());
    if (driver) {
        currentDriver = *driver;
        updateDriverInfo();
        updateReportList();
    }
}

void DriverWindow::onRadiusChanged(int value) {
    currentRadius = value;
    updateReportList();
}

void DriverWindow::onAddReport() {
    std::string desc = descEdit->text().toStdString();
    if (desc.empty()) {
        QMessageBox::warning(this, "Error", "Description cannot be empty.");
        return;
    }

    bool okLat, okLon;
    double lat = latEdit->text().toDouble(&okLat);
    double lon = longEdit->text().toDouble(&okLon);

    if (!okLat || !okLon) {
        QMessageBox::warning(this, "Error", "Invalid coordinates.");
        return;
    }

    Location reportLoc{lat, lon};
    if (!service->addReport(desc, currentDriver.getName(), reportLoc)) {
        QMessageBox::warning(this, "Error", "Report location too far or invalid data.");
        return;
    }

    descEdit->clear();
    latEdit->clear();
    longEdit->clear();
    emit dataChanged();
}

void DriverWindow::onValidateReport() {
    int idx = reportList->currentRow();
    if (idx < 0) {
        QMessageBox::warning(this, "Error", "Please select a report to validate.");
        return;
    }

    auto reports = service->getReportsInRadius(currentDriver.getLocation(), currentRadius);
    if (idx >= reports.size()) return;

    if (service->validateReport(reports[idx].getDescription(), currentDriver.getName())) {
        emit dataChanged();
    }
}

void DriverWindow::onMoveNorth() {
    if (service->updateDriverLocation(currentDriver.getName(), 1, 0)) {
        emit dataChanged();
    }
}

void DriverWindow::onMoveSouth() {
    if (service->updateDriverLocation(currentDriver.getName(), -1, 0)) {
        emit dataChanged();
    }
}

void DriverWindow::onMoveEast() {
    if (service->updateDriverLocation(currentDriver.getName(), 0, 1)) {
        emit dataChanged();
    }
}

void DriverWindow::onMoveWest() {
    if (service->updateDriverLocation(currentDriver.getName(), 0, -1)) {
        emit dataChanged();
    }
}