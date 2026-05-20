#include "ui2.h"
#include "ui_ui2.h"
#include"mainwindow.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include<QMessageBox>
#include <QScreen>


ui2::ui2(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ui2)
{
    ui->setupUi(this);
    setWindowTitle("Haven ");
    setGeometry(QGuiApplication::primaryScreen()->availableGeometry());
    net = new QNetworkAccessManager(this);
}

ui2::~ui2()
{
    delete ui;
}

void ui2::on_pushButton_2_clicked()
{
    MainWindow *back=new MainWindow();
    back->show();
    this->close();
}

void ui2::on_pushButton_clicked()
{
    lastMsg = ui->lineEdit->text();
    ui->textBrowser_2->append("انت : " + lastMsg);
    ui->lineEdit->clear();

    systemPrompt = "YOU ARE a sarcastic, funny Arab friend with average general knowledge. RULES: 1. Speak ONLY in Modern Standard Arabic (فصحى). 2. For easy questions, answer with a short original joke related to the topic. 3. Occasionally add a random witty remark. 4. If asked a scientific, technical, or complex question, DO NOT give a full answer; instead invent a fresh, playful insult or tease criticizing why the user needs it, be creative and NEVER reuse the same phrasing twice. 5. Keep replies to 2-3 sentences (25-45 words). 6. Always vary your humor, avoid canned phrases like 'اذهب وابحث بنفسك', and never be overly helpful.";    // أول محاولة: Groq
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
void ui2::sendToCerebras() {
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

void ui2::sendToMistral() {
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
            QMessageBox::information(this,"Errer","حذث خطا في الانترنت او ان السيرفرات ممتلئة");
        }
        reply->deleteLater();
    });
}
