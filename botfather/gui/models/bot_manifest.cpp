#include "bot_manifest.hpp"

#include <QFile>
#include <QJsonDocument>

BotManifest::BotManifest(const QString &config_path)
    : BotManifestNode(QJsonObject(), config_path) {}

bool BotManifest::loadFromFile(const QString &manifest_path) {
    QFile manifest_file(manifest_path);
    if (!manifest_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }

    QString manifest_text = manifest_file.readAll();
    manifest_file.close();

    QJsonDocument json_document =
        QJsonDocument::fromJson(manifest_text.toUtf8());
    QJsonObject json_object = json_document.object();

    setJsonObject(json_object);
    return true;
}

QString BotManifest::scriptPath() const { return getStringBykey("script"); }

QString BotManifest::version() const {
    return getStringBykey("manifest_version");
}

BotConfig BotManifest::config() const {
    if (!jsonObject().contains("config")) {
        return BotConfig();
    }
    return BotConfig(jsonObject()["config"].toObject(), configPath());
}
