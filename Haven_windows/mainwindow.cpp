#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include"ai1.h"
#include"ui2.h"
#include"ui3.h"
#include"ui4.h"
#include"ui5.h"
#include"ui6.h"
#include"termes.h"
#include <QScreen>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("Haven ");
    setGeometry(QGuiApplication::primaryScreen()->availableGeometry());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_2_clicked()
{
    ai1 *ui1 =new ai1();
    ui1->show();
    this->close();
}


void MainWindow::on_pushButton_clicked()
{
    ui2 *ai2=new ui2();
    ai2->show();
    this->close();
}


void MainWindow::on_pushButton_3_clicked()
{
    ui3 *ai3=new ui3();
    ai3->show();
    this->close();
}


void MainWindow::on_pushButton_4_clicked()
{
    ui4 *ai4=new ui4();
    ai4->show();
    this->close();
}


void MainWindow::on_pushButton_5_clicked()
{
    ui5 *ai5=new ui5();
    ai5->show();
    this->close();
}


void MainWindow::on_pushButton_6_clicked()
{
    ui6 *ai6=new ui6();
    ai6->show();
    this->close();
}


void MainWindow::on_pushButton_7_clicked()
{
    termes *trme=new termes();
    trme->show();
    this->close();
}

