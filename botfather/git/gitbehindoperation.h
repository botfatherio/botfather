#ifndef GITBEHINDOPERATION_H
#define GITBEHINDOPERATION_H

#include <QObject>
#include "abstractgitoperation.h"

class GitBehindOperation : public AbstractGitOperation
{
	Q_OBJECT

public:
	GitBehindOperation(const QString &repo_path);

public slots:
	void process() override;

signals:
	void differencesToRemote(int differences);

private:
	QString m_repo_path;
};

#endif // GITBEHINDOPERATION_H
