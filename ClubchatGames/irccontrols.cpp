#include "irccontrols.h"

#include <IrcCommand>
#include <IrcMessage>

IrcControls::IrcControls(QObject* parent)
    : IrcConnection(parent)
    , _standardOutput(stdout)
{
    connect(this, SIGNAL(privateMessageReceived(IrcPrivateMessage*)), this, SLOT(processMessage(IrcPrivateMessage*)));
    connect(this, SIGNAL(connected()), this, SLOT(handleIRCConnected()));
    connect(this, SIGNAL(connecting()), this, SLOT(handleIRCConnecting()));
    connect(this, SIGNAL(disconnected()), this, SLOT(handleIRCDisconnected()));
    //QObject::connect(this, SIGNAL());
    setHost(IRC_HOST);
    setPort(IRC_PORT);
    setSecure(IRC_SECURE);
    setNickName(IRC_USERNAME);
    setUserName(IRC_USERNAME);
    setRealName(IRC_USERNAME);

    _voteMode = NOTHING_MODE;

    connect(&_voteTimer, SIGNAL(timeout()), this, SLOT(handleVoteTimer()));
    _currentTimeLeft = 15;
    _totalTime = 15;
    _currentVotes.resize(8);
    _currentVotes.fill(0);
    //setPassword(IRC_PASSWORD);
}

IrcControls::~IrcControls()
{
    _voteTimer.stop();
    ircDisconnect();
}

void IrcControls::processMessage(IrcPrivateMessage* message)
{
    if (message->isPrivate()) {
        return;
    } else {
        QString possibleAction = message->content().toLower().simplified();
        _standardOutput << message->nick().append(":") << Qt::endl;
        _standardOutput << possibleAction << Qt::endl;
        emit sendNewRecentPerson(message->nick().toLower());

        quint64 action;
        if (_voteMode == ANARCHY_MODE) {
            action = _extractButtonAction(possibleAction);
            if (action != _inpEmu.NO_INPUT)
                emit sendControllerAction(action);
        } else if (_voteMode == ONE_ON_ONE_MODE) {
            if (message->nick().toLower() == _oneOnOnePerson) {
                action = _extractButtonAction(possibleAction);
                if (action != _inpEmu.NO_INPUT)
                    emit sendControllerAction(action);
            }
        } else if (_voteMode == VOTE_MODE) {
            action = _extractButtonAction(possibleAction);
            //if (action != _inpEmu.NO_INPUT)
            //emit sendControllerAction(action);
        }
    }
}

quint64 IrcControls::_extractButtonAction(QString messageContent)
{
    quint64 action = _inpEmu.NO_INPUT;
    if (_voteMode != VOTE_MODE) {
        //_currentVotes.clear();
        //_currentVotes.resize(8);
        _currentVotes.fill(0);
    }
    //bool vote_mode = (_voteMode == VOTE_MODE);
    // Checking to make sure it's just one word.
    if (messageContent.split(" ").count() == 1) {
        if (_inpEmu._pressACommands.contains(messageContent)) {
            action = (_inpEmu.BTN_A);
            _currentVotes[bA]++;
        } else if (_inpEmu._pressBCommands.contains(messageContent)) {
            action = (_inpEmu.BTN_B);
            _currentVotes[bB]++;
        } else if (_inpEmu._pressXCommands.contains(messageContent)) {
            action = (_inpEmu.BTN_X);
            _currentVotes[bX]++;
        } else if (_inpEmu._pressYCommands.contains(messageContent)) {
            action = (_inpEmu.BTN_Y);
            _currentVotes[bY]++;
        } else if (_inpEmu._moveUpCommands.contains(messageContent)) {
            action = (_inpEmu.DPAD_U);
            _currentVotes[bUP]++;
        } else if (_inpEmu._moveDownCommands.contains(messageContent)) {
            action = (_inpEmu.DPAD_D);
            _currentVotes[bDOWN]++;
        } else if (_inpEmu._moveLeftCommands.contains(messageContent)) {
            action = (_inpEmu.DPAD_L);
            _currentVotes[bLEFT]++;
        } else if (_inpEmu._moveRightCommands.contains(messageContent)) {
            action = (_inpEmu.DPAD_R);
            _currentVotes[bRIGHT]++;
        }
        if (action != _inpEmu.NO_INPUT) {
            emit sendCurrentVotes(_currentVotes);
            if (_voteMode != VOTE_MODE) {
                emit sendTimerInfo(0, 1, true);
            }
        }
    }
    //QList<quint64> actions = { _inpEmu.BTN_A, _inpEmu.BTN_B, _inpEmu.BTN_X, _inpEmu.BTN_Y, _inpEmu.DPAD_U, _inpEmu.DPAD_D, _inpEmu.DPAD_L, _inpEmu.DPAD_R };

    return (action);
}

