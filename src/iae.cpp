#include "iae.h"
#include <iostream>
#include <QDebug>
#include <utility>
#include <QFileDialog>

class iaeBackend {
public:
    QString m_status; // for example
};

IAE::IAE(QObject *parent) : QObject(parent), iae(std::make_unique<iaeBackend>()) {} //constructor

QString IAE::openFileDialog() {
    return QFileDialog::getOpenFileName(nullptr, "Select File", "", "All Files (*.*)");
}

IAE::~IAE() { //destructor , todo
    std::cout << "hello!";
    //close connections when necessary.
}

QString IAE::status() const { //read
    return iae->m_status;
}

void IAE::setStatus(const QString &status) { //write
    iae->m_status = status;
    qDebug() << "Status changed to:" << status;
    Q_EMIT statusChanged(); //notify
}

