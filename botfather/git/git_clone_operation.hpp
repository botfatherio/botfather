#ifndef GITCLONEOPERATION_H
#define GITCLONEOPERATION_H

#include <QObject>

#include "abstract_git_operation.hpp"

class GitCloneOperation : public AbstractGitOperation {
    Q_OBJECT

   public:
    GitCloneOperation(const QString &repo_url, const QString &dir_path, const QString &branch);

   public slots:
    void process() override;

   private:
    QString m_repo_url;
    QString m_dir_path;
    QString m_branch;
};

#endif  // GITCLONEOPERATION_H
