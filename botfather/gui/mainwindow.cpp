#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCloseEvent>
#include <QTimer>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	m_bot_list_model = new BotListModel(this);
	ui->bot_list_view->setModel(m_bot_list_model);

	ui->bot_list_view->hideColumn(1); // Path
	ui->bot_list_view->hideColumn(2); // Repo

	m_script_manager_dialog = new ScriptManagerDialog(this);

	connect(ui->add_bot_button, &QPushButton::clicked, m_script_manager_dialog, &QDialog::exec);
	connect(ui->add_bot_action, &QAction::triggered, m_script_manager_dialog, &QDialog::exec);
	connect(m_script_manager_dialog, &ScriptManagerDialog::botCreated, m_bot_list_model, &BotListModel::list);

	connect(QApplication::instance(), &QApplication::aboutToQuit, [this]() {
		m_bot_list_model->save(m_bot_list_model->defaultLocation());
	});

	QTimer::singleShot(1, this, [this]() { // Don't block the constructor while loading model data
		m_bot_list_model->load(m_bot_list_model->defaultLocation());
	});
}

MainWindow::~MainWindow()
{
	delete ui;
}
