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

	Q_INVOKABLE int getWidth() const;
	Q_INVOKABLE int getHeight() const;
	Q_INVOKABLE bool isNull() const;
	
	// Load image data from a file (relative paths are considered relative to the script)
	Q_INVOKABLE void load(QString filepath);
	
	// Save the image (relative paths are considered relative to the script)
	Q_INVOKABLE void save(QString filepath);
	
	Q_INVOKABLE QString toString() const;
};

#endif // BFP_ENGINE_PROTOTYPES_IMAGE_PROTOTYPE_H