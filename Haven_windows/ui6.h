#ifndef UI6_H
#define UI6_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

namespace Ui {
class ui6;
}

class ui6 : public QMainWindow
{
    Q_OBJECT

public:
    explicit ui6(QWidget *parent = nullptr);
    ~ui6();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::ui6 *ui;
    QNetworkAccessManager *net;
    QString lastMsg;
    void sendToCerebras();
    void sendToMistral();
    QString systemPrompt;
};

#endif // UI6_H
