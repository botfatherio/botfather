#ifndef BFP_ENGINE_PROTOTYPES_MATCH_PROTOTYPE_H
#define BFP_ENGINE_PROTOTYPES_MATCH_PROTOTYPE_H
#define MATCH_PROTOTYPE_DOCS QString("https://botfather.io/docs/apiref/match/")

#include "abstract_prototype.h"
#include "../types/match.h"

class MatchPrototype : public AbstractPrototype
{
	Q_OBJECT
	
public:
	static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);
	using AbstractPrototype::AbstractPrototype;
	
	Q_INVOKABLE double getScore() const;
	Q_INVOKABLE void setScore(double score);

	Q_INVOKABLE QRect getRect() const;
	Q_INVOKABLE void setRect(const QRect &rect);
	
	Q_INVOKABLE bool found() const;
	
	Q_INVOKABLE QString toString() const;
};

#endif // BFP_ENGINE_PROTOTYPES_MATCH_PROTOTYPE_H
