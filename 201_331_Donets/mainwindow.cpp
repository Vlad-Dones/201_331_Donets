#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QPushButton"
#include "QMessageBox"
#include <windows.h>
#include <openssl/evp.h>
#include "QDebug"
#include "QFile"
#include "QVBoxLayout"
#include "QLabel"



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

void MainWindow::show_pin_check_window() {

    pin_window = new QWidget();
    pin_window->setGeometry(683,350,270,120);

    QVBoxLayout *layout_pin = new QVBoxLayout;

    QLabel *pin_code_label = new QLabel("Пин код");
    input_pincode = new QLineEdit();
    input_pincode->setEchoMode(QLineEdit::Password);

    QPushButton *check_pin_btn = new QPushButton("Вход");
    connect(check_pin_btn, SIGNAL(clicked()), this, SLOT(check_pin_code()));

    //Добавляем в окно лейбл кнопку и инпут
    layout_pin->addWidget(pin_code_label);
    layout_pin->addWidget(input_pincode);
    layout_pin->addWidget(check_pin_btn);
    pin_window->setLayout(layout_pin);

    pin_window->show();
}

int MainWindow::check_pin_code() {
    const QString orig_code = "0000";
    QString code = input_pincode->text();

    if (orig_code == code) {
        delete[] pin_window;
        this->show_account_window();
        this->show();
        return 0;

    }
    else {
        QMessageBox::critical(NULL,QObject::tr("Ошибка"),tr("Ошибка ввода пин-кода"));
        return 0;
    }
}
int MainWindow::show_account_window() {
    qDebug() << "show_account_window";
    //Открываем файл на чтение
    QFile secret_file("C:/Users/Vlad/Desktop/DOSAS/201_331_Donets/encrypt_data.json");
    secret_file.open(QIODevice::ReadOnly | QIODevice::Text);

    if (!secret_file.isOpen()) {
        qDebug() << "Ошибка в открытии исходного файла";
        return 0;
    }

    //Считываем зашифрованные данные
    QString secret_data = secret_file.readAll();

    //Расшифровываем данные
    QString decrypt_data = decrypt_file(secret_data.toUtf8());
    return 0;
}

QString MainWindow::decrypt_file(QByteArray secret_data) {
    // Расшифровка
    QByteArray decrypted_data;  // Общие расшифрованные данные

    // Размер блока для расшифровки
    const int block_size = 512;

    // Цикл по блокам данных
    for (int i = 0; i < secret_data.size(); i += block_size) {
        // Выделение блока данных
        QByteArray block = secret_data.mid(i, block_size); //берём подстроку от i до block_size

        // Расшифровка блока
        unsigned char decrypt_number[block_size];
        int decrypt_len = decrypt((unsigned char*)QByteArray::fromBase64(block).data(), block.length(), key, iv, decrypt_number);
        decrypt_number[decrypt_len] = '\0';

        // Добавление расшифрованного блока к общим данным
        decrypted_data.append(QByteArray::fromRawData((const char*)decrypt_number, decrypt_len));
    }

    // Возвращаем общую строку для расшифрованных данных
    return decrypted_data;
}

int MainWindow::decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
                        unsigned char *iv, unsigned char *decryptext)
{
    EVP_CIPHER_CTX *ctx;

    int len;

    int decryptext_len;

    if(!(ctx = EVP_CIPHER_CTX_new()))
        crypt_error();


    if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
        crypt_error();


    if(1 != EVP_DecryptUpdate(ctx, decryptext, &len, ciphertext, ciphertext_len))
        crypt_error();
    decryptext_len = len;


    if(1 != EVP_DecryptFinal_ex(ctx, decryptext + len, &len))
        crypt_error();
    decryptext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    return decryptext_len;
}

int MainWindow::crypt_error(void)
{
    return 1;
}
