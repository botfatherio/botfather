#include "match.hpp"

Match::Match() : QRect()
{
	
}

Match::Match(QRect rect, double score)
	: QRect(rect), score(score)
{
	
}

bool Match::isValid() const
{
	return score > 0 && score < 1.001;
}

double Match::getScore() const
{
	return score;
}

void Match::setScore(double new_score)
{
	score = new_score;
}
