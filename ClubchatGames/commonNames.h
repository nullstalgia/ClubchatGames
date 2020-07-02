#ifndef IRCCOMMON_H
#define IRCCOMMON_H

#define ONE_ON_ONE_MODE 0
#define ANARCHY_MODE 1
#define VOTE_MODE 2
#define NOTHING_MODE 3

#define bA 0
#define bB 1
#define bY 2
#define bX 3

#define bUP 4
#define bDOWN 5
#define bLEFT 6
#define bRIGHT 7

//#define BTN_SELECT 8
//#define BTN_START 9
//#define BTN_CAPTURE 10
//#define BTN_HOME 11

#define RELEASED true
#define PRESSED false

#define TEMPORARY true
#define MANUAL false

#define NOT_CONNECTED 0
#define CONNECTED_OK 1
#define CONNECTING 2
#define CONNECTION_FAILED 3

#define SYNCING 2
#define SYNC_FAILED 3

#define M_ERROR 4

#define NOT_SYNCED 5
#define SYNC_STATE_0 6
#define SYNC_STATE_1 7
#define SYNC_STATE_2 8
#define SYNC_SUCCESS 9
#define CHOCO_SYNC_STATE_0 10
#define CHOCO_SYNC_STATE_1 11
#define CHOCO_SYNC_STATE_2 12
#define CHOCO_SYNCED_PRO 13
#define CHOCO_SYNCED_JC_L 14
#define CHOCO_SYNCED_JC_R 15

#define PRO_CON 0x03
#define JOYCON_L 0x01
#define JOYCON_R 0x02

//#define IRC_HOST "irc.chat.twitch.tv"
//#define IRC_SECURE true
//#define IRC_PORT 6697

#define IRC_HOST "localhost"
#define IRC_SECURE false
#define IRC_PORT 6667

#define IRC_USERNAME "justinfan223"
//#define IRC_PASSWORD

#define CONN_TIMEOUT_TIME 5000

#endif // IRCCOMMON_H
