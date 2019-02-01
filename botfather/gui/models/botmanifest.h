#ifndef BOTMANIFEST_H
#define BOTMANIFEST_H

#include "jsonobjectwrapper.h"
#include "botconfig.h"

class BotManifest : public JsonObjectWrapper
{
public:
	using JsonObjectWrapper::JsonObjectWrapper;
	bool loadFromFile(const QString &manifest_path);

	QString name() const;
	QString version() const;
	BotConfig config() const;
};

#endif // BOTMANIFEST_H
