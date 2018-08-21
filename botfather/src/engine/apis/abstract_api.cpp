#include "abstract_api.h"

AbstractAPI::AbstractAPI(Bot *bot, QObject *parent)
	: QObject(parent)
	, bot_p(bot)
{
	
}

Bot* AbstractAPI::bot() const
{
	return bot_p;
}