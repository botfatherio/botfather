#include "main_window.hpp"

#include <QCloseEvent>
#include <QDebug>
#include <QDesktopServices>
#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>

#include "color_finder/color_finder.hpp"
#include "ui_main_window.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    m_tray_icon = new QSystemTrayIcon(windowIcon(), this);
    m_bot_list_model = new BotListModel(this);
    m_script_manager_dialog = new ScriptManagerDialog(this);
    m_android_dialog = new AndroidDialog(this);
    m_preferences_dialog = new PreferencesDialog(this);
    m_maintenance_tool = new MaintenanceTool(this);

    ui->bot_list_view->setModel(m_bot_list_model);
    ui->bot_list_view->hideColumn(1);  // Path
    ui->bot_list_view->hideColumn(2);  // Repo
    ui->bot_list_view->setContextMenuPolicy(
        Qt::ContextMenuPolicy::ActionsContextMenu);
    ui->bot_list_view->addAction(ui->add_bot_action);
    ui->bot_list_view->addAction(ui->add_local_action);

    m_tray_icon->show();
    connect(m_tray_icon, &QSystemTrayIcon::activated, this, &MainWindow::raise);
    connect(m_tray_icon, &QSystemTrayIcon::messageClicked, this,
            &MainWindow::raise);
    connect(m_tray_icon, &QSystemTrayIcon::activated, this,
            &MainWindow::activateWindow);
    connect(m_tray_icon, &QSystemTrayIcon::messageClicked, this,
            &MainWindow::activateWindow);

    connect(ui->add_bot_button, &QPushButton::clicked, m_script_manager_dialog,
            &QDialog::exec);
    connect(ui->add_bot_action, &QAction::triggered, m_script_manager_dialog,
            &QDialog::exec);
    connect(ui->del_bot_action, &QAction::triggered, this,
            &MainWindow::deleteSelectedBot);
    connect(ui->run_bot_action, &QAction::triggered, this,
            &MainWindow::startSelectedBot);
    connect(ui->stop_bot_action, &QAction::triggered, this,
            &MainWindow::stopSelectedBot);
    connect(ui->add_local_action, &QAction::triggered, this,
            &MainWindow::addLocalBot);

    connect(m_script_manager_dialog, &ScriptManagerDialog::botCreated,
            m_bot_list_model, &BotListModel::list);
    connect(m_bot_list_model, &BotListModel::botListed, this,
            &MainWindow::setupWidgetForBot);
    connect(m_bot_list_model, &BotListModel::botRemoved, this,
            &MainWindow::removeWidgetForBot);
    connect(ui->bot_list_view->selectionModel(),
            &QItemSelectionModel::currentRowChanged, this,
            &MainWindow::handleRowChange);

    connect(ui->open_android_action, &QAction::triggered, m_android_dialog,
            &AndroidDialog::show);
    connect(ui->preferences_action, &QAction::triggered, m_preferences_dialog,
            &PreferencesDialog::exec);
    connect(ui->donate_action, &QAction::triggered, this,
            &MainWindow::openDonateLink);
    connect(ui->about_qt_action, &QAction::triggered,
            [this]() { QMessageBox::aboutQt(this); });
    connect(ui->about_action, &QAction::triggered, this,
            &MainWindow::showAboutDialog);
    connect(ui->open_color_finder, &QAction::triggered, this,
            &MainWindow::openColorFinder);
    connect(ui->quit_action, &QAction::triggered, QApplication::instance(),
            &QApplication::closeAllWindows);

    ui->update_action->setEnabled(MaintenanceTool::exists());
    connect(m_maintenance_tool, &MaintenanceTool::updaterStarted,
            QApplication::instance(), &QApplication::quit);
    connect(ui->update_action, &QAction::triggered, m_maintenance_tool,
            &MaintenanceTool::startDetachedAsUpdater);

    // Check for updates and notify about vailable ones
    if (QSettings().value("check_for_updates", true).toBool() &&
        !MaintenanceTool::filePath().isEmpty()) {
        MaintenanceTool *mtool = new MaintenanceTool(this);
        connect(mtool, &MaintenanceTool::updatesAvailable, this,
                &MainWindow::notifyAboutUpdate);
        QTimer::singleShot(7000, mtool, &MaintenanceTool::checkForUpdates);
    }

    connect(QApplication::instance(), &QApplication::aboutToQuit, [this]() {
        // Store the BotListModels data before the application quits
        m_bot_list_model->save(m_bot_list_model->defaultLocation());
    });

    // Don't block the constructor while loading model data
    QTimer::singleShot(1, this, [this]() {
        if (QFileInfo(m_bot_list_model->defaultLocation()).exists()) {
            m_bot_list_model->load(m_bot_list_model->defaultLocation());
        } else if (QFileInfo(m_bot_list_model->legacyLocation()).exists()) {
            m_bot_list_model->loadLegacy(m_bot_list_model->legacyLocation());
        }

        // Select the first bot in the list. (looks better visually)
        ui->bot_list_view->setCurrentIndex(m_bot_list_model->index(0));
    });
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::handleRowChange(const QModelIndex &current,
                                 const QModelIndex &) {
    QString bot_path =
        m_bot_list_model->data(current, BotListModel::BOT_PATH_ROLE).toString();
    focusWidgetForBot(bot_path);
    syncButtonsWithBotState();
}

