#include "image_prototype.h"
#include "../bot.h"

#define THIS_IMAGE() qscriptvalue_cast<QImage>(thisObject())
#define THIS_IMAGE_P() qscriptvalue_cast<QImage*>(thisObject())

QScriptValue ImagePrototype::constructor(QScriptContext *context, QScriptEngine *engine)
{
	// Instead of casting the engines parent to a Bot* we could expose required bot functionality
	// via the HelperAPI to engine and call it from here.
	Bot *bot = qobject_cast<Bot *>(engine->parent());
	Q_ASSERT(bot);
	
	if (context->argumentCount() == 0) {
		return engine->toScriptValue(QImage());
	}
	
	if (context->argumentCount() == 1 && context->argument(0).isString()) {
		QString filepath = context->argument(0).toString();
		QString absolute_path = bot->normalisePath(filepath);
		
		QImage image;
		image.load(absolute_path);
		
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
	return QString("Image(width: %1, height: %2, bytes: %3)").arg(image.width()).arg(image.height()).arg(ImageSizeInBytes(image));
}