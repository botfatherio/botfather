#ifndef BOTMANIFESTNODE_H
#define BOTMANIFESTNODE_H

#include <QJsonObject>
#include <QVariant>
#include <QVector>

class BotManifestNode {
   public:
    BotManifestNode();
    BotManifestNode(const QJsonObject &json_object, const QString &config_path);

    bool isNull() const;

    // Returns the the value corresponding to the key if such exists, otherwise
    // an null QVariant
    QVariant getVariantByKey(const QString &property_key) const;

    // Returns the the string value corresponding to the key if such exists,
    // otherwise an null QString
    QString getStringBykey(const QString &property_key) const;

    // Returns the integer value corresponding to the key if such exists,
    // otherwise 0
    int getIntegerByKey(const QString &property_key) const;

    // Returns the number value corresponding to the key if such exists,
    // otherwise 0
    double getNumberByKey(const QString &property_key) const;

   protected:
    QJsonObject jsonObject() const;
    void setJsonObject(const QJsonObject &json_object);
    QString configPath() const;

   private:
    QJsonObject m_json_object;
    QString m_config_path;
};

#endif  // BOTMANIFESTNODE_H
