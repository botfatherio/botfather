#ifndef BFB__SCRIPTING__BOT_H
#define BFB__SCRIPTING__BOT_H

#include <QObject>
#include <QString>
#include "bot_thread.h"

class Bot : public QObject
{
	Q_OBJECT

public:
	Bot(BotThread* thread_p, QString script_path);
	
public slots:
	void runScript();

signals:
	void started();
	void stopped(bool without_errors);
	void message(QString log_message, bool from_client);
	
private:
	BotThread* m_thread_p;
	QString m_script_path;
};

#endif // BFB__SCRIPTING__BOT_H