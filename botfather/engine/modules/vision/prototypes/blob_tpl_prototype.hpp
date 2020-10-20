#ifndef BFP_ENGINE_PROTOTYPES_BLOB_TPL_PROTOTYPE_H
#define BFP_ENGINE_PROTOTYPES_BLOB_TPL_PROTOTYPE_H
#define BLOB_TPL_PROTOTYPE_DOCS \
    QString("https://botfather.io/docs/apiref/blobtpl/")

#include "../../common/prototypes/abstract_prototype.hpp"
#include "../types/blob_tpl.hpp"

class BlobTplPrototype : public AbstractPrototype {
    Q_OBJECT
    Q_PROPERTY(int min_area READ getMinArea WRITE setMinArea)
    Q_PROPERTY(int max_area READ getMaxArea WRITE setMaxArea)
    Q_PROPERTY(
        double min_circularity READ getMinCircularity WRITE setMinCircularity)
    Q_PROPERTY(
        double max_circularity READ getMaxCircularity WRITE setMaxCircularity)
    Q_PROPERTY(double min_convexity READ getMinConvexity WRITE setMinConvexity)
    Q_PROPERTY(double max_convexity READ getMaxConvexity WRITE setMaxConvexity)
    Q_PROPERTY(double min_inertia READ getMinInertia WRITE setMinInertia)
    Q_PROPERTY(double max_inertia READ getMaxInertia WRITE setMaxInertia)
    Q_PROPERTY(int min_threshold READ getMinThreshold WRITE setMinThreshold)
    Q_PROPERTY(int max_threshold READ getMaxThreshold WRITE setMaxThreshold)
    Q_PROPERTY(int threshold_step READ getThresholdStep WRITE setThresholdStep)

   public:
    static QScriptValue constructor(QScriptContext *context,
                                    QScriptEngine *engine);
    using AbstractPrototype::AbstractPrototype;

    // 0 <= min <= max
    Q_INVOKABLE void setMinArea(int new_min_area);
    Q_INVOKABLE int getMinArea() const;

    Q_INVOKABLE void setMaxArea(int new_max_area);
    Q_INVOKABLE int getMaxArea() const;

    // 0 <= min <= max <= 1.0f
    Q_INVOKABLE void setMinCircularity(double new_min_circularity);
    Q_INVOKABLE double getMinCircularity() const;

    Q_INVOKABLE void setMaxCircularity(double new_max_circularity);
    Q_INVOKABLE double getMaxCircularity() const;

    // 0 <= min <= max <= 1.0f
    Q_INVOKABLE void setMinConvexity(double new_min_convexity);
    Q_INVOKABLE double getMinConvexity() const;

    Q_INVOKABLE void setMaxConvexity(double new_max_convexity);
    Q_INVOKABLE double getMaxConvexity() const;

    // 0 <= min <= max <= 1.0f
    Q_INVOKABLE void setMinInertia(double new_min_inertia_ratio);
    Q_INVOKABLE double getMinInertia() const;

    Q_INVOKABLE void setMaxInertia(double new_max_inertia_ratio);
    Q_INVOKABLE double getMaxInertia() const;

    // 0 <= min <= max <= 255
    Q_INVOKABLE void setMinThreshold(int new_min_threshold);
    Q_INVOKABLE int getMinThreshold() const;

    Q_INVOKABLE void setMaxThreshold(int new_max_threshold);
    Q_INVOKABLE int getMaxThreshold() const;

    // 1 <= new_threshold_step <= max threshold - min threshold
    Q_INVOKABLE void setThresholdStep(int new_threshold_step);
    Q_INVOKABLE int getThresholdStep() const;

    Q_INVOKABLE QString toString() const;
};

#endif  // BFP_ENGINE_PROTOTYPES_BLOB_TPL_PROTOTYPE_H
