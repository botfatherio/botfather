#ifndef BOTCONFIG_H
#define BOTCONFIG_H

#include <QVector>

#include "bot_config_group.hpp"
#include "bot_manifest_node.hpp"

class BotConfig : public BotManifestNode {
   public:
    using BotManifestNode::BotManifestNode;

    // Returns true when atleast one valid config option could be found
    bool isValid() const;

    // Returns the config groups
    QVector<BotConfigGroup> groups() const;
};

#endif  // BOTCONFIG_H
