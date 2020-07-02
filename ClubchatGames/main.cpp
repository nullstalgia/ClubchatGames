#include "clubchatgames.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ClubChatGames w;
    w.show();
    return a.exec();
}
