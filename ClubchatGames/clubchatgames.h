#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "commonNames.h"
#include "inputemulator.h"
#include "irccontrols.h"
#include "manualcontrol.h"
#include "serialcontroller.h"
#include "showvotes.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class ClubChatGames;
}
QT_END_NAMESPACE

class ClubChatGames : public QMainWindow {
    Q_OBJECT

public:
    ClubChatGames(QWidget* parent = nullptr);
    ~ClubChatGames();

private slots:
    void on_serialRefreshButton_clicked();

    void on_buttonManualOpen_clicked();

    void on_serialConnectButton_clicked();

    void handleSerialStatus(uint8_t status);

    void recieveButtonAction(quint64 action, bool temporary);

    //void on_forceVanillaConnection_clicked();

    void on_IRCConnectButton_clicked();

    void handleIRCStatus(uint8_t status);

    void on_info1on1_clicked();

    void on_infoVote_clicked();

    void on_infoAnarchy_clicked();

    void on_set1on1Person_clicked();

    void on_buttonClearRecent_clicked();

    void recieveNewRecentPerson(QString newPerson);

    void on_showVotesButton_clicked();

    void voteVotesMiddleman(QVector<int> votes);
    void voteTimerMiddleman(int current, int max, bool notVotes);
    void voteRecieveNewTimerLength(int newLength);

    void on_actionQuit_triggered();

    void manControlDeletedSignal();

    void on_comboBox1on1_currentIndexChanged();

    void showVotesDeletedSignal();

signals:
    void sendButtonAction(quint64 action, bool temporary);
    void send1on1Person(QString newPerson);
    void sendNewTimerLength(int newLength);
    void sendVotes(QVector<int> votes);
    void sendCurrentTimer(int current, int max, bool notVotes);
    //void changeIRCMode(uint8_t newMode);

private:
    InputEmulator _inpEmu;
    Ui::ClubChatGames* ui;
    QStringList serialPorts;
    void fillSerialPorts();
    bool test;
    ManualControl* manControl = nullptr;
    bool manControlDeleted = true;
    SerialController* serialController = nullptr;
    IrcControls* ircController = nullptr;
    void createSerial();
    QSignalMapper* signalMapper;
    ShowVotes* voteDisplay = nullptr;
    bool showVotesDeleted = true;
};
#endif // MAINWINDOW_H
