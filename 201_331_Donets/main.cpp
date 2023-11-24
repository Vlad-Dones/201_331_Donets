#include "mainwindow.h"
#include <iostream>
#include <windows.h>
#include <QApplication>

int main(int argc, char *argv[])
{

    //Запуск приложения
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    wchar_t cmdLine[] = L"C:/Users/Vlad/Desktop/DOSAS/201_331_Donets/build-sputnik-Desktop_Qt_6_4_3_MSVC2019_64bit-Debug/debug/sputnik.exe";

    if(CreateProcess(cmdLine, NULL, NULL, NULL, TRUE, NULL, NULL, NULL, &si, &pi)) {
        std::cout << "Process id created !" << std::endl;
        std::cout << "Pid = " << std::dec << pi.dwProcessId << std::endl;
    }
    else {
        std::cout << "Process is not created!"<< std::endl;
    }

    QApplication a(argc, argv);
    MainWindow w;
    w.show_pin_check_window();
    return a.exec();
}
