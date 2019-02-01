#ifndef BOTCONFIGSETTING_H
#define BOTCONFIGSETTING_H

#include <QVariant>
#include <QMap>
#include "botmanifestnode.h"

class BotConfigSetting : public BotManifestNode
{
public:
	using BotManifestNode::BotManifestNode;

	enum Type
	{
		String,
		Password,
		Boolean,
		Number,
		Integer,
		Choice,
		Unknown
	};

	QVariant value() const;
	void setValue(const QVariant &new_value);

	bool isValid() const;
	QString id() const;
	Type type() const;
	QString label() const;
	QVariant fallback() const;
	QMap<QString, QString> choices() const;
};

#endif // BOTCONFIGSETTING_H
