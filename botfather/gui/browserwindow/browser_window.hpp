#ifndef BFP__GUI__BROWSER_WINDOW_H
#define BFP__GUI__BROWSER_WINDOW_H

#include <QImage>
#include <QMainWindow>

#include "browser_address_bar.hpp"
#include "browser_widget.hpp"

namespace Ui {
class BrowserWindow;
}

class BrowserWindow : public QMainWindow {
    Q_OBJECT

   public:
    explicit BrowserWindow(const QString &title, QWidget *parent = nullptr);
    ~BrowserWindow();

    BrowserWidget *browserWidget() const;
    BrowserAddressBar *addressBar() const;

   public slots:
    void show();
    void paintSlot(QImage browser_image);
    void updateNavigationButtons(bool is_loading, bool can_go_back,
                                 bool can_go_forward);

   signals:
    void homeTriggered();
    void reloadTriggered();
    void stopTriggered();
    void forwardTriggered();
    void backTriggered();
    void globalWidgetPositionChanged(const QPoint &new_position);

    void windowMoved();
    void windowActivated();

   protected:
    bool event(QEvent *event);

   private:
    Ui::BrowserWindow *m_ui;
    BrowserWidget *m_browser_widget;
    BrowserAddressBar *m_adressbar;
};

#endif  // BFP__GUI__BROWSER_WINDOW_H
