#ifndef BFP_ENGINE_PROTOTYPES_MARGINS_PROTOTYPE_H
#define BFP_ENGINE_PROTOTYPES_MARGINS_PROTOTYPE_H
#define MARGINS_PROTOTYPE_DOCS "https://botfather.io/docs/apiref/margins/"

#include <QMargins>

#include "abstract_prototype.hpp"

Q_DECLARE_METATYPE(QMargins)
Q_DECLARE_METATYPE(QMargins *)

class MarginsPrototype : public AbstractPrototype {
    Q_OBJECT
    Q_PROPERTY(int top READ getTop WRITE setTop)
    Q_PROPERTY(int left READ getLeft WRITE setLeft)
    Q_PROPERTY(int right READ getRight WRITE setRight)
    Q_PROPERTY(int bottom READ getBottom WRITE setBottom)

   public:
    static QScriptValue constructor(QScriptContext *context,
                                    QScriptEngine *engine);
    using AbstractPrototype::AbstractPrototype;

    // Returns true if all margins are is 0; otherwise returns false.
    // http://doc.qt.io/qt-5/qmargins.html#isNull
    Q_INVOKABLE bool isNull() const;

    // Returns the top margin.
    // http://doc.qt.io/qt-5/qmargins.html#top
    Q_INVOKABLE int getTop() const;

    // Sets the Top margin to Top.
    // http://doc.qt.io/qt-5/qmargins.html#setTop
    Q_INVOKABLE void setTop(int top);

    // Returns the left margin.
    // http://doc.qt.io/qt-5/qmargins.html#left
    Q_INVOKABLE int getLeft() const;

    // Sets the left margin to left.
    // http://doc.qt.io/qt-5/qmargins.html#setLeft
    Q_INVOKABLE void setLeft(int left);

    // Returns the right margin.
    // http://doc.qt.io/qt-5/qmargins.html#right
    Q_INVOKABLE int getRight() const;

    // Sets the right margin to right.
    // http://doc.qt.io/qt-5/qmargins.html#setRight
    Q_INVOKABLE void setRight(int right);

    // Returns the bottom margin.
    // http://doc.qt.io/qt-5/qmargins.html#bottom
    Q_INVOKABLE int getBottom() const;

    // Sets the bottom margin to bottom
    // http://doc.qt.io/qt-5/qmargins.html#setBottom
    Q_INVOKABLE void setBottom(int bottom);

    // Returns a QMargins object that is the sum of other_margins and this
    // margins object; each component is added separately.
    // http://doc.qt.io/qt-5/qmargins.html#operator-2b
    Q_INVOKABLE QMargins marginsAdded(const QMargins &other_margins) const;

    // Returns a QMargins object that is formed by subtracting other_margins
    // from this margins object; each component is subtracted separately.
    // http://doc.qt.io/qt-5/qmargins.html#operator-
    Q_INVOKABLE QMargins marginsSubtracted(const QMargins &other_margins) const;

    Q_INVOKABLE QString toString() const;
};

#endif  // BFP_ENGINE_PROTOTYPES_MARGINS_PROTOTYPE_H
