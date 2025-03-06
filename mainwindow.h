#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkAccessManager>
#include <QNetworkReply>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onChatGPTReply(QNetworkReply *reply);
    void on_sendButton_clicked();

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager *networkManager;
    void sendChatGPTRequest(const QString &message);
};
#endif // MAINWINDOW_H
