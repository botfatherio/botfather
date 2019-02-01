#ifndef BOTCONFIGGROUP_H
#define BOTCONFIGGROUP_H

#include <QVector>
#include "botmanifestnode.h"
#include "botconfigsetting.h"

class BotConfigGroup : public BotManifestNode
{
public:
	using BotManifestNode::BotManifestNode;

	QString name() const;
	QVector<BotConfigSetting> settings() const;
};

#endif // BOTCONFIGGROUP_H
