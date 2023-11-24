#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QPushButton"
#include "QMessageBox"
#include "QByteArray"
#include "QFile"
#include "QJsonDocument"
#include "QJsonObject"
#include <openssl/evp.h>
#include "QDebug"
#include "QByteArray"
#include "QBuffer"
#include "QFile"
#include "QJsonParseError"
#include "QJsonObject"
#include "QJsonArray"
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

    //Отсикаем лишнии детали
    QString trimdecrypt_data = decrypt_data.left(decrypt_data.lastIndexOf('}'));
    trimdecrypt_data[trimdecrypt_data.length() - 1] = '\n';
    trimdecrypt_data[trimdecrypt_data.length() - 1] = '}';

    qDebug() << "Расшифрованный файл: " + trimdecrypt_data;

                    //Переводим данные в json
                    QByteArray decrypt_data_byte = trimdecrypt_data.toUtf8();
    QJsonDocument jsonDocument = QJsonDocument::fromJson(decrypt_data_byte);
    QJsonObject jsonObject = jsonDocument.object();


    for (auto it = jsonObject.begin(); it != jsonObject.end(); ++it) {
        // Получаем подобъект для каждого аккаунта
        QJsonObject accountObject = it.value().toObject();

        // Получаем данные
        QString id = accountObject.value("id").toString();
        int sum = accountObject.value("sum").toInt();
        QString date = accountObject.value("date").toString();

        //Создаем временный вектор
        QVector<QString> current_data {id, QString::number(sum), date};

        //Добавление данные в общий лист
        vector_of_accounts.push_back(current_data);
    }


    //Отрисовываем текущий аккаунт
    display_account(counter_account);

    return 0;
}

int MainWindow::display_account(int counter_account)
{
    if (counter_account == 0) {
        this->setStyleSheet("background-color: #aa71e3;");
    }
    else if (counter_account == 1) {
        this->setStyleSheet("background-color: #a6608c;");
    }
    else if (counter_account == 2) {
        this->setStyleSheet("background-color: #95c7b8;");
    }

    ui->label_account->setText(vector_of_accounts[counter_account][0]);
    ui->label_sum->setText(vector_of_accounts[counter_account][1]);
    ui->label_date->setText(vector_of_accounts[counter_account][2]);



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


void MainWindow::on_next_btn_clicked()
{

    counter_account+= 1;
    if (counter_account == 3) {
        counter_account = 0;
    }

    display_account(counter_account);

}

void MainWindow::on_last_btn_clicked()
{
    counter_account-= 1;
    if (counter_account == -1) {
        counter_account = 2;
    }
    display_account(counter_account);
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


