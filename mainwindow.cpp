// mainwindow.cpp
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPalette>
#include <QColor>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QMessageBox>

const QString AUTH_SERVER_URL = "http://76.29.169.184:1232/check-authorization";
const QString EXECUTE_TEST_URL = "http://76.29.169.184:1232/execute-test";
const QString GET_TEST_CASES_URL = "http://76.29.169.184:1232/get-test-cases";
const QString GET_FOLDERS_URL = "http://76.29.169.184:1232/get-folders";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(800, 600);
    setWindowTitle("Test Case Step Automation");

    setDarkBlueTheme();

    manager = new QNetworkAccessManager(this);

    // Start by checking authorization and loading folders
    checkAuthorizationAndLoadFolders();

    // Connect the execute button
    connect(ui->executeButton, &QPushButton::clicked, this, &MainWindow::runTestCasesIndividually);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// Apply custom theme
void MainWindow::setDarkBlueTheme()
{
    QPalette palette;
    palette.setColor(QPalette::Window, QColor("#183A5D"));
    palette.setColor(QPalette::WindowText, QColor("#D9E1EC"));
    palette.setColor(QPalette::Base, QColor("#2E5984"));
    palette.setColor(QPalette::Text, QColor("#D9E1EC"));
    palette.setColor(QPalette::Button, QColor("#405D7D"));
    palette.setColor(QPalette::ButtonText, QColor("#D9E1EC"));
    setPalette(palette);
    setStyleSheet("QPushButton { color: #D9E1EC; background-color: #405D7D; }"
                  "QTextEdit { background-color: #2E5984; color: #D9E1EC; }"
                  "QComboBox { background-color: #2E5984; color: #D9E1EC; }");
}

// Check authorization and load folders from the server
void MainWindow::checkAuthorizationAndLoadFolders()
{
    QNetworkRequest request((QUrl(AUTH_SERVER_URL)));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["password"] = "OSAAS95";
    QNetworkReply *reply = manager->post(request, QJsonDocument(json).toJson());

    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QJsonDocument responseDoc = QJsonDocument::fromJson(reply->readAll());
            bool authorized = responseDoc.object().value("authorized").toBool();
            if (authorized) {
                logToTerminal("Authorization successful...");
                loadFolders();
            } else {
                logToTerminal("Authorization failed. You are not permitted to execute tests.");
            }
        } else {
            logToTerminal("Authorization request failed: " + reply->errorString());
        }
        reply->deleteLater();
    });
}

// Load folder names from the server
void MainWindow::loadFolders()
{
    QNetworkRequest request((QUrl(GET_FOLDERS_URL)));
    QNetworkReply *reply = manager->get(request);

    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QJsonDocument responseDoc = QJsonDocument::fromJson(reply->readAll());
            QJsonArray foldersArray = responseDoc.object().value("folders").toArray();

            for (const QJsonValue &value : foldersArray) {
                ui->folderDropdown->addItem(value.toString());
            }
            logToTerminal("Folders successfully retrieved.");
        } else {
            logToTerminal("Failed to retrieve folders: " + reply->errorString());
        }
        reply->deleteLater();
    });
}

// Retrieve test cases for the selected folder and execute each test case
void MainWindow::runTestCasesIndividually()
{
    QString selectedFolder = ui->folderDropdown->currentText();
    if (selectedFolder.isEmpty()) {
        logToTerminal("Please select a folder.");
        return;
    }

    QNetworkRequest request((QUrl(GET_TEST_CASES_URL)));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["folderName"] = selectedFolder;
    QNetworkReply *reply = manager->post(request, QJsonDocument(json).toJson());

    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QJsonDocument responseDoc = QJsonDocument::fromJson(reply->readAll());
            QJsonArray testKeys = responseDoc.object().value("testKeys").toArray();

            if (testKeys.isEmpty()) {
                logToTerminal("No test keys found for the selected folder.");
            } else {
                for (const QJsonValue &testKey : testKeys) {
                    executeTest(testKey.toString());
                }
            }
        } else {
            logToTerminal("Failed to retrieve test keys: " + reply->errorString());
        }
        reply->deleteLater();
    });
}

// Execute a single test case by sending the test key to the server
void MainWindow::executeTest(const QString &testKey)
{
    QNetworkRequest request((QUrl(EXECUTE_TEST_URL)));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["testKey"] = testKey;
    QNetworkReply *reply = manager->post(request, QJsonDocument(json).toJson());

    connect(reply, &QNetworkReply::finished, this, [=]() {
        if (reply->error() == QNetworkReply::NoError) {
            QJsonDocument responseDoc = QJsonDocument::fromJson(reply->readAll());
            QJsonArray messages = responseDoc.object().value("messages").toArray();

            for (const QJsonValue &message : messages) {
                logToTerminal(message.toString());
            }
        } else {
            logToTerminal("Failed to execute test for Test Key: " + testKey + ". Error: " + reply->errorString());
        }
        reply->deleteLater();
    });
}

// Log messages to the terminal
void MainWindow::logToTerminal(const QString &message)
{
    ui->terminalOutput->append(message);
}

