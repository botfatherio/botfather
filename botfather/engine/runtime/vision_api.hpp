#ifndef BFP_ENGINE_APIS_VISION_API_H
#define BFP_ENGINE_APIS_VISION_API_H

#include <QImage>

#include "../vision/blob_tpl.hpp"
#include "../vision/match.hpp"
#include "abstract_api.hpp"

class VisionAPI : public AbstractAPI {
    Q_OBJECT

   public:
    using AbstractAPI::AbstractAPI;
    static QImage withoutAlphaChannel(const QImage &image);

    // Note: Remember most OpenCV matching methods can only handle images
    // without alpha channel.

    Q_INVOKABLE QScriptValue findMaskedMatches(const QImage &image,
                                               const QImage &tpl,
                                               const QImage &mask,
                                               double threshold = 0.8,
                                               int max_matches = -1);
    Q_INVOKABLE QScriptValue findMaskedMatch(const QImage &image,
                                             const QImage &tpl,
                                             const QImage &mask,
                                             double threshold = 0.8);

    Q_INVOKABLE QScriptValue findMatches(const QImage &image, const QImage &tpl,
                                         double threshold = 0.8,
                                         int max_matches = -1);
    Q_INVOKABLE QScriptValue findMatch(const QImage &image, const QImage &tpl,
                                       double threshold = 0.8);

    Q_INVOKABLE QScriptValue findBlobs(const QImage &image,
                                       const BlobTpl &blob_tpl,
                                       int min_distance = 10,
                                       int min_repeatability = 2);

    Q_INVOKABLE QScriptValue markMatches(const QImage &image,
                                         const QScriptValue &matches,
                                         const QColor &color,
                                         int thickness = 2);
    Q_INVOKABLE QScriptValue markMatch(const QImage &image, const Match &match,
                                       const QColor &color, int thickness = 2);
};

#endif  // BFP_ENGINE_APIS_VISION_API_H
