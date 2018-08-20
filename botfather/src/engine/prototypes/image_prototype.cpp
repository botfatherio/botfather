#include "image_prototype.h"

#define THIS_IMAGE() qscriptvalue_cast<QImage>(thisObject())
#define THIS_IMAGE_P() qscriptvalue_cast<QImage*>(thisObject())

QScriptValue ImagePrototype::constructor(QScriptContext *context, QScriptEngine *engine)
{
	if (context->argumentCount() == 0) {
		return engine->toScriptValue(QImage());
	}
	
	if (context->argumentCount() == 1 && context->argument(0).isString()) {
		QString filepath = context->argument(0).toString();
		QImage image;
		
		if (!image.load(filepath))
		{
			QString message = QString("Couldn't load image from file %1").arg(filepath);
			return context->throwError(QScriptContext::URIError, message);
		}
		
		return engine->toScriptValue(image);
	}
	
	NO_MATCHING_CTOR("Image", IMAGE_PROTOTYPE_DOCS)
}

int ImagePrototype::getWidth() const
{
	return THIS_IMAGE().width();
}

int ImagePrototype::getHeight() const
{
	return THIS_IMAGE().height();
}

bool ImagePrototype::isNull() const
{
	return THIS_IMAGE().isNull();
}

QString ImagePrototype::toString() const
{
	QImage image = THIS_IMAGE();
	return QString("Image(%1x%2, %3 bytes)").arg(image.width()).arg(image.height()).arg(ImageSizeInBytes(image));
}