#ifndef SCRIPTMANAGERDIALOG_H
#define SCRIPTMANAGERDIALOG_H

#include <QDialog>
#include <QSortFilterProxyModel>

#include "../models/bot.hpp"

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
    void cloneRepository(const Bot::Data &bot_data);
    void showHelp();

   signals:
    void botCreated(const Bot::Data &bot_data);

   private:
    Ui::ScriptManagerDialog *m_ui;
};

#endif  // SCRIPTMANAGERDIALOG_H
