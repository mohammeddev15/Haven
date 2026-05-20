#include "ui4.h"
#include "ui_ui4.h"
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QScreen>
#include"mainwindow.h"
ui4::ui4(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ui4)
{
    ui->setupUi(this);
    setWindowTitle("Haven ");
    setGeometry(QGuiApplication::primaryScreen()->availableGeometry());
    net = new QNetworkAccessManager(this);
}

ui4::~ui4()
{
    delete ui;
}

void ui4::on_pushButton_2_clicked()
{
    MainWindow *back=new MainWindow();
    back->show();
    this->close();
}


void ui4::on_pushButton_clicked()
{
    lastMsg = ui->lineEdit->text();
    ui->textBrowser_2->append("انت : " + lastMsg);
    ui->lineEdit->clear();

    // أول محاولة: Groq
    systemPrompt = "YOU ARE an older brother figure and a safe haven. RULES: 1. Speak ONLY in Modern Standard Arabic (فصحى) with a warm, calm, and reassuring tone. 2. Do not give long informational explanations; instead offer concise advice, moral lessons, relevant wisdom quotes, and brief invented personal experiences. 3. Be empathetic, protective, patient, and non-judgmental, always making the user feel heard and safe. 4. Encourage reflection rather than giving direct orders, use 'we' and supportive language. 5. Keep replies to 2-4 sentences, avoid technical detail, and vary your wisdom each time without repeating phrases.";

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
