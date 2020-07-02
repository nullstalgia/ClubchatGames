#ifndef SHOWVOTES_H
#define SHOWVOTES_H

#include "inputemulator.h"
#include <QDialog>
#include <QTimer>
namespace Ui {
class showvotes;
}

class ShowVotes : public QDialog {
    Q_OBJECT

public:
    explicit ShowVotes(QWidget* parent = nullptr);
    ~ShowVotes();

public slots:
    void recieveVoteCount(QVector<int> votes);
    void recieveTimerTick(int timeRemaining, int totalTime, bool notVotes);
    void unpauseVotes();

signals:
    void sendNewTimerLength(int);
    void showVotesDeletedSignal();

private slots:
    void on_spinBox_valueChanged(int arg1);

    void on_buttonBGColor_clicked();

    void on_buttonTextColor_clicked();

private:
    InputEmulator _inpEmu;
    Ui::showvotes* ui;
    QVector<int> _currentVotes;
    QTimer _timerForEndPause;
    bool _votesVisuallyPaused;
    QColor _backgroundColor;
    QColor _textColor;
    void setColor();
    bool _notVotes;
    //int _voteTimerLength;
};

#endif // SHOWVOTES_H
