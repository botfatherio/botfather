#include "match.h"

Match::Match()
{}

Match::Match(double score, int left, int top, int width, int height)
{
	this->m_score = score;
	this->m_width = width;
	this->m_height = height;
	this->m_left = left;
	this->m_right = left + width;
	this->m_top = top;
	this->m_bottom = top + height;
	this->m_x = left + (width / 2);
	this->m_y = top + (height / 2);
}

bool Match::found() const
{
	return m_score != -1;
}

double Match::getScore() const
{
	return this->m_score;
}

int Match::getLeft() const
{
	return this->m_left;
}

int Match::getRight() const
{
	return this->m_right;
}

int Match::getTop() const
{
	return this->m_top;
}

int Match::getBottom() const
{
	return this->m_bottom;
}

int Match::getWidth() const
{
	return this->m_width;
}

int Match::getHeight() const
{
	return this->m_height;
}

int Match::getX() const
{
	return this->m_x;
}

int Match::getY() const
{
	return this->m_y;
}