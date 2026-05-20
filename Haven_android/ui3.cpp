#include "ui3.h"
#include "ui_ui3.h"
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QScreen>
#include"mainwindow.h"
ui3::ui3(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ui3)
{
    ui->setupUi(this);
    setWindowTitle("Haven ");
    setGeometry(QGuiApplication::primaryScreen()->availableGeometry());
    net = new QNetworkAccessManager(this);
}

ui3::~ui3()
{
    delete ui;
}

void ui3::on_pushButton_2_clicked()
{
    MainWindow *back=new MainWindow();
    back->show();
    this->close();
}


void ui3::on_pushButton_clicked()
{
    lastMsg = ui->lineEdit->text();
    ui->textBrowser_2->append("انت : " + lastMsg);
    ui->lineEdit->clear();

    // أول محاولة: Groq
    systemPrompt = "YOU ARE a formal, professional work colleague. RULES: 1. Speak ONLY in Modern Standard Arabic (فصحى). 2. Maintain a serious, respectful, and concise tone at all times. 3. Do not participate in jokes, gossip, or any non-serious discussions; politely steer back to work topics. 4. Speak only when providing useful, factual, or task-oriented information, avoid filler talk. 5. Keep replies brief and clear, typically 1-2 sentences. 6. Prioritize efficiency, accuracy, and professionalism over friendliness, never use slang or casual language.";

    QNetworkRequest req(QUrl("https://api.groq.com/openai/v1/chat/completions"));
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setRawHeader("Authorization", "Bearer your_secret_key");

    QJsonObject body;
    body["model"] = "llama-3.3-70b-versatile";


    QJsonObject sysMsg;
    sysMsg["role"] = "system";
    sysMsg["content"] = systemPrompt;


    QJsonObject msg;
    msg["role"] = "user";
    msg["content"] = lastMsg;

    QJsonArray messages;
    messages.append(sysMsg);
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

void ui6::sendToCerebras() {
    QNetworkRequest req(QUrl("https://api.cerebras.ai/v1/chat/completions"));
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setRawHeader("Authorization", "Bearer your_secret_key");

    QJsonObject body;
    body["model"] = "llama3.3-70b";

    // نفس البرومت هنا
    QJsonArray messages;
    messages.append(QJsonObject{{"role","system"},{"content", systemPrompt}});
    messages.append(QJsonObject{{"role","user"},{"content", lastMsg}});
    body["messages"] = messages;

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

void ui6::sendToMistral() {
    QNetworkRequest req(QUrl("https://api.mistral.ai/v1/chat/completions"));
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setRawHeader("Authorization", "Bearer your_secret_key");

    QJsonObject body;
    body["model"] = "mistral-large-latest";

    // وهنا كذلك
    QJsonArray messages;
    messages.append(QJsonObject{{"role","system"},{"content", systemPrompt}});
    messages.append(QJsonObject{{"role","user"},{"content", lastMsg}});
    body["messages"] = messages;

    auto reply = net->post(req, QJsonDocument(body).toJson());
    connect(reply, &QNetworkReply::finished, this, [=](){
        if(reply->error() == QNetworkReply::NoError){
            auto doc = QJsonDocument::fromJson(reply->readAll());
            ui->textBrowser_2->append("Haven : " + doc["choices"][0]["message"]["content"].toString());
        } else {
            QMessageBox::information(this,"نتأسف عن هذا ","يبدو أن جميع السيرفرات قد امتلأت يرجى العودة لاحقاً");
        }
        reply->deleteLater();
    });
}