void IrcControls::ircConnect(QString channel)
{
    //sendCommand(IrcCommand::createQuit());
    ircDisconnect();
    sendCommand(IrcCommand::createJoin(channel.prepend("#")));
    open();
}

void IrcControls::ircDisconnect()
{
    _voteTimer.stop();
    if (status() != Closed) {
        close();
    }
}

void IrcControls::handleIRCDisconnected()
{
    emit reportIRCStatus(NOT_CONNECTED);
}

void IrcControls::handleIRCConnecting()
{
    emit reportIRCStatus(CONNECTING);
}

void IrcControls::handleIRCConnected()
{
    emit reportIRCStatus(CONNECTED_OK);
}

void IrcControls::changeMode(int newMode)
{
    QString newModeString;
    _voteTimer.stop();
    //_currentVotes.clear();
    //_currentVotes.resize(8);
    _currentVotes.fill(0);
    switch (newMode) {
    case ONE_ON_ONE_MODE:
        newModeString = "One on One";
        break;
    case ANARCHY_MODE:
        newModeString = "Anarchy";
        break;
    case VOTE_MODE:
        newModeString = "Vote Mode";
        _voteTimer.start(1000);
        _currentTimeLeft = _totalTime;
        break;
    case NOTHING_MODE:
        newModeString = "Do nothing";
        break;
    default:
        newModeString = "????";
        break;
    }
    _standardOutput << QString("New mode is: %1").arg(newModeString) << Qt::endl;
    _voteMode = newMode;
}

void IrcControls::change1on1Person(QString newPerson)
{
    _standardOutput << QString("New 1-on-1 person is: %1").arg(newPerson) << Qt::endl;
    _oneOnOnePerson = newPerson;
}

void IrcControls::handleVoteTimer()
{
    if (_currentTimeLeft == 0) {
        int biggestIndex = 0;
        int lastValue = 0;
        for (int i = 0; i < _currentVotes.count(); i++) {
            if (_currentVotes[i] > lastValue) {
                biggestIndex = i;
                lastValue = _currentVotes[i];
            }
        }
        if (lastValue != 0) {
            switch (biggestIndex) {
            case bA:
                emit sendControllerAction(_inpEmu.BTN_A);
                break;
            case bB:
                emit sendControllerAction(_inpEmu.BTN_B);
                break;
            case bX:
                emit sendControllerAction(_inpEmu.BTN_X);
                break;
            case bY:
                emit sendControllerAction(_inpEmu.BTN_Y);
                break;

            case bUP:
                emit sendControllerAction(_inpEmu.DPAD_U);
                break;
            case bDOWN:
                emit sendControllerAction(_inpEmu.DPAD_D);
                break;
            case bLEFT:
                emit sendControllerAction(_inpEmu.DPAD_L);
                break;
            case bRIGHT:
                emit sendControllerAction(_inpEmu.DPAD_R);
                break;
            }
        }
        _currentTimeLeft = _totalTime;
        //_currentVotes.clear();
        //_currentVotes.resize(8);
        _currentVotes.fill(0);
    }
    _currentTimeLeft--;
    emit sendCurrentVotes(_currentVotes);
    emit sendTimerInfo(_currentTimeLeft, _totalTime, false);
}

void IrcControls::changeTimerLength(int newLength)
{
    _totalTime = newLength;
    _currentTimeLeft = newLength;
    emit sendTimerInfo(_currentTimeLeft, _totalTime, false);
}
