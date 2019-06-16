#ifndef BFP_ENGINE_PROTOTYPES_MATCH_PROTOTYPE_H
#define BFP_ENGINE_PROTOTYPES_MATCH_PROTOTYPE_H
#define MATCH_PROTOTYPE_DOCS QString("https://botfather.io/docs/apiref/match/")

#include "../../common/prototypes/abstract_prototype.hpp"
#include "../types/match.hpp"

class MatchPrototype : public AbstractPrototype
{
	Q_OBJECT
	
public:
	static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);
	using AbstractPrototype::AbstractPrototype;
	
	// Returns the matches score. The score describes how good the match matches what has been searched for.
	// A score of 0.0 means the match matches the subject 0%. 0.42 means the match matches 42% and 1.0 means the match matches 100%.
	Q_INVOKABLE double getScore() const;

	// Sets the matches score to `new_score`.
	Q_INVOKABLE void setScore(double new_score);

	// Returns where the match matches the object searched for.
	Q_INVOKABLE QRect getRect() const;

	// Sets the matches matching location/rect to new_rect.
	Q_INVOKABLE void setRect(const QRect &new_rect);
	
	// Returns true if the matches score is greater than 0.0.
	Q_INVOKABLE bool isValid() const;
	
	Q_INVOKABLE QString toString() const;
};

#endif // BFP_ENGINE_PROTOTYPES_MATCH_PROTOTYPE_H
