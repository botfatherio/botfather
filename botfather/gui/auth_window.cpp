#include "auth_window.h"
#include "ui_auth_window.h"
#include <QDebug>
#include <QSettings>
#include <QMessageBox>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QJsonDocument>
#include <QJsonObject>
#include <QCryptographicHash>
#include <QMap>

AuthWindow::AuthWindow(QString software_slug, QString version_string, QString version_secret, QWidget* parent) :
	QWidget(parent),
	ui(new Ui::AuthWindow)
{
	this->ui->setupUi(this);
	this->software_slug = software_slug;
	this->version_string = version_string;
	this->version_secret = version_secret;
	this->loadSettings();
	
	// Make hitting the enter button trigger the authenticate process.
	connect(this->ui->username, SIGNAL(returnPressed()), SLOT(on_authenticate_pressed()));
	connect(this->ui->password, SIGNAL(returnPressed()), SLOT(on_authenticate_pressed()));
}

AuthWindow::~AuthWindow()
{
	delete this->ui;
}

void AuthWindow::on_authenticate_pressed()
{
	this->username = this->ui->username->text();
	this->password = this->ui->password->text();
	
	if (this->username.isEmpty()){
		QMessageBox::warning(this, "Username missing", "Username missing. Please fill out all fields.");
		return;
	}
	
	if (this->password.isEmpty()){
		QMessageBox::warning(this, "Password missing", "Password missing. Please fill out all fields.");
		return;
	}
	
	// Disable the button until we received a message to prevent spamming.
	this->ui->authenticate->setEnabled(false);
	
	// Generate a random string the webservice will use to send back an salted hash of it's answer.
	this->magic = this->getRandomString(10);

	// Prepare post date to be send to the webservice
	QUrlQuery post_data;
	post_data.addQueryItem("username", this->username);
	post_data.addQueryItem("password", this->password);
	post_data.addQueryItem("software", this->software_slug);
	post_data.addQueryItem("version", this->version_string);
	post_data.addQueryItem("magic", this->magic);
	
	// Setup network manager to handle replies aswell as errors.
	QNetworkAccessManager *network_manager = new QNetworkAccessManager(this);
	connect(network_manager, SIGNAL(finished(QNetworkReply*)), SLOT(networkReplyReceived(QNetworkReply*)));
	
	// Send the request
	QNetworkRequest network_request(QUrl("https://botfather.io/api/v3/auth/"));
	network_request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
	network_manager->post(network_request, post_data.toString(QUrl::FullyEncoded).toUtf8());
}

void AuthWindow::networkReplyReceived(QNetworkReply *reply)
{
	// Reenable the authenticate button.
	this->ui->authenticate->setEnabled(true);
	
	// Check for network errors
	if (reply->error() != QNetworkReply::NoError){
		QString error_message = QString("Network errors occured. Please check your Internet Connection. (Internal error code: %1)").arg(reply->error());
		QMessageBox::critical(this, "Network error", error_message);
		reply->deleteLater();
		return;
	}	
	
	// Turn the replys plain text into json and free it's ressources.
	QByteArray response_data = reply->readAll();
	QJsonDocument json = QJsonDocument::fromJson(response_data);
	reply->deleteLater();
	
	QJsonArray errors = json.object().value("errors").toArray();
	if (!errors.isEmpty()){
		// => There are non network related errors.
		this->handleErrors(errors);
		return;
	}
	
	int curtime = json.object().value("curtime").toInt();
	int premend = json.object().value("premend").toInt();
	bool stable = json.object().value("stable").toBool();
	QString hashhex = json.object().value("hash").toString();
	
	if (!this->verifyHash(hashhex, premend, curtime)){
		// => This reply has been manipulated and was not send by our webservice.
		QMessageBox::critical(this, "Ooops", "Ooops. Looks like something manipulated the response from our license server.");
		return;
	}
	
	// Login successfully, save the login info if remember me is checked.
	QSettings settings;
	if (settings.value("auth/remember_me", false).toBool()) {
		settings.setValue("auth/username", this->ui->username->text());
		settings.setValue("auth/password", this->ui->password->text());
	}
	
	if (curtime > premend || premend == 0){
		// => The user has no active premium license for this bot.
		// premend is 0 by default and > 0 if the user once bought a license.
		QMessageBox message_box;
		
		if (premend > 0){
			// => The user once had a license for this bot.
			message_box.setText("Unfortunately your license for this bot expired. Enjoy the free version <3");
		} else {
			message_box.setText("Unfortunately you do not have a license for this bot. Enjoy the free version <3");
		}
		
		message_box.setIcon(QMessageBox::Information);
		message_box.exec();
		// return;
	}
	
	// The user is permitted to use the bot.
	emit this->permitted(stable, premend < curtime);
	this->close();
}

void AuthWindow::on_remember_me_toggled(bool checked)
{
	QSettings settings;
	settings.setValue("auth/remember_me", checked);
	if (!checked) {
		settings.remove("auth/username");
		settings.remove("auth/password");
	}
}

void AuthWindow::loadSettings()
{
	QSettings settings;
	this->ui->remember_me->setChecked(settings.value("auth/remember_me", false).toBool());
	this->ui->username->setText(settings.value("auth/username", QString()).toString());
	this->ui->password->setText(settings.value("auth/password", QString()).toString());
}

QString AuthWindow::getRandomString(int random_string_length) const
{
	const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
	QString random_string;
	for (int i=0; i<random_string_length; ++i) {
		int index = qrand() % possibleCharacters.length();
		QChar next_char = possibleCharacters.at(index);
		random_string.append(next_char);
	}
	return random_string;
}

bool AuthWindow::verifyHash(QString hashhex, int premend, int curtime) const
{
	QCryptographicHash hash(QCryptographicHash::Algorithm::Sha256);
	hash.addData(QString::number(premend).toUtf8());
	hash.addData(QString::number(curtime).toUtf8());
	hash.addData(this->magic.toUtf8());
	hash.addData(this->username.toUtf8());
	hash.addData(this->password.toUtf8());
	hash.addData(this->version_secret.toUtf8());
	return hash.result().toHex() == hashhex;
}

void AuthWindow::handleErrors(QJsonArray error_codes) const
{
	static const QMap<QString, QString> error_messages = {
		{"username_missing", "Username missing (this is an bug, please report it)."},
		{"password_missing", "Password missing (this is an bug, please report it)."},
		{"bot_missing", "Software slug missing (this is an bug, please report it)."},
		{"version_missing", "Version string missing (this is an bug, please report it)."},
		{"magic_missing", "Magic missing (this is an bug, please report it)."},
		{"auth_failed", "Authentication failed. Please check your botfather.io username and password."},
		{"bot_unknown", "Unknown software. This client is probably broken. Visit botfather.io to get a new one."},
		{"version_unknown", "Unknown version. This client is probably broken. Visit botfather.io to get a new one."},
		{"version_unsupported", "This version is no longer supported. Please get the latest version from botfather.io"},
		{"version_offline", "This version is CURRENTLY offline. "}
	};
	
	for (QJsonValue error_code : error_codes) {
		QMessageBox message_box;
		// Show the error message matching the error code, or the show the error code instead if there is no matching error message.
		message_box.setText(error_messages.value(error_code.toString(), error_code.toString()));
		message_box.setIcon(QMessageBox::Warning);
		message_box.exec();
	}
}