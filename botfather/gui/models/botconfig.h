#ifndef BOTCONFIG_H
#define BOTCONFIG_H

#include "jsonobjectwrapper.h"
#include "botconfiggroup.h"
#include <QVector>

class BotConfig : public JsonObjectWrapper
{
public:
	using JsonObjectWrapper::JsonObjectWrapper;
	QVector<BotConfigGroup> groups() const;
};

#endif // BOTCONFIG_H
