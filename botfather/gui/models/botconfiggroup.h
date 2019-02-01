#ifndef BOTCONFIGGROUP_H
#define BOTCONFIGGROUP_H

#include <QVector>
#include "jsonobjectwrapper.h"
#include "botconfigsetting.h"

class BotConfigGroup : public JsonObjectWrapper
{
public:
	using JsonObjectWrapper::JsonObjectWrapper;

	QString name() const;
	QVector<BotConfigSetting> settings() const;
};

#endif // BOTCONFIGGROUP_H
