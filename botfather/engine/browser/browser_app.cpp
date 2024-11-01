#include "browser_app.hpp"

#include <QApplication>
#include <QDir>
#include <QFileInfo>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSettings>

#include "browser_settings.hpp"

static QString parseFlashVersionFromManifest(QString flash_manifest_path) {
    QFile manifest_file(flash_manifest_path);
    if (!manifest_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return "";
    }

    QJsonDocument json = QJsonDocument::fromJson(manifest_file.readAll());
    QString version = json.object().value("version").toString();
    return version;
}

void BrowserApp::OnBeforeCommandLineProcessing(
    const CefString& process_type, CefRefPtr<CefCommandLine> command_line) {
    Q_UNUSED(process_type);
    QSettings settings;

    QString flash_so =
        settings.value(browser::options::FLASH_SO, "").toString();
    QString flash_manifest =
        settings.value(browser::options::FLASH_MANIFEST, "").toString();

    if (!flash_so.isEmpty() && !flash_manifest.isEmpty()) {
        QFileInfo flash_so_info(flash_so);
        QFileInfo flash_manifest_info(flash_manifest);

        bool is_flash_so_file = flash_so_info.isFile();
        bool is_flash_manifest_file = flash_manifest_info.isFile();

        if (is_flash_so_file && is_flash_manifest_file) {
            QString flash_version =
                parseFlashVersionFromManifest(flash_manifest);
            if (!flash_version.isEmpty()) {
                command_line->AppendSwitchWithValue("ppapi-flash-path",
                                                    flash_so.toStdString());
                command_line->AppendSwitchWithValue(
                    "ppapi-flash-version", flash_version.toStdString());
            }
        }
    }

    // Fix page scrolling. (scrolling ingame worked but not on normal pages)
    // https://bitbucket.org/chromiumembedded/cef/issues/2214/osr-scroll-is-erratic-after-using-mouse
    // TODO: eventually remove this "fix" when we use a fixed cef version
    command_line->AppendSwitch("disable-smooth-scrolling");
    command_line->AppendSwitchWithValue(
        "disable-features", "TouchpadAndWheelScrollLatching,AsyncWheelEvents");

    // Loads system plugins like flash in newer CEF versions.
    command_line->AppendSwitch("load-extension");

    //  Turn on hardware acceleration (required by some webgl apps and games)
    command_line->AppendSwitch("enable-gpu");

    // Synchronize the frame rate between all processes. This results in
    // decreased CPU usage by avoiding the generation of extra frames that
    // would otherwise be discarded. The frame rate can be set at browser
    // creation time via CefBrowserSettings.windowless_frame_rate or changed
    // dynamically using CefBrowserHost::SetWindowlessFrameRate. In cefclient
    // it can be set via the command-line using `--off-screen-frame-rate=XX`.
    // See https://bitbucket.org/chromiumembedded/cef/issues/1368 for details.
    command_line->AppendSwitch("enable-begin-frame-scheduling");

    // Necesarry to view DRM protected content
    command_line->AppendSwitch("enable-widevine-cdm");

    // On some CEF versions only commandline args disable sandbox properly
    command_line->AppendSwitch("no-sandbox");
    command_line->AppendSwitch("disable-setuid-sandbox");
}
