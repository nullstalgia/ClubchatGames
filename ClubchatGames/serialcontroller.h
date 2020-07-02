#ifndef SERIALCONTROLLER_H
#define SERIALCONTROLLER_H

#include <QByteArray>
#include <QObject>
#include <QSerialPort>
#include <QTextStream>
#include <QTimer>

#include "commonNames.h"
#include "inputemulator.h"
#define BAUD_RATE 19200

//# Responses from MCU
#define RESP_USB_ACK 0x90
#define RESP_UPDATE_ACK 0x91
#define RESP_UPDATE_NACK 0x92

//#define COUNTS_FOR_CHOCOLATE 1
#define COUNTS_FOR_VANILLA 2

class SerialController : public QObject {
    Q_OBJECT

public:
    SerialController(QObject* parent = nullptr);
    ~SerialController();
    void openAndSync(QString portName);
    void close();
    bool opened;

public slots:
    void recieveButtonAction(quint64 action, bool temporary);

signals:
    void reportSerialStatus(uint8_t status);

private slots:
    void handleBytesWritten(qint64 bytes);
    void handleReadTimeout();
    void handleError(QSerialPort::SerialPortError error);
    void handleReadyRead();
    void handleWriteTimeout();
    void handleConnectionTimeout();
    void temporaryButtonTimeout();

private:
    InputEmulator _inpEmu;
    void _write(const QByteArray& writeData);
    QSerialPort _serialPort;
    QString _portName;
    QByteArray _writeData;
    QByteArray _readData;
    QTextStream _standardOutput;
    qint64 _bytesWritten = 0;
    QTimer _readTimeoutTimer;
    QTimer _writeTimeoutTimer;
    QTimer _connectionTimeoutTimer;
    uint8_t _connectionTimeoutState;
    uint16_t _syncState;
    QTimer _temporaryButtonTimer;
    uint8_t _crc8(uint8_t old_crc, uint8_t new_data);
    void _sendPacket(const QByteArray& packet);
    uint8_t _decrypt_dpad(uint8_t dpad);
    void _cmd_to_packet(quint64 command, QByteArray& packetList);
    void _printConnectionType();

    const QByteArray _blankPacket = QByteArrayLiteral("\x00\x00\x08\x80\x80\x80\x80\x00");

    const quint8 _vanilla_sync_bytes[3] = { 0xFF, 0x33, 0xCC };
    const quint8 _vanilla_sync_resp[3] = { 0xFF, 0xCC, 0x33 };

    const quint8 _chocolate_sync_bytes[3] = { 0xFF, 0x44, 0xEE };
    const quint8 _chocolate_sync_resp[2] = { 0xFF, 0xEE }; // Last response is the CONTROLLER_TYPE
};

#endif // SERIALCONTROLLER_H
