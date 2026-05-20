#ifndef AI1_H
#define AI1_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

namespace Ui {
class ai1;
}

class ai1 : public QMainWindow
{
    Q_OBJECT

public:
    explicit ai1(QWidget *parent = nullptr);
    ~ai1();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::ai1 *ui;
    QNetworkAccessManager *net;
    QString lastMsg;
    void sendToCerebras();
    void sendToMistral();
};

#endif // AI1_H
