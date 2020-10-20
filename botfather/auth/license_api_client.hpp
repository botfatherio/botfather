#ifndef BFP_AUTH_LICENSE_API_CLIENT_H
#define BFP_AUTH_LICENSE_API_CLIENT_H

#include "rest_api_client.hpp"

class LicenseApiClient : public RestApiClient {
    Q_OBJECT

   public:
    using RestApiClient::RestApiClient;

    bool isLicenseActive() const;

    bool isBotRuntimeLimited() const;
    int allowedBotRuntimeInSecs() const;

    bool isNumberOfRunningBotsLimited() const;
    int maxNumberOfRunningBotsAllowed() const;

   public slots:
    void requestLicense(const QString &software, const QString &username,
                        const QString &password);
    void resetLicense();

   signals:
    void licenseReceived(int curtime, int premend);
    void errorsReceived(QJsonArray error_codes);

   protected:
    QUrl provideApiEndpoint() override;
    void processJsonResponse(QJsonDocument json) override;

   private:
    int m_curtime = 1;
    int m_premend = 0;
};

#endif  // BFP_AUTH_LICENSE_API_CLIENT_H
