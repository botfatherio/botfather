#include "match.h"

Match::Match() : QRect()
{}

Match::Match(double score, QRect rect) : QRect(rect)
{
	this->m_score = score;
}

double Match::getScore() const
{
	return this->m_score;
}

int Match::getLeft() const
{
	return this->QRect::left();
}

int Match::getRight() const
{
	return this->QRect::right();
}

int Match::getTop() const
{
	return this->QRect::top();
}

int Match::getBottom() const
{
	return this->QRect::bottom();
}

int Match::getWidth() const
{
	return this->QRect::width();
}

int Match::getHeight() const
{
	return this->QRect::height();
}

int Match::getX() const
{
	return this->QRect::x();
}

int Match::getY() const
{
	return this->QRect::y();
}