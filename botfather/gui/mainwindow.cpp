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
	m_tray_icon = new QSystemTrayIcon(windowIcon(), this);
	m_bot_list_model = new BotListModel(this);
	m_script_manager_dialog = new ScriptManagerDialog(this);
	m_browser_window = new BrowserWindow; // Don't give it a parent, otherwise it's blocking the control window on microsoft windows
	m_android_dialog = new AndroidDialog(this);
	m_preferences_dialog = new PreferencesDialog(this);
	m_license_api_client = new LicenseApiClient(this);
	m_auth_dialog = new AuthDialog(m_license_api_client, this);
	m_maintenance_tool = new MaintenanceTool(this);

	ui->bot_list_view->setModel(m_bot_list_model);
	ui->bot_list_view->hideColumn(1); // Path
	ui->bot_list_view->hideColumn(2); // Repo
	ui->bot_list_view->setContextMenuPolicy(Qt::ContextMenuPolicy::ActionsContextMenu);
	ui->bot_list_view->addAction(ui->add_bot_action);
	ui->bot_list_view->addAction(ui->add_local_action);

	m_tray_icon->show();
	connect(m_tray_icon, &QSystemTrayIcon::activated, this, &MainWindow::raise);
	connect(m_tray_icon, &QSystemTrayIcon::messageClicked, this, &MainWindow::raise);
	connect(m_tray_icon, &QSystemTrayIcon::activated, this, &MainWindow::activateWindow);
	connect(m_tray_icon, &QSystemTrayIcon::messageClicked, this, &MainWindow::activateWindow);

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
	connect(ui->preferences_action, &QAction::triggered, m_preferences_dialog, &PreferencesDialog::exec);
	connect(ui->about_qt_action, &QAction::triggered, [this](){ QMessageBox::aboutQt(this); });
	connect(ui->about_action, &QAction::triggered, this, &MainWindow::showAboutDialog);
	connect(ui->quit_action, &QAction::triggered, QApplication::instance(), &QApplication::quit);

	ui->update_action->setEnabled(MaintenanceTool::exists());
	connect(m_maintenance_tool, &MaintenanceTool::updaterStarted, QApplication::instance(), &QApplication::quit);
	connect(ui->update_action, &QAction::triggered, m_maintenance_tool, &MaintenanceTool::startDetachedAsUpdater);

	connect(ui->login_action, &QAction::triggered, m_auth_dialog, &AuthDialog::exec);
	connect(ui->logout_action, &QAction::triggered, m_auth_dialog, &AuthDialog::logout);
	connect(ui->logout_action, &QAction::triggered, m_license_api_client, &LicenseApiClient::resetLicense);
	connect(ui->logout_action, &QAction::triggered, this, &MainWindow::updateLicenseInfo);
	connect(m_auth_dialog, &AuthDialog::authenticated, this, &MainWindow::updateLicenseInfo);
	connect(m_auth_dialog, &AuthDialog::triedAutoLogin, [this](){ ui->menuAccount->setEnabled(true); });
	updateLicenseInfo();
	QTimer::singleShot(0, m_auth_dialog, &AuthDialog::tryAutoLogin);

	// Check for updates and notify about vailable ones
	if (QSettings().value("check_for_updates", true).toBool() && !MaintenanceTool::filePath().isEmpty())
	{
		MaintenanceTool *mtool = new MaintenanceTool(this);
		connect(mtool, &MaintenanceTool::updatesAvailable, this, &MainWindow::notifyAboutUpdate);
		mtool->checkForUpdates();
	}

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
	if (!model_index.isValid()) return; // There might not be a single bot listed

	Bot *bot = qvariant_cast<Bot*>(m_bot_list_model->data(model_index, BotListModel::BOT_PTR_ROLE));
	Q_ASSERT(bot);

	if (bot->isRunning())
	{
		qDebug() << "Can't delete running bot";
		return;
	}

	// Ask whether the bots files shall be deleted
	QMessageBox::StandardButton clicked_button = QMessageBox::question(
		this,
		tr("Delete bot files?"),
		tr("Do you wan to delete the bots files, settings and config?\nNote: You can remove the bot without deleting its files."),
		QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel,
		QMessageBox::No
	);

	switch (clicked_button) {
	case QMessageBox::Yes:
		m_bot_list_model->removeRow(model_index.row());
		bot->deleteFiles();
		break;
	case QMessageBox::No:
		m_bot_list_model->removeRow(model_index.row());
		// Keep the bots files
		break;
	default:
		qDebug() << "Bot deletion canceled";
		return;
	}
}

