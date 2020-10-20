#ifndef COLOR_FINDER_H
#define COLOR_FINDER_H

#include <QWidget>

static const int H_MIN = 0;
static const int H_MAX = 359;
static const int S_MIN = 0;
static const int S_MAX = 255;
static const int V_MIN = 0;
static const int V_MAX = 255;

namespace Ui {
class ColorFinder;
}

class ColorFinder : public QWidget {
    Q_OBJECT

   public:
    explicit ColorFinder(QWidget *parent = nullptr);
    ~ColorFinder();

   public slots:
    void openPreview();
    void updateCode();
    void onMinHValueChanged(int value);
    void onMaxHValueChanged(int value);
    void onMinSValueChanged(int value);
    void onMaxSValueChanged(int value);
    void onMinVValueChanged(int value);
    void onMaxVValueChanged(int value);

   signals:
    void colorsChanged(int min_h, int min_s, int min_v, int max_h, int max_s,
                       int max_v);

   protected:
    void emitColorsChanged();

   private:
    Ui::ColorFinder *ui;
};

#endif  // COLOR_FINDER_H
