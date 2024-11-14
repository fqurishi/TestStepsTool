// mainwindow.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

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
    void checkAuthorizationAndLoadFolders();
    void loadFolders();
    void runTestCasesIndividually();
    void executeTest(const QString &testKey);
    void logToTerminal(const QString &message);

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager *manager;

    void setDarkBlueTheme();
};

#endif // MAINWINDOW_H