void MainWindow::startSelectedBot()
{
	QModelIndex model_index = ui->bot_list_view->selectionModel()->currentIndex();
	if (!model_index.isValid()) return; // There might not be a single bot listed

	if (m_bot_list_model->numberOfRunningBots() >= m_license_api_client->maxNumberOfRunningBotsAllowed() && m_license_api_client->isNumberOfRunningBotsLimited())
	{
		QMessageBox::information(
			this,
			tr("Login to run more bots"),
			tr("Please login to run more than %0 bots at the same time.").arg(m_license_api_client->maxNumberOfRunningBotsAllowed())
		);
		return;
	}

	// TODO: Remove this warning once we implemented multi browser support
	if (m_bot_list_model->numberOfRunningBots() >= 1)
	{
		QMessageBox::information(
			this,
			tr("Limited functionality"),
			tr("Current all bots SHARE THE SAME BROWSER and Android device!\n\nSupport for multiple browsers and android devices will come with version 6.0.0")
		);
	}

	QString bot_path = m_bot_list_model->data(model_index, BotListModel::BOT_PATH_ROLE).toString();
	BotWidget *bot_widget = m_bot_path_to_widget_map[bot_path];

	Q_ASSERT(bot_widget);
	bot_widget->tryBotStart(m_license_api_client->allowedBotRuntimeInSecs());
}

void MainWindow::stopSelectedBot()
{
	QModelIndex model_index = ui->bot_list_view->selectionModel()->currentIndex();
	if (!model_index.isValid()) return; // There might not be a single bot listed

	QString bot_path = m_bot_list_model->data(model_index, BotListModel::BOT_PATH_ROLE).toString();
	BotWidget *bot_widget = m_bot_path_to_widget_map[bot_path];

	Q_ASSERT(bot_widget);
	bot_widget->tryBotStop();
}

void MainWindow::setupWidgetForBot(Bot *bot)
{
	Q_ASSERT(bot);
	BotWidget *bot_widget = new BotWidget(bot, m_tray_icon);
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
	QDir bot_dir(bot_path);
	QString bot_name = bot_dir.dirName();

	Bot::Data bot_data(bot_path, bot_name, QString());
	m_bot_list_model->list(bot_data);
}

void MainWindow::updateLicenseInfo()
{
	if (m_license_api_client->isLicenseActive())
	{
		QString status_info = tr("Status: unlimited bots, no time limit");
		ui->status_action->setText(status_info);
		ui->logout_action->setVisible(true);
		ui->login_action->setVisible(false);
	}
	else
	{
		int number_of_bots = m_license_api_client->maxNumberOfRunningBotsAllowed();
		int runtime_in_min = qRound(m_license_api_client->allowedBotRuntimeInSecs() / 60.0);
		QString status_info = tr("Status: %0 bot(s), %1min sessions per bot").arg(number_of_bots).arg(runtime_in_min);
		ui->status_action->setText(status_info);
		ui->logout_action->setVisible(false);
		ui->login_action->setVisible(true);
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

void MainWindow::notifyAboutUpdate()
{
	m_tray_icon->showMessage(
		tr("Updates available!"),
		tr("There are Botfather updates available. Install them to get the latest features and fixes."),
		windowIcon()
	);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	m_tray_icon->hide();
	event->accept();
}
