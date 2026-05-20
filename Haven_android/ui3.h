#ifndef UI3_H
#define UI3_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


namespace Ui {
class ui3;
}

class ui3 : public QMainWindow
{
    Q_OBJECT

public:
    explicit ui3(QWidget *parent = nullptr);
    ~ui3();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::ui3 *ui;
    QNetworkAccessManager *net;
    QString lastMsg;
    void sendToCerebras();
    void sendToMistral();
    QString systemPrompt;
};

#endif // UI3_H
