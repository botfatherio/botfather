#include "auth_dialog.h"
#include "ui_auth_dialog.h"
#include <QApplication>
#include <QMessageBox>
#include <QCloseEvent>
#include <QDebug>
#include <QSettings>
#include "../auth/auth_settings.h"
#include "../auth/authenticator.h"

AuthDialog::AuthDialog(QString software_slug, QString version_string, QString version_secret, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::AuthDialog)
{
	ui->setupUi(this);
	setModal(true);
	int h = height();
	adjustSize();
	setFixedSize(width(), h);
	
	authenticator = new Authenticator(software_slug, version_string, version_secret, this);
	connect(authenticator, SIGNAL(networkError(QNetworkReply::NetworkError)), this, SLOT(onNetworkError(QNetworkReply::NetworkError)));
	connect(authenticator, SIGNAL(integrityError()), this, SLOT(onIntegrityError()));
	connect(authenticator, SIGNAL(remoteApiError(QJsonArray)), this, SLOT(onRemoteApiError(QJsonArray)));
	connect(authenticator, SIGNAL(authenticated(int,int,bool)), this, SLOT(onAuthenticated(int,int,bool)));
	
	// On error after trying to autologin the auth dialog must be shown to the user.
	// This will also be triggered if the an error occours while the auth window is
	// already shown to the user. But thats not an problem.
	connect(authenticator, SIGNAL(networkError(QNetworkReply::NetworkError)), this, SLOT(show()));
	connect(authenticator, SIGNAL(integrityError()), this, SLOT(show()));
	connect(authenticator, SIGNAL(remoteApiError(QJsonArray)), this, SLOT(show()));
	
	// Make hitting the enter button trigger the authenticate process.
	connect(ui->username, SIGNAL(returnPressed()), SLOT(on_login_pressed()));
	connect(ui->password, SIGNAL(returnPressed()), SLOT(on_login_pressed()));
	
	QSettings settings;
	QString username = settings.value(auth::options::USERNAME).toString();
	QString password = settings.value(auth::options::PASSWORD).toString();
	bool remember_me = settings.value(auth::options::REMEMBER_ME, false).toBool();
	
	this->ui->username->setText(username);
	this->ui->password->setText(password);
	this->ui->remember_me->setChecked(remember_me);
}

AuthDialog::~AuthDialog()
{
	delete ui;
}

void AuthDialog::allowInput(bool input_allowed)
{
	this->ui->login->setEnabled(input_allowed);
	this->ui->username->setEnabled(input_allowed);
	this->ui->password->setEnabled(input_allowed);
	this->ui->remember_me->setEnabled(input_allowed);
}

void AuthDialog::tryAutoLogin()
{
	QSettings settings;
	
	// Check whether autologin requirements are met. We don't want to trigger the
	// auth process if the username or password field is empty. Because this would
	// end up in an error message being presented to the user who didn't even hit
	// any button yet.
	if (!settings.value(auth::options::REMEMBER_ME, false).toBool()
		|| ui->username->text().isEmpty() || ui->password->text().isEmpty()) {
		
		// Present the auth dialog to the user if auto login requirements are not met.
		show();
		return;
	}
	
	// All auto login requirements are met. We do not expect any error messages not
	// related to authentication errors.
	on_login_pressed();
}

void AuthDialog::on_login_pressed()
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
	
	// Disable the button until we received a message to prevent spamming.
	allowInput(false);
	authenticator->authenticate(username, password);
}

void AuthDialog::on_remember_me_toggled(bool checked)
{
	QSettings settings;
	settings.setValue(auth::options::REMEMBER_ME, checked);
	if (!checked) {
		settings.remove(auth::options::USERNAME);
		settings.remove(auth::options::PASSWORD);
	}
}

void AuthDialog::closeEvent(QCloseEvent *event)
{
	QMessageBox::StandardButton reply;
	QString message("Are you sure you want to exit BotFather?");
	reply = QMessageBox::question(this, "Exit BotFather?", message, QMessageBox::Yes|QMessageBox::No, QMessageBox::No);
	if (reply == QMessageBox::Yes) {
		QApplication::quit();
	}
	else {
		event->ignore();
	}
}

void AuthDialog::onNetworkError(QNetworkReply::NetworkError network_error)
{
	QString error_message = QString("Network errors occured. Please check your Internet Connection. (Internal error code: %1)").arg(network_error);
	QMessageBox::critical(this, "Network error", error_message);
	allowInput(true);
}

void AuthDialog::onIntegrityError()
{
	QString message = QString("Ooops. Looks like something manipulated the response from our license server.");
	QMessageBox::critical(this, "Ooops", message);
	allowInput(true);
}

void AuthDialog::onRemoteApiError(QJsonArray error_codes)
{
	static const QMap<QString, QString> error_messages = {
		{"username_missing", "Username missing (this is an bug, please report it)."},
		{"password_missing", "Password missing (this is an bug, please report it)."},
		{"bot_missing", "Software slug missing (this is an bug, please report it)."},
		{"version_missing", "Version string missing (this is an bug, please report it)."},
		{"magic_missing", "Magic missing (this is an bug, please report it)."},
		{"auth_failed", "Authentication failed. Please check your username and password. Also make sure your account is active and your email address confirmed."},
		{"bot_unknown", "Unknown software. This client is probably broken. Visit botfather.io to get a new one."},
		{"version_unknown", "Unknown version. This client is probably broken. Visit botfather.io to get a new one."},
		{"version_unsupported", "This version is no longer supported. Please get the latest version from botfather.io"},
		{"version_offline", "This version is CURRENTLY offline. "},
		{"user_inactive", "Your account is inactive. Make sure you confirmed your email address."} // This will never be sent, as the auth fails when the account is inactive.
	};
	
	for (QJsonValue error_code : error_codes) {
		QMessageBox message_box;
		// Show the error message matching the error code, or the show the error code instead if there is no matching error message.
		message_box.setText(error_messages.value(error_code.toString(), error_code.toString()));
		message_box.setIcon(QMessageBox::Warning);
		message_box.exec();
	}
	
	allowInput(true);
}

void AuthDialog::onAuthenticated(int curtime, int premend, bool stable)
{
	// Login successfully, save the login info if remember me is checked.
	QSettings settings;
	if (settings.value(auth::options::REMEMBER_ME, false).toBool()) {
		settings.setValue(auth::options::USERNAME, this->ui->username->text());
		settings.setValue(auth::options::PASSWORD, this->ui->password->text());
	}
	
	// TODO: Reenalbe this one we actually sell premium licenses.
	/*
	if (curtime > premend || premend == 0){
		// => The user has no active premium license for this bot.
		// premend is 0 by default and > 0 if the user once bought a license.
		QMessageBox message_box;
		if (premend > 0){
			// => The user once had a license for this bot.
			message_box.setText("Unfortunately your license expired. Anyway, enjoy the trial version <3");
		} else {
			message_box.setText("Unfortunately you do not have a license. Anyway, enjoy the free version <3");
		}
		message_box.setIcon(QMessageBox::Information);
		message_box.exec();
	}
	*/
	
	emit authenticated(curtime, premend, stable);
	accept();
}