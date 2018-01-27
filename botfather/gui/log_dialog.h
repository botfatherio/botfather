#ifndef LOG_DIALOG_H
#define LOG_DIALOG_H

#include <QDialog>
#include <QFileDialog>

namespace Ui {
class LogDialog;
}

class LogDialog : public QDialog
{
	Q_OBJECT
	
public:
	explicit LogDialog(QWidget *parent = 0);
	~LogDialog();

public slots:
	void on_save_button_clicked();
	void on_clear_button_clicked();
	void on_close_button_clicked();
	void appendMessage(QString log_message);
	
private:
	Ui::LogDialog *ui;
	QFileDialog file_dialog;
};

#endif // LOG_DIALOG_H
