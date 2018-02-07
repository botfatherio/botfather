#ifndef BFP__VISION__MATCH_H
#define BFP__VISION__MATCH_H

#include <QObject>
#include <QRect>

class Match : public QObject, public QRect
{
	Q_OBJECT
	
public:
	Match();
	Match(double score, QRect rect);

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
	double m_score = 0.0;
};

#endif // BFP__VISION__MATCH_H