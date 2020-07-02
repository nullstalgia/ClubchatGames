#include "serialcontroller.h"
#include "inputemulator.h"

SerialController::SerialController(QObject* parent)
    : QObject(parent)
    , _standardOutput(stdout)
{
    /*
    if (forceVanilla) {
        _connectionTimeoutState = 2;
    } else {
        _connectionTimeoutState = 4;
    }*/
    _syncState = NOT_SYNCED;
    _connectionTimeoutState = COUNTS_FOR_VANILLA;
    connect(&_readTimeoutTimer, SIGNAL(timeout()), this, SLOT(handleReadTimeout()));
    connect(&_writeTimeoutTimer, SIGNAL(timeout()), this, SLOT(handleWriteTimeout()));
    connect(&_connectionTimeoutTimer, SIGNAL(timeout()), this, SLOT(handleConnectionTimeout()));
    connect(&_temporaryButtonTimer, SIGNAL(timeout()), this, SLOT(temporaryButtonTimeout()));
    _writeTimeoutTimer.setSingleShot(true);
    _readTimeoutTimer.setSingleShot(true);
    _connectionTimeoutTimer.setSingleShot(true);
    _temporaryButtonTimer.setSingleShot(true);
    connect(this->parent(), SIGNAL(sendButtonAction(quint64, bool)), this, SLOT(recieveButtonAction(quint64, bool)));
}

SerialController::~SerialController()
{
    close();
    emit reportSerialStatus(NOT_CONNECTED);
}

void SerialController::_write(const QByteArray& writeData)
{
    _writeData = writeData;
    _serialPort.clear();
    const qint64 bytesWritten = _serialPort.write(writeData);

    if (bytesWritten == -1) {
        _standardOutput << QObject::tr("Failed to write the data to port %1, error: %2")
                               .arg(_serialPort.portName())
                               .arg(_serialPort.errorString())
                        << Qt::endl;
        emit reportSerialStatus(M_ERROR);
    } else if (bytesWritten != _writeData.size()) {
        _standardOutput << QObject::tr("Failed to write all the data to port %1, error: %2")
                               .arg(_serialPort.portName())
                               .arg(_serialPort.errorString())
                        << Qt::endl;
        // emit reportSerialStatus(M_ERROR);
    }
    _writeTimeoutTimer.start(5000);
}

void SerialController::handleBytesWritten(qint64 bytes)
{
    _writeTimeoutTimer.stop();
    _bytesWritten += bytes;
    if (_bytesWritten == _writeData.size()) {
        _bytesWritten = 0;
        _standardOutput << QObject::tr("Sent to port OK: %1 - ")
                               .arg(_serialPort.portName());
        for (char c : _writeData) {
            _standardOutput << QString("0x%1 ").arg((uint8_t)c, 1, 16);
        }
        _standardOutput << Qt::endl;
    }
}

void SerialController::handleWriteTimeout()
{
    _standardOutput << QObject::tr("Operation timed out for port %1, error: %2")
                           .arg(_serialPort.portName())
                           .arg(_serialPort.errorString())
                    << Qt::endl;
}

void SerialController::close()
{
    _readTimeoutTimer.stop();
    _writeTimeoutTimer.stop();
    _connectionTimeoutTimer.stop();
    if (true) {
        //_serialPort.clear();
        //_serialPort.clearError();
        if (opened)
            _serialPort.close();
    }
    opened = false;
    //emit reportSerialStatus(NOT_CONNECTED);
}

void SerialController::handleError(QSerialPort::SerialPortError serialPortError)
{
    if (
        /*serialPortError == QSerialPort::WriteError
        || serialPortError == QSerialPort::OpenError
        || serialPortError == QSerialPort::ReadError
        || serialPortError == QSerialPort::PermissionError
        || serialPortError == QSerialPort::NotOpenError
        || serialPortError == QSerialPort::TimeoutError
        || serialPortError == QSerialPort::DeviceNotFoundError
        */
        serialPortError != QSerialPort::NoError) {
        _standardOutput << QObject::tr("An I/O error occurred with port %1, error: %2")
                               .arg(_serialPort.portName())
                               .arg(_serialPort.errorString())
                        << Qt::endl;
        emit reportSerialStatus(CONNECTION_FAILED);
        close();
    }
}

