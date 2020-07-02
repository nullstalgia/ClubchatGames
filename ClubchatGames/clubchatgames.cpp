#include "clubchatgames.h"
#include "ui_clubchatgames.h"
#include <QFile>
#include <QMessageBox>
#include <QRadioButton>
#include <QSerialPortInfo>
#include <QTextStream>
ClubChatGames::ClubChatGames(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::ClubChatGames)
{
    ui->setupUi(this);
    fillSerialPorts();

    QList<QRadioButton*> radioModes = {
        ui->radio1on1,
        ui->radioAnarchy,
        ui->radioVote,
        ui->radioIRCNothing
    };

    signalMapper = new QSignalMapper(this);

    for (int i = 0; i < radioModes.count(); i++) {
        connect(radioModes[i], SIGNAL(clicked()), signalMapper, SLOT(map()));
        signalMapper->setMapping(radioModes[i], i);
    }
    connect(ui->IRCChannelLineEdit, SIGNAL(returnPressed()), this, SLOT(on_IRCConnectButton_clicked()));
}

ClubChatGames::~ClubChatGames()
{
    delete ui;
}

void ClubChatGames::fillSerialPorts()
{
    QList<QSerialPortInfo> serialList = QSerialPortInfo::availablePorts();
    ui->serialPortsBox->clear();
    serialPorts.clear();
    for (const QSerialPortInfo& serialPortInfo : serialList) {
        serialPorts.append(serialPortInfo.systemLocation());
    }
    ui->serialPortsBox->addItems(serialPorts);
}

void ClubChatGames::on_serialRefreshButton_clicked()
{
    fillSerialPorts();
}

void ClubChatGames::manControlDeletedSignal()
{
    manControlDeleted = true;
}

void ClubChatGames::on_buttonManualOpen_clicked()
{
    if (manControlDeleted) {
        manControlDeleted = false;
        manControl = new ManualControl();
        manControl->setAttribute(Qt::WA_DeleteOnClose);
        manControl->show();
        connect(manControl, SIGNAL(buttonAction(quint64, bool)), this, SLOT(recieveButtonAction(quint64, bool)));
        connect(manControl, SIGNAL(manControlDeletedSignal()), this, SLOT(manControlDeletedSignal()));
    }
}

void ClubChatGames::recieveButtonAction(quint64 action, bool temporary)
{
    emit sendButtonAction(action, temporary);
}

void ClubChatGames::handleSerialStatus(uint8_t status)
{
    if (status == CONNECTING) {
        ui->serialStatusLabel->setText("Connecting");
        ui->serialPortsBox->setEnabled(false);
        ui->serialRefreshButton->setEnabled(false);
        //ui->forceVanillaConnection->setEnabled(false);
        ui->serialConnectButton->setText("Disconnect");
    } else if (status == CONNECTED_OK) {
        ui->serialStatusLabel->setText("Connected! (Vanilla)");
        //connect(this, SIGNAL(), serialController, SLOT(recieveButtonAction(quint64, bool)));
    } else if (status == CHOCO_SYNCED_JC_L) {
        ui->serialStatusLabel->setText("Connected as Left JoyCon!");
    } else if (status == CHOCO_SYNCED_JC_R) {
        ui->serialStatusLabel->setText("Connected as Right JoyCon!");
    } else if (status == CHOCO_SYNCED_PRO) {
        ui->serialStatusLabel->setText("Connected as Pro Controller!");
    } else if (status == CONNECTION_FAILED) {
        ui->serialStatusLabel->setText("FAIL");
    } else {
        ui->serialStatusLabel->setText("Disconnected");
        ui->serialPortsBox->setEnabled(true);
        ui->serialRefreshButton->setEnabled(true);
        ui->serialConnectButton->setText("Connect");
        ui->serialConnectButton->setEnabled(true);
        //ui->forceVanillaConnection->setEnabled(true);
    }
}

void ClubChatGames::createSerial()
{
    if (ui->serialPortsBox->currentText().isEmpty() == false) {
        if (!ui->serialPortsBox->isEnabled()) {
            ui->serialConnectButton->setEnabled(false);
            //ui->forceVanillaConnection->setEnabled(false);
            serialController->deleteLater();
        } else {
            serialController = new SerialController(this);
            connect(serialController, SIGNAL(reportSerialStatus(uint8_t)), this, SLOT(handleSerialStatus(uint8_t)));
            serialController->openAndSync(ui->serialPortsBox->currentText());
        }
    }
}

void ClubChatGames::on_serialConnectButton_clicked()
{
    createSerial();
}

//void ClubChatGames::on_forceVanillaConnection_clicked()
//{
//createSerial(true);
//}

