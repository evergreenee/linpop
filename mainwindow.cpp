#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "database.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    DataDB *dbInstance = DataDB::GetInstance();
    QString username = ui->lineEdit_username->text();
    quint32 userID = ui->lineEdit_userID->text().toUInt();
    QString password = ui->lineEdit_password->text();
    dbInstance->loginUser(userID,username,password);
}

void MainWindow::on_pushButton_2_clicked()
{
    DataDB *dbInstance = DataDB::GetInstance();
    QString username = ui->lineEdit_username->text();
    quint32 userID = ui->lineEdit_userID->text().toUInt();
    QString password = ui->lineEdit_password->text();
    dbInstance->registerUser(userID,username,password);
}
