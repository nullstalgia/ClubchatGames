#include "inputemulator.h"

QStringList InputEmulator::_moveLeftCommands = { "l", "left" };
QStringList InputEmulator::_moveUpCommands = { "u", "up" };
QStringList InputEmulator::_moveDownCommands = { "d", "down" };
QStringList InputEmulator::_moveRightCommands = { "r", "right" };

QStringList InputEmulator::_pressACommands = { "a", "press" };
QStringList InputEmulator::_pressBCommands = { "b", "back" };
QStringList InputEmulator::_pressXCommands = { "x" };
QStringList InputEmulator::_pressYCommands = QStringList() << "y";
