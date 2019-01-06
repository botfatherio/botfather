#ifndef BFP_GUI_AUTH_DIALOG_H
#define BFP_GUI_AUTH_DIALOG_H

#include <QDialog>
#include <QNetworkReply>
#include <QJsonArray>

class LicenseApiClient;

namespace Ui {
	class AuthDialog;
}

class AuthDialog : public QDialog
{
	Q_OBJECT
	
public:
	explicit AuthDialog(QWidget *parent = nullptr);
	~AuthDialog();
	
signals:
	void authenticated(int curtime, int premend);
	void triedAutoLogin();
	
public slots:
	void allowInput(bool input_allowed);
	
	void tryAutoLogin();
	void login();
	void rememberMe(bool checked);

	void onNetworkError(QNetworkReply::NetworkError network_error);
	void onErrorsReceived(QJsonArray error_codes);
	void onLicenseReceived(int curtime, int premend);

private:
	Ui::AuthDialog* ui;
	LicenseApiClient* license_api;
	QString software = "botfather";
};

#endif // BFP_GUI_AUTH_DIALOG_H