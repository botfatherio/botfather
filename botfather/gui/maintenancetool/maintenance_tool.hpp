#ifndef MAINTENANCETOOL_H
#define MAINTENANCETOOL_H

#include <QObject>
#include <QProcess>

class MaintenanceTool : public QObject {
    Q_OBJECT
   public:
    explicit MaintenanceTool(QObject *parent = nullptr);
    static QString filePath();
    static bool exists();

   public slots:
    void checkForUpdates();
    void startDetachedAsUpdater();

   signals:
    void errorsOccured();
    void started();
    void finished();
    void updatesAvailable();
    void updaterStarted();

   protected:
    QProcess *getPreparedProcess();
};

#endif  // MAINTENANCETOOL_H
