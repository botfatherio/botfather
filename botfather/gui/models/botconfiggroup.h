#ifndef BOTCONFIGGROUP_H
#define BOTCONFIGGROUP_H

#include <QVector>
#include "botmanifestnode.h"
#include "botconfigoption.h"

class BotConfigGroup : public BotManifestNode
{
public:
	using BotManifestNode::BotManifestNode;

	QString name() const;
	QVector<BotConfigOption> options() const;
};

#endif // BOTCONFIGGROUP_H
