#ifndef BFP__VISION__MATCH_H
#define BFP__VISION__MATCH_H

#include <QObject>
#include <QRect>

class Match : public QObject
{
	Q_OBJECT
	
public:
	Match();
	Match(double score, int left, int top, int width, int height);

	Q_INVOKABLE bool found() const;
	Q_INVOKABLE double getScore() const;
	Q_INVOKABLE int getLeft() const;
	Q_INVOKABLE int getRight() const;
	Q_INVOKABLE int getTop() const;
	Q_INVOKABLE int getBottom() const;
	Q_INVOKABLE int getWidth() const;
	Q_INVOKABLE int getHeight() const;
	Q_INVOKABLE int getX() const;
	Q_INVOKABLE int getY() const;
	
private:
	// The tpls matching score on the reference (0.0 to 1.0).
	double m_score = -1;
	
	int m_left = 0;
	int m_top = 0;
	int m_width = 0;
	int m_height = 0;
	int m_right = 0;
	int m_bottom = 0;
	int m_x = 0;
	int m_y = 0;
};

#endif // BFP__VISION__MATCH_H