void ClubChatGames::on_IRCConnectButton_clicked()
{
    if (ui->IRCChannelLineEdit->text().isEmpty() == false) {
        if (!ui->IRCChannelLineEdit->isEnabled()) {
            ui->IRCConnectButton->setEnabled(false);
            //ircController->ircDisconnect();
            ircController->deleteLater();
        } else {
            ircController = new IrcControls(this);
            connect(ircController, SIGNAL(reportIRCStatus(uint8_t)), this, SLOT(handleIRCStatus(uint8_t)));
            connect(ircController, SIGNAL(sendControllerAction(quint64, bool)), this, SLOT(recieveButtonAction(quint64, bool)));
            connect(ircController, SIGNAL(sendNewRecentPerson(QString)), this, SLOT(recieveNewRecentPerson(QString)));
            connect(signalMapper, SIGNAL(mappedInt(int)), ircController, SLOT(changeMode(int)));
            connect(this, SIGNAL(send1on1Person(QString)), ircController, SLOT(change1on1Person(QString)));

            connect(this, SIGNAL(sendNewTimerLength(int)), ircController, SLOT(changeTimerLength(int)));
            connect(ircController, SIGNAL(sendCurrentVotes(QVector<int>)), this, SLOT(voteVotesMiddleman(QVector<int>)));
            connect(ircController, SIGNAL(sendTimerInfo(int, int, bool)), this, SLOT(voteTimerMiddleman(int, int, bool)));

            ircController->ircConnect(ui->IRCChannelLineEdit->text());
        }
    }
}

void ClubChatGames::handleIRCStatus(uint8_t status)
{
    if (status == CONNECTING) {
        ui->IRCStatusLabel->setText("Connecting");
        ui->IRCChannelLineEdit->setEnabled(false);
        ui->IRCConnectButton->setText("Disconnect");
    } else if (status == CONNECTED_OK) {
        ui->IRCStatusLabel->setText("Connected!");
    } else {
        ui->IRCStatusLabel->setText("Disconnected");
        ui->IRCChannelLineEdit->setEnabled(true);
        ui->IRCConnectButton->setText("Connect");
        ui->IRCConnectButton->setEnabled(true);
        //ui->forceVanillaConnection->setEnabled(true);
    }
}

void ClubChatGames::on_info1on1_clicked()
{
    QMessageBox msgBox(this);
    msgBox.setText("A person you choose will have instant access to the controls, via the same vote commands.");
    msgBox.exec();
}

void ClubChatGames::on_infoVote_clicked()
{
    QMessageBox msgBox(this);
    msgBox.setText("The whole chat will have a number of seconds to vote on an action.");
    msgBox.exec();
}

void ClubChatGames::on_infoAnarchy_clicked()
{
    QMessageBox msgBox(this);
    msgBox.setText("The whole chat will have instant access to the controls.");
    msgBox.exec();
}

void ClubChatGames::on_set1on1Person_clicked()
{
    QString newPerson = ui->comboBox1on1->currentText().toLower();
    if (newPerson.isEmpty() == false) {
        ui->current1on1->setText(newPerson);
        recieveNewRecentPerson(newPerson);
    } else {
        ui->current1on1->setText("N/A");
    }
    emit send1on1Person(newPerson);
}

void ClubChatGames::on_buttonClearRecent_clicked()
{
    ui->comboBox1on1->clear();
    ui->comboBox1on1->clearEditText();
    on_set1on1Person_clicked();
}

void ClubChatGames::recieveNewRecentPerson(QString newPerson)
{
    if (ui->comboBox1on1->findText(newPerson) == -1)
        ui->comboBox1on1->addItem(newPerson);
}

void ClubChatGames::showVotesDeletedSignal()
{
    showVotesDeleted = true;
}

void ClubChatGames::on_showVotesButton_clicked()
{
    if (showVotesDeleted) {
        showVotesDeleted = false;
        voteDisplay = new ShowVotes();
        voteDisplay->setAttribute(Qt::WA_DeleteOnClose);
        voteDisplay->show();
        connect(voteDisplay, SIGNAL(showVotesDeletedSignal()), this, SLOT(showVotesDeletedSignal()));
        connect(voteDisplay, SIGNAL(sendNewTimerLength(int)), this, SLOT(voteRecieveNewTimerLength(int)));
        connect(this, SIGNAL(sendVotes(QVector<int>)), voteDisplay, SLOT(recieveVoteCount(QVector<int>)));
        connect(this, SIGNAL(sendCurrentTimer(int, int, bool)), voteDisplay, SLOT(recieveTimerTick(int, int, bool)));
    }
}

void ClubChatGames::voteVotesMiddleman(QVector<int> votes)
{
    emit sendVotes(votes);
}
void ClubChatGames::voteTimerMiddleman(int current, int max, bool notVotes)
{
    emit sendCurrentTimer(current, max, notVotes);
}
void ClubChatGames::voteRecieveNewTimerLength(int newLength)
{
    emit sendNewTimerLength(newLength);
}
void ClubChatGames::on_actionQuit_triggered()
{
    this->close();
}
void ClubChatGames::on_comboBox1on1_currentIndexChanged()
{
    on_set1on1Person_clicked();
}
