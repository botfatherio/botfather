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

	// Returns true when there is a value stored for this setting.
	bool isStored() const;

	bool isValid() const;
	QString id() const;
	Type type() const;
	QString label() const;
	QVariant fallback() const;
	QMap<QString, QString> choices() const;

	// Returns true if the setting has a valid range.
	bool hasRange() const;

	// Returns the settings (range) min value
	int min() const;

	// Returns the settings (range) max value
	int max() const;
};

#endif // BOTCONFIGSETTING_H
