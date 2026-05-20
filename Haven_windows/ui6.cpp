#include "ui6.h"
#include "ui_ui6.h"
#include"mainwindow.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include<QMessageBox>
#include <QScreen>

ui6::ui6(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ui6)
{
    ui->setupUi(this);
    setWindowTitle("Haven ");
    setGeometry(QGuiApplication::primaryScreen()->availableGeometry());
    net = new QNetworkAccessManager(this);
}

ui6::~ui6()
{
    delete ui;
}

void ui6::on_pushButton_2_clicked()
{
    MainWindow *back=new MainWindow();
    back->show();
    this->close();
}

void ui6::on_pushButton_clicked()
{
    lastMsg = ui->lineEdit->text();
    ui->textBrowser_2->append("انت : " + lastMsg);
    ui->lineEdit->clear();

    QString systemPrompt = "YOU ARE a friendly foreigner genuinely interested in the user. RULES: 1. Speak ONLY in clear Modern Standard Arabic with a slight curious foreign tone. 2. Show sincere cultural curiosity — ask thoughtful open questions about the user's life, opinions, and Arab culture, and remember details to follow up later. 3. Share brief, interesting snippets from your own culture for contrast, without lecturing. 4. Give warm, respectful compliments about ideas or character, not appearance, and listen more than you talk. 5. Be attentive, encouraging, and a little playfully naive about local customs, never pushy or overly familiar. 6. Keep replies to 2-3 sentences, vary your questions, and never reuse the same phrasing.";
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
            QMessageBox::information(this,"Errer","حذث خطا في الانترنت او ان السيرفرات ممتلئة");
        }
        reply->deleteLater();
    });
}
