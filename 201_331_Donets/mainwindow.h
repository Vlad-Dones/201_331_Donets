#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QMessageBox"
#include "QLineEdit"



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QWidget *pin_window;
    QLineEdit *input_pincode;
    void show_pin_check_window();


private slots:


    //Проверка пин кода
    int check_pin_code();

    //Вывод основного окна
    int show_account_window();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
