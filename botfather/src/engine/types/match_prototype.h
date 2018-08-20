#ifndef ENGINE_TYPES_MATCH_PROTOTYPE_H
#define ENGINE_TYPES_MATCH_PROTOTYPE_H
#define MATCH_PROTOTYPE_DOCS QString("https://botfather.io/docs/apiref/match/")

#include "rect_prototype.h"
#include "match.h"

class MatchPrototype : public RectPrototype
{
	Q_OBJECT
	
public:
	static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);
	using RectPrototype::RectPrototype;
	
	Q_INVOKABLE double getScore() const;
	Q_INVOKABLE void setScore(double score);
	
	Q_INVOKABLE bool found() const;
	
	Q_INVOKABLE QString toString() const;
};

#endif // ENGINE_TYPES_MATCH_PROTOTYPE_H
