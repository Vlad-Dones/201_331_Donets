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

    //Данные для расшифровки
    int decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,unsigned char *iv, unsigned char *decryptext);
    unsigned char *key = (unsigned char *)"8b1a9953c4611296a827abf8c47804d7";
    unsigned char *iv = (unsigned char *)"0123456789012345";
    int crypt_error(void);
    QString decrypt_file(QByteArray secret_data);


    //Вектор в котором все банковские счета
    QVector<QVector<QString>> vector_of_accounts;

    //Номер счета
    int counter_account = 0;

private slots:


    //Проверка пин кода
    int check_pin_code();

    //Вывод основного окна
    int show_account_window();

    //Функции отрисовки текущего счета
    int display_account(int counter_account);

    //Нажатие на кнопки
    void on_next_btn_clicked();
    void on_last_btn_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
