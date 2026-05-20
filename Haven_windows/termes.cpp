#include "termes.h"
#include "ui_termes.h"
#include"mainwindow.h"
#include <QScreen>

termes::termes(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::termes)
{
    ui->setupUi(this);
    setWindowTitle("KinSight ");
    setGeometry(QGuiApplication::primaryScreen()->availableGeometry());
}

termes::~termes()
{
    delete ui;
}

void termes::on_pushButton_clicked()
{
    MainWindow *greet=new MainWindow();
    greet->show();
    this->close();
}

