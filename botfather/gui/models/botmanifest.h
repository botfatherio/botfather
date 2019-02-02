#ifndef BOTMANIFEST_H
#define BOTMANIFEST_H

#include "botmanifestnode.h"
#include "botconfig.h"

class BotManifest : public BotManifestNode
{
public:
	BotManifest(const QString &config_path);
	bool loadFromFile(const QString &manifest_path);

	QString version() const;
	BotConfig config() const;
};

#endif // BOTMANIFEST_H
