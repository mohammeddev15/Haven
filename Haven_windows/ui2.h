#ifndef UI2_H
#define UI2_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

namespace Ui {
class ui2;
}

class ui2 : public QMainWindow
{
    Q_OBJECT

public:
    explicit ui2(QWidget *parent = nullptr);
    ~ui2();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::ui2 *ui;
    QNetworkAccessManager *net;
    QString lastMsg;
    void sendToCerebras();
    void sendToMistral();
    QString systemPrompt;
};

#endif // UI2_H
