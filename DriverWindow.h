#pragma once
#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QLineEdit>
#include "Service.h"

class DriverWindow : public QWidget {
    Q_OBJECT

public:
    DriverWindow(Service* service, const Driver& driver, QWidget* parent = nullptr);
    void updateAll();

    signals:
        void dataChanged();

private:
    Service* service;
    Driver currentDriver;
    double currentRadius;

    QLabel* nameLabel;
    QLabel* statusLabel;
    QLabel* scoreLabel;
    QLabel* locationLabel;
    QListWidget* reportList;
    QSlider* radiusSlider;
    QLineEdit* descEdit;
    QLineEdit* latEdit;
    QLineEdit* longEdit;
    QPushButton* addReportBtn;
    QPushButton* validateBtn;
    QPushButton* northBtn;
    QPushButton* southBtn;
    QPushButton* eastBtn;
    QPushButton* westBtn;

    void setupUI();
    void updateDriverInfo();
    void updateReportList();
    void updateWindowColor();

    private slots:
        void onRadiusChanged(int value);
    void onAddReport();
    void onValidateReport();
    void onMoveNorth();
    void onMoveSouth();
    void onMoveEast();
    void onMoveWest();
};