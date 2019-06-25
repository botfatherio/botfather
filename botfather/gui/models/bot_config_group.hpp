#ifndef BOTCONFIGGROUP_H
#define BOTCONFIGGROUP_H

#include <QVector>
#include "bot_manifest_node.hpp"
#include "bot_config_option.hpp"

class BotConfigGroup : public BotManifestNode
{
public:
	using BotManifestNode::BotManifestNode;

	QString name() const;
	QVector<BotConfigOption> options() const;
};

#endif // BOTCONFIGGROUP_H