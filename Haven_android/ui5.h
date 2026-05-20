#ifndef UI5_H
#define UI5_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


namespace Ui {
class ui5;
}

class ui5 : public QMainWindow
{
    Q_OBJECT

public:
    explicit ui5(QWidget *parent = nullptr);
    ~ui5();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::ui5 *ui;
    QNetworkAccessManager *net;
    QString lastMsg;
    void sendToCerebras();
    void sendToMistral();
    QString systemPrompt;
};

#endif // UI5_H
