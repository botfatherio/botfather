#ifndef GITFETCHOPERATION_H
#define GITFETCHOPERATION_H

#include <QObject>
#include "abstractgitoperation.h"

class GitFetchOperation : public AbstractGitOperation
{
	Q_OBJECT

public:
	GitFetchOperation(const QString &repo_path);

public slots:
	void process() override;

private:
	QString m_repo_path;
};

#endif // GITFETCHOPERATION_H
