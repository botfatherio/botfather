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
		QImage image;
		engine->reportAdditionalMemoryCost(ImageSizeInBytes(image));
		return engine->toScriptValue(QImage());
	}
	
	// new Image(filepath);
	if (context->argumentCount() == 1 && context->argument(0).isString())
	{
		QString filepath = context->argument(0).toString();
		QString absolute_path = bot->normalisePath(filepath);
		
		QImage image;
		if (!image.load(absolute_path))
		{
			QString message = QString("can't load image from path %1").arg(absolute_path);
			return context->throwError(QScriptContext::TypeError, message);
		}

		engine->reportAdditionalMemoryCost(ImageSizeInBytes(image));
		return engine->toScriptValue(image);
	}
	
	// new Image(new Size());
	if (context->argumentCount() == 1 && isQSize(context->argument(0)))
	{
		QSize size = qscriptvalue_cast<QSize>(context->argument(0));
		QImage image(size, QImage::Format_RGB32);

		engine->reportAdditionalMemoryCost(ImageSizeInBytes(image));
		return engine->toScriptValue(image);
	}
	
	// new Image(new Size(), new Color());
	if (context->argumentCount() == 2 && isQSize(context->argument(0)) && isQColor(context->argument(1)))
	{
		QSize size = toQSize(context->argument(0));
		QColor color = toQColor(context->argument(1));
		QImage image(size, QImage::Format_RGB32);
		image.fill(color);

		engine->reportAdditionalMemoryCost(ImageSizeInBytes(image));
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

void ImagePrototype::load(const QString &filepath)
{
	Bot *bot = qobject_cast<Bot *>(engine()->parent());
	Q_ASSERT(bot);
	QString absolute_path = bot->normalisePath(filepath);

	if (!THIS_IMAGE_P()->load(absolute_path))
	{
		QString message = QString("can't load image from path %1").arg(absolute_path);
		context()->throwError(QScriptContext::TypeError, message);
		return void();
	}
}

void ImagePrototype::save(const QString &filepath)
{
	Bot *bot = qobject_cast<Bot *>(engine()->parent());
	Q_ASSERT(bot);
	QString absolute_path = bot->normalisePath(filepath);
	THIS_IMAGE().save(absolute_path);
}

void ImagePrototype::fill(const QColor &color)
{
	THIS_IMAGE_P()->fill(color);
}

QColor ImagePrototype::getPixelColor(const QPoint &position) const
{
	return THIS_IMAGE().pixelColor(position);
}

void ImagePrototype::setPixelColor(const QPoint &position, const QColor &color)
{
	THIS_IMAGE_P()->setPixelColor(position, color);
}

QImage ImagePrototype::copy(const QRect &sub_area)
{
	QImage copy = THIS_IMAGE().copy(sub_area);
	engine()->reportAdditionalMemoryCost(ImageSizeInBytes(copy));
	return copy;
}
	
QImage ImagePrototype::mirrored(bool horizontally, bool vertically)
{
	QImage mirrored = THIS_IMAGE().mirrored(horizontally, vertically);
	engine()->reportAdditionalMemoryCost(ImageSizeInBytes(mirrored));
	return mirrored;
}
	
QImage ImagePrototype::grayed()
{
	QImage grayed = THIS_IMAGE().convertToFormat(QImage::Format_Grayscale8);
	engine()->reportAdditionalMemoryCost(ImageSizeInBytes(grayed));
	return grayed;
}
	
QImage ImagePrototype::createMaskFromColor(const QColor &color)
{
	QImage mask = THIS_IMAGE().createMaskFromColor(color.rgb(), Qt::MaskOutColor);
	engine()->reportAdditionalMemoryCost(ImageSizeInBytes(mask));
	return mask;
}

QImage ImagePrototype::createMaskFromAlpha()
{
	QImage mask = THIS_IMAGE().createAlphaMask();
	mask.invertPixels();
	engine()->reportAdditionalMemoryCost(ImageSizeInBytes(mask));
	return mask;
}

QImage ImagePrototype::isolateColorRange(const QColor &min, const QColor &max, bool keep_color)
{
	if (THIS_IMAGE().isNull()) {
		context()->throwError(QScriptContext::TypeError, "The image must not be null.");
		return QImage();
	}

	cv::Mat image_mat = Vision::qimageToBGRMat(THIS_IMAGE());
	cv::Scalar min_hsv(min.hsvHue() / 2, min.hsvSaturation(), min.value());
    cv::Scalar max_hsv(max.hsvHue() / 2, max.hsvSaturation(), max.value());
	cv::Mat result_mat = Vision::isolateColor(image_mat, min_hsv, max_hsv, keep_color);

	QImage result = Vision::cvMatToQImage(result_mat);
	image_mat.release();
	result_mat.release();

	engine()->reportAdditionalMemoryCost(ImageSizeInBytes(result));
	return result;
}

int ImagePrototype::countDifferentPixels(const QImage &other_image) const
{
	if (THIS_IMAGE().isNull() || other_image.isNull()) {
		context()->throwError(QScriptContext::TypeError, "Both images must not be null.");
		return int();
	}
	if (THIS_IMAGE().size() != other_image.size()) {
		context()->throwError(QScriptContext::TypeError, "Both images must have the same size.");
		return int();
	}

	cv::Mat image_1 = Vision::qimageToBGRMat(THIS_IMAGE());
	cv::Mat image_2 = Vision::qimageToBGRMat(other_image);

	image_1.release();
	image_2.release();

	return Vision::countDifferentPixels(image_1, image_2);
}

double ImagePrototype::pixelEquality(const QImage &other_image) const
{
	if (THIS_IMAGE().isNull() || other_image.isNull()) {
		context()->throwError(QScriptContext::TypeError, "Both images must not be null.");
		return double();
	}
	if (THIS_IMAGE().size() != other_image.size()) {
		context()->throwError(QScriptContext::TypeError, "Both images must have the same size.");
		return double();
	}

	cv::Mat image_1 = Vision::qimageToBGRMat(THIS_IMAGE());
	cv::Mat image_2 = Vision::qimageToBGRMat(other_image);

	int total_pixels = THIS_IMAGE().width() * THIS_IMAGE().height();
	int equal_pixels = total_pixels - Vision::countDifferentPixels(image_1, image_2);

	image_1.release();
	image_2.release();

	return static_cast<double>(equal_pixels) / total_pixels;
}

QString ImagePrototype::toString() const
{
	QImage image = THIS_IMAGE();
	return QString("Image(width: %1, height: %2, bytes: %3)").arg(image.width()).arg(image.height()).arg(ImageSizeInBytes(image));
}
