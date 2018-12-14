#include "auth_dialog.h"
#include "ui_auth_dialog.h"
#include <QHash>
#include <QMessageBox>
#include <QDebug>
#include <QSettings>
#include "../../auth/auth_settings.h"
#include "../../auth/license_api_client.h"

AuthDialog::AuthDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::AuthDialog)
{
	ui->setupUi(this);
	int h = height();
	adjustSize();
	setFixedSize(width(), h);

	license_api = new LicenseApiClient(this);
	connect(license_api, &LicenseApiClient::networkError, this, &AuthDialog::onNetworkError);
	connect(license_api, &LicenseApiClient::errorsReceived, this, &AuthDialog::onErrorsReceived);
	connect(license_api, &LicenseApiClient::licenseReceived, this, &AuthDialog::onLicenseReceived);

	// Make hitting the enter button trigger the authenticate process.
	connect(ui->username, &QLineEdit::returnPressed, this, &AuthDialog::login);
	connect(ui->password, &QLineEdit::returnPressed, this, &AuthDialog::login);
	connect(ui->login, &QPushButton::pressed, this, &AuthDialog::login);
	connect(ui->remember_me, &QCheckBox::toggled, this, &AuthDialog::rememberMe);

	QSettings settings;
	QString username = settings.value(auth::options::USERNAME).toString();
	QString password = settings.value(auth::options::PASSWORD).toString();
	bool remember_me = settings.value(auth::options::REMEMBER_ME, false).toBool();
	
	ui->username->setText(username);
	ui->password->setText(password);
	ui->remember_me->setChecked(remember_me);
}

AuthDialog::~AuthDialog()
{
	delete ui;
}

void AuthDialog::allowInput(bool input_allowed)
{
	ui->login->setEnabled(input_allowed);
	ui->username->setEnabled(input_allowed);
	ui->password->setEnabled(input_allowed);
	ui->remember_me->setEnabled(input_allowed);
}

void AuthDialog::tryAutoLogin()
{
	// Try the auto login if the requirements to do so are met.
	QSettings settings;

	if (settings.value(auth::options::REMEMBER_ME, false).toBool() && !ui->username->text().isEmpty() && !ui->password->text().isEmpty())
	{
		// Show the login dialog when the auto login failes
		connect(license_api, &LicenseApiClient::networkError, this, &AuthDialog::show);
		connect(license_api, &LicenseApiClient::errorsReceived, this, &AuthDialog::show);

		login();
	}

	emit triedAutoLogin();
}

void AuthDialog::login()
{
	QString username = ui->username->text();
	QString password = ui->password->text();
	
	if (username.isEmpty()){
		QMessageBox::warning(this, "Username missing", "Username missing. Please fill out all fields.");
		return;
	}
	
	if (password.isEmpty()){
		QMessageBox::warning(this, "Password missing", "Password missing. Please fill out all fields.");
		return;
	}

	allowInput(false); // Prevent spamming until we receive an reply from the server
	license_api->requestLicense(software, username, password);
}

void AuthDialog::rememberMe(bool checked)
{
	QSettings settings;
	settings.setValue(auth::options::REMEMBER_ME, checked);
	if (!checked)
	{
		// Remove ones username and password from disk if they do no longer want them to be saved.
		settings.remove(auth::options::USERNAME);
		settings.remove(auth::options::PASSWORD);
	}
}

void AuthDialog::onNetworkError(QNetworkReply::NetworkError network_error)
{
	QString error_message = QString("Network errors occured. Please check your Internet Connection. (Internal error code: %1)").arg(network_error);
	QMessageBox::critical(this, "Network error", error_message);
	allowInput(true);
}

void AuthDialog::onErrorsReceived(QJsonArray error_codes)
{
	static const QHash<QString, QString> error_messages = {
		{"username_missing", "Username missing... (this is a bug, please report it)."},
		{"password_missing", "Password missing... (this is a bug, please report it)."},
		{"software_missing", "Software missing... (this is a bug, please report it)."},
		{"software_unknown", "Software unknown... (this is a bug, please report it)."},
		{"user_auth_failed", "Login failed. Please check your username and password."},
	};
	
	for (QJsonValue error_code : error_codes) {
		QMessageBox message_box;
		message_box.setText(error_messages.value(error_code.toString(), error_code.toString())); // Fallback to the error code
		message_box.setIcon(QMessageBox::Warning);
		message_box.exec();
	}
	
	allowInput(true);
}

void AuthDialog::onLicenseReceived(int curtime, int premend)
{
	// Login successfully, save the login info if remember me is checked.
	QSettings settings;
	if (settings.value(auth::options::REMEMBER_ME, false).toBool()) {
		settings.setValue(auth::options::USERNAME, this->ui->username->text());
		settings.setValue(auth::options::PASSWORD, this->ui->password->text());
	}
	emit authenticated(curtime, premend);
	allowInput(true);
	accept();
}
