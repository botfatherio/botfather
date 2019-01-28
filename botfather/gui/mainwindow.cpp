#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QTimer>
#include <QFileDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	m_bot_list_model = new BotListModel(this);
	m_script_manager_dialog = new ScriptManagerDialog(this);
	m_browser_window = new BrowserWindow; // Don't give it a parent, otherwise it's blocking the control window on microsoft windows
	m_android_dialog = new AndroidDialog(this);
	m_settings_dialog = new SettingsDialog(this);
	m_auth_dialog = new AuthDialog(this);
	m_maintainance_tool = new MtoolWrapper(this);

	ui->bot_list_view->setModel(m_bot_list_model);
	ui->bot_list_view->hideColumn(1); // Path
	ui->bot_list_view->hideColumn(2); // Repo

	connect(ui->add_bot_button, &QPushButton::clicked, m_script_manager_dialog, &QDialog::exec);
	connect(ui->add_bot_action, &QAction::triggered, m_script_manager_dialog, &QDialog::exec);
	connect(ui->del_bot_action, &QAction::triggered, this, &MainWindow::deleteSelectedBot);
	connect(ui->run_bot_action, &QAction::triggered, this, &MainWindow::startSelectedBot);
	connect(ui->stop_bot_action, &QAction::triggered, this, &MainWindow::stopSelectedBot);
	connect(ui->add_local_action, &QAction::triggered, this, &MainWindow::addLocalBot);

	connect(m_script_manager_dialog, &ScriptManagerDialog::botCreated, m_bot_list_model, &BotListModel::list);
	connect(m_bot_list_model, &BotListModel::botListed, this, &MainWindow::setupWidgetForBot);
	connect(m_bot_list_model, &BotListModel::botRemoved, this, &MainWindow::removeWidgetForBot);
	connect(ui->bot_list_view->selectionModel(), &QItemSelectionModel::currentRowChanged, this, &MainWindow::handleRowChange);

	connect(ui->open_browser_action, &QAction::triggered, m_browser_window, &BrowserWindow::show);
	connect(ui->open_android_action, &QAction::triggered, m_android_dialog, &AndroidDialog::show);
	connect(ui->settings_action, &QAction::triggered, m_settings_dialog, &SettingsDialog::exec);
	connect(ui->about_qt_action, &QAction::triggered, [this](){ QMessageBox::aboutQt(this); });
	connect(ui->about_action, &QAction::triggered, this, &MainWindow::showAboutDialog);
	connect(ui->quit_action, &QAction::triggered, QApplication::instance(), &QApplication::quit);

	connect(ui->menuFile, &QMenu::aboutToShow, m_maintainance_tool, &MtoolWrapper::findExecutable);
	connect(m_maintainance_tool, &MtoolWrapper::executableFound, ui->update_action, &QAction::setEnabled);
	connect(ui->update_action, &QAction::triggered, m_maintainance_tool, &MtoolWrapper::startDetachedUpdater);
	connect(m_maintainance_tool, &MtoolWrapper::startedDetached, QApplication::instance(), &QApplication::quit);

	connect(ui->login_action, &QAction::triggered, m_auth_dialog, &AuthDialog::exec);
	connect(ui->logout_action, &QAction::triggered, [this](){ adjustLimitations(); });
	connect(ui->logout_action, &QAction::triggered, [this]() { ui->logout_action->setVisible(false); ui->login_action->setVisible(true); });
	connect(m_auth_dialog, &AuthDialog::authenticated, [this]() { ui->logout_action->setVisible(true); ui->login_action->setVisible(false); });
	connect(m_auth_dialog, &AuthDialog::authenticated, this, &MainWindow::adjustLimitations);
	connect(m_auth_dialog, &AuthDialog::triedAutoLogin, [this](){ ui->menuAccount->setEnabled(true); });
	adjustLimitations(false);
	QTimer::singleShot(0, m_auth_dialog, &AuthDialog::tryAutoLogin);

	connect(QApplication::instance(), &QApplication::aboutToQuit, [this]() {
		// Store the BotListModels data before the application quits
		m_bot_list_model->save(m_bot_list_model->defaultLocation());
	});

	QTimer::singleShot(1, this, [this]() {
		// Don't block the constructor while loading model data
		m_bot_list_model->load(m_bot_list_model->defaultLocation());
	});
}

MainWindow::~MainWindow()
{
	m_browser_window->deleteLater(); // Has no parent
	delete ui;
}

void MainWindow::handleRowChange(const QModelIndex &current, const QModelIndex &)
{
	QString bot_path = m_bot_list_model->data(current, BotListModel::BOT_PATH_ROLE).toString();
	focusWidgetForBot(bot_path);
}

