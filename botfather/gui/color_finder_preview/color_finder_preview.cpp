#include "color_finder_preview.hpp"

#include <QIcon>
#include <QVBoxLayout>

ColorFinderPreview::ColorFinderPreview(const QString &file_name,
                                       QWidget *parent)
    : QWidget(parent) {
    setWindowTitle("Color finder preview");
    setWindowIcon(QIcon("://logo_v2.png"));

    setLayout(new QVBoxLayout());
    label = new QLabel(this);
    layout()->addWidget(label);
    layout()->setMargin(0);

    original = cv::imread(file_name.toStdString());
    setFixedSize(QSize(original.cols, original.rows));
}

void ColorFinderPreview::updateColors(int min_h, int min_s, int min_v,
                                      int max_h, int max_s, int max_v) {
    cv::Scalar min_hsv(min_h, min_s, min_v);
    cv::Scalar max_hsv(max_h, max_s, max_v);
    cv::Mat isolated = Vision::isolateColor(original, min_hsv, max_hsv, true);
    QImage image = Vision::cvMatToQImage(isolated);
    label->setPixmap(QPixmap::fromImage(image));
}