void SerialController::handleReadyRead()
{
    if (_syncState == SYNCING || _syncState == SYNC_STATE_0 || _syncState == SYNC_STATE_1 || _syncState == SYNC_STATE_2 || _syncState == SYNC_SUCCESS || _syncState == CHOCO_SYNC_STATE_0 || _syncState == CHOCO_SYNC_STATE_1 || _syncState == CHOCO_SYNC_STATE_2) {
        _readData.append(_serialPort.readAll());
        if (!_readTimeoutTimer.isActive())
            _readTimeoutTimer.start(75);
    } else {
        _serialPort.clear();
    }
}

void SerialController::handleConnectionTimeout()
{
    QByteArray data;
    data.resize(1);
    data[0] = 0xFF;
    if (true) {
        if (_connectionTimeoutState >= 1) {
            _connectionTimeoutState--;
            _printConnectionType();
            _syncState = SYNCING;
            _serialPort.clear();
            _readData.clear();
            _write(data);
            _connectionTimeoutTimer.start(CONN_TIMEOUT_TIME);
        } else {
            _standardOutput << QObject::tr("Connection attempts failed") << Qt::endl;
            _connectionTimeoutState = 0;
            _syncState = SYNC_FAILED;
            close();
            emit reportSerialStatus(CONNECTION_FAILED);
        }
    }
}

void SerialController::handleReadTimeout()
{
    QByteArray data = _readData;
    QByteArray masterReplyData;
    masterReplyData.resize(1);
    if ((uint8_t)data[0] != 0x90) {
        _standardOutput << QObject::tr("READ FROM port (len: %2) : %1 - ")
                               .arg(_serialPort.portName())
                               .arg(data.length());
        for (char c : data) {
            _standardOutput << QString("0x%1 ").arg((uint8_t)c, 1, 16);
            if ((uint8_t)c == 0x90) {
                break;
            }
        }
        _standardOutput << Qt::endl;
    }
    _readData.clear();
    // Trying chocolate sync first.
    //if (_connectionTimeoutState >= 3) {
    if (false) {
        if (_syncState == SYNCING && data.endsWith(_chocolate_sync_resp[0])) {
            _syncState = CHOCO_SYNC_STATE_0;
            masterReplyData[0] = _chocolate_sync_bytes[0];
            _serialPort.clear();
            _write(masterReplyData);
        } else if (_syncState == CHOCO_SYNC_STATE_0) {
            if (data.startsWith(_chocolate_sync_resp[0])) {
                _syncState = CHOCO_SYNC_STATE_1;
                masterReplyData[0] = _chocolate_sync_bytes[1];
                _serialPort.clear();
                _write(masterReplyData);
            } else {
                _syncState = SYNCING;
                masterReplyData[0] = _chocolate_sync_bytes[0];
                _write(masterReplyData);
                /*
                _syncState = SYNC_FAILED;
                close();
                emit reportSerialStatus(CONNECTION_FAILED);
                */
            }
        } else if (_syncState == CHOCO_SYNC_STATE_1) {
            if (data.startsWith(_chocolate_sync_resp[1])) {
                _syncState = CHOCO_SYNC_STATE_2;
                masterReplyData[0] = _chocolate_sync_bytes[2];
                _serialPort.clear();
                _write(masterReplyData);
            } else {
                _syncState = SYNCING;
                masterReplyData[0] = _chocolate_sync_bytes[0];
                _write(masterReplyData);
                /*
                _syncState = SYNC_FAILED;
                close();
                emit reportSerialStatus(CONNECTION_FAILED);
                */
            }
        } else if (_syncState == CHOCO_SYNC_STATE_2) {
            if (data.startsWith(PRO_CON) || data.startsWith(JOYCON_R) || data.startsWith(JOYCON_L)) {
                if (data.startsWith(PRO_CON)) {
                    emit reportSerialStatus(CHOCO_SYNCED_PRO);
                } else if (data.startsWith(JOYCON_R)) {
                    emit reportSerialStatus(CHOCO_SYNCED_JC_R);
                } else if (data.startsWith(JOYCON_L)) {
                    emit reportSerialStatus(CHOCO_SYNCED_JC_L);
                }
                _syncState = SYNC_SUCCESS;
                _connectionTimeoutTimer.stop();
                _sendPacket(_blankPacket);
            } else {
                _syncState = SYNCING;
                masterReplyData[0] = _chocolate_sync_bytes[0];
                _write(masterReplyData);
                /*
                _syncState = SYNC_FAILED;
                close();
                emit reportSerialStatus(CONNECTION_FAILED);
                */
            }
        }
    } else if (_connectionTimeoutState >= 1) {
        // Chocolate timed out, let's try vanilla.
        if (_syncState == SYNCING && data.endsWith(_vanilla_sync_resp[0])) {
            _syncState = SYNC_STATE_0;
            masterReplyData[0] = _vanilla_sync_bytes[0];
            _serialPort.clear();
            _write(masterReplyData);
        } else if (_syncState == SYNC_STATE_0) {
            if (data.startsWith(_vanilla_sync_resp[0])) {
                _syncState = SYNC_STATE_1;
                masterReplyData[0] = _vanilla_sync_bytes[1];
                _serialPort.clear();
                _write(masterReplyData);
            } else {
                _syncState = SYNCING;
                masterReplyData[0] = _chocolate_sync_bytes[0];
                _write(masterReplyData);
                /*
                _syncState = SYNC_FAILED;
                close();
                emit reportSerialStatus(CONNECTION_FAILED);
                */
            }
        } else if (_syncState == SYNC_STATE_1) {
            if (data.startsWith(_vanilla_sync_resp[1])) {
                _syncState = SYNC_STATE_2;
                masterReplyData[0] = _vanilla_sync_bytes[2];
                _serialPort.clear();
                _write(masterReplyData);
            } else {
                _syncState = SYNCING;
                masterReplyData[0] = _chocolate_sync_bytes[0];
                _write(masterReplyData);
                /*
                _syncState = SYNC_FAILED;
                close();
                emit reportSerialStatus(CONNECTION_FAILED);
                */
            }
        } else if (_syncState == SYNC_STATE_2) {
            if (data.startsWith(_vanilla_sync_resp[2])) {
                _syncState = SYNC_SUCCESS;
                _connectionTimeoutTimer.stop();
                emit reportSerialStatus(CONNECTED_OK);
                _sendPacket(_blankPacket);
            } else {
                _syncState = SYNCING;
                masterReplyData[0] = _chocolate_sync_bytes[0];
                _write(masterReplyData);
                /*
                _syncState = SYNC_FAILED;
                close();
                emit reportSerialStatus(CONNECTION_FAILED);
                */
            }
        }
    }
}