void MainWindow::syncButtonsWithBotState() {
    QModelIndex model_index =
        ui->bot_list_view->selectionModel()->currentIndex();
    if (!model_index.isValid()) {
        // Not bot selected
        ui->del_bot_action->setEnabled(false);
        ui->run_bot_action->setEnabled(false);
        ui->stop_bot_action->setEnabled(false);
        return;
    }

    Bot *bot = qvariant_cast<Bot *>(
        m_bot_list_model->data(model_index, BotListModel::BOT_PTR_ROLE));
    Q_ASSERT(bot);

    ui->del_bot_action->setDisabled(bot->isRunning());
    ui->run_bot_action->setDisabled(bot->isRunning());
    ui->stop_bot_action->setEnabled(bot->isRunning());
}

void MainWindow::deleteSelectedBot() {
    QModelIndex model_index =
        ui->bot_list_view->selectionModel()->currentIndex();
    if (!model_index.isValid())
        return;  // There might not be a single bot listed

    Bot *bot = qvariant_cast<Bot *>(
        m_bot_list_model->data(model_index, BotListModel::BOT_PTR_ROLE));
    Q_ASSERT(bot);

    if (bot->isRunning()) {
        qDebug() << "Can't delete running bot";
        return;
    }

    // Ask whether the bots files shall be deleted
    QMessageBox::StandardButton clicked_button = QMessageBox::question(
        this, tr("Delete bot files?"),
        tr("Do you want to delete the bots files, settings and config?\nNote, "
           "you can remove the bot without deleting its files."),
        QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
        QMessageBox::No);

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

void MainWindow::startSelectedBot() {
    QModelIndex model_index =
        ui->bot_list_view->selectionModel()->currentIndex();
    if (!model_index.isValid())
        return;  // There might not be a single bot listed

    QString bot_path =
        m_bot_list_model->data(model_index, BotListModel::BOT_PATH_ROLE)
            .toString();
    BotWidget *bot_widget = m_bot_path_to_widget_map[bot_path];

    Q_ASSERT(bot_widget);
    bot_widget->tryBotStart();
}

void MainWindow::stopSelectedBot() {
    QModelIndex model_index =
        ui->bot_list_view->selectionModel()->currentIndex();
    if (!model_index.isValid())
        return;  // There might not be a single bot listed

    QString bot_path =
        m_bot_list_model->data(model_index, BotListModel::BOT_PATH_ROLE)
            .toString();
    BotWidget *bot_widget = m_bot_path_to_widget_map[bot_path];

    Q_ASSERT(bot_widget);
    bot_widget->tryBotStop();
}

void MainWindow::setupWidgetForBot(Bot *bot) {
    Q_ASSERT(bot);
    BotWidget *bot_widget = new BotWidget(bot, m_tray_icon);
    ui->stackedWidget->addWidget(
        bot_widget);  // QtDocs: Ownership of widget is passed on to the
                      // QStackedWidget.
    m_bot_path_to_widget_map.insert(bot->path(), bot_widget);

    connect(bot, &Bot::started, this, &MainWindow::syncButtonsWithBotState);
    connect(bot, &Bot::stopped, this, &MainWindow::syncButtonsWithBotState);
}

void MainWindow::focusWidgetForBot(const QString &bot_path) {
    BotWidget *bot_widget = m_bot_path_to_widget_map[bot_path];
    if (!bot_widget) {
        qDebug() << "Fallback to default widget, because path" << bot_path
                 << "did not correspond to a widget.";
        ui->stackedWidget->setCurrentIndex(0);
        return;
    }
    ui->stackedWidget->setCurrentWidget(bot_widget);
}

void MainWindow::removeWidgetForBot(const QString &bot_path) {
    BotWidget *bot_widget = m_bot_path_to_widget_map[bot_path];
    Q_ASSERT(bot_widget);
    ui->stackedWidget->removeWidget(bot_widget);
}

void MainWindow::addLocalBot() {
    QString bot_path =
        QFileDialog::getExistingDirectory(this, "Select a bot folder");
    if (bot_path.isEmpty()) {
        qDebug() << "No local bot path selected";
        return;
    }

    // Use the bots filename as name
    QDir bot_dir(bot_path);
    QString bot_name = bot_dir.dirName();

    m_bot_list_model->list(bot_path, bot_name, QString(), QString());
}

void MainWindow::openDonateLink() {
    QDesktopServices::openUrl(QUrl("https://buymeacoffee.com/je"));
}

void MainWindow::showAboutDialog() {
    QString text =
        QString(
            "<html><body>"
            "<p><b>Botfather</b> is a universal automation framework for "
            "creating and running bots playing online games, android apps and "
            "more.</p>"
            "<p><a href='https://botfather.io/'>Website</a> | <a "
            "href='https://botfather.io/privacy/'>Privacy</a> | <a "
            "href='https://botfather.io/terms/'>EULA</a> | Version: "
            "%1</p>"
            "<b>Software used by Botfather:</b>"
            "<p>Those beautiful application icons are made by Icons8.<br>"
            "<a href='https://icons8.com/'>website</a> | <a "
            "href='https://icons8.com/license/'>license</a></p>"
            "<p>Chromium Embedded Framework. Copyright (c) 2008-2014 Marshall "
            "A. Greenblatt.<br>"
            "<a href='https://bitbucket.org/chromiumembedded/cef'>website</a> "
            "| <a "
            "href='https://bitbucket.org/chromiumembedded/cef/src/master/"
            "LICENSE.txt'>license</a></p>"
            "<p>OpenSSL. Copyright © 1998-2018 The OpenSSL Project.<br>"
            "<a href='https://www.openssl.org/'>website</a> | <a "
            "href='https://www.openssl.org/source/license.html'>license</a></p>"
            "<p>Libgit2. Copyright (C) the libgit2 contributors.<br>"
            "<a href='https://libgit2.org/'>website</a> | <a "
            "href='https://github.com/libgit2/libgit2/blob/master/"
            "COPYING'>license</a></p>"
            "</body></html>")
            .arg(QCoreApplication::applicationVersion());

    QMessageBox::about(this, "About Botfather", text);
}

void MainWindow::openColorFinder() {
    ColorFinder *color_finder = new ColorFinder();
    color_finder->show();
}

void MainWindow::notifyAboutUpdate() {
    QMessageBox::information(
        this, tr("Updates available!"),
        tr("There are updates available! Install updates to get the latest "
           "features and fixes.\n(You can disable automatic update checks in "
           "the apps preferences)."));

    /* Seems like this currently doesn't work on win10
    m_tray_icon->showMessage(
            tr("Updates available!"),
            tr("There are Botfather updates available. Install them to get the
    latest features and fixes."), windowIcon()
    );
    */
}

void MainWindow::closeEvent(QCloseEvent *event) {
    m_tray_icon->hide();
    event->accept();

    // The application quits by default when all parentless windows are closed.
    // Usually we only have one parentless window, the MainWindow. However,
    // under MS Windows we need the BrowserWindows also to be parentless,
    // otherwise they are always displayed in front of the MainWindow. Thus
    // closing the MainWindow would not result in the application quitting. To
    // restore this behavious we explicitly close all windows when the
    // MainWindow closes. Note that explicitly quitting the application
    // sometimes caused crashes.
    QApplication::closeAllWindows();
}