void MainWindow::deleteSelectedBot()
{
	QModelIndex model_index = ui->bot_list_view->selectionModel()->currentIndex();
	if (!model_index.isValid()) return;

	Bot *bot = qvariant_cast<Bot*>(m_bot_list_model->data(model_index, BotListModel::BOT_PTR_ROLE));
	Q_ASSERT(bot);

	if (bot->isRunning())
	{
		qDebug() << "Can't delete running bot";
		return;
	}

	m_bot_list_model->removeRow(model_index.row());
}

void MainWindow::startSelectedBot()
{
	QModelIndex model_index = ui->bot_list_view->selectionModel()->currentIndex();
	QString bot_path = m_bot_list_model->data(model_index, BotListModel::BOT_PATH_ROLE).toString();

	BotWidget *bot_widget = m_bot_path_to_widget_map[bot_path];
	Q_ASSERT(bot_widget);

	bot_widget->tryBotStart(120); // FIXME: apply real limitations.
}

void MainWindow::stopSelectedBot()
{
	QModelIndex model_index = ui->bot_list_view->selectionModel()->currentIndex();
	QString bot_path = m_bot_list_model->data(model_index, BotListModel::BOT_PATH_ROLE).toString();

	BotWidget *bot_widget = m_bot_path_to_widget_map[bot_path];
	Q_ASSERT(bot_widget);

	bot_widget->tryBotStop();
}

void MainWindow::setupWidgetForBot(Bot *bot)
{
	Q_ASSERT(bot);
	BotWidget *bot_widget = new BotWidget(bot);
	ui->stackedWidget->addWidget(bot_widget); // QtDocs: Ownership of widget is passed on to the QStackedWidget.
	m_bot_path_to_widget_map.insert(bot->path(), bot_widget);
}

void MainWindow::focusWidgetForBot(const QString &bot_path)
{
	BotWidget *bot_widget = m_bot_path_to_widget_map[bot_path];
	if (!bot_widget)
	{
		qDebug() << "Fallback to default widget, because path" << bot_path << "did not correspond to a widget.";
		ui->stackedWidget->setCurrentIndex(0);
		return;
	}
	ui->stackedWidget->setCurrentWidget(bot_widget);
}

void MainWindow::removeWidgetForBot(const QString &bot_path)
{
	BotWidget *bot_widget = m_bot_path_to_widget_map[bot_path];
	Q_ASSERT(bot_widget);
	bot_widget->saveBotSettings();
	ui->stackedWidget->removeWidget(bot_widget);
}

void MainWindow::addLocalBot()
{
	QString bot_path = QFileDialog::getExistingDirectory(this, "Select a bot folder");
	if (bot_path.isEmpty())
	{
		qDebug() << "No local bot path selected";
		return;
	}

	// Use the bots filename as name
	QFileInfo bot_file(bot_path);
	QString bot_name = bot_file.fileName();

	Bot::Data bot_data(bot_path, bot_name, QString());
	m_bot_list_model->list(bot_data);
}

void MainWindow::adjustLimitations(bool is_premium)
{
	if (is_premium)
	{
		ui->status_action->setText("Status: 1 bot, no time limit");
	}
	else
	{
		ui->status_action->setText("Status: 1 bot, stops after 90 minutes");
	}
}

void MainWindow::showAboutDialog()
{
	QString text = QString(
		"<html><body>"
		"<p><b>Botfather</b> is a universal automation framework for creating and running bots playing online games, android apps and more.</p>"
		"<p><a href='https://botfather.io/'>Website</a> | <a href='https://botfather.io/docs/legal/privacy/'>Privacy</a> | <a href='https://botfather.io/docs/legal/tos/'>EULA</a> | Version: %1</p>"
		"<b>Software used by botfather:</b>"
		"<p>Those beautiful application icons are made by Icons8.<br>"
		"<a href='https://icons8.com/'>website</a> | <a href='https://icons8.com/license/'>license</a></p>"
		"<p>Chromium Embedded Framework. Copyright (c) 2008-2014 Marshall A. Greenblatt.<br>"
		"<a href='https://bitbucket.org/chromiumembedded/cef'>website</a> | <a href='https://bitbucket.org/chromiumembedded/cef/src/master/LICENSE.txt'>license</a></p>"
		"<p>OpenSSL. Copyright © 1998-2018 The OpenSSL Project.<br>"
		"<a href='https://www.openssl.org/'>website</a> | <a href='https://www.openssl.org/source/license.html'>license</a></p>"
		"<p>Libgit2. Copyright (C) the libgit2 contributors.<br>"
		"<a href='https://libgit2.org/'>website</a> | <a href='https://github.com/libgit2/libgit2/blob/master/COPYING'>license</a></p>"
		"</body></html>"
	).arg(QCoreApplication::applicationVersion());

	QMessageBox::about(this, "About Botfather", text);
}
