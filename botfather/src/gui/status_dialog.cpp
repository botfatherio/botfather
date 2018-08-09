#include "status_dialog.h"
#include "ui_status_dialog.h"
#include <QMessageBox>
#include <QStandardPaths>
#include <QProcess>
#include "../auth/status_api_client.h"

StatusDialog::StatusDialog(QString version_secret, QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::StatusDialog)
{
	ui->setupUi(this);
	status_api_client = new StatusApiClient("botfather", QCoreApplication::applicationVersion(), version_secret, this);
	connect(status_api_client, &StatusApiClient::networkError, this, &StatusDialog::handleNetworkError);
	connect(status_api_client, &StatusApiClient::restApiError, this, &StatusDialog::handleRestApiError);
	connect(status_api_client, &StatusApiClient::integrityError, this, &StatusDialog::handleIntegrityError);
	connect(status_api_client, &StatusApiClient::statusReceived, this, &StatusDialog::handleStatus);
}

StatusDialog::~StatusDialog()
{
	delete ui;
}

int StatusDialog::exec()
{
	status_api_client->requestStatus();
	return QDialog::exec();
}

void StatusDialog::show()
{
	QDialog::show();
	status_api_client->requestStatus();
}

void StatusDialog::open()
{
	QDialog::open();
	status_api_client->requestStatus();
}

void StatusDialog::handleNetworkError(QNetworkReply::NetworkError network_error)
{
	QString message = QString("Please check your internet connection and our website. (Error code: %1)").arg(network_error);
	QMessageBox::critical(this, "Network error", message);
	reject();
}

void StatusDialog::handleRestApiError(QJsonArray error_codes)
{
	Q_UNUSED(error_codes)
	QMessageBox::critical(this, "Version broken", "This program has trouble checking it's status. This might be a bug. Please consider updating or reinstalling.");
	reject();
}

void StatusDialog::handleIntegrityError()
{
	QMessageBox::critical(this, "Integrity error", "The response from our license server has been manipulated. Please check your internet connection and our website.");
	reject();
}

void StatusDialog::handleStatus(bool supported, bool stable, QString latest_stable_version)
{
	if (!supported) {
		QMessageBox::warning(this, "Version unsupported", "This version of the program is no longer supported. Please update to a supported version.");
	}	
	
	bool wants_to_update = false;
	
	if (stable && latest_stable_version != QCoreApplication::applicationVersion()) {
		// new stable version available
		wants_to_update = recommendUpdate();
	}

	if (!supported || wants_to_update) {
		reject();
	} else {
		accept();
	}
}

bool StatusDialog::recommendUpdate()
{
	QString mtool_path = QStandardPaths::findExecutable("maintenancetool", {"./"});
	
	if (mtool_path.isEmpty()) {
		QMessageBox::information(
			this,
			"Updates available",
			"Updates are availabe. We couldn't locate the maintenancetool though, please update manually."
		);
		return false;
	}
	
	QMessageBox::StandardButton answer = QMessageBox::question(
		this,
		"Updates available",
		"Updates available. Do you want to update now?",
		QMessageBox::Yes | QMessageBox::No
	);
	
	if (answer == QMessageBox::Yes) {
		QProcess::startDetached(mtool_path, {"--updater"});
		return true;
	}
	
	return false;
}