#ifndef BOTCONFIGGROUP_H
#define BOTCONFIGGROUP_H

#include <QVector>
#include "botmanifestnode.hpp"
#include "botconfigoption.hpp"

class BotConfigGroup : public BotManifestNode
{
public:
	using BotManifestNode::BotManifestNode;

	QString name() const;
	QVector<BotConfigOption> options() const;
};

#endif // BOTCONFIGGROUP_H
