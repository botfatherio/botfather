#ifndef BOTCONFIG_H
#define BOTCONFIG_H

#include "botmanifestnode.h"
#include "botconfiggroup.h"
#include <QVector>

class BotConfig : public BotManifestNode
{
public:
	using BotManifestNode::BotManifestNode;
	QVector<BotConfigGroup> groups() const;
};

#endif // BOTCONFIG_H
