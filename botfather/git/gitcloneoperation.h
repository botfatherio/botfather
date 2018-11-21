#ifndef GITCLONEOPERATION_H
#define GITCLONEOPERATION_H

#include <QObject>
#include "abstractgitoperation.h"

class GitCloneOperation : public AbstractGitOperation
{
	Q_OBJECT

public:
	GitCloneOperation(const QString &repo_url, const QString &dir_path);

public slots:
	void process() override;

private:
	QString m_repo_url;
	QString m_dir_path;
};

#endif // GITCLONEOPERATION_H