void SerialController::openAndSync(QString portName)
{
    _portName = portName;

    //_serialPort = new QSerialPort();
    connect(&_serialPort, SIGNAL(bytesWritten(qint64)),
        this, SLOT(handleBytesWritten(qint64)));
    connect(&_serialPort, SIGNAL(errorOccurred(QSerialPort::SerialPortError)),
        this, SLOT(handleError(QSerialPort::SerialPortError)));

    connect(&_serialPort, SIGNAL(readyRead()), this, SLOT(handleReadyRead()));

    _serialPort.setPortName(_portName);
    _serialPort.setBaudRate(BAUD_RATE);
    opened = _serialPort.open(QIODevice::ReadWrite);
    if (opened) {
        QByteArray syncBits;
        syncBits.resize(10);
        syncBits.fill(0xFF);
        //syncBit[0] = _sync_bytes[0];
        _serialPort.clear();
        _write(syncBits);
        _syncState = SYNCING;
        emit reportSerialStatus(SYNCING);
        _connectionTimeoutTimer.start(CONN_TIMEOUT_TIME);
        _printConnectionType();
    } else {
        close();
        emit reportSerialStatus(CONNECTION_FAILED);
    }
}

void SerialController::_printConnectionType()
{
    //    if (_connectionTimeoutState >= 3) {
    //        _standardOutput << QObject::tr("Connecting via Chocolate") << Qt::endl;
    //} else if (_connectionTimeoutState >= 1) {
    _standardOutput << QObject::tr("Connecting via Vanilla") << Qt::endl;
    //}
}

