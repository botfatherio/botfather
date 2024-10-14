#ifndef GITRECLONEOPERATION_H
#define GITRECLONEOPERATION_H

#include <QObject>
#include <QTemporaryDir>

#include "abstract_git_operation.hpp"

class GitRecloneOperation : public AbstractGitOperation {
    Q_OBJECT

   public:
    GitRecloneOperation(const QString &remote_url, const QString &local_path, const QString &branch);

   public slots:
    void process() override;
    void replaceRepo();

   signals:
    void replacingRepo();

   private:
    QString m_remote_url;
    QString m_local_path;
    QString m_branch;
    QTemporaryDir m_tmp_clone_dir;
};

#endif  // GITRECLONEOPERATION_H
