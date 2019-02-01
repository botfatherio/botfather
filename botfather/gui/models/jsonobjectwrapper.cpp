#include "jsonobjectwrapper.h"

JsonObjectWrapper::JsonObjectWrapper()
{

}

JsonObjectWrapper::JsonObjectWrapper(const QJsonObject &json_object) : m_json_object(json_object)
{

}

bool JsonObjectWrapper::isNull() const
{
	return m_json_object.isEmpty();
}

QVariant JsonObjectWrapper::getVariantByKey(const QString &property_key) const
{
	if (!jsonObject().contains(property_key) || jsonObject()[property_key].isUndefined())
	{
		return QVariant();
	}
	return jsonObject()[property_key].toVariant();
}

QString JsonObjectWrapper::getStringBykey(const QString &property_key) const
{
	if (!jsonObject().contains(property_key) || !jsonObject()[property_key].isString())
	{
		return QString();
	}
	return jsonObject()[property_key].toString();
}

QJsonObject JsonObjectWrapper::jsonObject() const
{
	return m_json_object;
}

void JsonObjectWrapper::setJsonObject(const QJsonObject &json_object)
{
	m_json_object = json_object;
}
