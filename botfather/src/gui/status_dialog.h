#ifndef STATUS_DIALOG_H
#define STATUS_DIALOG_H

#include <QDialog>
#include <QNetworkReply>
#include <QJsonArray>

class StatusApiClient;

namespace Ui {
class StatusDialog;
}

class StatusDialog : public QDialog
{
	Q_OBJECT
	
public:
	explicit StatusDialog(QString version_secret, QWidget *parent = nullptr);
	~StatusDialog();
	
public slots:
	int exec();
	void show();
	void open();
	void handleNetworkError(QNetworkReply::NetworkError network_error);
	void handleRestApiError(QJsonArray error_codes);
	void handleIntegrityError();
	void handleStatus(bool supported, bool stable, QString latest_stable_version);
	
private:
	Ui::StatusDialog *ui;
	StatusApiClient *status_api_client;
	bool recommendUpdate();
};

#endif // STATUS_DIALOG_H