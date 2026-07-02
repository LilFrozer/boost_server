#ifndef MAINCLIENT_H
#define MAINCLIENT_H

#include <QMainWindow>
#include <memory>
#include <boost/asio.hpp>
#include <iostream>
#include "connection.h"
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainClient;
}
QT_END_NAMESPACE

class MainClient : public QMainWindow
{
    Q_OBJECT

public:
    MainClient(QWidget *parent, boost::asio::io_context &io);
    ~MainClient();
    QVector<unsigned> parseRcvFile(const QString& filePath) {
        QVector<unsigned> result;
        QFile file(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qWarning() << "Не удалось открыть файл:" << filePath;
            return result;
        }
        QTextStream in(&file);
        QRegularExpression re("\"([0-9a-fA-F]{8})\"");
        QString line;
        while (in.readLineInto(&line)) {
            QRegularExpressionMatchIterator it = re.globalMatch(line);
            while (it.hasNext()) {
                QRegularExpressionMatch match = it.next();
                QString hex = match.captured(1);
                bool ok;
                unsigned value = hex.toUInt(&ok, 16);
                if (ok)
                    result.append(value);
                else
                    qWarning() << "Неверный hex:" << hex;
            }
        }
        file.close();
        return result;
    }
private:
    Ui::MainClient *ui;
    std::shared_ptr<IConnection> Iserver_{nullptr};
    std::unique_ptr<QCustomPlot> plot_{nullptr};
private slots:
    void slot_connect();
    void slot_disconnect();
    void slot_send();
};
#endif // MAINCLIENT_H
