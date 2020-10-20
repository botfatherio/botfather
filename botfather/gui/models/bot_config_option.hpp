#ifndef BOTCONFIGOPTION_H
#define BOTCONFIGOPTION_H

// Unregister windows macros clashing with this classes method names
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

#include <QMap>
#include <QVariant>

#include "bot_manifest_node.hpp"

class BotConfigOption : public BotManifestNode {
   public:
    using BotManifestNode::BotManifestNode;

    enum Type { String, Password, Boolean, Number, Integer, Choice, Unknown };

    QVariant value() const;
    void setValue(const QVariant &new_value);

    // Returns true when there is a value stored for this setting.
    bool isStored() const;

    bool isValid() const;
    QString id() const;
    Type type() const;
    QString label() const;
    QVariant fallback() const;
    QMap<QString, QString> choices() const;

    // Returns true if the setting has a valid range.
    bool hasRange() const;

    // Returns the settings (range) min value
    int min() const;

    // Returns the settings (range) max value
    int max() const;
};

#endif  // BOTCONFIGOPTION_H
