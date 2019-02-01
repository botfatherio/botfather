#ifndef JSONOBJECTWRAPPER_H
#define JSONOBJECTWRAPPER_H

#include <QJsonObject>
#include <QVector>
#include <QVariant>

class JsonObjectWrapper
{
public:
	JsonObjectWrapper();
	JsonObjectWrapper(const QJsonObject &json_object);

	bool isNull() const;

	// Returns the the value corresponding to the key if such exists, otherwise an null QVariant
	QVariant getVariantByKey(const QString &property_key) const;

	// Returns the the string value corresponding to the key if such exists, otherwise an null QString
	QString getStringBykey(const QString &property_key) const;

protected:
	QJsonObject jsonObject() const;
	void setJsonObject(const QJsonObject &json_object);

private:
	QJsonObject m_json_object;
};

#endif // JSONOBJECTWRAPPER_H
