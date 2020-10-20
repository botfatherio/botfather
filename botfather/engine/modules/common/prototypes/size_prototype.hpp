#ifndef BFP_ENGINE_PROTOTYPES_SIZE_PROTOTYPE_H
#define BFP_ENGINE_PROTOTYPES_SIZE_PROTOTYPE_H
#define SIZE_PROTOTYPE_DOCS "https://botfather.io/docs/apiref/size/"

#include <QSize>

#include "abstract_prototype.hpp"

Q_DECLARE_METATYPE(QSize)
Q_DECLARE_METATYPE(QSize *)

class SizePrototype : public AbstractPrototype {
    Q_OBJECT
    Q_PROPERTY(int width READ getWidth WRITE setWidth)
    Q_PROPERTY(int height READ getHeight WRITE setHeight)

   public:
    static QScriptValue constructor(QScriptContext *context,
                                    QScriptEngine *engine);
    using AbstractPrototype::AbstractPrototype;

    // Returns true if either of the width and height is less than or equal to
    // 0; otherwise returns false. https://doc.qt.io/qt-5/qsize.html#isEmpty
    Q_INVOKABLE bool isEmpty() const;

    // Returns the sizes width
    // https://doc.qt.io/qt-5/qsize.html#width
    Q_INVOKABLE int getWidth() const;

    // Sets the sizes width
    // https://doc.qt.io/qt-5/qsize.html#setWidth
    Q_INVOKABLE void setWidth(int width);

    // Returns the sizes height
    // https://doc.qt.io/qt-5/qsize.html#height
    Q_INVOKABLE int getHeight() const;

    // Sets the sizes height
    // https://doc.qt.io/qt-5/qsize.html#setHeight
    Q_INVOKABLE void setHeight(int height);

    // Returns the sum of this size and other_size; each component is added
    // separately. https://doc.qt.io/qt-5/qsize.html#operator-2b
    Q_INVOKABLE QSize sizeAdded(const QSize &other_size) const;

    // Returns other_size subtracted from this size; each component is
    // subtracted separately. https://doc.qt.io/qt-5/qsize.html#operator-
    Q_INVOKABLE QSize sizeSubtracted(const QSize &other_size) const;

    // Returns a size holding the minimum width and height of this size and the
    // given other_size. https://doc.qt.io/qt-5/qsize.html#boundedTo
    Q_INVOKABLE QSize boundedTo(const QSize &other_size) const;

    // Returns a size holding the maximum width and height of this size and the
    // given other_size. https://doc.qt.io/qt-5/qsize.html#expandedTo
    Q_INVOKABLE QSize expandedTo(const QSize &other_size) const;

    // Swaps the width and height values.
    // https://doc.qt.io/qt-5/qsize.html#transpose
    Q_INVOKABLE void transpose();

    // Returns a QSize with width and height swapped.
    // https://doc.qt.io/qt-5/qsize.html#transposed
    Q_INVOKABLE QSize transposed() const;

    Q_INVOKABLE QString toString() const;
};

#endif  // BFP_ENGINE_PROTOTYPES_SIZE_PROTOTYPE_H
