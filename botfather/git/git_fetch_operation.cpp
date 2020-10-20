#include "git_fetch_operation.hpp"

#include <QDebug>

GitFetchOperation::GitFetchOperation(const QString &repo_path)
    : AbstractGitOperation(), m_repo_path(repo_path) {}

GitFetchOperation::~GitFetchOperation() {
    if (m_remote) git_remote_free(m_remote);
    if (m_repo) git_repository_free(m_repo);
}

void GitFetchOperation::process() {
    int return_code = 0;

    return_code = git_repository_open_ext(
        &m_repo, m_repo_path.toUtf8(), GIT_REPOSITORY_OPEN_NO_SEARCH, nullptr);
    if (return_code) {
        qDebug() << "Opening the repo failed. ReturnCode:" << return_code
                 << "; RepoPath:" << m_repo_path;
        emit failure(return_code);
        return;
    }

    return_code = git_remote_lookup(&m_remote, m_repo, "origin");
    if (return_code != 0) {
        qDebug() << "Remote lookup failed. ReturnCode:" << return_code
                 << "; RepoPath:" << m_repo_path;
        emit failure(return_code);
        return;
    }

    git_fetch_options fetch_options = GIT_FETCH_OPTIONS_INIT;
    fetch_options.callbacks.transfer_progress =
        &AbstractGitOperation::
            transferProgressCallback;  // TODO: find out whether to use & or not
    fetch_options.callbacks.payload = this;

    return_code = git_remote_fetch(m_remote, nullptr, &fetch_options, "fetch");
    if (return_code != 0) {
        qDebug() << "Fetching failed. ReturnCode:" << return_code
                 << "; RepoPath:" << m_repo_path;
        emit failure(return_code);
        return;
    }

    /*
    const git_transfer_progress *final_stats = git_remote_stats(remote);
    final_stats->received_bytes;
    final_stats->indexed_deltas;
    final_stats->total_deltas;
    final_stats->local_objects;
    final_stats->received_objects;
    final_stats->indexed_objects;
    final_stats->total_objects;
    */

    emit success();
}
