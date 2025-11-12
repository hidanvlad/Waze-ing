#pragma once
#include <QWidget>
#include "Service.h"

class MapWindow : public QWidget {
    Q_OBJECT
public:
    explicit MapWindow(Service* service, QWidget* parent = nullptr);
    void paintEvent(QPaintEvent* event) override;

    public slots:
        void updateMap();

private:
    Service* service;
    void setupUI();
};