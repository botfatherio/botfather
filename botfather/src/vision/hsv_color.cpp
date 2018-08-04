#include "hsv_color.h"

HSVColor::HSVColor()
{}

HSVColor::HSVColor(int h, int s, int v)
	: m_h(h), m_s(s), m_v(v)
{}

QScriptValue HSVColor::toScriptValue(QScriptEngine *engine, HSVColor* const &in)
{
	return engine->newQObject(in, QScriptEngine::AutoOwnership);
}

void HSVColor::fromScriptValue(const QScriptValue &object, HSVColor* &out)
{
	out = qobject_cast<HSVColor*>(object.toQObject());
}

void HSVColor::setH(int h)
{
	this->m_h = h;
}

int HSVColor::getH() const
{
	return this->m_h;
}

void HSVColor::setS(int s)
{
	this->m_s = s;
}

int HSVColor::getS() const
{
	return this->m_s;
}

void HSVColor::setV(int v)
{
	this->m_v = v;
}

int HSVColor::getV() const
{
	return this->m_v;
}

cv::Scalar HSVColor::getScalar()
{
	return cv::Scalar(
		(m_h / 2.0),
		(m_s * (255.0 / 100.0)),
		(m_v * (255.0 / 100.0))
	);
}