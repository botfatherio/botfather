#include "botmanifestnode.hpp"

BotManifestNode::BotManifestNode()
{

}

BotManifestNode::BotManifestNode(const QJsonObject &json_object, const QString &config_path)
	: m_json_object(json_object)
	, m_config_path(config_path)
{

}

bool BotManifestNode::isNull() const
{
	return m_json_object.isEmpty();
}

QVariant BotManifestNode::getVariantByKey(const QString &property_key) const
{
	if (!jsonObject().contains(property_key) || jsonObject()[property_key].isUndefined())
	{
		return QVariant();
	}
	return jsonObject()[property_key].toVariant();
}

QString BotManifestNode::getStringBykey(const QString &property_key) const
{
	if (!jsonObject().contains(property_key) || !jsonObject()[property_key].isString())
	{
		return QString();
	}
	return jsonObject()[property_key].toString();
}

int BotManifestNode::getIntegerByKey(const QString &property_key) const
{
	if (!jsonObject().contains(property_key) || !jsonObject()[property_key].isDouble())
	{
		return 0;
	}
	return jsonObject()[property_key].toInt();
}

double BotManifestNode::getNumberByKey(const QString &property_key) const
{
	if (!jsonObject().contains(property_key) || !jsonObject()[property_key].isDouble())
	{
		return 0;
	}
	return jsonObject()[property_key].toDouble();
}

QJsonObject BotManifestNode::jsonObject() const
{
	return m_json_object;
}

void BotManifestNode::setJsonObject(const QJsonObject &json_object)
{
	m_json_object = json_object;
}

QString BotManifestNode::configPath() const
{
	return m_config_path;
}
