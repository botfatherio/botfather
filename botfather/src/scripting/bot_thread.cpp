#include "bot_thread.h"

BotThread::BotThread(QObject *parent) : QThread(parent)
{}

void BotThread::setTerminationEnabled(bool enabled)
{
	this->QThread::setTerminationEnabled(enabled);
}