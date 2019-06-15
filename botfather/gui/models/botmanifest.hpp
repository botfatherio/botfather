#ifndef BOTMANIFEST_H
#define BOTMANIFEST_H

#include "botmanifestnode.hpp"
#include "botconfig.hpp"

class BotManifest : public BotManifestNode
{
public:
	BotManifest(const QString &config_path);
	bool loadFromFile(const QString &manifest_path);

	QString scriptPath() const;
	QString version() const;
	BotConfig config() const;
};

#endif // BOTMANIFEST_H
