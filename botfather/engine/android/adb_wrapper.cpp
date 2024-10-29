#include "adb_wrapper.hpp"

#include <QDebug>
#include <QDir>
#include <QPixmap>
#include <QProcess>
#include <QRegularExpression>

AdbWrapper::AdbWrapper(QObject *parent, const QString &adb_binary)
    : QObject(parent), adb_binary(adb_binary) {}

bool AdbWrapper::startAdbServer() {
    QStringList args("start-server");
    return evaluateAdbCommand(args);
}

bool AdbWrapper::queryForDevices(QVector<AdbDeviceInfo> &devices) {
    QStringList args;
    args << "devices"
         << "-l";

    QString standard_output;  // "List of devices
                              // attached\n31464723a1f4d623\tdevice\n\n"
    bool result = evaluateAdbCommand(args, standard_output);

    QRegularExpression regex(
        "^(?<serial_number>\\S*)(\\s*)(?<state>no "
        "device|device|offline)(.*)product:(?<product>\\S*)(.*)model:(?<model>"
        "\\S*)(.*)$");

    for (QString line : standard_output.split("\n")) {
        QRegularExpressionMatch match = regex.match(line);

        if (match.hasMatch()) {
            AdbDeviceInfo device_info;
            device_info.setSerialNumber(match.captured("serial_number"));
            device_info.setState(match.captured("state"));
            device_info.setProduct(match.captured("product"));
            device_info.setModel(match.captured("model"));
            devices.append(device_info);
        }
    }

    return result;
}

bool AdbWrapper::listPackages(QString serial_number,
                              QVector<QString> &packages) {
    QStringList args;
    args << "-s" << serial_number << "shell"
         << "pm"
         << "list"
         << "packages";

    QString standard_output;
    bool result = evaluateAdbCommand(args, standard_output);

    for (QString line : standard_output.split("\n")) {
        // Example line: package:com.google.android.youtube

        if (line.startsWith("package:")) {
            line.remove("package:");
            packages.append(line);
        }
    }

    return result;
}

bool AdbWrapper::startApp(QString serial_number, QString package) {
    QStringList args;
    args << "-s" << serial_number << "shell"
         << "monkey"
         << "-p" << package << "-c"
         << "android.intent.category.LAUNCHER"
         << "1";
    return evaluateAdbCommand(args);
}

bool AdbWrapper::sendTap(QString serial_number, int x, int y) {
    // adb shell ( > 2.3.5) required
    QStringList args;
    args << "-s" << serial_number << "shell"
         << "input"
         << "tap" << QString::number(x) << QString::number(y);
    return evaluateAdbCommand(args);
}

bool AdbWrapper::sendSwipe(QString serial_number, int x1, int y1, int x2,
                           int y2, int duration_in_ms) {
    QStringList args;
    args << "-s" << serial_number << "shell"
         << "input"
         << "touchscreen"
         << "swipe" << QString::number(x1) << QString::number(y1)
         << QString::number(x2) << QString::number(y2)
         << QString::number(duration_in_ms);
    return evaluateAdbCommand(args);
}

bool AdbWrapper::sendKeyEvent(QString serial_number, QString key_event_code) {
    QStringList args;
    args << "-s" << serial_number << "shell"
         << "input"
         << "keyevent" << key_event_code;
    return evaluateAdbCommand(args);
}

bool AdbWrapper::sendEvent(QString serial_number, QString device, int type,
                           int code, int value) {
    QStringList args;
    args << "-s" << serial_number << "shell"
         << "sendevent" << device << QString::number(type)
         << QString::number(code) << QString::number(value);
    return evaluateAdbCommand(args);
}

bool AdbWrapper::sendTextInput(QString serial_number, QString text) {
    QStringList args;
    args << "-s" << serial_number << "shell"
         << "input"
         << "text" << text;
    return evaluateAdbCommand(args);
}

bool AdbWrapper::takeScreenshot(QString serial_number, QImage &screenshot) {
#ifdef Q_OS_WIN
    QStringList cap_args;
    cap_args << "-s" << serial_number << "shell"
             << "screencap"
             << "-p"
             << "/mnt/sdcard/BFTMP.png";

    QStringList pull_args;
    pull_args << "-s" << serial_number << "pull"
              << "/mnt/sdcard/BFTMP.png" << QDir::tempPath() + "/BFTMP.png";

    if (!evaluateAdbCommand(cap_args)) {
        return false;
    }

    if (!evaluateAdbCommand(pull_args)) {
        return false;
    }

    QPixmap pixmap(QDir::tempPath() + "/BFTMP.png");
    screenshot = pixmap.toImage();
    return true;
#else
    QStringList args;
    args << "-s" << serial_number << "exec-out"
         << "screencap"
         << "-p";

    QByteArray std_out;
    QByteArray std_err;
    QProcess::ProcessError result = executeAdbCommand(args, std_out, std_err);

    if (std_out.isEmpty() || !std_err.isEmpty() ||
        result != QProcess::ProcessError::UnknownError) {
        return false;
    }

    screenshot = QImage::fromData(std_out, "PNG");
    return true;
#endif
}

bool AdbWrapper::evaluateAdbCommand(QStringList arguments, int timeout_in_ms) {
    QString sink;
    return evaluateAdbCommand(arguments, sink, timeout_in_ms);
}

bool AdbWrapper::evaluateAdbCommand(QStringList arguments,
                                    QString &standard_output,
                                    int timeout_in_ms) {
    QByteArray std_out;
    QByteArray std_err;
    QProcess::ProcessError result =
        executeAdbCommand(arguments, std_out, std_err, timeout_in_ms);
    standard_output = std_out;
    return std_err.isEmpty() && result == QProcess::ProcessError::UnknownError;
}

QProcess::ProcessError AdbWrapper::executeAdbCommand(QStringList arguments,
                                                     QString &standard_output,
                                                     QString &standard_error,
                                                     int timeout_in_ms) {
    QByteArray std_out;
    QByteArray std_err;
    QProcess::ProcessError result =
        executeAdbCommand(arguments, std_out, std_err, timeout_in_ms);
    standard_output = QString(std_out);
    standard_error = QString(std_err);
    return result;
}

QProcess::ProcessError AdbWrapper::executeAdbCommand(
    QStringList arguments, QByteArray &standard_output,
    QByteArray &standard_error, int timeout_in_ms) {
    if (adb_binary.isEmpty()) {
        return QProcess::ProcessError::FailedToStart;
    }

    QProcess process(this);
    process.start(adb_binary, arguments);
    process.waitForFinished(timeout_in_ms);

    standard_output = process.readAllStandardOutput();
    standard_error = process.readAllStandardError();

    // QProcess::ProcessError::UnknownError is the default return value of
    // QProcess::error() even if there was no error and shall thus not been
    // threaded as an real error.
    return process.error();
}
