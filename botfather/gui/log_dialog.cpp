#include "log_dialog.h"
#include "ui_log_dialog.h"
#include <QFile>
#include <QDebug>

LogDialog::LogDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::LogDialog)
{
	ui->setupUi(this);
}

LogDialog::~LogDialog()
{
	delete ui;
}

void LogDialog::on_save_button_clicked()
{
	QString log = this->ui->log_text->toPlainText();
	
	QString filename = QFileDialog::getSaveFileName(
		this,
		tr("Save Logfile"),
		"",
		tr("Text files (*.txt *.log)"),
		Q_NULLPTR,
		// Triggering the file dialog more than once using the native dialog made the program get stuck.
		QFileDialog::DontUseNativeDialog
	);
	
	if (filename.isEmpty()){
		qDebug() << "No file selected.";
		return;
	}
	
	QFile file(filename);
	if (!file.open(QIODevice::WriteOnly)) {
		qDebug() << "Can't open logfile" << filename;
		return;
	}
	file.write(log.toUtf8());
	file.close();
}

void LogDialog::on_clear_button_clicked()
{
	this->ui->log_text->clear();
}

void LogDialog::on_close_button_clicked()
{
	this->close();
}

void LogDialog::appendMessage(QString log_message)
{
	this->ui->log_text->append(log_message);
}