#include "script_manager_dialog.hpp"

#include <QAbstractButton>
#include <QDebug>
#include <QDesktopServices>
#include <QDir>
#include <QInputDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QStandardPaths>
#include <QThread>
#include <QUuid>

#include "git_progress_dialog.hpp"
#include "ui_script_manager_dialog.h"

ScriptManagerDialog::ScriptManagerDialog(QWidget *parent)
    : QDialog(parent), m_ui(new Ui::ScriptManagerDialog) {
    m_ui->setupUi(this);
    connect(m_ui->button_box, &QDialogButtonBox::accepted, this,
            &QDialog::accept);
    connect(m_ui->button_box, &QDialogButtonBox::rejected, this,
            &QDialog::reject);

    m_ui->button_box->button(QDialogButtonBox::StandardButton::Save)
        ->setDisabled(true);
    connect(m_ui->bot_name_line_edit, &QLineEdit::textChanged, this,
            &ScriptManagerDialog::validateInputs);
    connect(m_ui->git_url_line_edit, &QLineEdit::textChanged, this,
            &ScriptManagerDialog::validateInputs);

    connect(m_ui->button_box->button(QDialogButtonBox::StandardButton::Help),
            &QPushButton::clicked, this, &ScriptManagerDialog::showHelp);

    QRegularExpression https_git_url_regex("https:\\/\\/.*\\.git");
    QValidator *https_git_url_validator =
        new QRegularExpressionValidator(https_git_url_regex, this);
    m_ui->git_url_line_edit->setValidator(https_git_url_validator);

    setFixedSize(size());
    connect(this, SIGNAL(accepted()), this, SLOT(installScript()));
}

ScriptManagerDialog::~ScriptManagerDialog() { delete m_ui; }

void ScriptManagerDialog::validateInputs() {
    bool disabled = m_ui->bot_name_line_edit->text().isEmpty() ||
                    m_ui->git_url_line_edit->text().isEmpty();
    m_ui->button_box->button(QDialogButtonBox::StandardButton::Save)
        ->setDisabled(disabled);
}

void ScriptManagerDialog::installScript() {
    QDir bot_parent_dir(
        QStandardPaths::writableLocation(QStandardPaths::AppDataLocation));
    bot_parent_dir.mkpath(bot_parent_dir.absolutePath());

    QString git_url = m_ui->git_url_line_edit->text();
    QString bot_name = m_ui->bot_name_line_edit->text();
    QString bot_uuid = QUuid::createUuid().toString(QUuid::WithoutBraces);

    if (!bot_parent_dir.mkdir(bot_uuid)) {
        // Fatal error, couldn't create the repo dir
        return;
    }

    QString bot_path = bot_parent_dir.filePath(bot_uuid);

    Bot::Data bot_data(bot_path, bot_name, git_url);
    qDebug() << "Bot created. Path:" << bot_data.path
             << "Name:" << bot_data.name << "Repo:" << bot_data.repo;

    cloneRepository(bot_data);
}

void ScriptManagerDialog::cloneRepository(const Bot::Data &bot_data) {
    GitProgressDialog *dialog = new GitProgressDialog(this);
    connect(dialog, &GitProgressDialog::cloned,
            [this, bot_data]() { emit botCreated(bot_data); });
    dialog->clone(bot_data.repo, bot_data.path);
}

void ScriptManagerDialog::showHelp() {
    QDesktopServices::openUrl(
        QUrl("https://botfather.io/docs/player/cloning-scripts/"));
}
