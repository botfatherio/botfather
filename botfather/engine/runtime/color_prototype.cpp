#include "color_prototype.hpp"

#include <QHash>

#define THIS_COLOR() qscriptvalue_cast<QColor>(thisObject())
#define THIS_COLOR_P() qscriptvalue_cast<QColor *>(thisObject())

static QHash<QString, QColor> color_map = {
    {"black", Qt::black},
    {"blue", Qt::blue},
    {"cyan", Qt::cyan},
    {"darkblue", Qt::darkBlue},
    {"darkcyan", Qt::darkCyan},
    {"darkgray", Qt::darkGray},
    {"darkgreen", Qt::darkGreen},
    {"darkmagenta", Qt::darkMagenta},
    {"darkred", Qt::darkRed},
    {"darkyellow", Qt::darkYellow},
    {"gray", Qt::gray},
    {"green", Qt::green},
    {"lightgray", Qt::lightGray},
    {"magenta", Qt::magenta},
    {"red", Qt::red},
    {"transparent", Qt::transparent},
    {"white", Qt::white},
    {"yellow", Qt::yellow},
};

QScriptValue ColorPrototype::constructor(QScriptContext *context,
                                         QScriptEngine *engine) {
    // Default constructor
    if (context->argumentCount() == 0) {
        return engine->toScriptValue(QColor());
    }

    // Construct by color name from color_map
    if (context->argumentCount() == 1 && context->argument(0).isString()) {
        QString color_name = context->argument(0).toString().toLower();

        if (!color_map.contains(color_name)) {
            QString message = QString("Unknown color '%1'. %2")
                                  .arg(color_name)
                                  .arg(COLOR_PROTOTYPE_DOCS);
            return context->throwError(QScriptContext::TypeError, message);
        }

        return engine->toScriptValue(color_map[color_name]);
    }

    // Construct from RGB value
    if (context->argumentCount() == 3 && context->argument(0).isNumber() &&
        context->argument(1).isNumber() && context->argument(2).isNumber()) {
        int r = context->argument(0).toInt32();
        int g = context->argument(1).toInt32();
        int b = context->argument(2).toInt32();

        if (r < 0 || g < 0 || b < 0 || r > 255 || g > 255 || b > 255) {
            return context->throwError(QScriptContext::RangeError,
                                       "r, g and b must be in range of 0-255");
        }

        return engine->toScriptValue(QColor(r, g, b));
    }

    if (context->argumentCount() == 4 && context->argument(0).isNumber() &&
        context->argument(1).isNumber() && context->argument(2).isNumber() &&
        context->argument(3).isString() &&
        context->argument(3).toString().toLower() == "hsv") {
        int h = context->argument(0).toInt32();
        int s = context->argument(1).toInt32();
        int v = context->argument(2).toInt32();

        if (h < 0 || s < 0 || v < 0 || h > 359 || s > 255 || v > 255) {
            return context->throwError(QScriptContext::RangeError,
                                       "Values must be in the following "
                                       "ranges: h 0-359, s and v 0-255");
        }

        return engine->toScriptValue(QColor::fromHsv(h, s, v));
    }

    NO_MATCHING_CTOR("Color", COLOR_PROTOTYPE_DOCS)
}

bool ColorPrototype::isValid() const { return THIS_COLOR().isValid(); }

int ColorPrototype::getRed() const { return THIS_COLOR().red(); }

void ColorPrototype::setRed(int red) { THIS_COLOR_P()->setRed(red); }

int ColorPrototype::getGreen() const { return THIS_COLOR().green(); }

void ColorPrototype::setGreen(int green) { THIS_COLOR_P()->setGreen(green); }

int ColorPrototype::getBlue() const { return THIS_COLOR().blue(); }

void ColorPrototype::setBlue(int blue) { THIS_COLOR_P()->setBlue(blue); }

void ColorPrototype::setHsv(int h, int s, int v) {
    if (h < 0 || s < 0 || v < 0 || h > 359 || s > 255 || v > 255) {
        context()->throwError(
            QScriptContext::RangeError,
            "Values must be in the following ranges: h 0-359, s and v 0-255");
        return void();
    }
    THIS_COLOR_P()->setHsv(h, s, v);
}

int ColorPrototype::getHue() const { return THIS_COLOR().hsvHue(); }

int ColorPrototype::getSaturation() const {
    return THIS_COLOR().hsvSaturation();
}

int ColorPrototype::getValue() const { return THIS_COLOR().value(); }

QString ColorPrototype::toString() const {
    QColor color = THIS_COLOR();
    return QString("Color(r: %1, g: %2, b: %3)")
        .arg(color.red())
        .arg(color.green())
        .arg(color.blue());
}
