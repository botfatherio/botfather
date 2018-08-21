#ifndef BFP_ENGINE_PROTOTYPES_SIZE_PROTOTYPE_H
#define BFP_ENGINE_PROTOTYPES_SIZE_PROTOTYPE_H
#define SIZE_PROTOTYPE_DOCS "https://botfather.io/docs/apiref/size/"

#include <QSize>
#include "abstract_prototype.h"

Q_DECLARE_METATYPE(QSize)
Q_DECLARE_METATYPE(QSize*)

class SizePrototype : public AbstractPrototype
{
	Q_OBJECT
	
public:
	static QScriptValue constructor(QScriptContext *context, QScriptEngine *engine);
	using AbstractPrototype::AbstractPrototype;

	Q_INVOKABLE int getWidth() const;
	Q_INVOKABLE void setWidth(int width);
	
	Q_INVOKABLE int getHeight() const;
	Q_INVOKABLE void setHeight(int height);
	
	Q_INVOKABLE QString toString() const;
};

#endif // BFP_ENGINE_PROTOTYPES_SIZE_PROTOTYPE_H