#ifndef ANDROID_DIALOG_H
#define ANDROID_DIALOG_H

#include <QDialog>

namespace Ui {
class AndroidDialog;
}

class AndroidDialog : public QDialog
{
	Q_OBJECT
	
public:
	explicit AndroidDialog(QWidget *parent = 0);
	~AndroidDialog();
	
public slots:
	void refreshListOfDevicesAttached();
	void save();
	
private:
	Ui::AndroidDialog *ui;
};

#endif // ANDROID_DIALOG_H
