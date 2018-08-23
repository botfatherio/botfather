#ifndef BFP_ENGINE_PROTOTYPES_IMAGE_PROTOTYPE_H
#define BFP_ENGINE_PROTOTYPES_IMAGE_PROTOTYPE_H
#define IMAGE_PROTOTYPE_DOCS "https://botfather.io/docs/apiref/image/"

#include "abstract_prototype.h"
#include <QImage>

Q_DECLARE_METATYPE(QImage)
Q_DECLARE_METATYPE(QImage*)

#if QT_VERSION < QT_VERSION_CHECK(5, 10, 0)
#define ImageSizeInBytes(qimage) qimage.byteCount()
#else
#define ImageSizeInBytes(qimage) qimage.sizeInBytes()
#endif

class ImagePrototype : public AbstractPrototype
{
	Q_OBJECT
	
public:
	static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);
	using AbstractPrototype::AbstractPrototype;

	// A null image has all parameters set to zero and no allocated data.
	// https://doc.qt.io/qt-5/qimage.html#isNull
	Q_INVOKABLE bool isNull() const;
	
	// Returns true if the image has a format that respects the alpha channel, otherwise returns false.
	// https://doc.qt.io/qt-5/qimage.html#hasAlphaChannel
	Q_INVOKABLE bool hasAlphaChannel() const;
	
	// Returns the images width
	// https://doc.qt.io/qt-5/qimage.html#width
	Q_INVOKABLE int getWidth() const;
	
	// Returns the images height
	// https://doc.qt.io/qt-5/qimage.html#height
	Q_INVOKABLE int getHeight() const;
	
	// Returns the images size
	// https://doc.qt.io/qt-5/qimage.html#size
	Q_INVOKABLE QSize getSize() const;
	
	// Load image data from a file (relative paths are considered relative to the script)
	// https://doc.qt.io/qt-5/qimage.html#load
	Q_INVOKABLE void load(QString filepath);
	
	// Save the image (relative paths are considered relative to the script)
	// https://doc.qt.io/qt-5/qimage.html#save
	Q_INVOKABLE void save(QString filepath);
	
	// Fills the entire image with the given color.
	// https://doc.qt.io/qt-5/qimage.html#fill-1
	Q_INVOKABLE void fill(QColor color);
	
	// Returns the color of the pixel at the given position as a QColor.
	// https://doc.qt.io/qt-5/qimage.html#pixelColor
	Q_INVOKABLE QColor getPixelColor(QPoint position) const;
	
	// Sets the color at the given position to color.
	// https://doc.qt.io/qt-5/qimage.html#setPixelColor
	Q_INVOKABLE void setPixelColor(QPoint position, QColor color);
	
	// Returns a sub-area of the image as a new image.
	// https://doc.qt.io/qt-5/qimage.html#copy
	Q_INVOKABLE QImage copy(QRect sub_area);
	
	// Returns a mirror of the image
	// https://doc.qt.io/qt-5/qimage.html#mirrored
	Q_INVOKABLE QImage mirrored(bool horizontally, bool vertically);
	
	// Returns the image in grayscale format
	// https://doc.qt.io/qt-5/qimage.html#convertToFormat
	// https://doc.qt.io/qt-5/qimage.html#Format-enum
	Q_INVOKABLE QImage grayed();
	
	// Creates and returns a mask for this image based on the given color value
	// https://doc.qt.io/qt-5/qimage.html#createMaskFromColor
	Q_INVOKABLE QImage createMaskFromColor(QColor color);
	
	// Builds and returns a 1-bpp mask from the alpha buffer in this image.
	// https://doc.qt.io/qt-5/qimage.html#createAlphaMask
	Q_INVOKABLE QImage createMaskFromAlpha();
		
	Q_INVOKABLE QString toString() const;
};

#endif // BFP_ENGINE_PROTOTYPES_IMAGE_PROTOTYPE_H