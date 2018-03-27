#ifndef AUTH_DIALOG_H
#define AUTH_DIALOG_H

#include <QDialog>
#include <QNetworkReply>
#include <QJsonArray>

namespace Ui {
class AuthDialog;
}

class Authenticator;

class AuthDialog : public QDialog
{
	Q_OBJECT
	
public:
	explicit AuthDialog(QString software_slug, QString version_string, QString version_secret, QWidget *parent = 0);
	~AuthDialog();
	
signals:
	void remoteApiInfo(int curtime, int premend, bool stable);
	
public slots:
	void on_login_pressed();
	void on_remember_me_toggled(bool checked);
	void closeEvent(QCloseEvent *event);
	
	void onNetworkError(QNetworkReply::NetworkError network_error);
	void onRemoteApiError(QJsonArray error_codes);
	void onIntegrityError();
	void onAuthenticated(int curtime, int premend, bool stable);
	
private:
	Ui::AuthDialog *ui;
	Authenticator *authenticator;
};

#endif // AUTH_DIALOG_H