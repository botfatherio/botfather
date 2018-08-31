#include "match.h"

Match::Match() : QRect()
{
	
}

Match::Match(QRect rect, double score)
	: QRect(rect), score(score)
{
	
}

bool Match::found() const
{
	return score > 0;
}

double Match::getScore() const
{
	return score;
}

void Match::setScore(double new_score)
{
	score = new_score;
}
