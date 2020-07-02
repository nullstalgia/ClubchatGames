#pragma once
#include <QStringList>
#include <Qt>
#ifndef INPUTEMULATOR_H
#define INPUTEMULATOR_H

class InputEmulator {
public:
    static QStringList _moveLeftCommands;
    static QStringList _moveUpCommands;
    static QStringList _moveDownCommands;
    static QStringList _moveRightCommands;

    static QStringList _pressACommands;
    static QStringList _pressBCommands;
    static QStringList _pressXCommands;
    static QStringList _pressYCommands;
    //ActualSwitchDPADValues;
    const static uint8_t A_DPAD_CENTER = 0x08;
    const static uint8_t A_DPAD_U = 0x00;
    const static uint8_t A_DPAD_U_R = 0x01;
    const static uint8_t A_DPAD_R = 0x02;
    const static uint8_t A_DPAD_D_R = 0x03;
    const static uint8_t A_DPAD_D = 0x04;
    const static uint8_t A_DPAD_D_L = 0x05;
    const static uint8_t A_DPAD_L = 0x06;
    const static uint8_t A_DPAD_U_L = 0x07;

    //EnumDIRValues;
    const static uint8_t DIR_CENTER = 0x00;
    const static uint8_t DIR_U = 0x01;
    const static uint8_t DIR_R = 0x02;
    const static uint8_t DIR_D = 0x04;
    const static uint8_t DIR_L = 0x08;
    const static uint8_t DIR_U_R = DIR_U + DIR_R;
    const static uint8_t DIR_D_R = DIR_D + DIR_R;
    const static uint8_t DIR_U_L = DIR_U + DIR_L;
    const static uint8_t DIR_D_L = DIR_D + DIR_L;

    const static quint64 BTN_NONE = 0x0000000000000000;
    const static quint64 BTN_Y = 0x0000000000000001;
    const static quint64 BTN_B = 0x0000000000000002;
    const static quint64 BTN_A = 0x0000000000000004;
    const static quint64 BTN_X = 0x0000000000000008;
    const static quint64 BTN_L = 0x0000000000000010;
    const static quint64 BTN_R = 0x0000000000000020;
    const static quint64 BTN_ZL = 0x0000000000000040;
    const static quint64 BTN_ZR = 0x0000000000000080;
    const static quint64 BTN_MINUS = 0x0000000000000100;
    const static quint64 BTN_PLUS = 0x0000000000000200;
    const static quint64 BTN_LCLICK = 0x0000000000000400;
    const static quint64 BTN_RCLICK = 0x0000000000000800;
    const static quint64 BTN_HOME = 0x0000000000001000;
    const static quint64 BTN_CAPTURE = 0x0000000000002000;
    const static quint64 BTN_SL = 0x0000000000004000;
    const static quint64 BTN_SR = 0x0000000000008000;

    const static quint64 DPAD_CENTER = 0x0000000000000000;
    const static quint64 DPAD_U = 0x0000000000010000;
    const static quint64 DPAD_R = 0x0000000000020000;
    const static quint64 DPAD_D = 0x0000000000040000;
    const static quint64 DPAD_L = 0x0000000000080000;
    const static quint64 DPAD_U_R = DPAD_U + DPAD_R;
    const static quint64 DPAD_D_R = DPAD_D + DPAD_R;
    const static quint64 DPAD_U_L = DPAD_U + DPAD_L;
    const static quint64 DPAD_D_L = DPAD_D + DPAD_L;

    const static quint64 LSTICK_CENTER = 0x0000000000000000;
    const static quint64 LSTICK_R = 0x00000000FF000000; //0(000);
    const static quint64 LSTICK_U_R = 0x0000002DFF000000; //45(02D);
    const static quint64 LSTICK_U = 0x0000005AFF000000; //90(05A);
    const static quint64 LSTICK_U_L = 0x00000087FF000000; //135(087);
    const static quint64 LSTICK_L = 0x000000B4FF000000; //180(0B4);
    const static quint64 LSTICK_D_L = 0x000000E1FF000000; //225(0E1);
    const static quint64 LSTICK_D = 0x0000010EFF000000; //270(10E);
    const static quint64 LSTICK_D_R = 0x0000013BFF000000; //315(13B);

    const static quint64 RSTICK_CENTER = 0x0000000000000000;
    const static quint64 RSTICK_R = 0x000FF00000000000; //0(000);
    const static quint64 RSTICK_U_R = 0x02DFF00000000000; //45(02D);
    const static quint64 RSTICK_U = 0x05AFF00000000000; //90(05A);
    const static quint64 RSTICK_U_L = 0x087FF00000000000; //135(087);
    const static quint64 RSTICK_L = 0x0B4FF00000000000; //180(0B4);
    const static quint64 RSTICK_D_L = 0x0E1FF00000000000; //225(0E1);
    const static quint64 RSTICK_D = 0x10EFF00000000000; //270(10E);
    const static quint64 RSTICK_D_R = 0x13BFF00000000000; //315(13B);

    const static quint64 NO_INPUT = BTN_NONE + DPAD_CENTER + LSTICK_CENTER + RSTICK_CENTER;

    //CommandstosendtoMCU;
    //const uint8_t COMMAND_NOP = 0x00;
    //const uint8_t COMMAND_SYNC_1 = 0x33;
    //const uint8_t COMMAND_SYNC_2 = 0xCC;
    //const uint8_t COMMAND_SYNC_START = 0xFF;
    //ResponsesfromMCU;
    //const uint8_t RESP_USB_ACK = 0x90;
    //const uint8_t RESP_UPDATE_ACK = 0x91;
    //const uint8_t RESP_UPDATE_NACK = 0x92;
    //const uint8_t RESP_SYNC_START = 0xFF;
    //const uint8_t RESP_SYNC_1 = 0xCC;
    //const uint8_t RESP_SYNC_OK = 0x33;
    /*
class inputemulator {
public:
    inputemulator();
    void angle(int InputAngle, uint8_t intensity, uint8_t AngleList[]);
    quint64 lstick_angle(int InputAngle, uint8_t intensity);
    quint64 rstick_angle(int InputAngle, uint8_t intensity);
    void p_wait(float waitTime);
    void wait_for_data(float timeout = 1.0, float sleepTime = 0.001);
    void read_uint8_ts(int numuint8_ts, uint8_t uint8_tArray[]);
    uint8_t read_uint8_t();
    uint8_t read_uint8_t_latest();
    void write_uint8_ts(uint8_t Writeuint8_tsList[], int uint8_tsListLength);
    void write_uint8_t(uint8_t uint8_tToSend);
    uint8_t crc8_ccitt(uint8_t old_crc, uint8_t new_data);
    bool send_packet(uint8_t packet[], int packetLength, float delayTime = 0.001);
    uint8_t decrypt_dpad(uint8_t dpad);
    void cmd_to_packet(quint64 command, uint8_t packetList[]);
    bool send_cmd(quint64 command = NO_INPUT);
    bool force_sync();
    bool sync();
    bool isSynced;
};
*/
};
#endif // INPUTEMULATOR_H
