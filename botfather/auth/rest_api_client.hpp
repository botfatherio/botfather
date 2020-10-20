#ifndef BFP_AUTH_REST_API_CLIENT_H
#define BFP_AUTH_REST_API_CLIENT_H

#include <QJsonArray>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QVector>

class RestApiClient : public QObject {
    Q_OBJECT

   public:
    RestApiClient(QObject *parent = nullptr);
    virtual ~RestApiClient() {}

   public slots:
    void replyReceived(QNetworkReply *reply);

   signals:
    void networkError(QNetworkReply::NetworkError network_error);
    void finished();  // Will help thread to know when RestApiClient derivatives
                      // are done with their work

   protected:
    void sendPostData(QUrlQuery post_data);
    void prepareRequest(QNetworkRequest &network_request);
    // virtual void adjustSslConfiguration(QSslConfiguration &ssl_config);
    virtual QUrl provideApiEndpoint() = 0;
    // virtual QString certificateChecksum(QCryptographicHash::Algorithm);
    // bool verifyReply(QNetworkReply* reply);
    virtual void processJsonResponse(QJsonDocument json) = 0;

   private:
    QNetworkAccessManager *network_manager;
};

#endif  // BFP_AUTH_REST_API_CLIENT_H
