#ifndef ENGINE_TYPES_IMAGE_PROTOTYPE_H
#define ENGINE_TYPES_IMAGE_PROTOTYPE_H
#define IMAGE_PROTOTYPE_DOCS "https://botfather.io/docs/apiref/image/"

#include <QImage>
#include "abstract_prototype.h"

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
	
	// Load and save image from and to the passed filepaths relativ to the script path, if the filepath is not absolute.
	//Q_INVOKABLE void load(QString filepath);
	//Q_INVOKABLE void save(QString filepath);
	
	Q_INVOKABLE QString toString() const;
};

#endif // ENGINE_TYPES_IMAGE_PROTOTYPE_H