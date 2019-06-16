#ifndef BFP_ENGINE_TYPES_MATCH_H
#define BFP_ENGINE_TYPES_MATCH_H

#include <QRect>
#include <QMetaType>

class Match : public QRect
{
public:
	Match();
	Match(QRect rect, double score);
	
	bool isValid() const;
	double getScore() const;
	void setScore(double new_score);
	
private:
	// 0.0 - 1.0. -1 = invalid
	double score = -1;
};

Q_DECLARE_METATYPE(Match)
Q_DECLARE_METATYPE(Match*)

#endif // BFP_ENGINE_TYPES_MATCH_H
