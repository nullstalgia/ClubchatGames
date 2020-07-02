#ifndef MANUALCONTROL_H
#define MANUALCONTROL_H
#include "commonNames.h"
#include "inputemulator.h"
#include <QDialog>
#include <QPushButton>
#include <QSignalMapper>
namespace Ui {
class ManualControl;
}

class ManualControl : public QDialog {
    Q_OBJECT

public:
    explicit ManualControl(QWidget* parent = nullptr);
    ~ManualControl();

    //private slots:
signals:
    void buttonAction(quint64 action, bool temporary);
    void buttonRelease();
    void manControlDeletedSignal();

private slots:
    void on_buttonReleased();

    void on_buttonPressed(const QString& text);

private:
    InputEmulator _inpEmu;
    Ui::ManualControl* ui;
    QSignalMapper* signalMapper;
};

#endif // MANUALCONTROL_H
