#include "ai1.h"
#include "ui_ai1.h"
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QScreen>
#include"mainwindow.h"
ai1::ai1(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ai1)
{
    ui->setupUi(this);
    setWindowTitle("Haven ");
    setGeometry(QGuiApplication::primaryScreen()->availableGeometry());
    net = new QNetworkAccessManager(this);
}

ai1::~ai1()
{
    delete ui;
}

void ai1::on_pushButton_2_clicked()
{
    MainWindow *back=new MainWindow();
    back->show();
    this->close();
}


void ai1::on_pushButton_clicked()
{
    lastMsg = ui->lineEdit->text();
    ui->textBrowser_2->append("انت : " + lastMsg);
    ui->lineEdit->clear();

    // أول محاولة: Groq
    QNetworkRequest req(QUrl("https://api.groq.com/openai/v1/chat/completions"));
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setRawHeader("Authorization", "Bearer your_secret_key");

    QJsonObject body;
    body["model"] = "llama-3.3-70b-versatile";



    QJsonObject msg;
    msg["role"] = "user";
    msg["content"] = lastMsg;

    QJsonArray messages;
    messages.append(msg);


    body["model"] = "llama-3.3-70b-versatile";
    body["messages"] = messages;

    QNetworkReply *reply = net->post(req, QJsonDocument(body).toJson());

    connect(reply, &QNetworkReply::finished, this, [this, reply](){
        if(reply->error() == QNetworkReply::NoError){
            // نجح Groq
            auto doc = QJsonDocument::fromJson(reply->readAll());
            QString ans = doc["choices"][0]["message"]["content"].toString();
            ui->textBrowser_2->append("Haven : " + ans);
        } else {
            // فشل → دوز ل Cerebras
            sendToCerebras();
        }
        reply->deleteLater();
    });
}

void ai1::sendToCerebras() {
    QNetworkRequest req(QUrl("https://api.cerebras.ai/v1/chat/completions"));
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setRawHeader("Authorization", "Bearer your_secret_key");

    QJsonObject body;
    body["model"] = "llama3.3-70b";
    body["messages"] = QJsonArray{QJsonObject{{"role","user"},{"content",lastMsg}}};

    auto reply = net->post(req, QJsonDocument(body).toJson());
    connect(reply, &QNetworkReply::finished, this, [=](){
        if(reply->error() == QNetworkReply::NoError){
            auto doc = QJsonDocument::fromJson(reply->readAll());
            ui->textBrowser_2->append("Haven : " + doc["choices"][0]["message"]["content"].toString());
        } else {
            sendToMistral(); // فشل → الثالث
        }
        reply->deleteLater();
    });
}



void ai1::sendToMistral() {
    QNetworkRequest req(QUrl("https://api.mistral.ai/v1/chat/completions"));
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setRawHeader("Authorization", "Bearer your_secret_key");

    QJsonObject body;
    body["model"] = "mistral-large-latest";
    body["messages"] = QJsonArray{QJsonObject{{"role","user"},{"content",lastMsg}}};

    auto reply = net->post(req, QJsonDocument(body).toJson());
    connect(reply, &QNetworkReply::finished, this, [=](){
        if(reply->error() == QNetworkReply::NoError){
            auto doc = QJsonDocument::fromJson(reply->readAll());
            ui->textBrowser_2->append("Haven : " + doc["choices"][0]["message"]["content"].toString());
        } else {
            sendToMistral(); // فشل → الثالث
        }
        reply->deleteLater();
    });
}
