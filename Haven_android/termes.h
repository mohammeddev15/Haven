#ifndef TERMES_H
#define TERMES_H

#include <QMainWindow>

namespace Ui {
class termes;
}

class termes : public QMainWindow
{
    Q_OBJECT

public:
    explicit termes(QWidget *parent = nullptr);
    ~termes();

private slots:
    void on_pushButton_clicked();

private:
    Ui::termes *ui;
};

#endif // TERMES_H