uint8_t SerialController::_crc8(uint8_t old_crc, uint8_t new_data)
{
    uint8_t data = old_crc ^ new_data;
    for (int i = 0; i < 8; i = i + 1) {
        if ((data & 0x80) != 0) {
            data = data << 1;
            data = data ^ 0x07;
        } else {
            data = data << 1;
        }
        data = data & 0xFF;
    }
    return data;
}

void SerialController::_sendPacket(const QByteArray& packet)
{
    if (_syncState == SYNC_SUCCESS) {
        uint8_t crc = 0;
        uint8_t packetLength = packet.count();
        QByteArray toSend(packet);
        toSend.resize(9);
        for (int d = 0; d < packetLength; d = d + 1) {
            crc = _crc8(crc, packet[d]);
        }
        toSend[8] = crc;
        _write(toSend);
    }
}

void SerialController::_cmd_to_packet(quint64 command, QByteArray& packetList)
{
    quint64 cmdCopy = command;
    uint8_t low = (cmdCopy & 0xFF);
    cmdCopy = cmdCopy >> 8;
    uint8_t high = (cmdCopy & 0xFF);
    cmdCopy = cmdCopy >> 8;
    uint8_t dpad = (cmdCopy & 0xFF);
    cmdCopy = cmdCopy >> 8;
    //uint8_t lstick_intensity = (cmdCopy & 0xFF);
    //cmdCopy = cmdCopy >> 8;
    //int lstick_angle = (cmdCopy & 0xFFF);
    //cmdCopy = cmdCopy >> 12;
    //uint8_t rstick_intensity = (cmdCopy & 0xFF);
    //cmdCopy = cmdCopy >> 8;
    //int rstick_angle = (cmdCopy & 0xFFF);
    dpad = _decrypt_dpad(dpad);
    //uint8_t leftxy[2];
    //angle(lstick_angle, lstick_intensity, leftxy);
    //uint8_t rightxy[2];
    //angle(rstick_angle, rstick_intensity, rightxy);
    //packetList = {high, low, dpad, leftxy[0], leftxy[1], rightxy[0], rightxy[1], 0x00}
    packetList[0] = high;
    packetList[1] = low;
    packetList[2] = dpad;
    packetList[3] = 0x80;
    packetList[4] = 0x80;
    packetList[5] = 0x80;
    packetList[6] = 0x80;
    packetList[7] = 0x00;
}

uint8_t SerialController::_decrypt_dpad(uint8_t dpad)
{
    uint8_t dpadDecrypt;
    if (dpad == _inpEmu.DIR_U) {
        dpadDecrypt = _inpEmu.A_DPAD_U;
    } else if (dpad == _inpEmu.DIR_R) {
        dpadDecrypt = _inpEmu.A_DPAD_R;
    } else if (dpad == _inpEmu.DIR_D) {
        dpadDecrypt = _inpEmu.A_DPAD_D;
    } else if (dpad == _inpEmu.DIR_L) {
        dpadDecrypt = _inpEmu.A_DPAD_L;
    } else if (dpad == _inpEmu.DIR_U_R) {
        dpadDecrypt = _inpEmu.A_DPAD_U_R;
    } else if (dpad == _inpEmu.DIR_U_L) {
        dpadDecrypt = _inpEmu.A_DPAD_U_L;
    } else if (dpad == _inpEmu.DIR_D_R) {
        dpadDecrypt = _inpEmu.A_DPAD_D_R;
    } else if (dpad == _inpEmu.DIR_D_L) {
        dpadDecrypt = _inpEmu.A_DPAD_D_L;
    } else {
        dpadDecrypt = _inpEmu.A_DPAD_CENTER;
    }
    return dpadDecrypt;
}

void SerialController::temporaryButtonTimeout()
{
    _sendPacket(_blankPacket);
}

void SerialController::recieveButtonAction(quint64 action, bool temporary)
{
    QByteArray actionArray;
    actionArray.resize(8);
    _cmd_to_packet(action, actionArray);
    _sendPacket(actionArray);
    if (temporary) {
        if (action != _inpEmu.NO_INPUT)
            _temporaryButtonTimer.start(100);
    }
}
