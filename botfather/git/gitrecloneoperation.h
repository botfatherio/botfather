#ifndef GITRECLONEOPERATION_H
#define GITRECLONEOPERATION_H

#include <QObject>
#include <QTemporaryDir>
#include "abstractgitoperation.h"

class GitRecloneOperation : public AbstractGitOperation
{
	Q_OBJECT

public:
	GitRecloneOperation(const QString &remote_url, const QString &local_path);

public slots:
	void process() override;
	void replaceRepo();

signals:
	void replacingRepo();

private:
	QString m_remote_url;
	QString m_local_path;
	QTemporaryDir m_tmp_clone_dir;
};

#endif // GITRECLONEOPERATION_H
