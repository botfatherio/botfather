#ifndef ABSTRACTGITOPERATION_H
#define ABSTRACTGITOPERATION_H

#include <git2.h>

#include <QObject>

class AbstractGitOperation : public QObject {
    Q_OBJECT

   public:
    AbstractGitOperation();  // Operations are intended to be used in seperate
                             // threads. In such situation won't give it a
                             // parent.
    virtual ~AbstractGitOperation();

    static const int CANCELED_CODE = 42;

    bool isCanceled() const;

    // NOTE: Move them at the time we make each callback its own class

    uint totalObjects() const;
    void setTotalObjects(uint objects);

    uint receivedObjects() const;
    void setReceivedObjects(uint objects);

    ulong receivedBytes() const;
    void setReceivedBytes(ulong bytes);

    ulong completedSteps() const;
    void setCompletedSteps(ulong steps);

    ulong totalSteps() const;
    void setTotalSteps(ulong steps);

    QString checkoutPath() const;
    void setCheckoutPath(const QString &path);

    // NOTE: we could make those callbacks classes. But how to emit the
    // valueChanged signals then? UpdateTipsCallbacks constructor would take an
    // AbstractGitOperation, so that we can emit the operations signals.
    // TODO: Make it work as it is first. Then make it better <3

    // During the download of new data, this will be regularly called with the
    // current count of progress done by the indexer.
    static int transferProgressCallback(const git_transfer_progress *stats,
                                        void *abstract_git_operation_p);

    // Optional callback to notify the consumer of checkout progress.
    static void checkoutProgressCallback(const char *path,
                                         size_t completed_steps,
                                         size_t total_steps,
                                         void *abstract_git_operation_p);

   public slots:
    void cancel();
    virtual void process() = 0;

   signals:
    void success();
    void failure(int error_code = 0);
    void finished();

    void totalObjectsChanged(uint total);
    void receivedObjectsChanged(uint received);
    void transferProgressChanged(uint received, uint total, ulong bytes);
    void checkoutCurrentChanged(ulong current);
    void checkoutTotalChanged(ulong total);
    void checkoutProgressChanged(ulong current, ulong total,
                                 const QString &path);

   private:
    bool m_is_canceled = false;

    // Transfer // TODO: move this into a dedicated GitTransferProgress class
    uint m_total_objects = 0;
    uint m_received_objects = 0;
    ulong m_received_bytes = 0;

    // Checkout // TODO: move this into a dedicated GitCheckoutProgress class
    ulong m_completed_steps = 0;
    ulong m_total_steps = 0;
    QString m_checkout_path;
};

#endif  // ABSTRACTGITOPERATION_H
