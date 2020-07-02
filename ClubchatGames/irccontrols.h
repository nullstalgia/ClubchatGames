#ifndef IRCCONTROLS_H
#define IRCCONTROLS_H

#include "commonNames.h"
#include "inputemulator.h"
#include <IrcConnection>
#include <QTimer>

class IrcControls : public IrcConnection {
    Q_OBJECT

public:
    IrcControls(QObject* parent = nullptr);
    ~IrcControls();

public slots:
    void ircConnect(QString channel);
    void ircDisconnect();
    void changeMode(int newMode);
    void change1on1Person(QString newPerson);
    void changeTimerLength(int newLength);

private slots:
    void processMessage(IrcPrivateMessage* message);
    void handleIRCConnecting();
    void handleIRCDisconnected();
    void handleIRCConnected();
    void handleVoteTimer();

signals:
    void sendControllerAction(quint64 button, bool temporary = TEMPORARY);
    void reportIRCStatus(uint8_t status);
    void sendNewRecentPerson(QString newPerson);
    void sendTimerInfo(int currentTime, int totalTime, bool notVotes);
    void sendCurrentVotes(QVector<int> votes);

private:
    InputEmulator _inpEmu;
    uint8_t _voteMode;
    QString _oneOnOnePerson;
    QTextStream _standardOutput;
    QVector<int> _currentVotes;
    QTimer _voteTimer;
    int _currentTimeLeft;
    int _totalTime;

    quint64 _extractButtonAction(QString messageContent);
};

#endif // IRCCONTROLS_H
