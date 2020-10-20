#include "color_finder.hpp"

#include <QFileDialog>

#include "../color_finder_preview/color_finder_preview.hpp"
#include "ui_color_finder.h"

ColorFinder::ColorFinder(QWidget *parent)
    : QWidget(parent), ui(new Ui::ColorFinder) {
    ui->setupUi(this);
    connect(ui->open_button, &QPushButton::clicked, this,
            &ColorFinder::openPreview);

    // Connect H values
    connect(ui->min_h_slider, &QSlider::valueChanged, ui->min_h_spinner,
            &QSpinBox::setValue);
    connect(ui->max_h_slider, &QSlider::valueChanged, ui->max_h_spinner,
            &QSpinBox::setValue);
    connect(ui->min_h_spinner, QOverload<int>::of(&QSpinBox::valueChanged),
            ui->min_h_slider, &QSlider::setValue);
    connect(ui->max_h_spinner, QOverload<int>::of(&QSpinBox::valueChanged),
            ui->max_h_slider, &QSlider::setValue);

    // Connect S values
    connect(ui->min_s_slider, &QSlider::valueChanged, ui->min_s_spinner,
            &QSpinBox::setValue);
    connect(ui->max_s_slider, &QSlider::valueChanged, ui->max_s_spinner,
            &QSpinBox::setValue);
    connect(ui->min_s_spinner, QOverload<int>::of(&QSpinBox::valueChanged),
            ui->min_s_slider, &QSlider::setValue);
    connect(ui->max_s_spinner, QOverload<int>::of(&QSpinBox::valueChanged),
            ui->max_s_slider, &QSlider::setValue);

    // Connect V values
    connect(ui->min_v_slider, &QSlider::valueChanged, ui->min_v_spinner,
            &QSpinBox::setValue);
    connect(ui->max_v_slider, &QSlider::valueChanged, ui->max_v_spinner,
            &QSpinBox::setValue);
    connect(ui->min_v_spinner, QOverload<int>::of(&QSpinBox::valueChanged),
            ui->min_v_slider, &QSlider::setValue);
    connect(ui->max_v_spinner, QOverload<int>::of(&QSpinBox::valueChanged),
            ui->max_v_slider, &QSlider::setValue);

    // Connect H ranges
    connect(ui->min_h_slider, &QSlider::rangeChanged, ui->min_h_spinner,
            &QSpinBox::setRange);
    connect(ui->max_h_slider, &QSlider::rangeChanged, ui->max_h_spinner,
            &QSpinBox::setRange);

    // Connect S ranges
    connect(ui->min_s_slider, &QSlider::rangeChanged, ui->min_s_spinner,
            &QSpinBox::setRange);
    connect(ui->max_s_slider, &QSlider::rangeChanged, ui->max_s_spinner,
            &QSpinBox::setRange);

    // Connect V ranges
    connect(ui->min_v_slider, &QSlider::rangeChanged, ui->min_v_spinner,
            &QSpinBox::setRange);
    connect(ui->max_v_slider, &QSlider::rangeChanged, ui->max_v_spinner,
            &QSpinBox::setRange);

    // Listen
    connect(ui->min_h_slider, &QSlider::valueChanged, this,
            &ColorFinder::onMinHValueChanged);
    connect(ui->max_h_slider, &QSlider::valueChanged, this,
            &ColorFinder::onMaxHValueChanged);
    connect(ui->min_s_slider, &QSlider::valueChanged, this,
            &ColorFinder::onMinSValueChanged);
    connect(ui->max_s_slider, &QSlider::valueChanged, this,
            &ColorFinder::onMaxSValueChanged);
    connect(ui->min_v_slider, &QSlider::valueChanged, this,
            &ColorFinder::onMinVValueChanged);
    connect(ui->max_v_slider, &QSlider::valueChanged, this,
            &ColorFinder::onMaxVValueChanged);
}

ColorFinder::~ColorFinder() { delete ui; }

void ColorFinder::openPreview() {
    QString file_name = QFileDialog::getOpenFileName(this, "Select an image");
    ui->open_button->clearFocus();
    if (!file_name.isEmpty()) {
        ColorFinderPreview *preview = new ColorFinderPreview(file_name);
        connect(this, &ColorFinder::colorsChanged, preview,
                &ColorFinderPreview::updateColors);
        emitColorsChanged();
        preview->show();
    }
}

void ColorFinder::updateCode() {
    QString tpl(
        "var minHSV(%1, %2, %3, 'HSV');\nvar maxHSV(%4, %5, %6, "
        "'HSV');\nyourImage.isolateColorRange(minHSV, maxHSV);");
    QString code = tpl.arg(ui->min_h_slider->value())
                       .arg(ui->min_s_slider->value())
                       .arg(ui->min_v_slider->value())
                       .arg(ui->max_h_slider->value())
                       .arg(ui->max_s_slider->value())
                       .arg(ui->max_v_slider->value());
    ui->code_text_edit->setPlainText(code);
}

void ColorFinder::onMinHValueChanged(int value) {
    ui->max_h_slider->setRange(value, H_MAX);
    updateCode();
    emitColorsChanged();
}

void ColorFinder::onMaxHValueChanged(int value) {
    ui->min_h_slider->setRange(H_MIN, value);
    updateCode();
    emitColorsChanged();
}

void ColorFinder::onMinSValueChanged(int value) {
    ui->max_s_slider->setRange(value, S_MAX);
    updateCode();
    emitColorsChanged();
}

void ColorFinder::onMaxSValueChanged(int value) {
    ui->min_s_slider->setRange(S_MIN, value);
    updateCode();
    emitColorsChanged();
}

void ColorFinder::onMinVValueChanged(int value) {
    ui->max_v_slider->setRange(value, V_MAX);
    updateCode();
    emitColorsChanged();
}

void ColorFinder::onMaxVValueChanged(int value) {
    ui->min_v_slider->setRange(V_MIN, value);
    updateCode();
    emitColorsChanged();
}

void ColorFinder::emitColorsChanged() {
    emit colorsChanged(ui->min_h_slider->value(), ui->min_s_slider->value(),
                       ui->min_v_slider->value(), ui->max_h_slider->value(),
                       ui->max_s_slider->value(), ui->max_v_slider->value());
}
