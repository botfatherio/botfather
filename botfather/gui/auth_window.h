#ifndef BFP__AUTH_WINDOW_H
#define BFP__AUTH_WINDOW_H

#include <QWidget>
#include <QString>
#include <QNetworkReply>
#include <QJsonArray>

namespace Ui {
class AuthWindow;
}

class AuthWindow : public QWidget
{
	Q_OBJECT
	
public:
	explicit AuthWindow(QString software_slug, QString version_string, QString version_secret, QWidget *parent = 0);
	~AuthWindow();

public slots:
	void on_authenticate_pressed();
	void networkReplyReceived(QNetworkReply* reply);
	void on_remember_me_toggled(bool checked);
	
signals:
	void permitted(bool stable, bool trial);
	
private:
	void loadSettings();
	QString getRandomString(int random_string_length) const;
	bool verifyHash(QString hashhex, int premend, int curtime) const;
	void handleErrors(QJsonArray errors) const;
	Ui::AuthWindow *ui;
	QString software_slug;
	QString version_string;
	QString version_secret;
	QString magic;
	QString username;
	QString password;
};

#endif // BFP__AUTH_WINDOW_H
