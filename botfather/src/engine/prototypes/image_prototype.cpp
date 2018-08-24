#include "image_prototype.h"
#include <QDebug>
#include "../modules/vision/vision.h"
#include "../bot.h"

#define THIS_IMAGE() qscriptvalue_cast<QImage>(thisObject())
#define THIS_IMAGE_P() qscriptvalue_cast<QImage*>(thisObject())

QScriptValue ImagePrototype::constructor(QScriptContext *context, QScriptEngine *engine)
{
	// Instead of casting the engines parent to a Bot* we could expose required bot functionality
	// via the HelperAPI to engine and call it from here.
	Bot *bot = qobject_cast<Bot *>(engine->parent());
	Q_ASSERT(bot);
	
	// new Image();
	if (context->argumentCount() == 0)
	{
		return engine->toScriptValue(QImage());
	}
	
	// new Image(color_name);
	if (context->argumentCount() == 1 && context->argument(0).isString())
	{
		QString filepath = context->argument(0).toString();
		QString absolute_path = bot->normalisePath(filepath);
		
		QImage image;
		image.load(absolute_path);
		
		return engine->toScriptValue(image);
	}
	
	// new Image(new Size());
	if (context->argumentCount() == 1 && isQSize(context->argument(0)))
	{
		QSize size = qscriptvalue_cast<QSize>(context->argument(0));
		QImage image(size, QImage::Format_RGB32);
		return engine->toScriptValue(image);
	}
	
	// new Image(new Size(), new Color());
	if (context->argumentCount() == 2 && isQSize(context->argument(0)) && isQColor(context->argument(1)))
	{
		QSize size = toQSize(context->argument(0));
		QColor color = toQColor(context->argument(1));
		QImage image(size, QImage::Format_RGB32);
		image.fill(color);
		return engine->toScriptValue(image);
	}
	
	NO_MATCHING_CTOR("Image", IMAGE_PROTOTYPE_DOCS)
}

bool ImagePrototype::isNull() const
{
	return THIS_IMAGE().isNull();
}

bool ImagePrototype::hasAlphaChannel() const
{
	return THIS_IMAGE().hasAlphaChannel();
}

int ImagePrototype::getWidth() const
{
	return THIS_IMAGE().width();
}

int ImagePrototype::getHeight() const
{
	return THIS_IMAGE().height();
}

QSize ImagePrototype::getSize() const
{
	return THIS_IMAGE().size();
}

void ImagePrototype::load(QString filepath)
{
	Bot *bot = qobject_cast<Bot *>(engine()->parent());
	Q_ASSERT(bot);
	QString absolute_path = bot->normalisePath(filepath);
	THIS_IMAGE_P()->load(absolute_path);
}

void ImagePrototype::save(QString filepath)
{
	Bot *bot = qobject_cast<Bot *>(engine()->parent());
	Q_ASSERT(bot);
	QString absolute_path = bot->normalisePath(filepath);
	THIS_IMAGE().save(absolute_path);
}

void ImagePrototype::fill(QColor color)
{
	THIS_IMAGE_P()->fill(color);
}

QColor ImagePrototype::getPixelColor(QPoint position) const
{
	return THIS_IMAGE().pixelColor(position);
}

void ImagePrototype::setPixelColor(QPoint position, QColor color)
{
	THIS_IMAGE_P()->setPixelColor(position, color);
}

QImage ImagePrototype::copy(QRect sub_area)
{
	return THIS_IMAGE().copy(sub_area);
}
	
QImage ImagePrototype::mirrored(bool horizontally, bool vertically)
{
	return THIS_IMAGE().mirrored(horizontally, vertically);
}
	
QImage ImagePrototype::grayed()
{
	return THIS_IMAGE().convertToFormat(QImage::Format_Grayscale8);
}
	
QImage ImagePrototype::createMaskFromColor(QColor color)
{
	return THIS_IMAGE().createMaskFromColor(color.rgb(), Qt::MaskOutColor);
}

QImage ImagePrototype::createMaskFromAlpha()
{
	QImage image = THIS_IMAGE().createAlphaMask();
	image.invertPixels();
	return image;
}

QImage ImagePrototype::isolateColorRange(QColor min, QColor max, bool keep_color)
{
	if (THIS_IMAGE().isNull()) {
		context()->throwError(QScriptContext::TypeError, "The image must not be null.");
	}
	cv::Mat image = Vision::qimageToBGRMat(THIS_IMAGE());
	cv::Scalar min_hsv(min.hsvHue() / 2, min.hsvSaturation(), min.value());
    cv::Scalar max_hsv(max.hsvHue() / 2, max.hsvSaturation(), max.value());
	cv::Mat result = Vision::isolateColor(image, min_hsv, max_hsv, keep_color);
	return Vision::cvMatToQImage(result);
}

int ImagePrototype::countDifferentPixels(QImage other_image) const
{
	if (THIS_IMAGE().isNull() || other_image.isNull()) {
		context()->throwError(QScriptContext::TypeError, "Both images must not be null.");
	}
	cv::Mat image_1 = Vision::qimageToBGRMat(THIS_IMAGE());
	cv::Mat image_2 = Vision::qimageToBGRMat(other_image);
	return Vision::countDifferentPixels(image_1, image_2);
}

QString ImagePrototype::toString() const
{
	QImage image = THIS_IMAGE();
	return QString("Image(width: %1, height: %2, bytes: %3)").arg(image.width()).arg(image.height()).arg(ImageSizeInBytes(image));
}