#include "image.h"
#include <QtDebug>

Image::Image()
{}

Image::Image(QImage qimage)
	: qimage(qimage)
{}

QScriptValue Image::toScriptValue(QScriptEngine *engine, Image* const &in)
{
	engine->reportAdditionalMemoryCost(static_cast<int>(in->getQImage().sizeInBytes()));
	return engine->newQObject(in, QScriptEngine::ScriptOwnership);
}

void Image::fromScriptValue(const QScriptValue &object, Image* &out)
{
	out = qobject_cast<Image*>(object.toQObject());
}

QImage Image::getQImage() const
{
	return qimage;
}

bool Image::isValid()
{
	return !qimage.isNull();
}

int Image::getWidth()
{
	return qimage.width();
}

int Image::getHeight()
{
	return qimage.height();
}