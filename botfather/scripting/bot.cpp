#include "bot.h"
#include <QThread>
#include <QDebug>

Bot::Bot(QString script_path) : m_script_path(script_path)
{}

void Bot::runScript()
{
	emit this->started();
	
	while (!QThread::currentThread()->isInterruptionRequested()) {
		
		// This sleep represents the bots algorithm lol.
		QThread::sleep(1);
		qDebug() << "Working...";
		
	}
	
	emit this->stopped();
}