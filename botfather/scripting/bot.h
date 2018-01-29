#ifndef BFB__SCRIPTING__BOT_H
#define BFB__SCRIPTING__BOT_H

#include <QObject>
#include <QString>

class Bot : public QObject
{
	Q_OBJECT

public:
	Bot(QString script_path);
	
public slots:
	void runScript();

signals:
	void started();
	void stopped(bool without_errors);
	void message(QString log_message);
	
private:
	QString m_script_path;
};

#endif // BFB__SCRIPTING__BOT_H