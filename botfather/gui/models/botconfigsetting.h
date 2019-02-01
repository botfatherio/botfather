#ifndef BOTCONFIGSETTING_H
#define BOTCONFIGSETTING_H

#include <QVariant>
#include <QMap>
#include "jsonobjectwrapper.h"

class BotConfigSetting : public JsonObjectWrapper
{
public:
	using JsonObjectWrapper::JsonObjectWrapper;

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

	bool isValid() const;
	QString id() const;
	Type type() const;
	QString label() const;
	QVariant fallback() const;
	QMap<QString, QString> choices() const;
};

#endif // BOTCONFIGSETTING_H
