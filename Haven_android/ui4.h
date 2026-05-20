#ifndef UI4_H
#define UI4_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


namespace Ui {
class ui4;
}

class ui4 : public QMainWindow
{
    Q_OBJECT

public:
    explicit ui4(QWidget *parent = nullptr);
    ~ui4();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::ui4 *ui;
    QNetworkAccessManager *net;
    QString lastMsg;
    void sendToCerebras();
    void sendToMistral();
    QString systemPrompt;
};

#endif // UI4_H
