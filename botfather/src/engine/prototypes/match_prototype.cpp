#include "match_prototype.h"

#define THIS_MATCH() qscriptvalue_cast<Match>(thisObject())
#define THIS_MATCH_P() qscriptvalue_cast<Match*>(thisObject())

QScriptValue MatchPrototype::constructor(QScriptContext *context, QScriptEngine *engine)
{
	if (context->argumentCount() == 0)
	{
		return engine->toScriptValue(Match());
	}
	
	if (context->argumentCount() == 2 && isQRect(context->argument(0)) && context->argument(1).isNumber())
	{
		QRect rect = qscriptvalue_cast<QRect>(context->argument(0));
		double score = context->argument(1).toNumber();
		if (score > 1.0)
		{
			QString message = QString("The matches score can't be higher than 1.0. %1").arg(MATCH_PROTOTYPE_DOCS);
			return context->throwError(QScriptContext::TypeError, message);
		}
		return engine->toScriptValue(Match(rect, score));
	}
	
	NO_MATCHING_CTOR("Match", MATCH_PROTOTYPE_DOCS)
}

double MatchPrototype::getScore() const
{
	return THIS_MATCH().getScore();
}

void MatchPrototype::setScore(double new_score)
{
	THIS_MATCH_P()->setScore(new_score);
}

QRect MatchPrototype::getRect() const
{
	return static_cast<QRect>(THIS_MATCH());
}

void MatchPrototype::setRect(const QRect &new_rect)
{
	Match *match = THIS_MATCH_P();
	match->setTopLeft(new_rect.topLeft());
	match->setBottomRight(new_rect.bottomRight());
}

bool MatchPrototype::isValid() const
{
	return THIS_MATCH().isValid();
}

QString MatchPrototype::toString() const
{
	Match match = THIS_MATCH();
	return QString("Match(x: %1, y: %2, score: %3)").arg(match.x()).arg(match.y()).arg(match.getScore());
}
