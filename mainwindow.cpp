#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QSslSocket>
#include <QDebug>

#define OPENAI_API_KEY ""
#define OPENAI_URL "https://api.openai.com/v1/chat/completions"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    networkManager = new QNetworkAccessManager(this);
    connect(ui->sendButton, &QPushButton::clicked, this, &MainWindow::on_sendButton_clicked);
    connect(networkManager, &QNetworkAccessManager::finished, this, &MainWindow::onChatGPTReply);
    qDebug() << QSslSocket::supportsSsl();

    this->setStyleSheet("background-color: white;");

        ui->textEdit->setStyleSheet(
            "background-color: #AAD4FF;"
            "color: black;"
            "border-radius: 10px;"
            "padding: 5px;"
        );

        ui->lineEdit->setStyleSheet(
            "background-color: #FFC6D9;"
            "color: black;"
            "border-radius: 10px;"
            "padding: 5px;"
        );

        ui->sendButton->setStyleSheet(
            "background-color: #FFC6D9;"
            "color: black;"
            "border-radius: 10px;"
            "padding: 5px;"
            "border: 2px solid #FF92B2;"
            "font-weight: bold;"
            "transition: 0.3s;"
            "min-width: 80px;"
        );

        ui->sendButton->setStyleSheet(
            "QPushButton {"
            "   background-color: #FFC6D9;"
            "   color: black;"
            "   border-radius: 10px;"
            "   padding: 5px;"
            "   border: 2px solid #FF92B2;"
            "   font-weight: bold;"
            "}"
            "QPushButton:hover {"
            "   background-color: #AAD4FF;"
            "   border: 2px solid #7BA6E3;"
            "}"
            "QPushButton:pressed {"
            "   background-color: white;"
            "   border: 2px solid gray;"
            "}"
        );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_sendButton_clicked(){
    QString userInput = ui->lineEdit->text().trimmed();
    if (!userInput.isEmpty()) {
        ui->textEdit->append("You: " + userInput);
        sendChatGPTRequest(userInput);
        ui->lineEdit->clear();
    }
}

void MainWindow::sendChatGPTRequest(const QString &message) {
    QNetworkRequest request(QUrl(OPENAI_URL));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", QByteArray("Bearer ") + OPENAI_API_KEY);

    QJsonObject json;
    json["model"] = "gpt-3.5-turbo";
    QJsonArray messages;
    messages.append(QJsonObject{{"role", "user"}, {"content", message}});
    json["messages"] = messages;
    QJsonDocument jsonDoc(json);
    networkManager->post(request, jsonDoc.toJson());
}

void MainWindow::onChatGPTReply(QNetworkReply *reply) {
    if (reply->error() != QNetworkReply::NoError) {
        ui->textEdit->append("Error: " + reply->errorString());
    } else {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonResponse = QJsonDocument::fromJson(responseData);
        QJsonArray choices = jsonResponse.object()["choices"].toArray();

        if (!choices.isEmpty()) {
            QJsonObject choice = choices[0].toObject();
            if (choice.contains("message") && choice["message"].toObject().contains("content")) {
                QString replyText = choice["message"].toObject()["content"].toString();
                ui->textEdit->append("ChatGPT: " + replyText);
            } else {
                ui->textEdit->append("Error: Invalid response format.");
            }
        } else {
            ui->textEdit->append("Error: No choices found in the response.");
        }
    }
    reply->deleteLater();
}
