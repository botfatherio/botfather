#ifndef COLORFINDERPREVIEW_HPP
#define COLORFINDERPREVIEW_HPP

#include <QLabel>
#include <QWidget>

#include "../../engine/modules/vision/vision.hpp"

class ColorFinderPreview : public QWidget {
    Q_OBJECT
   public:
    explicit ColorFinderPreview(const QString &file_name,
                                QWidget *parent = nullptr);

   public slots:
    void updateColors(int min_h, int min_s, int min_v, int max_h, int max_s,
                      int max_v);

   private:
    cv::Mat original;
    QLabel *label;
};

#endif  // COLORFINDERPREVIEW_HPP
