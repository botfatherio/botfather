#ifndef BFB__SCRIPTING__BOT_THREAD_H
#define BFB__SCRIPTING__BOT_THREAD_H

#include <QThread>

class BotThread : public QThread
{
public:
	void setTerminationEnabled(bool enabled=true);
};

#endif // BFB__SCRIPTING__BOT_THREAD_H