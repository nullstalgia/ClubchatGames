#include "showvotes.h"
#include "ui_showvotes.h"
#include <QColorDialog>
//#include <QMessageBox>
ShowVotes::ShowVotes(QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::showvotes)
{
    ui->setupUi(this);
    //this->setStyleSheet("background-color: blue;");
    QList<QLabel*> uiLabels = {
        ui->labelA,
        ui->labelB,
        ui->labelX,
        ui->labelY,
        ui->labelUp,
        ui->labelDown,
        ui->labelLeft,
        ui->labelRight
    };
    QList<QStringList> commandStrings = {
        _inpEmu._pressACommands,
        _inpEmu._pressBCommands,
        _inpEmu._pressXCommands,
        _inpEmu._pressYCommands,
        _inpEmu._moveUpCommands,
        _inpEmu._moveDownCommands,
        _inpEmu._moveLeftCommands,
        _inpEmu._moveRightCommands
    };

    for (int i = 0; i < uiLabels.count(); i++) {
        //int count = commandStrings[i].count();
        uiLabels[i]->setText(commandStrings[i].join(", "));
    }
    _timerForEndPause.setSingleShot(true);
    connect(&_timerForEndPause, SIGNAL(timeout()), this, SLOT(unpauseVotes()));
    _votesVisuallyPaused = false;
    _currentVotes.resize(8);
    _currentVotes.fill(0);
    _backgroundColor = QColor(1, 1, 1);
    _textColor = QColor(1, 1, 1);
}

ShowVotes::~ShowVotes()
{
    _timerForEndPause.stop();
    delete ui;
    emit showVotesDeletedSignal();
}

void ShowVotes::recieveVoteCount(QVector<int> votes)
{
    _currentVotes = QVector<int>(votes);
    if (_votesVisuallyPaused == false) {
        int totalVotes = 0;
        for (int voteCount : _currentVotes) {
            totalVotes += voteCount;
        }
        if (totalVotes == 0) {
            totalVotes++;
        }
        QList<QProgressBar*> uiBars = {
            ui->votesA,
            ui->votesB,
            ui->votesX,
            ui->votesY,
            ui->votesUp,
            ui->votesDown,
            ui->votesLeft,
            ui->votesRight
        };
        for (int i = 0; i < uiBars.count(); i++) {
            uiBars[i]->setMaximum(totalVotes);
            uiBars[i]->setValue(_currentVotes.value(i, 0));
        }
    }
}

void ShowVotes::recieveTimerTick(int timeRemaining, int totalTime, bool notVotes)
{
    ui->timerBar->setMaximum(totalTime);
    ui->timerBar->setValue(timeRemaining);
    if (timeRemaining == 0) {
        _votesVisuallyPaused = true;
        _notVotes = notVotes;
        if (!_notVotes) {
            _timerForEndPause.start(1000);
        } else {
            _timerForEndPause.start(300);
        }
        int biggestIndex = 0;
        int lastValue = 0;
        QList<QProgressBar*> uiBars = {
            ui->votesA,
            ui->votesB,
            ui->votesX,
            ui->votesY,
            ui->votesUp,
            ui->votesDown,
            ui->votesLeft,
            ui->votesRight
        };
        for (int i = 0; i < _currentVotes.count(); i++) {
            if (_currentVotes[i] > lastValue) {
                biggestIndex = i;
                lastValue = _currentVotes[i];
            }
            uiBars[i]->setMaximum(1);
            uiBars[i]->setValue(0);
        }
        if (lastValue != 0)
            uiBars[biggestIndex]->setMaximum(0);
    }
}

void ShowVotes::unpauseVotes()
{
    _votesVisuallyPaused = false;
    if (_notVotes) {
        //_currentVotes.clear();
        //_currentVotes.resize(8);
        _currentVotes.fill(0);
    }
    recieveVoteCount(_currentVotes);
}

void ShowVotes::on_spinBox_valueChanged(int arg1)
{
    emit sendNewTimerLength(arg1);
}

void ShowVotes::on_buttonBGColor_clicked()
{
    _backgroundColor = QColorDialog::getColor();
    setColor();
    //this->setStyle();
    //QMessageBox msgBox(this);
    //msgBox.setText();
    //msgBox.exec();
}

void ShowVotes::on_buttonTextColor_clicked()
{
    _textColor = QColorDialog::getColor();
    setColor();
}

void ShowVotes::setColor()
{
    QString newSheet = "";
    if (_textColor != QColor(1, 1, 1)) {
        newSheet += QString("color: %1;").arg(_textColor.name(QColor::HexRgb));
    }
    if (_backgroundColor != QColor(1, 1, 1)) {
        newSheet += QString("background-color: %1;").arg(_backgroundColor.name(QColor::HexRgb));
    }

    this->setStyleSheet(newSheet);
}
