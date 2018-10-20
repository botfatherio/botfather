#ifndef MTOOLWRAPPER_H
#define MTOOLWRAPPER_H

#include <QObject>

class MtoolWrapper : public QObject
{
	Q_OBJECT
public:
	explicit MtoolWrapper(QObject *parent = nullptr);

	enum Mode{
		Updater
	};

signals:
	void executableFound(bool found);
	void startedDetached();

public slots:
	void findExecutable();
	void startDetached(Mode mode);
	void startDetachedUpdater();

protected:
	QString getExecutablePath();
};

#endif // MTOOLWRAPPER_H
