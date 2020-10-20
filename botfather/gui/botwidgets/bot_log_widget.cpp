#include "bot_log_widget.hpp"

#include <QDateTime>
#include <QDebug>
#include <QFileDialog>

#include "ui_bot_log_widget.h"

BotLogWidget::BotLogWidget(Bot *bot, QWidget *parent)
    : QWidget(parent), m_ui(new Ui::BotLogWidget) {
    m_ui->setupUi(this);
    m_bot_settings = new QSettings(bot->settingsPath(), QSettings::IniFormat);

    connect(bot, &Bot::log, this, &BotLogWidget::log);
    connect(m_ui->clear_button, &QPushButton::clicked, m_ui->log,
            &QTextEdit::clear);
    connect(m_ui->save_button, &QPushButton::clicked, this,
            &BotLogWidget::saveLogToFile);
}

BotLogWidget::~BotLogWidget() { delete m_ui; }

void BotLogWidget::log(const QString &message,
                       const Engine::LogSource &source) {
    QString time(QDateTime::currentDateTime().toString("HH:mm:ss"));
    QString color, source_name;

    if (source == Engine::LogSource::Debug &&
        !m_bot_settings->value("debug_mode").toBool()) {
        // Do not show debug message when not in debug mode
        return;
    }

    switch (source) {
        case Engine::LogSource::System:
            color = "#209cee";
            source_name = "system";
            break;
        case Engine::LogSource::Error:
            color = "#ff3860";
            source_name = "error";
            break;
        case Engine::LogSource::Script:
            color = "#4a4a4a";
            source_name = "script";
            break;
        case Engine::LogSource::Debug:
            color = "#85732d";  // #ffdd57
            source_name = "debug";
            break;
    }

    QString text = QString("<span style='color:%1'>%2 | %3 &gt; %4</span>")
                       .arg(color)
                       .arg(time)
                       .arg(source_name)
                       .arg(message);
    m_ui->log->append(text);
}

void BotLogWidget::saveLogToFile() {
    QString filename =
        QFileDialog::getSaveFileName(this, tr("Save Logfile"), QDir::homePath(),
                                     tr("Text files (*.txt *.log)"), nullptr);

    if (filename.isEmpty()) {
        qDebug() << "No logfile selected.";
        return;
    }

    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Can't open logfile" << filename;
        return;
    }
    file.write(m_ui->log->toPlainText().toUtf8());
    file.close();
}
