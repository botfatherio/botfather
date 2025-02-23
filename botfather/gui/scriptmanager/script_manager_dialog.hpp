#ifndef SCRIPTMANAGERDIALOG_H
#define SCRIPTMANAGERDIALOG_H

#include <QDialog>
#include <QSortFilterProxyModel>

namespace Ui {
class ScriptManagerDialog;
}

class ScriptManagerDialog : public QDialog {
    Q_OBJECT

   public:
    explicit ScriptManagerDialog(QWidget *parent = nullptr);
    ~ScriptManagerDialog();

   public slots:
    void validateInputs();
    void installScript();
    void cloneRepository(const QString &path, const QString &name,
                         const QString &repo, const QString &branch);
    void showHelp();

   signals:
    void botCreated(const QString &path, const QString &name,
                    const QString &repo, const QString &branch);

   private:
    Ui::ScriptManagerDialog *m_ui;
};

#endif  // SCRIPTMANAGERDIALOG_H
