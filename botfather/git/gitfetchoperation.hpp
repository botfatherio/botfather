#ifndef GITFETCHOPERATION_H
#define GITFETCHOPERATION_H

#include <QObject>
#include "abstractgitoperation.hpp"

class GitFetchOperation : public AbstractGitOperation
{
	Q_OBJECT

public:
	GitFetchOperation(const QString &repo_path);
	~GitFetchOperation() override;

public slots:
	void process() override;

private:
	QString m_repo_path;
	git_repository *m_repo = nullptr;
	git_remote *m_remote = nullptr;
};

#endif // GITFETCHOPERATION_H
