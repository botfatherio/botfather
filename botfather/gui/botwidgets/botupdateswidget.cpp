#include "botupdateswidget.hpp"
#include "ui_botupdateswidget.h"
#include "../scriptmanager/gitprogressdialog.hpp"
#include <QTimer>
#include <QDateTime>
#include <QDebug>

BotUpdatesWidget::BotUpdatesWidget(Bot *bot, QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::BotUpdatesWidget)
	, m_bot(bot)
{
	ui->setupUi(this);
	m_bot_settings = new QSettings(bot->settingsPath(), QSettings::IniFormat);

	QSizePolicy progressbar_size_policy = ui->progressBar->sizePolicy();
	progressbar_size_policy.setRetainSizeWhenHidden(true);
	ui->progressBar->setSizePolicy(progressbar_size_policy);
	ui->progressBar->setHidden(true); // hide initially

	connect(ui->check_button, &QPushButton::clicked, this, &BotUpdatesWidget::checkForUpdates);
	connect(ui->update_button, &QPushButton::clicked, this, &BotUpdatesWidget::updateTheBot);

	connect(bot, &Bot::statusChanged, this, &BotUpdatesWidget::botStatusChanged);
	connect(bot, &Bot::statusCheckFailed, this, &BotUpdatesWidget::resetProgressIndicators);
	botStatusChanged(bot->status());

	if (!bot->repo().isEmpty() && m_bot_settings->value("auto_update_check", true).toBool())
	{
		QTimer::singleShot(1, bot, &Bot::checkStatus);
	}
}

BotUpdatesWidget::~BotUpdatesWidget()
{
	delete ui;
}

void BotUpdatesWidget::checkForUpdates()
{
	ui->progressBar->setHidden(false);
	ui->check_button->setDisabled(true);
	ui->check_button->clearFocus(); // looks weird otherwise
	m_bot->checkStatus();
}

void BotUpdatesWidget::updateTheBot()
{
	ui->progressBar->setHidden(false);
	ui->update_button->setDisabled(true);
	ui->update_button->clearFocus(); // looks weird otherwise

	GitProgressDialog *git_progress_dialog = new GitProgressDialog(this);
	connect(git_progress_dialog, &GitProgressDialog::accepted, this, &BotUpdatesWidget::checkForUpdates);
	connect(git_progress_dialog, &GitProgressDialog::rejected, this, &BotUpdatesWidget::resetProgressIndicators);
	connect(git_progress_dialog, &GitProgressDialog::cloned, this, &BotUpdatesWidget::botUpdated);
	git_progress_dialog->reclone(m_bot->repo(), m_bot->path());
}

void BotUpdatesWidget::resetProgressIndicators()
{
	ui->progressBar->setHidden(true);
	ui->check_button->setEnabled(true);
	ui->update_button->setEnabled(true);
}

void BotUpdatesWidget::botStatusChanged(const Bot::Status &status)
{
	switch (status) {
	case Bot::Status::Outdated:
		ui->update_button->show();
		ui->check_button->hide();
		ui->updates_available_label->show();
		ui->up_to_date_label->hide();
		break;
	case Bot::Status::UpToDate:
		ui->check_button->show();
		ui->update_button->hide();
		ui->up_to_date_label->show();
		ui->updates_available_label->hide();
		break;
	case Bot::Status::Unavailabe:
		ui->check_button->show();
		ui->update_button->hide();
		ui->updates_available_label->hide();
		ui->up_to_date_label->hide();
		break;
	}

	QString time_string = QDateTime::currentDateTime().toString("ddd MMM, hh:mm:ss");
	ui->last_check_label->setText(time_string);

	// Reset what checkForUpdates() and updateTheBot() changed
	resetProgressIndicators();
}
